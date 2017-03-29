// Graphic.cpp : 实现文件
//
#include "stdafx.h"
#include "../stdafx.h"
//#include "GraphicTest-1.h"
#include "Graphic.h"
#include "../resource.h"

#include <cmath>

CRITICAL_SECTION g_csA;

 

// CGraphic

IMPLEMENT_DYNAMIC(CGraphic, CWnd)

CGraphic::CGraphic() :
m_slider_zoom(0),
m_y_graduation_num(GraphicCoordinate::y_graduation_num),
m_y_graduation_offset(GraphicCoordinate::y_graduation_offset),
m_cur_model(Graphic::MonitoringModel),
m_cur_time(_T("00:00:00:000"))
{
	InitializeCriticalSection(&g_csA);
	 
	double start_x_scale = 0.05;
 
	//0.05 -> 0.93 - GraphicCoordinate::y_graduation_length_scale = 0.92
	for (int i = 0; i < Graphic::Deque_Point_Num; i++)
	{
		start_x_scale += GraphicCoordinate::x_offset;// = 0.0174
		m_x_scale_vec.push_back(start_x_scale);
	}
}

CGraphic::~CGraphic()
{
	DeleteCriticalSection(&g_csA);
}


BEGIN_MESSAGE_MAP(CGraphic, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CGraphic 消息处理程序

//void CGraphic::ReconstructControl()
//{
//	  Invalidate();
//}



void CGraphic::OnPaint()
{ 
	CPaintDC dc(this);  

	//定义各种类型的画笔
	CPen PenForDraw(PS_SOLID, 1, RGB(0, 232, 255));

	//将对话框中静态文本框控件的大小坐标赋给m_rectCtrl	
	GetClientRect(&m_rectCtrl);

	//memDC为一虚拟的dc，在CMemoryDC类的构造函数中将实际的dc
	CMemoryDC memDC(&dc, &m_rectCtrl);

	//在需要的时候才绘制背景部分，否则只绘制曲线
	if (m_requiresRedraw)
	{
		if ((m_dcPlate == nullptr) || (m_bitmapPlate.m_hObject == nullptr))
		{
			m_dcPlate.CreateCompatibleDC(&dc);

			DrawStaticInOnPaint(&dc);
		}
		else
		{
			m_dcPlate.DeleteDC();
			m_dcPlate.CreateCompatibleDC(&dc);
			m_bitmapPlate.DeleteObject();

			DrawStaticInOnPaint(&dc);
		}

		m_requiresRedraw = false;
	}

	//将内存中画好的图像直接拷贝到屏幕指定区域上
	memDC->BitBlt(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(), &m_dcPlate, 0, 0, SRCCOPY);

	//绘制曲线
	DrawDynamicCurve(&memDC, m_ChartDataDeque);
}




/*********************************************************
函数名   :	DrawStaticInOnPaint
功能     :	用于在OnPaint中绘制静态文本的初始化和具体绘制
参数     :	[IN]	: p_dc DC指针
			[OUT]	: 无
返回值   : 无
备注     :  
时间     : 2016/8/10 陈登龙
**********************************************************/
void CGraphic::DrawStaticInOnPaint(CDC* p_dc)
{ 
	m_bitmapPlate.DeleteObject();
	//创建一个内存中的图像 
	m_bitmapPlate.CreateCompatibleBitmap(p_dc, m_rectCtrl.Width(), m_rectCtrl.Height());

	//指定内存显示设备在内存中的图像上画图
	m_pbitmapOldPlate = m_dcPlate.SelectObject(&m_bitmapPlate);

	//先用一种颜色作为内存显示设备的背景色
	m_dcPlate.FillSolidRect(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(), RGB(1, 4, 1));

	//绘制坐标轴
	//DrawStaticAxis(&m_dcPlate, _T("/s"), _T("/") + m_cur_y_unit);
	DrawStaticAxis(&m_dcPlate, _T("/s"), m_cur_y_unit);

	//绘制Y轴的动态刻度值
	DrawDynamicYScale(&m_dcPlate, m_y_min, m_y_max);
}


 

/*********************************************************
函数名   : DrawStaticAxis
功能     : 绘制坐标系中的静态元素
参数     :	[IN]	:	CDC指针
					:	X轴单位
					:	Y轴单位
			[OUT]	: 无
返回值   : 无
备注     :  新增绘制X和Y轴的单位
时间     : 2016/8/17 陈登龙
**********************************************************/
void CGraphic::DrawStaticAxis(CDC* CDC, const CString p_x_unit, const CString p_y_unit)
{
	using namespace GraphicCoordinate;

	//获得控件区域
	GetClientRect(&m_rectCtrl); 

	//选择画坐标轴的黑色画笔
	CPen PenForDrawAxis(PS_SOLID, 1, GraphicColor::WHITE);
	CDC->SelectObject(PenForDrawAxis);

	//得到原点坐标
	POINT ordinary_point = GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale);
 
	//得到x轴终点坐标
	POINT x_end_point = GetRealPoint(m_rectCtrl, x_scale_of_x_end, y_scale_of_x_end);

	//得到y轴终点坐标
	POINT y_end_point = GetRealPoint(m_rectCtrl, x_scale_of_y_end, y_scale_of_y_end);


	//绘制X轴
	DrawLine(CDC, ordinary_point, x_end_point);

	//绘制当前时间
	DrawGraphicText(CDC, m_cur_time, ordinary_point.x - 35, ordinary_point.y + 8, font_type, coordinate_cur_time_font_size);

	//绘制左侧Y轴
	DrawLine(CDC, ordinary_point, y_end_point);

	//绘制右侧Y轴
	DrawLine(CDC, GetRealPoint(m_rectCtrl, right_y_start_x_scale, right_y_start_y_scale), GetRealPoint(m_rectCtrl, right_y_end_x_scale, right_y_end_y_scale));

	//绘制X轴箭头
	DrawLine(CDC, x_end_point, GetRealPoint(m_rectCtrl, x_arrow_end_x_scale, x_arrow_start_y_scale_top));
	DrawLine(CDC, x_end_point, GetRealPoint(m_rectCtrl, x_arrow_end_x_scale, x_arrow_start_y_scale_bot));

	//绘制X轴单位
	DrawGraphicText(CDC, p_x_unit, x_end_point.x + 5, x_end_point.y - 7, font_type, x_unit_font_size);

	//绘制左侧Y轴箭头
	DrawLine(CDC, y_end_point, GetRealPoint(m_rectCtrl, y_arrow_end_x_scale_left, y_arrow_end_y_scale));
	DrawLine(CDC, y_end_point, GetRealPoint(m_rectCtrl, y_arrow_end_x_scale_right, y_arrow_end_y_scale));
	 
	//绘制Y轴单位
	//DrawGraphicText(CDC, p_y_unit, y_end_point.x - 40, y_end_point.y, font_type, y_unit_font_size);


	CString min_str("");
	CString max_str("");
	min_str.Format(_T("%.2f"), m_value_min);
	max_str.Format(_T("%.2f"), m_value_max);
	
	//DrawGraphicText(CDC, m_cur_data_name + _T("[") + min_str + _T(",") + max_str + _T("]"), y_end_point.x + 10, y_end_point.y, font_type, y_data_name_font_size, GraphicColor::RED);
	//_T("[") + min_str + _T(",") + max_str + _T("]")
	//绘制当前数据名称加数据单位
	//DrawGraphicText(CDC, m_cur_data_name, y_end_point.x + 10, y_end_point.y, font_type, y_data_name_font_size, GraphicColor::WHITE);
	DrawGraphicText(CDC, m_cur_data_name + _T("(") + p_y_unit + _T(")"), y_end_point.x + 10, y_end_point.y, font_type, y_data_name_font_size, GraphicColor::WHITE);

	//绘制当前数据范围
	DrawGraphicText(CDC, _T("Value[") + min_str + _T(",") + max_str + _T("]"), y_end_point.x + 650, y_end_point.y, font_type, y_data_name_font_size, GraphicColor::RED);





	POINT start_point;
	POINT end_point;
	CString show_str("");

	//绘制X轴10个刻度线
	for (int i = 0; i < x_graduation_num; i++)
	{
		start_point = GetRealPoint(m_rectCtrl,
			x_graduation_offset * i + ordinary_x_scale,
			ordinary_y_scale);

		end_point = GetRealPoint(m_rectCtrl,
			x_graduation_offset * i + ordinary_x_scale,
			ordinary_y_scale - x_graduation_length_scale);

		DrawLine(CDC, start_point, end_point);  

		//绘制横坐标时间刻度，间隔1s
		if (0 == i)
		{
			continue;
		}
		 
		show_str.Format(_T("%d.%d"), i, 0); 
		//show_str.Format(_T("%.1f"), i);
		//绘制x轴文本，相对于刻度左移10，下移5，楷体，大小：120
		DrawGraphicText(CDC, show_str, start_point.x - 10, start_point.y + 5, font_type, 120);
		 
	} 

	//绘制左右Y轴的刻度线
	//for (int i = 0; i < y_graduation_num + 1; i++)
	//for (int i = 0; i < m_y_graduation_num + 1; i++)
	//for (int i = 0; i < m_y_graduation_num; i++)
	{
		//2 * y_graduation_offset为第一个刻度线(从下向上数)在y轴起点坐标上方的位置比例
		//0.9 - (0.046 * 15 + 0.046 * 2) = 0.118
// 		DrawLine(CDC,
// 			GetRealPoint(m_rectCtrl, ordinary_x_scale, -y_graduation_offset * i + ordinary_y_scale - 2 * y_graduation_offset),
// 			GetRealPoint(m_rectCtrl, ordinary_x_scale + y_graduation_length_scale, -y_graduation_offset * i + ordinary_y_scale - 2 * y_graduation_offset));
// 
// 		 
// 		DrawLine(CDC,
// 			GetRealPoint(m_rectCtrl, right_y_end_x_scale, -y_graduation_offset * i + ordinary_y_scale - 2 * y_graduation_offset),
// 			GetRealPoint(m_rectCtrl, right_y_end_x_scale - y_graduation_length_scale, -y_graduation_offset * i + ordinary_y_scale - 2 * y_graduation_offset));

// 		DrawLine(CDC,
// 			GetRealPoint(m_rectCtrl, ordinary_x_scale, -m_y_graduation_offset * i + ordinary_y_scale - m_y_graduation_offset),
// 			GetRealPoint(m_rectCtrl, ordinary_x_scale + y_graduation_length_scale, -m_y_graduation_offset * i + ordinary_y_scale - m_y_graduation_offset));
// 
// 
// 		DrawLine(CDC,
// 			GetRealPoint(m_rectCtrl, right_y_end_x_scale, -m_y_graduation_offset * i + ordinary_y_scale - m_y_graduation_offset),
// 			GetRealPoint(m_rectCtrl, right_y_end_x_scale - y_graduation_length_scale, -m_y_graduation_offset * i + ordinary_y_scale - m_y_graduation_offset));
// 		 
	}

	//绘制上方外侧红线
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - y_graduation_offset * (y_graduation_num) - first_y_graduation_offset - red_line_y_offset / 2), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - y_graduation_offset * (y_graduation_num) - first_y_graduation_offset - red_line_y_offset / 2), GraphicColor::RED);
	//绘制上方内侧红线
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - y_graduation_offset * (y_graduation_num) - first_y_graduation_offset + red_line_y_offset), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - y_graduation_offset * (y_graduation_num) - first_y_graduation_offset + red_line_y_offset), GraphicColor::RED);

	//绘制上方外侧红线
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - m_y_graduation_offset * (m_y_graduation_num) - m_y_graduation_offset - red_line_y_offset / 2), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - m_y_graduation_offset * (m_y_graduation_num)-m_y_graduation_offset - red_line_y_offset / 2), GraphicColor::RED);
	//绘制上方内侧红线
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - m_y_graduation_offset * (m_y_graduation_num) - m_y_graduation_offset + red_line_y_offset), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - m_y_graduation_offset * (m_y_graduation_num)-m_y_graduation_offset + red_line_y_offset), GraphicColor::RED);

	 
	//绘制上方外侧红线
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - m_y_graduation_offset * (m_y_graduation_num) - m_y_graduation_offset), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - m_y_graduation_offset * (m_y_graduation_num)-m_y_graduation_offset), GraphicColor::RED);
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, y_scale_of_y_end + (m_y_graduation_offset * temp_red_line_scale) - red_line_y_offset / 2), GetRealPoint(m_rectCtrl, right_y_end_x_scale, y_scale_of_y_end + (m_y_graduation_offset * temp_red_line_scale) - red_line_y_offset / 2), GraphicColor::RED);
	//绘制上方内侧红线
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - m_y_graduation_offset * (m_y_graduation_num)-m_y_graduation_offset + red_line_y_offset), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - m_y_graduation_offset * (m_y_graduation_num)-m_y_graduation_offset + red_line_y_offset), GraphicColor::RED);
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, y_scale_of_y_end + (m_y_graduation_offset * temp_red_line_scale) + red_line_y_offset), GetRealPoint(m_rectCtrl, right_y_end_x_scale, y_scale_of_y_end + (m_y_graduation_offset * temp_red_line_scale) + red_line_y_offset), GraphicColor::RED);






	/////////////////////////////////下方/////////////////////////////////////////




	//绘制下方内侧红线
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - 2 * y_graduation_offset - red_line_y_offset), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - 2 * y_graduation_offset - red_line_y_offset), GraphicColor::RED);
	//绘制下方外侧红线
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - 2 * y_graduation_offset + red_line_y_offset / 2), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - 2 * y_graduation_offset + red_line_y_offset / 2), GraphicColor::RED);
	


	//绘制下方内侧红线
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - m_y_graduation_offset - red_line_y_offset), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - m_y_graduation_offset - red_line_y_offset), GraphicColor::RED);
	//绘制下方外侧红线
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - m_y_graduation_offset + red_line_y_offset / 2), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - m_y_graduation_offset + red_line_y_offset / 2), GraphicColor::RED);



	//绘制下方内侧红线
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - (m_y_graduation_offset * temp_red_line_scale) - red_line_y_offset), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - (m_y_graduation_offset * temp_red_line_scale) - red_line_y_offset), GraphicColor::RED);
	//绘制下方外侧红线
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - m_y_graduation_offset), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - m_y_graduation_offset), GraphicColor::RED);
	//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - (m_y_graduation_offset * temp_red_line_scale) + red_line_y_offset / 2), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - (m_y_graduation_offset * temp_red_line_scale) + red_line_y_offset / 2), GraphicColor::RED);

}






