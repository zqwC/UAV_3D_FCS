#include "stdafx.h"
#include "UavView.h"


CUavView::CUavView()
{
	memset(&m_TSPIDC, 0, sizeof(m_TSPIDC));
}


CUavView::~CUavView()
{
}


BEGIN_MESSAGE_MAP(CUavView, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CUavView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()

	//将对话框中静态文本框控件的大小坐标赋给m_rectCtrl
	GetClientRect(&m_rectCtrl);

	//memDC为一虚拟的dc，在CMemoryDC类的构造函数中将实际的dc
	CMemoryDC memDC(&dc, &m_rectCtrl);

	if (m_reqRedrawStaticBgd)
	{
		//在缓冲区内绘制HUD静态图形
		if ((m_dcPlate == nullptr) || (m_bitmapPlate.m_hObject == nullptr))
		{
			//创建一个和实际dc匹配的虚拟设备描述表(m_dcHudPlate)
			m_dcPlate.CreateCompatibleDC(&dc);

			DrawStaticPrepare(&dc);
		}
		else
		{
			m_dcPlate.DeleteDC();
			//创建一个和实际dc匹配的虚拟设备描述表(m_dcHudPlate)
			m_dcPlate.CreateCompatibleDC(&dc);
			m_bitmapPlate.DeleteObject();

			DrawStaticPrepare(&dc);
		}

		//在位图m_bitmapHUDPlate中利用虚拟dc(m_dcHUDPlate)绘制HUD静态背景
		DrawStaticBackground(&m_dcPlate);

		//只需要绘制一次静态背景，如果拖出界面不能重新显示静态背景，就去掉这个标记
		m_reqRedrawStaticBgd = false;
	}

	//动态背景需要重复绘制，才能实现动画效果
	DrawDynamicBackground(&m_dcPlate);

	//将缓冲区内绘制完成的位图一次性拷贝到显示屏中显示
	memDC.BitBlt(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(), &m_dcPlate, 0, 0, SRCCOPY);

	
}



/*! @function
********************************************************************************
* 函数名   : SetCurrentData
* 功能     : 留给HUD控制器的接口，用来设置HUD控件的数据
* 参数     : tspidc 从HUDViewControl发送来的数据包
* 返回值   : void
* 作者     : 陈登龙 南昌航空大学信息工程学院自动控制系
* 日期     : 2017-03-04
*******************************************************************************/
void CUavView::SetCurrentData(const TSPIDC& tspidc)
{
	//数据更新赋值
	m_TSPIDC = tspidc;
	//重绘
	Invalidate();
}



/*! @function
********************************************************************************
* 函数名   : DrawStaticPrepare
* 功能     : 绘图函数
* 参数     : void
* 返回值   : void
* 备注     : 绘制控件的静态部分
*******************************************************************************/
void CUavView::DrawStaticPrepare(CDC* pDC)
{
	//创建一个和实际dc、实际矩形控件区域相互兼容的位图
	m_bitmapPlate.CreateCompatibleBitmap(pDC, m_rectCtrl.Width(), m_rectCtrl.Height());

	//利用m_dcHUDPlate将位图选中
	m_pbitmapOldPlate = m_dcPlate.SelectObject(&m_bitmapPlate);

	//先用一种颜色作为内存显示设备的背景色
	m_dcPlate.FillSolidRect(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(), RGB(0, 0, 0));
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
时间     : 2017/3/6 陈登龙
**********************************************************/
void CUavView::DrawUavText(CDC* p_dc, const CString& p_show_text, int p_start_x, int p_start_y, CString p_font_type, int p_font_size, COLORREF p_color)
{

	CFont currentFont;

	currentFont.CreatePointFont(p_font_size, p_font_type, nullptr);

	CFont* pOldFont = p_dc->SelectObject(&currentFont);

	p_dc->SetTextColor(p_color);

	//画出文本
	p_dc->TextOut(p_start_x, p_start_y, p_show_text);

	p_dc->SelectObject(pOldFont);
}


/*********************************************************
函数名   :	DrawGraphicText
功能     :	画曲线上文本内容
参数     :	[IN] :	p_dc CDC指针
			point_up_left 正方形左上角坐标
			point_low_right 正方形右下角坐标
			p_color 显示的颜色
			p_line_width 线宽
			p_line_type 线类型
[OUT] :	无
返回值   : 无
备注     :
时间     : 2017/3/9 陈登龙
**********************************************************/
void CUavView::DrawCricle(CDC* p_dc, POINT point_up_left, POINT point_low_right, COLORREF p_color, int p_line_width, int p_line_type)
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
	//CBrush brush(p_color);
	
	CBrush* pBrush = CBrush::FromHandle(static_cast<HBRUSH>(GetStockObject(NULL_BRUSH)));
	CBrush* p_OldBrush = p_dc->SelectObject(pBrush);
	//画圆
	p_dc->Ellipse(CRect(point_up_left, point_low_right));
	//p_dc->Ellipse(point_up_left.x, point_up_left.y, point_low_right.x, point_low_right.y);

	//恢复之前的画笔
	p_dc->SelectObject(p_OldPen);
	p_dc->SelectObject(p_OldBrush);
}