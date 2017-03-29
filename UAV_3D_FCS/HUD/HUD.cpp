#include "stdafx.h"
#include "../stdafx.h"
#include "HUD.h"

CHUD::CHUD()
{
	m_dRadius = 130;	// 圆弧半径
	
}


CHUD::~CHUD()
{
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
void CHUD::SetCurrentData(const TSPIDC& tspidc)
{ 
	// 数据更新赋值
	// double m_ANGP = 0.0;            //俯仰角  27、28 范围：(-180°,180°)
	// double m_ANGR = 0.0;            //横滚角  29、30 范围：(-180°,180°)
	//double m_ANGPS = 0.0;                  //俯仰角速率  31、32
	//double m_ANGRS = 0.0;                  //横滚角速率  33、34

	// 接收到的数据为角度制
	m_TSPIDC.m_ANGP = tspidc.m_ANGP;
	m_TSPIDC.m_ANGPS = tspidc.m_ANGPS;
	m_TSPIDC.m_ANGR = tspidc.m_ANGR;
	m_TSPIDC.m_ANGRS = tspidc.m_ANGRS;
	//重绘
	Invalidate();
}



BEGIN_MESSAGE_MAP(CHUD, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


/*! @function
********************************************************************************
* 函数名   : OnPaint
* 功能     : 绘图函数
* 参数     : void
* 返回值   : void 
* 备注     : 所有的绘图操作都应该在这个函数中完成，使用双缓存技术(MemoryDC.h)
			 防止屏幕闪烁,先绘制静态部分，再绘制动态部分。
*******************************************************************************/
void CHUD::OnPaint()
{
	CPaintDC dc(this);  

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
	DrawDynamicBackground(&memDC);	

	//将缓冲区内绘制完成的位图一次性拷贝到显示屏中显示
	//使用或操作进行颜色合并，避免覆盖动态背景
	memDC.BitBlt(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(), &m_dcPlate, 0, 0, SRCPAINT);

}


/*! @function
********************************************************************************
* 函数名   : DrawStaticPrepare
* 功能     : 绘图函数
* 参数     : void
* 返回值   : void
* 备注     : 绘制控件的静态部分
*******************************************************************************/
void CHUD::DrawStaticPrepare(CDC* p_dc)
{ 
	//创建一个和实际dc、实际矩形控件区域相互兼容的位图
	m_bitmapPlate.CreateCompatibleBitmap(p_dc, m_rectCtrl.Width(), m_rectCtrl.Height());

	//利用m_dcHUDPlate将位图选中
	m_pbitmapOldPlate = m_dcPlate.SelectObject(&m_bitmapPlate); 

	//先用一种颜色作为内存显示设备的背景色
	m_dcPlate.FillSolidRect(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(), RGB(0, 0, 0));
}

/*! @function
********************************************************************************
* 函数名   : DrawStaticBackground
* 功能     : 绘制HUD的静态背景
* 参数     : pDC 绘图指针
* 返回值   : void
* 备注     : 使用pDC指针来绘制
*******************************************************************************/
void CHUD::DrawStaticBackground(CDC *pDC)
{ 
	//!	在此处可设置控件中显示的大小和位置
	// 获取表盘中心点
	m_CenterPoint = m_rectCtrl.CenterPoint();
	// 设置显示的大小
	m_dRadius = 105;
}

/*! @function
********************************************************************************
* 函数名   : DrawDynamicBackground
* 功能     : 绘制HUD的动态背景
* 参数     : pDC 绘图指针
* 返回值   : void
* 备注     : 使用pDC指针来绘制
*******************************************************************************/
void CHUD::DrawDynamicBackground(CDC *pDC)
{
	//- [3/26/2017 张群伟]

	// 将地平仪以外的设置为黑色                 
	currentBrush.CreateSolidBrush(RGB(0, 0, 0));
	pDC->FillRect(&m_rectCtrl, &currentBrush);
	currentBrush.DeleteObject();
	// 绘制刻度和圆
	DrawTickCircle(pDC);
	// 绘制天空和大地
	DrawSkyGround(pDC);
	// 绘制两个三角形
	DrawTriangle(pDC);
	// 绘制飞机
	DrawAerofoil(pDC);
	// 绘制中央水平线
	DrawCenterHorizontal(pDC);

}





////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//-[3/26/2017 张群伟] 

/**************************************************************************************
 * 函数名	：DrawTickCircle
 * 功能		：绘制刻度和圆
 * 参数		：pDC 绘图指针
 * 返回值   : void
 * 说明  	：绘制地平仪横滚角刻度和圆,先画刻度再画圆效果更好
 * 作者		：张群伟
 * 日期		：[3/26/2017]
 **************************************************************************************/
inline void CHUD::DrawTickCircle(CDC *pDC)
{
	// 画圆弧上的刻度
	double dTickLength = 3.0 / 25.0*m_dRadius;	// 刻度的长度
	double dCurrentAngle = 0.0;					// 每条刻度线所对应的角度
	double dSinCurrentAngle = 0.0;				// 每条刻度线所对应的角度的正弦值
	double dCosCurrentAngle = 0.0;				// 每条刻度线所对应的角度的余弦值
	double powNegJ = 0.0;						// pow(-1, j)的值
	CPoint ptTickStart;							// 刻度的起点
	CPoint ptTickEnd;							// 刻度的终点

//- ON  动态刻度
//- OFF 静态刻度
#if ON
	for (int i = 0; i < 16; i++)
	{
		// 设置刻度的颜色
		switch (i)
		{
			// 0°刻度标识天空所在位置，设为天空的蓝色
		case 0:
			currentPen.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
			break;
			// +- 10°，+- 20°刻度设为绿色
		case 1:
		case 2:
		case 14:
		case 15:
			currentPen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
			break;
			// 180°刻度标识大地所在位置，设为黄色较明显
		case 8:
			//!currentPen.CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
			//!break;
			// +- 90°刻度为绿色
		case 5:
		case 11:
			currentPen.CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
			break;
			// 其他角度刻度设为红色
		default:
			currentPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			break;
		}
		// 刻度间距由小到大
		if (i < 3)			// 左边绿色
			dCurrentAngle = (i * 10 + m_TSPIDC.m_ANGR) * PI / 180;
		else if (i >= 14)	// 右边绿色
			dCurrentAngle = (i * 10 - 160 + m_TSPIDC.m_ANGR) * PI / 180;
		else
			dCurrentAngle = ((i - 2) * 30 + m_TSPIDC.m_ANGR)* PI / 180;

		// 计算刻度起点和终点坐标
		dSinCurrentAngle = sin(dCurrentAngle);
		dCosCurrentAngle = cos(dCurrentAngle);
		ptTickStart.x = (int)(m_CenterPoint.x - m_dRadius*dSinCurrentAngle);
		ptTickStart.y = (int)(m_CenterPoint.y - m_dRadius*dCosCurrentAngle);
		ptTickEnd.x = (int)(ptTickStart.x - dTickLength*dSinCurrentAngle);
		ptTickEnd.y = (int)(ptTickStart.y - dTickLength*dCosCurrentAngle);

		// 画刻度
		pDC->SelectObject(&currentPen);
		pDC->MoveTo(ptTickStart);
		pDC->LineTo(ptTickEnd);
		currentPen.DeleteObject();
	}
#else
	for (int j = 0; j < 2; j++)
	{
		powNegJ = pow(-1, j);
		for (int i = 0; i < 9; i++)
		{
			// 设置刻度的颜色
			switch (i)
			{
				// 0°蓝色刻度
			case 0:
				currentPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
				break;
				// 10°，20°绿色刻度
			case 1:
			case 2:
				currentPen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
				break;
				//其他角度为红色刻度
			default:
				currentPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
				break;
			}
			// 刻度间距由小到大
			if (i < 4)
				dCurrentAngle = i * 10 * PI / 180;
			else
				dCurrentAngle = (i - 2) * 30 * PI / 180;
			// 计算刻度起点和终点坐标
			dSinCurrentAngle = sin(dCurrentAngle);
			dCosCurrentAngle = cos(dCurrentAngle);
			ptTickStart.x = (int)(m_CenterPoint.x - powNegJ*m_dRadius*dSinCurrentAngle);
			ptTickStart.y = (int)(m_CenterPoint.y - m_dRadius*dCosCurrentAngle);
			ptTickEnd.x = (int)(ptTickStart.x - powNegJ*dTickLength*dSinCurrentAngle);
			ptTickEnd.y = (int)(ptTickStart.y - dTickLength*dCosCurrentAngle);
			// 画刻度
			pDC->SelectObject(&currentPen);
			pDC->MoveTo(ptTickStart);
			pDC->LineTo(ptTickEnd);
			currentPen.DeleteObject();
		}
	}
#endif
	// 画最外层大圆
	currentPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	pDC->SelectObject(&currentPen);
	CRect rectCenterArc(m_CenterPoint.x - 27.0 / 24.0*m_dRadius, m_CenterPoint.y - 27.0 / 24.0*m_dRadius,
		m_CenterPoint.x + 27.0 / 24.0*m_dRadius, m_CenterPoint.y + 27.0 / 24.0*m_dRadius);
	pDC->Arc(rectCenterArc,
		CPoint(m_CenterPoint.x + m_dRadius, m_CenterPoint.y),
		CPoint(m_CenterPoint.x + m_dRadius, m_CenterPoint.y));
	currentPen.DeleteObject();

	// 画内层小圆
	currentPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	pDC->SelectObject(&currentPen);
	rectCenterArc.DeflateRect(3.0 / 25.0*m_dRadius, 3.0 / 25.0*m_dRadius);
	pDC->Arc(rectCenterArc,
		CPoint(m_CenterPoint.x + m_dRadius, m_CenterPoint.y),
		CPoint(m_CenterPoint.x + m_dRadius, m_CenterPoint.y));
	currentPen.DeleteObject();
}

/**************************************************************************************
 * 函数名	：DrawSkyGround	
 * 功能		：绘制天空和大地
 * 参数		：pDC 绘图指针
 * 返回值   : void
 * 说明  	：绘制动态的天空和大地
 * 作者		：张群伟
 * 日期		：[3/26/2017]
 **************************************************************************************/
inline void CHUD::DrawSkyGround(CDC *pDC)
{
	// 裁减区，正方形，正方形内切圆 
	CRgn rgn, rgnRect, inCircle; 
	// 正方形
	CRect rect((int)(m_CenterPoint.x - m_dRadius), (int)(m_CenterPoint.y - m_dRadius),
		(int)(m_CenterPoint.x + m_dRadius), (int)(m_CenterPoint.y + m_dRadius));
	// 选择正方形矩形框
	rgnRect.CreateRectRgnIndirect(rect);
	// 选择正方形矩形框内切圆
	inCircle.CreateEllipticRgnIndirect(rect);
	// 裁减出内切圆部分
	rgn.CreateRectRgn(0, 0, 0, 1);
	rgn.CombineRgn(&rgnRect, &inCircle, RGN_AND);
	pDC->SelectClipRgn(&rgn);

	// 先用蓝色画刷填充整个内切圆
	currentBrush.CreateSolidBrush(RGB(0, 0, 255));
	pDC->SelectObject(&currentBrush);
	pDC->FillRgn(&rgn, &currentBrush);
	currentBrush.DeleteObject();

	// 然后用橙色画刷绘制地平仪的地面部分
	currentBrush.CreateSolidBrush(RGB(130, 80, 0));
	pDC->SelectObject(&currentBrush);

	CRgn rgnArc;		// 地面半圆形裁减区
	CPoint ptArcStart;	// 填充地面的圆弧起点
	CPoint ptArcEnd;	// 填充地面的圆弧终点

	// 起点加入横滚角分量
	ptArcStart.x = m_CenterPoint.x - m_dRadius*cos(m_TSPIDC.m_ANGR*PI / 180.0);
	ptArcStart.y = m_CenterPoint.y + m_dRadius*sin(m_TSPIDC.m_ANGR*PI / 180.0);
	// 起点加入俯仰角分量
	ptArcStart.x = ptArcStart.x + m_TSPIDC.m_ANGP/30.0*m_dRadius*sin(m_TSPIDC.m_ANGR*PI / 180.0);
	ptArcStart.y = ptArcStart.y + m_TSPIDC.m_ANGP / 30.0*m_dRadius*cos(m_TSPIDC.m_ANGR*PI / 180.0);
	// 终点加入横滚角分量
	ptArcEnd.x = m_CenterPoint.x + m_dRadius*cos(m_TSPIDC.m_ANGR*PI / 180.0);
	ptArcEnd.y = m_CenterPoint.y - m_dRadius*sin(m_TSPIDC.m_ANGR*PI / 180.0);
	// 终点加入俯仰角分量
	ptArcEnd.x = ptArcEnd.x + m_TSPIDC.m_ANGP / 30.0*m_dRadius*sin(m_TSPIDC.m_ANGR*PI / 180.0);
	ptArcEnd.y = ptArcEnd.y + m_TSPIDC.m_ANGP / 30.0*m_dRadius*cos(m_TSPIDC.m_ANGR*PI / 180.0);

	// 为裁减区打开一个路径层
	pDC->BeginPath();
	pDC->Arc(&rect, ptArcStart, ptArcEnd);
	// 关闭路径层
	pDC->EndPath();
	// 将路径转换为区域并填充该区域
	rgnArc.CreateFromPath(pDC);
	pDC->FillRgn(&rgnArc, &currentBrush);

	currentBrush.DeleteObject();
}

/**************************************************************************************
* 函数名	：DrawTriangle
* 功能		：绘制两个三角形
* 参数		：pDC 绘图指针
* 返回值   : void
* 说明  	：红色三角形  标识横滚角为0°所在的位置
			  绿色三角形  标识飞机的正上方
* 作者		：张群伟
* 日期		：[3/26/2017]
**************************************************************************************/
inline void CHUD::DrawTriangle(CDC *pDC)
{
	/*****************************绘制动态红色实心三角形***********************************/
	currentBrush.CreateSolidBrush(RGB(255, 0, 0));
	pDC->SelectObject(&currentBrush);
	ptTriangle[0].x = (int)(m_CenterPoint.x - 146.0 / 150.0*m_dRadius*sin(m_TSPIDC.m_ANGR*PI / 180));
	ptTriangle[0].y = (int)(m_CenterPoint.y - 146.0 / 150.0*m_dRadius*cos(m_TSPIDC.m_ANGR*PI / 180));
	ptTriangle[1].x = (int)(m_CenterPoint.x - (13 / 16.0*m_dRadius)*sin(m_TSPIDC.m_ANGR*PI / 180 - PI / 48.0));
	ptTriangle[1].y = (int)(m_CenterPoint.y - (13 / 16.0*m_dRadius)*cos(m_TSPIDC.m_ANGR*PI / 180 - PI / 48.0));
	ptTriangle[2].x = (int)(m_CenterPoint.x - (13 / 16.0*m_dRadius)*sin(m_TSPIDC.m_ANGR*PI / 180 + PI / 48.0));
	ptTriangle[2].y = (int)(m_CenterPoint.y - (13 / 16.0*m_dRadius)*cos(m_TSPIDC.m_ANGR*PI / 180 + PI / 48.0));
	pDC->SetPolyFillMode(WINDING);
	pDC->Polygon(ptTriangle, 3);
	currentBrush.DeleteObject();

	/***************************绘制顶部静态绿色实心小三角形*******************************/
	currentBrush.CreateSolidBrush(RGB(0, 255, 0));
	currentPen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	pDC->SelectObject(&currentPen);
	pDC->SelectObject(&currentBrush);
	ptTriangle[0].x = (int)(m_CenterPoint.x);
	ptTriangle[0].y = (int)(m_CenterPoint.y - 146.0 / 150.0*m_dRadius);
	ptTriangle[1].x = (int)(m_CenterPoint.x - (13.0 / 16.0*m_dRadius)*sin(-PI / 48.0));
	ptTriangle[1].y = (int)(m_CenterPoint.y - (13.0 / 16.0*m_dRadius)*cos(-PI / 48.0));
	ptTriangle[2].x = (int)(m_CenterPoint.x - (13.0 / 16.0*m_dRadius)*sin(PI / 48.0));
	ptTriangle[2].y = (int)(m_CenterPoint.y - (13.0 / 16.0*m_dRadius)*cos(PI / 48.0));
	pDC->SetPolyFillMode(WINDING);
	pDC->Polygon(ptTriangle, 3);
	currentPen.DeleteObject();
	currentBrush.DeleteObject();
}

/**************************************************************************************
 * 函数名	：DrawAerofoil	
 * 功能		：绘制飞机
 * 参数		：pDC 绘图指针
 * 返回值   : void
 * 说明  	：绘制静态飞机
 * 作者		：张群伟
 * 日期		：[3/26/2017]
 **************************************************************************************/
inline void CHUD::DrawAerofoil(CDC *pDC)
{
	// 画飞机的中心点
	CRect rectCenterSpot(m_CenterPoint.x - 2, m_CenterPoint.y - 2, m_CenterPoint.x + 2, m_CenterPoint.y + 2);
	pDC->FillSolidRect(rectCenterSpot, RGB(0, 255, 0));
	// 画飞机的机翼
	const int AerofoilLeng = 80;	// 机翼的长度
	const int AerofoilLengEx = 12;	// 机翼垂直部分的长度
	currentPen.CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	pDC->SelectObject(&currentPen);
	for (int i = 0; i < 2; i++)
	{
		int flage = pow(-1, i);
		// 机翼不随随横滚角旋转
		ptTriangle[0].x = m_CenterPoint.x - flage*AerofoilLeng;
		ptTriangle[0].y = m_CenterPoint.y;
		ptTriangle[1].x = m_CenterPoint.x - flage*AerofoilLeng*8.0 / 25.0;
		ptTriangle[1].y = m_CenterPoint.y;
		ptTriangle[2].x = ptTriangle[1].x;
		ptTriangle[2].y = ptTriangle[1].y + AerofoilLengEx;
		pDC->MoveTo(ptTriangle[0]);
		pDC->LineTo(ptTriangle[1]);
		pDC->LineTo(ptTriangle[2]);
	}
	currentPen.DeleteObject();
}

/**************************************************************************************
 * 函数名	：DrawCenterHorizontal	
 * 功能		：绘制中央水平线
 * 参数		：pDC 绘图指针
 * 返回值   : void
 * 说明  	：绘制动态水平线(显示俯仰角)
 * 作者		：张群伟
 * 日期		：[3/26/2017]
 **************************************************************************************/
inline void CHUD::DrawCenterHorizontal(CDC *pDC)
{
	// 刻度旁的俯仰角数值
	CString strNumAngP;		
	// 设置画笔
	currentPen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	pDC->SelectObject(&currentPen);
	// 设置字体
	currentFont.CreateFont((int)(0.14 * m_dRadius), 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FF_ROMAN, _T("楷体"));
	pDC->GetTextMetrics(&tm);
	pDC->SelectObject(&currentFont);
	pDC->SetBkMode(TRANSPARENT);	// 字体背景色透明 
	// 绘制中央水平线
	for (int j = 0; j < 2; j++)
	{
		// 左右两边都显示数字
		int PNflag = pow(-1, j);

		CPoint ptscale[3]; // 三种不同长度的水平线的左端起点坐标 
		for (int i = 0; i <= 17; i++)
		{	//+ 0°，1°，2°，3°，4°，5° （正负）
			if (i < 5)
			{
				// 计算水平线左端起点坐标
				ptscale[0].x = m_CenterPoint.x - 5.0 / 100.0*m_dRadius;
				ptscale[0].y = m_CenterPoint.y - 1.0 / 30.0*PNflag*i*m_dRadius+m_TSPIDC.m_ANGP/30.0*m_dRadius;
				//+ 零刻度线
				if (0 == i)
				{
					pDC->SetTextColor(RGB(255, 0, 0));
					// 左边的数字
					pDC->TextOutW(ptscale[0].x - 15, ptscale[0].y - 6, _T("0"));
					// 右边的数字
					pDC->TextOutW(ptscale[0].x + 20.0 / 100.0*m_dRadius, ptscale[0].y - 6, _T("0"));
					// 绘制水平线
					pDC->MoveTo(m_CenterPoint.x - 10.0 / 100.0*m_dRadius, ptscale[0].y);
					pDC->LineTo(m_CenterPoint.x + 10.0 / 100.0*m_dRadius, ptscale[0].y);

				}
				// 绘制水平线
				pDC->MoveTo(ptscale[0].x, ptscale[0].y);
				pDC->LineTo(ptscale[0].x + 11.0 / 100.0*m_dRadius, ptscale[0].y);
			}
			//+ 5°，15°，25°... 85°  （正负）
			if (0 != i % 2)
			{
				// 计算水平线左端起点坐标
				ptscale[1].x = m_CenterPoint.x - 18.0 / 100.0*m_dRadius;
				ptscale[1].y = m_CenterPoint.y - 5.0 / 30.0*PNflag*i*m_dRadius + m_TSPIDC.m_ANGP / 30.0*m_dRadius;
				// 水平线的数值
				strNumAngP.Format(L"%d", i * 5);
				pDC->SetTextColor(RGB(255, 255, 255));
				// 显示左边的数字
				pDC->SetTextAlign(TA_RIGHT);
				pDC->TextOutW(ptscale[1].x - 5, ptscale[1].y - 6, strNumAngP);
				// 显示右边的数字
				pDC->SetTextAlign(TA_LEFT);
				pDC->TextOutW(ptscale[1].x + 41.0 / 100.0*m_dRadius, ptscale[1].y - 6, strNumAngP);
				// 绘制水平线
				pDC->MoveTo(ptscale[1].x, ptscale[1].y);
				pDC->LineTo(m_CenterPoint.x + 18.0 / 100.0*m_dRadius, ptscale[1].y);
			}
			//+ 10°，20°，30°... 90°  （正负）
			if ((i <= 9) && (i>0))
			{
				// 计算水平线左端起点坐标
				ptscale[2].x = m_CenterPoint.x - 30.0 / 100.0*m_dRadius;
				ptscale[2].y = m_CenterPoint.y - 10.0 / 30.0*PNflag*i*m_dRadius + m_TSPIDC.m_ANGP / 30.0*m_dRadius;
				strNumAngP.Format(L"%d", i * 10);
				// 左边的数字
				pDC->SetTextAlign(TA_RIGHT);
				pDC->TextOutW(ptscale[2].x - 5, ptscale[2].y - 6, strNumAngP);
				// 右边的数字
				pDC->SetTextAlign(TA_LEFT);
				pDC->TextOutW(ptscale[2].x + 65.0 / 100.0*m_dRadius, (int)ptscale[2].y - 6, strNumAngP);
				// 绘制水平线
				pDC->MoveTo(ptscale[2].x, ptscale[2].y);
				pDC->LineTo(m_CenterPoint.x + 30.0 / 100.0*m_dRadius, ptscale[2].y);
			}
		}
	}
	currentFont.DeleteObject();
	currentPen.DeleteObject();
}