/*********************************************************
函数名   :	DrawGraphicText
功能     :	画曲线上文本内容
参数     :	[IN] :	p_dc CDC指针
					p_show_text 要显示的文本引用
					p_start_x 起始坐标
					p_start_y 结束坐标
					p_font_type 显示的字体
					p_font_size 显示的字体大小
					p_color 显示的颜色，默认为白色
		   [OUT] :	无
返回值   : 无
备注     : 
时间     : 2016/8/8 陈登龙
**********************************************************/
void CGraphic::DrawGraphicText(CDC* p_dc, const CString& p_show_text, int p_start_x, int p_start_y, CString p_font_type, int p_font_size, COLORREF p_color)
{

	CFont currentFont;  	

	currentFont.CreatePointFont(p_font_size, p_font_type, nullptr);

	CFont* pOldFont = p_dc->SelectObject(&currentFont);
 

	p_dc->SetTextColor(p_color); 

	//画出文本
	p_dc->TextOut(p_start_x, p_start_y, p_show_text);

	p_dc->SelectObject(pOldFont);
}




/********************************************************************
函数名   :	DrawDynamicCurve
功能     :	画动态的曲线
参数     :	[IN] :	MemDC 内存DC指针
					Pen 画笔的引用
					DisplayData 要显示的曲线数据队列引用
			[OUT]	: 无
返回值   : 无
备注     : 超过数据范围的点也要显示成红色，而不是显示在告警线位置
时间     : 2016/8/18 陈登龙
*********************************************************************/
void CGraphic::DrawDynamicCurve(CMemoryDC* MemDC, Graphic::Chart_Data_Queue &DisplayData)
{
	using namespace GraphicCoordinate;
	//进入临界区
	EnterCriticalSection(&g_csA);
 
	//更新当前一包曲线数据队列中的数据时，应保证数据中的坐标都已经确定
	if (DisplayData.size() >= 2)
	{
		POINT temp_start_point;
		POINT temp_end_point;

		POINT temp_cricle_start_point;
		POINT temp_cricle_end_point;

		for (int PointIndex = 1; PointIndex != DisplayData.size(); PointIndex++)
		{
			//初始化起点坐标-正常范围
			temp_start_point.x = DisplayData[PointIndex - 1].first.xPos;
			temp_start_point.y = DisplayData[PointIndex - 1].first.yPos; 

			//初始化终点坐标-正常范围
			temp_end_point.x = DisplayData[PointIndex].first.xPos;
			temp_end_point.y = DisplayData[PointIndex].first.yPos;
			
			
			//前一个点的数据超出Y轴Max范围显示在Y轴上限位置
			if (DisplayData[PointIndex - 1].second.base_data.arrive_value >  m_y_max)
			{ 
				//上限y比例--内侧红线
				//temp_start_point.y = (ordinary_y_scale - ((m_value_max - m_y_min) / (m_y_max - m_y_min)) * (ordinary_y_scale - (y_scale_of_y_end_top_line + y_graduation_offset)) + red_line_y_offset) * m_rectCtrl.Height();

				//上限y比例--外侧红线
				//temp_start_point.y = (ordinary_y_scale - ((m_value_max - m_y_min) / (m_y_max - m_y_min)) * (ordinary_y_scale - (y_scale_of_y_end_top_line + y_graduation_offset)) - red_line_y_offset / 2) * m_rectCtrl.Height();

				temp_start_point.y = (ordinary_y_scale - y_graduation_offset * (GraphicCoordinate::y_graduation_num - 1)) * m_rectCtrl.Height();

				//使得鼠标能够捕捉到超过Y轴max范围的点
				DisplayData[PointIndex - 1].first.yPos = temp_start_point.y;
 			}

			//后一个点的数据超出Y轴Max范围显示在Y轴上限位置
			if (DisplayData[PointIndex].second.base_data.arrive_value > m_y_max)
			{ 
				//上限y比例--内侧红线
				//temp_end_point.y = (ordinary_y_scale - ((m_value_max - m_y_min) / (m_y_max - m_y_min)) * (ordinary_y_scale - (y_scale_of_y_end_top_line + y_graduation_offset)) + red_line_y_offset) * m_rectCtrl.Height();

				//上限y比例--外侧红线
				//temp_end_point.y = (ordinary_y_scale - ((m_value_max - m_y_min) / (m_y_max - m_y_min)) * (ordinary_y_scale - (y_scale_of_y_end_top_line + y_graduation_offset)) - red_line_y_offset / 2) * m_rectCtrl.Height();

				temp_end_point.y = (ordinary_y_scale - y_graduation_offset * (GraphicCoordinate::y_graduation_num - 1)) * m_rectCtrl.Height();

				//使得鼠标能够捕捉到超过Y轴max范围的点
				DisplayData[PointIndex].first.yPos = temp_end_point.y;

			}


			//前一个点的数据低于Y轴min范围显示在Y轴下限位置
			if (DisplayData[PointIndex - 1].second.base_data.arrive_value < m_y_min)
			{
				//下限y比例-内侧红线  
				//temp_start_point.y = (ordinary_y_scale - ((m_value_min - m_y_min) / (m_y_max - m_y_min)) * (ordinary_y_scale - (y_scale_of_y_end_top_line + y_graduation_offset)) - red_line_y_offset) * m_rectCtrl.Height();

				//下限y比例-外侧红线
				//temp_start_point.y = (ordinary_y_scale - ((m_value_min - m_y_min) / (m_y_max - m_y_min)) * (ordinary_y_scale - (y_scale_of_y_end_top_line + y_graduation_offset)) + red_line_y_offset / 2) * m_rectCtrl.Height();

				temp_start_point.y = ordinary_y_scale * m_rectCtrl.Height();

				//使得鼠标能够捕捉到小于Y轴min范围的点
				DisplayData[PointIndex - 1].first.yPos = temp_start_point.y;
			} 

			//后一个点的数据低于Y轴min范围显示在Y轴下限位置
			if (DisplayData[PointIndex].second.base_data.arrive_value < m_y_min)
			{ 
				//下限y比例-内侧红线 
				//temp_end_point.y = (ordinary_y_scale - ((m_value_min - m_y_min) / (m_y_max - m_y_min)) * (ordinary_y_scale - (y_scale_of_y_end_top_line + y_graduation_offset)) - red_line_y_offset) * m_rectCtrl.Height();

				//下限y比例-外侧红线
				//temp_end_point.y = (ordinary_y_scale - ((m_value_min - m_y_min) / (m_y_max - m_y_min)) * (ordinary_y_scale - (y_scale_of_y_end_top_line + y_graduation_offset)) + red_line_y_offset / 2) * m_rectCtrl.Height();

				temp_end_point.y = ordinary_y_scale * m_rectCtrl.Height();

				//使得鼠标能够捕捉到小于Y轴min范围的点
				DisplayData[PointIndex].first.yPos = temp_end_point.y;

			}
			
			
			//画一条白色折线，默认为白色
			DrawLine(MemDC, temp_start_point, temp_end_point); 


			//画指定颜色起点
			temp_cricle_start_point.x = temp_start_point.x - point_radius;
			temp_cricle_start_point.y = temp_start_point.y - point_radius;

			temp_cricle_end_point.x = temp_start_point.x + point_radius;
			temp_cricle_end_point.y = temp_start_point.y + point_radius;

			
			if (GraphicColor::GREEN == DisplayData[PointIndex - 1].second.attributes.color)
			{  
				DrawCricle(MemDC, temp_cricle_start_point, temp_cricle_end_point, GraphicColor::GREEN);
			}
			else
			{
				DrawCricle(MemDC, temp_cricle_start_point, temp_cricle_end_point, GraphicColor::RED);
			}
			


			//画指定颜色终点
			temp_cricle_start_point.x = temp_end_point.x - GraphicCoordinate::point_radius;
			temp_cricle_start_point.y = temp_end_point.y - GraphicCoordinate::point_radius;

			temp_cricle_end_point.x = temp_end_point.x + GraphicCoordinate::point_radius;
			temp_cricle_end_point.y = temp_end_point.y + GraphicCoordinate::point_radius;

			
			if (GraphicColor::GREEN == DisplayData[PointIndex].second.attributes.color)
			{
				DrawCricle(MemDC, temp_cricle_start_point, temp_cricle_end_point, GraphicColor::GREEN);
			}
			else
			{
				DrawCricle(MemDC, temp_cricle_start_point, temp_cricle_end_point, GraphicColor::RED);
			} 
			
 		}  
	}

	//离开临界区
	LeaveCriticalSection(&g_csA); 

}
 

