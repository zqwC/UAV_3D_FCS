#include "stdafx.h"
#include "Compass.h"


CCompass::CCompass()
{
}


CCompass::~CCompass()
{
}



/*! @function
********************************************************************************
* 函数名   : DrawStaticBackground
* 功能     : 绘制指南针的静态背景
* 参数     : pDC 绘图指针
* 返回值   : void
* 备注     : 使用pDC指针来绘制
*******************************************************************************/
void CCompass::DrawStaticBackground(CDC *pDC)
{
	GetClientRect(&m_rectCtrl); 
	int start_x = m_rectCtrl.left;
	int start_y = m_rectCtrl.top;
	
	POINT start;
	start.x = m_rectCtrl.left;
	start.y = m_rectCtrl.top;
	POINT end;
	int r = 100;
	end.x = start.x + r;
	end.y = start.y + r;
	 
	DrawCricle(pDC, start, end);
}



/*! @function
********************************************************************************
* 函数名   : DrawDynamicBackground
* 功能     : 绘制指南针的动态背景
* 参数     : pDC 绘图指针
* 返回值   : void
* 备注     : 使用pDC指针来绘制
			 使用m_TSPIDC.m_TH作为为航向数据，范围[0, 360)
			 航向：顺时针偏离正北的角度
*******************************************************************************/
void CCompass::DrawDynamicBackground(CDC *pDC)
{
	//GetClientRect(&m_rectCtrl);
	//pDC->FillSolidRect(m_rectCtrl, RGB(0, 0, 0));
	//CPoint center,point;
	//center.x = m_rectCtrl.left + 50;
	//center.y = m_rectCtrl.top + 50;
	//point.x = center.x + sin(m_TSPIDC.m_TH) * 40;
	//point.y = center.y + cos(m_TSPIDC.m_TH) * 40;
	//CPen pen;
	//pen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	//pDC->SelectObject(pen);
	//pDC->MoveTo(center);
	//pDC->LineTo(point);

	//pen.DeleteObject();
}