void CGraphic::SetCurrentData_One_Package(Graphic::Chart_Data& chart_data)
{  
	//如果收到的一个没有初始化的数据点就直接返回
	if (!chart_data.second.attributes.hasInit)
	{
		return;
	} 


	//如果当前曲线队列已经装满了Graphic::Deque_Point_Num个数据
	if (m_ChartDataDeque.size() >= Graphic::Deque_Point_Num)
	{
		 
		m_ChartDataDeque.pop_front(); 
		//在曲线数据队列后端添加新的一个新的曲线数据
		m_ChartDataDeque.push_back(chart_data);   
	}
	else
	{  
		m_ChartDataDeque.push_back(chart_data); 
	}  

	//每次更新都需调整曲线队列中每个元素的位置
	AdjustQueuePoint();

	//200ms重绘一次曲线
	ReconstructControl();
}



void CGraphic::SetCurrentData_One_Deque(const Graphic::Chart_Data_Queue& chart_data_deque)
{
	//接收新的一包数据，应该需要其他的判断处理
	m_ChartDataDeque = chart_data_deque;

	//重绘曲线
	ReconstructControl();

}


/*********************************************************
函数名   : GetRealPoint
功能     : 根据对话框大小和相应的坐标比例获取实际的坐标位置
参数     :	[IN]	:	rect	总的长度和宽度
						x_scale	x轴的比例
						y_scale	y轴的比例 
			[OUT]	: 无
返回值   :	转换后的实际坐标点
备注     :  
时间     : 2016/8/7 陈登龙
**********************************************************/
inline POINT CGraphic::GetRealPoint(CRect rect, double x_scale, double y_scale)
{ 
	POINT temp;
	temp.x = x_scale * rect.Width();
	temp.y = y_scale * rect.Height();
	return temp; 
}



/*******************************************************************
函数名   : DrawLine
功能     : 画线函数
参数     :	[IN]	:	p_dc	DC指针
						p_start	起始点坐标
						p_end	终止点坐标
						p_color	线的颜色，默认为白色
						p_line_type		线的类型， 默认为实线
						p_line_width	线的宽度， 默认为1
			[OUT]	: 无
返回值   : 无
备注     :  
时间     : 2016/8/7 陈登龙
*******************************************************************/
void CGraphic::DrawLine(CDC* p_dc, POINT p_start, POINT p_end, COLORREF p_color, int p_line_type, int p_line_width)
{
	//点为窗口值
	if (!p_dc || !p_dc->GetSafeHdc())
	{
		return;
	}

	//选择画笔
	CPen pen(p_line_type, p_line_width, p_color);
	CPen* p_OldPen = p_dc->SelectObject(&pen);

	//开始画线
	p_dc->MoveTo(p_start);
	p_dc->LineTo(p_end);

	//恢复之前的画笔
	p_dc->SelectObject(p_OldPen);
}



void CGraphic::DrawPoint(CDC* p_dc, POINT point, COLORREF p_color, int p_line_width, int p_line_type)
{
	//点为窗口值
	if (!p_dc || !p_dc->GetSafeHdc())
	{
		return;
	}

	//选择画笔
	CPen pen(p_line_type, p_line_width, p_color);

	CPen* p_OldPen = p_dc->SelectObject(&pen);

	p_dc->SetPixel(point, p_color);

	//恢复之前的画笔
	p_dc->SelectObject(p_OldPen);
}


void CGraphic::DrawCricle(CDC* p_dc, POINT point_up_left, POINT point_low_right, COLORREF p_color, int p_line_width, int p_line_type)
{
	//点为窗口值
	if (!p_dc || !p_dc->GetSafeHdc())
	{
		return;
	}

	//选择画笔
	CPen pen(p_line_type, p_line_width, p_color);

	CPen* p_OldPen = p_dc->SelectObject(&pen);

	//选择指定颜色的画刷
	CBrush brush(p_color);
	CBrush* p_OldBrush = p_dc->SelectObject(&brush);

	//画圆
	p_dc->Ellipse(CRect(point_up_left, point_low_right));

	//恢复之前的画笔
	p_dc->SelectObject(p_OldPen);
	p_dc->SelectObject(p_OldBrush);
}


void CGraphic::AddCoordinatePointForChartData(Graphic::Chart_Data& chart_data)
{
	//获得控件区域
	GetClientRect(&m_rectCtrl);  
	//获取到达的值
	//double arrive_value = chart_data.second.base_data.arrive_value; 


	//获取到达数据的x坐标比例，先到达的数据在右边
	static int index = m_x_scale_vec.size() - 1;
	double arrive_x_scale = 0.0;
	if (index >= 0)
	{
		 arrive_x_scale = m_x_scale_vec.at(index--);
	}  


	//获取到达数据的y的坐标比例   
	double arrive_y_scale = GetArriveYScale(chart_data);

	//得到实际的坐标
 	POINT arrive_point = GetRealPoint(m_rectCtrl, arrive_x_scale, arrive_y_scale);
 	 
 	//放入实际坐标到到来的数据包中
 	chart_data.first.xPos = arrive_point.x;
 	chart_data.first.yPos = arrive_point.y;
}




/************************************************************************************************************
函数名   :	DrawDynamicYScale
功能     :	画动态Y轴刻度
参数     :	[IN]	: dc指针
					: y轴最小值
					: y轴最大值
			[OUT]	: 无
返回值   : 无
备注     : Y轴刻度比例(0.9 - 0.05) / 23保持不变，刻度数量25保持不变，共24 - 1 = 23个刻度间隔，刻度值可以通过参数变化
时间     : 2016/8/18 陈登龙
*************************************************************************************************************/
void CGraphic::DrawDynamicYScale(CDC* p_dc, const double p_y_min, const double p_y_max)
{
	using namespace GraphicCoordinate;

	//获得控件区域
	GetClientRect(&m_rectCtrl);

	//根据刻度值，计算每个刻度间隔代表的大小
	double temp_scale = (p_y_max - p_y_min) / (GraphicCoordinate::y_graduation_num - 1);

	CString show_str("0.0");
	POINT left_y_point;
	POINT right_y_point;
	 
	double temp_y_scale = 0.0;

	//绘制左右Y轴的24个刻度线
	for (int i = 0; i < GraphicCoordinate::y_graduation_num; i++)
	{
		//计算每个刻度线的Y轴坐标
		temp_y_scale = ordinary_y_scale - y_graduation_offset * i;// -y_graduation_offset;


		//画左边刻度线
		DrawLine(p_dc,
			GetRealPoint(m_rectCtrl, ordinary_x_scale, temp_y_scale),
			GetRealPoint(m_rectCtrl, ordinary_x_scale + y_graduation_length_scale, temp_y_scale));

		//画右边刻度线
		DrawLine(p_dc,
			GetRealPoint(m_rectCtrl, right_y_end_x_scale, temp_y_scale),
			GetRealPoint(m_rectCtrl, right_y_end_x_scale - y_graduation_length_scale, temp_y_scale));



		//左边刻度值的坐标
// 		left_y_point = GetRealPoint(m_rectCtrl,
// 			ordinary_x_scale - 0.02,
// 			ordinary_y_scale - y_graduation_offset * i - 0.03);

		left_y_point = GetRealPoint(m_rectCtrl,
			ordinary_x_scale,
			temp_y_scale);



		//右边刻度值的坐标
// 		right_y_point = GetRealPoint(m_rectCtrl,
// 			x_scale_of_x_end,
// 			ordinary_y_scale - y_graduation_offset * i - 0.03); 

		right_y_point = GetRealPoint(m_rectCtrl,
			x_scale_of_x_end,
			temp_y_scale);


		//计算出刻度值
		show_str.Format(_T("%5.2f"), p_y_min + i * temp_scale); 

		//画左边Y轴刻度值
		//DrawGraphicText(p_dc, show_str, left_y_point.x - 24, left_y_point.y + 5, font_type, y_scale_font_size, GraphicColor::WHITE);
		DrawGraphicText(p_dc, show_str, left_y_point.x - 42, left_y_point.y - 5, font_type, y_scale_font_size, GraphicColor::WHITE);

		if (0 == i)
		{
			//右边0刻度不画了，会挡住x箭头和单位
			continue;
		}
		//画右边Y轴刻度值
		//DrawGraphicText(p_dc, show_str, right_y_point.x - 13, right_y_point.y + 5, font_type, y_scale_font_size, GraphicColor::WHITE);
		DrawGraphicText(p_dc, show_str, right_y_point.x - 13, right_y_point.y - 5, font_type, y_scale_font_size, GraphicColor::WHITE);


	}





	//绘制上方外侧红线，红线的Y轴比例，应该是Max对应的范围Y轴比例，获取MaxY轴比例
	double top_red_line_scale = ordinary_y_scale - ((m_value_max - p_y_min) / (p_y_max - p_y_min)) * (ordinary_y_scale - (y_scale_of_y_end_top_line + y_graduation_offset));
	//绘制上方外侧红线
	//DrawLine(p_dc, GetRealPoint(m_rectCtrl, ordinary_x_scale, top_red_line_scale - red_line_y_offset / 2), GetRealPoint(m_rectCtrl, right_y_end_x_scale, top_red_line_scale - red_line_y_offset / 2), GraphicColor::RED);
	//绘制上方内侧红线
	//DrawLine(p_dc, GetRealPoint(m_rectCtrl, ordinary_x_scale, top_red_line_scale + red_line_y_offset), GetRealPoint(m_rectCtrl, right_y_end_x_scale, top_red_line_scale + red_line_y_offset), GraphicColor::RED);
	
	//只画一根上限Max红线
	DrawLine(p_dc, GetRealPoint(m_rectCtrl, ordinary_x_scale, top_red_line_scale), GetRealPoint(m_rectCtrl, right_y_end_x_scale, top_red_line_scale), GraphicColor::RED);

	//绘制下方外侧红线，红线的Y轴比例，应该是Min对应的范围Y轴比例，获取MinY轴比例
	double bot_red_line_scale = ordinary_y_scale - ((m_value_min - p_y_min) / (p_y_max - p_y_min)) * (ordinary_y_scale - (y_scale_of_y_end_top_line + y_graduation_offset));
	//绘制下方内侧红线
	//DrawLine(p_dc, GetRealPoint(m_rectCtrl, ordinary_x_scale, bot_red_line - red_line_y_offset), GetRealPoint(m_rectCtrl, right_y_end_x_scale, bot_red_line - red_line_y_offset), GraphicColor::RED);
	//绘制下方外侧红线
	//DrawLine(p_dc, GetRealPoint(m_rectCtrl, ordinary_x_scale, bot_red_line + red_line_y_offset / 2), GetRealPoint(m_rectCtrl, right_y_end_x_scale, bot_red_line + red_line_y_offset / 2), GraphicColor::RED);
	
	//只画一根下限Min红线
	DrawLine(p_dc, GetRealPoint(m_rectCtrl, ordinary_x_scale, bot_red_line_scale), GetRealPoint(m_rectCtrl, right_y_end_x_scale, bot_red_line_scale), GraphicColor::RED);


}








 






// 
// void CGraphic::DrawDynamicYScale(CDC* p_dc, const double p_y_min, const double p_y_max)
// {
// 	using namespace GraphicCoordinate;
// 
// 	//获得控件区域
// 	GetClientRect(&m_rectCtrl); 
// 
// 	 
//	//double temp_scale = (p_y_max - p_y_min) / GraphicCoordinate::y_graduation_num;
//	double temp_scale = (p_y_max - p_y_min) / m_y_graduation_num;
// 	//double temp_scale = (p_max - p_min) / m_y_graduation_num;
// 	
// 	CString show_str("0.0");
// 	POINT left_y_point;
// 	POINT right_y_point;
// 	COLORREF draw_color;
// 
// 	//绘制左右Y轴刻度        16
////  	for (int i = 0; i < GraphicCoordinate::y_graduation_num + 1; i += 2)
////  	//for (int i = 0; i < m_y_graduation_num + 1; i += 2)
////  	{
////  		
////  		//if ((GraphicCoordinate::y_graduation_num - 1) == i) i++;
////  		 
////  		//计算刻度值坐标
////  // 		left_y_point = GetRealPoint(m_rectCtrl,
////  // 			ordinary_x_scale - 0.02,
////  // 			ordinary_y_scale - y_graduation_offset * i - 2 * y_graduation_offset - 0.03);
////  // 
////  // 		right_y_point = GetRealPoint(m_rectCtrl,
////  // 			x_scale_of_x_end,
////  // 			ordinary_y_scale - y_graduation_offset * i - 2 * y_graduation_offset - 0.03);
////  
////   
////  		left_y_point = GetRealPoint(m_rectCtrl,
////  			ordinary_x_scale - 0.02,
////  			ordinary_y_scale - y_graduation_offset * i - first_y_graduation_offset - 0.03);
////  
////  		right_y_point = GetRealPoint(m_rectCtrl,
////  			x_scale_of_x_end,
////  			ordinary_y_scale - y_graduation_offset * i - first_y_graduation_offset - 0.03);
////  	
////  		 
////  
////  		//计算出刻度值
////  		show_str.Format(_T("%5.2f"), p_y_min + i * temp_scale); 
////  
////  
////  		//min和max画成红色
////  		if ((0 == i) || ((GraphicCoordinate::y_graduation_num) == i))
////  		//if ((0 == i) || ((m_y_graduation_num) == i))
////  			draw_color = GraphicColor::RED;
////  		else 
////  			draw_color = GraphicColor::WHITE; 
////  
////  
////  		//画左边Y轴刻度值
////  		DrawGraphicText(p_dc, show_str, left_y_point.x - 24, left_y_point.y + 5,font_type, y_scale_font_size, draw_color);
////  
////  		//画右边Y轴刻度值
////  		DrawGraphicText(p_dc, show_str, right_y_point.x - 11, right_y_point.y + 5,font_type, y_scale_font_size, draw_color);
////  		 
////  		//保证max刻度值可以画到，否则14 + 2 = 16 不大于16，max刻度不会画出
////  		//if ((GraphicCoordinate::y_graduation_num - 2) == i)
////  		//	i++;
////  		//else 
////  		//	i += 2; 
////  	} 
//
//
//	//m_y_graduation_num = (p_y_max - p_y_min)
//
//	//默认红线位置
//	double bot_red_line_y_scale = ordinary_y_scale - m_y_graduation_offset;
//	double top_red_line_y_scale = ordinary_y_scale - m_y_graduation_offset * (m_y_graduation_num)-m_y_graduation_offset;
//
//
//	//ordinary_y_scale -m_y_graduation_offset * i +  - m_y_graduation_offset
//
//	double y_offset = 0.0;
//	//Y轴范围比数据范围大，刻度变多，刻度比例变小,红线内缩，bot_red_line_scale变大，top_red_line_scale变小
//	if ((p_y_max - p_y_min) / (m_value_max - m_value_min) > 1)
//	{
//		y_offset = y_graduation_offset - ((((p_y_max - p_y_min) - (m_value_max - m_value_min))/ (m_value_max - m_value_min))) * 0.1;// < 0.046
//		bot_red_line_y_scale += ((((p_y_max - p_y_min) - (m_value_max - m_value_min)) / (m_value_max - m_value_min)));
//		top_red_line_y_scale -= ((((p_y_max - p_y_min) - (m_value_max - m_value_min)) / (m_value_max - m_value_min)));
//	} 
//	//Y轴范围比数据范围小，刻度变少，刻度比例变大，红线外扩，bot_red_line_scale变小，top_red_line_scale变大
//	else if ((p_y_max - p_y_min) / (m_value_max - m_value_min) < 1)
//	{
//		//y_offset = y_graduation_offset + (((m_value_max - m_value_min) - (p_y_max - p_y_min)) / (p_y_max - p_y_min)) * 0.1;// < 0.046
//		y_offset = y_graduation_offset - (((m_value_max - m_value_min) - (p_y_max - p_y_min)) / (p_y_max - p_y_min)) * 0.1;// < 0.046
//
//		bot_red_line_y_scale += (((m_value_max - m_value_min) - (p_y_max - p_y_min)) / (p_y_max - p_y_min));
//		top_red_line_y_scale -= (((m_value_max - m_value_min) - (p_y_max - p_y_min)) / (p_y_max - p_y_min));
//
//	}
//	//相等默认刻度比例0.046
//	else
//	{
//		y_offset = y_graduation_offset;
//	}
//	 
//	SetCurrentYGraduationOffset(y_offset);
//
//	double temp_y_graduation_num = ((GraphicCoordinate::ordinary_y_scale - GraphicCoordinate::y_scale_of_y_end) / y_offset);//默认19个刻度
//
//	SetCurrentYGraduationNum(temp_y_graduation_num);
//
//	
//	double temp_y_scale = 0.0;
//
//	//绘制左右Y轴的16个刻度线
//	for (int i = 0; i < m_y_graduation_num - 2; i++) 
//	{ 
//		temp_y_scale = -m_y_graduation_offset * i + ordinary_y_scale - m_y_graduation_offset;
//		DrawLine(p_dc,
//			GetRealPoint(m_rectCtrl, ordinary_x_scale, temp_y_scale),
//			GetRealPoint(m_rectCtrl, ordinary_x_scale + y_graduation_length_scale, temp_y_scale));
//				 
//				 
//		DrawLine(p_dc,
//			GetRealPoint(m_rectCtrl, right_y_end_x_scale, temp_y_scale),
//			GetRealPoint(m_rectCtrl, right_y_end_x_scale - y_graduation_length_scale, temp_y_scale));
//		   
//
//
//
//		left_y_point = GetRealPoint(m_rectCtrl,
//		  	ordinary_x_scale - 0.02,
//			ordinary_y_scale - m_y_graduation_offset * i - m_y_graduation_offset - 0.03);
//		  
//		right_y_point = GetRealPoint(m_rectCtrl,
//		  	x_scale_of_x_end,
//			ordinary_y_scale - m_y_graduation_offset * i - m_y_graduation_offset - 0.03);
//		  	
//		  		 
//		  
//		//计算出刻度值
//		show_str.Format(_T("%5.2f"), p_y_min + i * temp_scale); 
//		  
//		  
//		//min和max画成红色
//		//if ((0 == i) || ((GraphicCoordinate::y_graduation_num) == i))
//		//if ((0 == i) || ((m_y_graduation_num) == i))
//		//  	draw_color = GraphicColor::RED;
//		//else 
//		  	draw_color = GraphicColor::WHITE; 
//		  
//		//画左边Y轴刻度值
//		DrawGraphicText(p_dc, show_str, left_y_point.x - 24, left_y_point.y + 5, font_type, y_scale_font_size, draw_color);
//		  
//		//画右边Y轴刻度值
//		DrawGraphicText(p_dc, show_str, right_y_point.x - 11, right_y_point.y + 5, font_type, y_scale_font_size, draw_color);
//		 
//
//
//
//
//		//判断条件画告警红色线
//		if (temp_y_scale == top_red_line_y_scale)
//		{
//			//绘制上方外侧红线
//			//DrawLine(p_dc, GetRealPoint(m_rectCtrl, ordinary_x_scale, top_red_line_y_scale - red_line_y_offset / 2), GetRealPoint(m_rectCtrl, right_y_end_x_scale, top_red_line_y_scale - red_line_y_offset / 2), GraphicColor::RED);
//			//绘制上方内侧红线
//			//DrawLine(p_dc, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - m_y_graduation_offset * (m_y_graduation_num)-m_y_graduation_offset + red_line_y_offset), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - m_y_graduation_offset * (m_y_graduation_num)-m_y_graduation_offset + red_line_y_offset), GraphicColor::RED);
//			 
//		}
//
//		//判断条件画告警红色线
//		if (temp_y_scale == bot_red_line_y_scale)
//		{
//
//			//绘制下方内侧红线
//			//DrawLine(p_dc, GetRealPoint(m_rectCtrl, ordinary_x_scale, bot_red_line_y_scale - red_line_y_offset), GetRealPoint(m_rectCtrl, right_y_end_x_scale, bot_red_line_y_scale - red_line_y_offset), GraphicColor::RED);
//			//绘制下方外侧红线
//			//DrawLine(CDC, GetRealPoint(m_rectCtrl, ordinary_x_scale, ordinary_y_scale - m_y_graduation_offset + red_line_y_offset / 2), GetRealPoint(m_rectCtrl, right_y_end_x_scale, ordinary_y_scale - m_y_graduation_offset + red_line_y_offset / 2), GraphicColor::RED);
//
//
//		}
//
//	}
// 
// 	 
// }

/*
void CGraphic::DrawDynamicYScale(CDC* p_dc, const double p_y_min, const double p_y_max)
{
	using namespace GraphicCoordinate;

	//获得控件区域
	GetClientRect(&m_rectCtrl); 
	 
	double temp_scale = (p_y_max - p_y_min) / m_y_graduation_num;
	 

	CString show_str("0.0");
	POINT left_y_point;
	POINT right_y_point;
	COLORREF draw_color;

	//绘制左右Y轴刻度 
	for (int i = 0; i < m_y_graduation_num; i += 2)
	{ 

		left_y_point = GetRealPoint(m_rectCtrl,
			ordinary_x_scale - 0.02,
			ordinary_y_scale - m_y_graduation_offset * i - m_y_graduation_offset - 0.03);

		right_y_point = GetRealPoint(m_rectCtrl,
			x_scale_of_x_end,
			ordinary_y_scale - m_y_graduation_offset * i - m_y_graduation_offset - 0.03);


		double show_value = p_y_min + i * temp_scale;


		//计算出刻度值
		show_str.Format(_T("%5.2f"), show_value);


		if (m_value_min == show_value || m_value_max == show_value)
		{
			draw_color = GraphicColor::RED;
		}
		else
		{
			draw_color = GraphicColor::WHITE;
		}


		//y轴min和max画成红色
		//if ((0 == i) || ((GraphicCoordinate::y_graduation_num) == i))
// 		if ((0 == i) || ((m_y_graduation_num) == i))
// 			draw_color = GraphicColor::RED;
// 		else
// 			draw_color = GraphicColor::WHITE;


		//数据范围min和max画成红色
		 

		//画左边Y轴刻度值
		DrawGraphicText(p_dc, show_str, left_y_point.x - 20, left_y_point.y + 5,font_type, y_scale_font_size, draw_color);

		//画右边Y轴刻度值
		DrawGraphicText(p_dc, show_str, right_y_point.x - 5, right_y_point.y + 5,font_type, y_scale_font_size, draw_color);
 
	} 



}

*/

/*******************************************************************************
函数名   :	AdjustQueuePoint
功能     :	重新调整队列中元素的坐标
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   : 无
备注     : 在队列未满和满时都能够进行调整，修复了前50个点不能调整Y坐标的问题
时间     : 2016/8/18 陈登龙
*******************************************************************************/
void CGraphic::AdjustQueuePoint()
{
	//获得控件区域
	GetClientRect(&m_rectCtrl);
	POINT temp_adjust_point;
	//x坐标通过m_x_scale_vec.at(index)取得
	double arrive_y_scale(0.0);
	double arrive_x_scale(0.0);
	//当前曲线数据队列大小
	auto queue_size = m_ChartDataDeque.size();

	//队列满时进行调整
	if (queue_size >= Graphic::Deque_Point_Num)
	{  
		for (int index = 0; index != queue_size; index++)
		{
			//双端队列在随机访问时，速度不是很快，这里用临时变量存储index处的值，防止后面重复使用m_ChartDataDeque[index]导致效率低下
			auto& temp_queue_point = m_ChartDataDeque[index];

			//获取每个数据的x的坐标比例   
			arrive_x_scale = m_x_scale_vec.at(index);

			//获取每个数据的y的坐标比例
			arrive_y_scale = GetArriveYScale(temp_queue_point); 

			//得到实际的坐标
			temp_adjust_point = GetRealPoint(m_rectCtrl, arrive_x_scale, arrive_y_scale);

			//赋值坐标
			temp_queue_point.first.xPos = temp_adjust_point.x;
			temp_queue_point.first.yPos = temp_adjust_point.y;
		}
	}
	else 
	{ 
		//拿出最后一个元素点
		auto& temp_queue_point = m_ChartDataDeque.back();

		//获取数据的x的坐标比例    
		arrive_x_scale = GraphicCoordinate::right_y_start_x_scale - GraphicCoordinate::y_graduation_length_scale;

		//获取数据的y的坐标比例   
		arrive_y_scale = GetArriveYScale(temp_queue_point);

		//得到实际的坐标
		temp_adjust_point = GetRealPoint(m_rectCtrl, arrive_x_scale, arrive_y_scale);

		//赋值坐标
		temp_queue_point.first.xPos = temp_adjust_point.x;
		temp_queue_point.first.yPos = temp_adjust_point.y;


		//调整前面的每个元素的xPos，均向前移动两个点之间的间隔宽度
		for (int index = 0; index < queue_size - 1; index++)
		{
			//0 ... 46 47 48 49 end(50)
			//queue_size = 3                                      m_x_scale_vec.size() = 50 - 3 = 47 + index = 47 48 49
			m_ChartDataDeque[index].first.xPos = m_x_scale_vec.at(m_x_scale_vec.size() - queue_size + index) * m_rectCtrl.Width();

			//重新调整队列中的数据点的Y坐标,不能忘记乘以高度，GetArriveYScale返回比例
			m_ChartDataDeque[index].first.yPos = GetArriveYScale(m_ChartDataDeque[index]) * m_rectCtrl.Height();
		}
 
		//for (int index = 0; index != queue_size - 1; index++)
		//{  
		//将前面的元素点的xPos向前移动0.0174(横坐标两个点之间的比例)个比例,可以使用，但是第一个位置需要处理 
		//	m_ChartDataDeque[index].first.xPos -= m_rectCtrl.Width() * 0.0174;  
		//}
		 
	}
}





 

/*******************************************************************************
函数名   :	GetArriveXScale
功能     :	得到到达数据点的y坐标比例
参数     :	[IN]	: 无
			[OUT]	: arrive_data 到达数据点的引用
返回值   :	返回arrive_data实际的y的比例
备注     :	 
			 
时间     :	2016/8/14 陈登龙
********************************************************************************/
inline double CGraphic::GetArriveYScale(Graphic::Chart_Data& arrive_data)
{
// 	double temp = (GraphicCoordinate::ordinary_y_scale - GraphicCoordinate::y_graduation_offset) - (GraphicCoordinate::y_graduation_offset * GraphicCoordinate::y_graduation_num);
// 	return ((arrive_data.second.base_data.arrive_value - arrive_data.second.attributes.max) / (arrive_data.second.attributes.min - arrive_data.second.attributes.max)) * GraphicCoordinate::y_graduation_offset * GraphicCoordinate::y_graduation_num + temp;


// 	double temp = (GraphicCoordinate::ordinary_y_scale - m_y_graduation_offset) - (m_y_graduation_offset * m_y_graduation_num);
// 	return ((arrive_data.second.base_data.arrive_value - arrive_data.second.attributes.max) / (arrive_data.second.attributes.min - arrive_data.second.attributes.max)) * m_y_graduation_offset * m_y_graduation_num + temp;

// 	double temp = GraphicCoordinate::ordinary_y_scale - (m_y_graduation_offset * (m_y_graduation_num - 1));
// 	return ((arrive_data.second.base_data.arrive_value - arrive_data.second.attributes.max) / (arrive_data.second.attributes.min - arrive_data.second.attributes.max)) * m_y_graduation_offset * (m_y_graduation_num - 1) + temp;

	double temp = GraphicCoordinate::ordinary_y_scale - (m_y_graduation_offset * (m_y_graduation_num - 1));
	return ((arrive_data.second.base_data.arrive_value - m_y_max) / (m_y_min - m_y_max)) * m_y_graduation_offset * (m_y_graduation_num - 1) + temp;

}


BOOL CGraphic::PreTranslateMessage(MSG* pMsg)
{

	// TODO:  在此添加专用代码和/或调用基类

	CString temp_text("");

	//暂时去掉
	//GetParent()->GetDlgItem(IDC_BTN_PAUSE)->GetWindowText(temp_text);

	//回放模式并且当前暂停才可以响应鼠标移动消息
	//if ((m_hWnd == pMsg->hwnd) && (pMsg->message == WM_MOUSEMOVE) && (Graphic::PlaybackModel == m_cur_model) && (_T("Resume") == m_cur_btn_pause_text))
	if ((m_hWnd == pMsg->hwnd) && (pMsg->message == WM_MOUSEMOVE) && (Graphic::PlaybackModel == m_cur_model) && (_T("Resume") == temp_text))
	{
		OnMouseMove(MK_MBUTTON, pMsg->pt);
		return TRUE;
	}
	return CWnd::PreTranslateMessage(pMsg);
}


void CGraphic::OnMouseMove(UINT nFlags, CPoint point)
{
	//CRect rect_ctr;
	//GetClientRect(&rect_ctr);

	CRect rect_ctr;
	//获取Picture控件相对屏幕左上角的坐标，存储到rect_ctr中
	this->GetWindowRect(&rect_ctr);

	//ScreenToClient(rect_ctr);

	point.x -= rect_ctr.left;
	point.y -= rect_ctr.top;

	// TODO:  在此添加消息处理程序代码和/或调用默认值 
	for (int index = 0; index != m_ChartDataDeque.size(); index++)
	{
		if (((point.x > m_ChartDataDeque[index].first.xPos - GraphicCoordinate::point_radius) && (point.x < m_ChartDataDeque[index].first.xPos + GraphicCoordinate::point_radius))
			&& ((point.y > m_ChartDataDeque[index].first.yPos - GraphicCoordinate::point_radius) && (point.y < m_ChartDataDeque[index].first.yPos + GraphicCoordinate::point_radius)))
		{
			//MessageBox(_T("OK!"));

			CString temp_text("");

			//temp_text.Format(_T("%.2f"), m_ChartDataDeque[index].second.base_data.arrive_time);
			
			auto arrive_time = m_ChartDataDeque[index].second.base_data.arrive_time;
			temp_text.Format(_T("%2u:%2u:%2u:%3u"), arrive_time.hour, arrive_time.min, arrive_time.sec, arrive_time.ms);
			
			//暂时去掉
			//GetParent()->GetDlgItem(IDC_EDIT_ARRIVE_TIME)->SetWindowText(temp_text);


			temp_text.Format(_T("%.2f"), m_ChartDataDeque[index].second.base_data.arrive_value);
			
			//暂时去掉
			//GetParent()->GetDlgItem(IDC_EDIT_ARRIVE_VALUE)->SetWindowText(temp_text);
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}


/*********************************************************
函数名   :	GetArriveXScale
功能     :	得到到达数据点的x坐标比例
参数     :	[IN]	: 无
[OUT]	: arrive_data 到达数据点的引用
返回值   :	返回arrive_data的实际x比例
备注     :	此函数目前不使用
时间     :	2016/8/10 陈登龙
**********************************************************/
inline double CGraphic::GetArriveXScale(Graphic::Chart_Data& arrive_data)
{
	// 	static int index = m_x_scale_vec.size() - 1;
	// 	double arrive_x_scale = 0.0;
	// 	if (index >= 0)
	// 	{
	// 		return arrive_x_scale = m_x_scale_vec.at(index--);
	// 	}  

	//之前的数据xPos位置向前移动


	//新来的数据在最后0.92
	return 0.93 - GraphicCoordinate::y_graduation_length_scale;
}
 