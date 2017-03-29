// Meter.cpp : implementation file
//

#include "stdafx.h"
#include "Meter.h"
#include "MemoryDC.h"
#include "malloc.h"

#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PT_NUM 300
/////////////////////////////////////////////////////////////////////////////
// CMeter

CMeter::CMeter()
{
}

void CMeter::SetMeterAttribute(const MeterAttribute &meterValue)
{
	m_nStartAngleDeg = meterValue.StartAngleDeg;
	m_nEndAngleDeg = meterValue.EndAngleDeg;
	m_nTicks = meterValue.Ticks;
	m_nSubTicks = meterValue.SubTicks;
	m_dMaxValue = meterValue.MaxValue;
	m_dMinValue = meterValue.MinValue;
	m_dCurrentValue = meterValue.CurrentValue;
	m_nScaleDecimals = meterValue.ScaleDecimals;
	m_nValueDecimals = meterValue.ValueDecimals;
	m_colorNeedle = meterValue.colorNeedle;
	m_strUnits = meterValue.strUnits;
	m_bColorTick = meterValue.bColorTick;
	m_meterUnit = meterValue.meterUnit;

	m_colorTable[0] = meterValue.colorTable[0];
	m_colorTable[1] = meterValue.colorTable[1];
	m_colorTable[2] = meterValue.colorTable[2];
	//m_colorTable[3] = meterValue.colorTable[3];
	//m_colorTable[4] = meterValue.colorTable[4];

	m_space3[0] = meterValue.space_3[0];
	m_space3[1] = meterValue.space_3[1];
	m_space3[2] = meterValue.space_3[2];

	m_TotalDots = m_space3[0] + m_space3[1] + m_space3[2];

	m_meterStyle = meterValue.meterStyle;
	m_valueRange[0] = meterValue.valueRange[0];
	m_valueRange[1] = meterValue.valueRange[1];
	m_valueRange[2] = meterValue.valueRange[2];
	m_valueRange[3] = meterValue.valueRange[3];
}

CMeter::~CMeter()
{
}


BEGIN_MESSAGE_MAP(CMeter, CStatic)
	//{{AFX_MSG_MAP(CMeter)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeter message handlers

void CMeter::GetBaseLineLength(const CRect& rect)
{
	//判断实际矩形控件的宽度和高度的比例是否满足最佳比例1:1，并分情况处理
	if ((double)rect.Width() / (double)rect.Height() >= 1)
	{
		m_nRadiusFrame = rect.Height() * 4 / 10;			//设置表盘的单位长度，可调整表盘的大小
	}
	else
	{
		m_nRadiusFrame = rect.Width() * 4 / 10;
	}
}


void CMeter::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// 获得控件区域
	GetClientRect(&m_rectCtrl);

	CMemoryDC memDC(&dc, &m_rectCtrl);

	//扩大绘制仪表盘的范围，以避免显示边线
	m_rectCtrl = CRect(m_rectCtrl.TopLeft().x, m_rectCtrl.TopLeft().y,
		m_rectCtrl.TopLeft().x + m_rectCtrl.Width() + 2,
		m_rectCtrl.TopLeft().y + m_rectCtrl.Height() + 2);

	// 选取圆盘边框半径
	GetBaseLineLength(m_rectCtrl);
	//m_nRadiusFrame = static_cast<int>(max(m_rectCtrl.Height(), m_rectCtrl.Width()) * 9.48 / 21);

	// 获得仪表盘中心点
	m_ptMeterCenter = m_rectCtrl.CenterPoint();
	m_ptMeterCenter.y += m_nRadiusFrame / 10;

	//绘制仪表盘
	if (m_dcBackground.GetSafeHdc() == NULL || (m_bitmapBackground.m_hObject == NULL))
	{
		m_dcBackground.CreateCompatibleDC(&dc);
		m_bitmapBackground.CreateCompatibleBitmap(&dc, m_rectCtrl.Width(), m_rectCtrl.Height());
		m_pBitmapOldBackground = m_dcBackground.SelectObject(&m_bitmapBackground);
		DrawMeterBackground(&m_dcBackground, m_rectCtrl);
	}

	memDC.BitBlt(-1, -1, m_rectCtrl.Width() - 1, m_rectCtrl.Height() - 1,   //这么设置是为了看不到仪表的盘的边线
		&m_dcBackground, 0, 0, SRCCOPY);

	// 绘制指针
	DrawNeedle(&memDC);  //绘制指针的线
	DrawNode(&memDC);  //绘制指针的点

	//绘制仪表的名称和数值
	DrawValue(&memDC);
}

void CMeter::DrawMeterBackground(CDC *pDC, CRect &rect)
{
	int nInnerRadius = m_nRadiusFrame * 8 / 10;	 //内圆弧半径

	m_nCenterRadius = m_nRadiusFrame / 15;  //中心圆半径大小

	int nFrame = m_nRadiusFrame / 18;  //边框厚度

	double dstepTickDeg = (360.0 + m_nStartAngleDeg - m_nEndAngleDeg) / (m_nTicks*m_nSubTicks);	//刻度步进角度

	int nSubTickR = nInnerRadius + (m_nRadiusFrame - 2 * nFrame - nInnerRadius) / 2;

	double dDeg = (m_nStartAngleDeg + 360.0 - m_nEndAngleDeg) / (TABNUM * PT_NUM);//把METER按角度均分成300等份


	CRect rectPanel, rectInnerPanel;
	CPen penDraw, *pPenOld = NULL;
	//CFont *pFontOld;
	CBrush brushFill, *pBrushOld = NULL;
	POINT ptStart, ptEnd/*, ptInnerStart, ptInnerEnd*/;
	CPoint pointInner[BOUNDARY_POINTS], ptGroup1[PT_NUM*TABNUM + 1], ptGroup2[PT_NUM*TABNUM + 1];	//按照弧度均分成300等份
	CPoint *ptRgn0 = (CPoint *)malloc((2 * m_space3[0] + 2)* sizeof(CPoint));					//第一段所占弧度的大小
	CPoint *ptRgn1 = (CPoint *)malloc((2 * m_space3[1] + 2) * sizeof(CPoint));					//第二段所占弧度的大小
	CPoint *ptRgn2 = (CPoint *)malloc((2 * m_space3[2] + 2) * sizeof(CPoint));					//第三段所占弧度的大小
	CPoint pttemp;
	CString strtemp;
	double dRadPerDeg;
	double dTickAngleRad;
	double dTemp;
	int nRef = 0;
	int nTickAngle;
	//int nHeight;	// 字体大小
	/*double dtempangle;*/

	// 计算起始角终止角弧度
	dRadPerDeg = 4.0*atan(1.0) / 180.0;
	m_dLeftAngleRad = (m_nStartAngleDeg - 180.0)*dRadPerDeg;
	m_dRightAngleRad = (m_nEndAngleDeg - 360.0)*dRadPerDeg;

	// 计算圆弧起始终止点及区域
	ptStart.x = m_ptMeterCenter.x - (int)(m_nRadiusFrame * cos(m_dLeftAngleRad));
	ptStart.y = m_ptMeterCenter.y + (int)(m_nRadiusFrame * sin(m_dLeftAngleRad));
	ptEnd.x = m_ptMeterCenter.x + (int)(m_nRadiusFrame * cos(-m_dRightAngleRad));
	ptEnd.y = m_ptMeterCenter.y + (int)(m_nRadiusFrame * sin(-m_dRightAngleRad));

	rectPanel.SetRect(m_ptMeterCenter.x - m_nRadiusFrame, m_ptMeterCenter.y - m_nRadiusFrame,
		m_ptMeterCenter.x + m_nRadiusFrame, m_ptMeterCenter.y + m_nRadiusFrame);

	// 获取点的位置

	for (int i = 0; i <= (m_space3[0] + m_space3[1] + m_space3[2]); i++)
	{
		ptGroup1[i].x = m_ptMeterCenter.x + (int)(m_nRadiusFrame
			* cos((m_nStartAngleDeg - i * dDeg) * dRadPerDeg));
		ptGroup1[i].y = m_ptMeterCenter.y - (int)(m_nRadiusFrame
			*sin((m_nStartAngleDeg - i*dDeg)*dRadPerDeg));				//获取彩色圆弧的外半径的点
		ptGroup2[i].x = m_ptMeterCenter.x + (int)((m_nRadiusFrame - nFrame)
			* cos((m_nStartAngleDeg - i * dDeg) * dRadPerDeg));
		ptGroup2[i].y = m_ptMeterCenter.y - (int)((m_nRadiusFrame - nFrame)
			* sin((m_nStartAngleDeg - i * dDeg) * dRadPerDeg));	//获取彩色圆弧的内半径的点
	}

	// 获取系统颜色;
	m_colorWindow = GetSysColor(COLOR_WINDOW);
	m_colorButton = RGB(0, 0, 0);					//控件背景颜色（黑色）
	m_colorShadow = GetSysColor(COLOR_BTNSHADOW);
	m_colorHighlight = GetSysColor(COLOR_BTNHIGHLIGHT);

	// 临时使用的颜色
	COLORREF cEdge;
	COLORREF cMiddle;

	cMiddle = RGB(255, 255, 255);
	cEdge = RGB(96, 96, 255);

	// 用按钮色绘制背景
	brushFill.DeleteObject();
	brushFill.CreateSolidBrush(m_colorButton);
	pBrushOld = pDC->SelectObject(&brushFill);
	pDC->Rectangle(rect);
	pDC->SelectObject(pBrushOld);

	/*if (m_meterStyle == ZMETERSTYLE::N2)
	{
		CBrush currentBrush;
		CBrush *pOldBrush;

		CPen LineColor;
		CPen *pOldLineColor;

		penDraw.DeleteObject();
		LineColor.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		pOldLineColor = pDC->SelectObject(&LineColor);

		if (m_N2IsWorking)
		{
			currentBrush.CreateSolidBrush(RGB(0, 255, 0));
			pOldBrush = pDC->SelectObject(&currentBrush);
		}
		else
		{
			currentBrush.CreateSolidBrush(RGB(0, 0, 0));
			pOldBrush = pDC->SelectObject(&currentBrush);
		}

		pDC->Ellipse((int)(m_ptMeterCenter.x - m_nRadiusFrame / 4),
			(int)(m_ptMeterCenter.y - m_nRadiusFrame / 4),
			(int)(m_ptMeterCenter.x + m_nRadiusFrame / 4),
			(int)(m_ptMeterCenter.y + m_nRadiusFrame / 4));
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldLineColor);
		currentBrush.DeleteObject();
		LineColor.DeleteObject();
	}*/
	if (m_bColorTick)
	{
		CPen currentPen;
		CPen *pOldPen = NULL;

		// 绘制色彩刻度
		for (int i = 0; i < TABNUM; i++)
		{

			//确定区域
			switch (i)
			{
			case 0:
				for (int j = 0; j <= m_space3[i]; j++)
				{
					ptRgn0[j] = ptGroup1[j];
					ptRgn0[2 * m_space3[i] + 1 - j] = ptGroup2[j];
				}
				currentPen.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
				pOldPen = pDC->SelectObject(&currentPen);
				pDC->MoveTo(ptRgn0[0].x, ptRgn0[0].y);
				pDC->LineTo(m_ptMeterCenter.x + (int)(m_nRadiusFrame * 8
					* cos((m_nStartAngleDeg)* dRadPerDeg) / 10),
					m_ptMeterCenter.y - (int)(m_nRadiusFrame * 8
					* sin((m_nStartAngleDeg)* dRadPerDeg) / 10));
				pDC->SelectObject(pOldPen);
				currentPen.DeleteObject();

				currentPen.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
				pOldPen = pDC->SelectObject(&currentPen);
				pDC->MoveTo(ptRgn0[m_space3[i]].x, ptRgn0[m_space3[i]].y);
				pDC->LineTo(m_ptMeterCenter.x + (int)(m_nRadiusFrame * 8
					* cos((m_nStartAngleDeg - m_space3[i] * dDeg) * dRadPerDeg) / 10),
					m_ptMeterCenter.y - (int)(m_nRadiusFrame * 8
					* sin((m_nStartAngleDeg - m_space3[i] * dDeg) * dRadPerDeg) / 10));
				pDC->SelectObject(pOldPen);
				currentPen.DeleteObject();
				break;
			case 1:
				for (int j = 0; j <= m_space3[i]; j++)
				{
					ptRgn1[j] = ptGroup1[m_space3[0] + j];
					ptRgn1[2 * m_space3[i] + 1 - j] = ptGroup2[m_space3[0] + j];
				}
				currentPen.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
				pOldPen = pDC->SelectObject(&currentPen);
				pDC->MoveTo(ptRgn1[m_space3[i]].x, ptRgn1[m_space3[i]].y);
				pDC->LineTo(m_ptMeterCenter.x + (int)(m_nRadiusFrame * 8
					* cos((m_nStartAngleDeg - (m_space3[0] + m_space3[1]) * dDeg) * dRadPerDeg) / 10),
					m_ptMeterCenter.y - (int)(m_nRadiusFrame * 8
					* sin((m_nStartAngleDeg - (m_space3[0] + m_space3[1]) * dDeg) * dRadPerDeg) / 10));
				pDC->SelectObject(pOldPen);
				currentPen.DeleteObject();
				break;
			case 2:
				for (int j = 0; j <= m_space3[i]; j++)
				{
					ptRgn2[j] = ptGroup1[m_space3[0] + m_space3[1] + j];
					ptRgn2[2 * m_space3[i] + 1 - j] = ptGroup2[m_space3[0] + m_space3[1] + j];
				}
				currentPen.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
				pOldPen = pDC->SelectObject(&currentPen);
				pDC->MoveTo(ptRgn2[m_space3[i]].x, ptRgn2[m_space3[i]].y);
				pDC->LineTo(m_ptMeterCenter.x + (int)(m_nRadiusFrame * 8
					* cos((m_nStartAngleDeg - (m_TotalDots)* dDeg) * dRadPerDeg) / 10),
					m_ptMeterCenter.y - (int)(m_nRadiusFrame * 8
					* sin((m_nStartAngleDeg - (m_TotalDots)* dDeg) * dRadPerDeg) / 10));
				pDC->SelectObject(pOldPen);
				currentPen.DeleteObject();
				break;
			default:
				break;
			}

			brushFill.DeleteObject();
			brushFill.CreateSolidBrush(m_colorTable[i]);			//决定多边形的填充的颜色
			pBrushOld = pDC->SelectObject(&brushFill);
			penDraw.DeleteObject();
			penDraw.CreatePen(PS_SOLID, 2, m_colorTable[i]);		//决定多边形的线条的性质
			pPenOld = pDC->SelectObject(&penDraw);
			switch (i)
			{
			case 0: pDC->Polygon(ptRgn0, 2 * m_space3[i] + 2);	break;					//连接数组中元素点组成多边形
			case 1: pDC->Polygon(ptRgn1, 2 * m_space3[i] + 2);	break;					//连接数组中元素点组成多边形
			case 2: pDC->Polygon(ptRgn2, 2 * m_space3[i] + 2);	break;					//连接数组中元素点组成多边形
			default:
				break;
			}
			pDC->SelectObject(pBrushOld);
			pDC->SelectObject(pPenOld);


		}
		free(ptRgn0);
		free(ptRgn1);
		free(ptRgn2);
	}

	// 计算刻度点,避免不能整除引起较大误差*100
	for (nTickAngle = m_nStartAngleDeg * 100;
		nTickAngle >= (m_nEndAngleDeg - 360) * 100;
		nTickAngle -= (int)(dstepTickDeg * 100))
	{
		// 转换成弧度
		dTickAngleRad = (double)nTickAngle / 100 * dRadPerDeg;

		// 确定外圈坐标
		// 确定x坐标
		dTemp = m_ptMeterCenter.x + (m_nRadiusFrame - 2 * nFrame)*cos(dTickAngleRad);
		m_pointBoundary[nRef].x = ROUND(dTemp);

		// 确定y坐标
		dTemp = m_ptMeterCenter.y - (m_nRadiusFrame - 2 * nFrame)*sin(dTickAngleRad);
		m_pointBoundary[nRef].y = ROUND(dTemp);

		// 确定刻度点(主刻度和子刻度)
		//主刻度及文本标注点
		if (nRef%m_nSubTicks == 0)
		{
			dTemp = m_ptMeterCenter.x + nInnerRadius*cos(dTickAngleRad);
			pointInner[nRef].x = ROUND(dTemp);
			dTemp = m_ptMeterCenter.y - nInnerRadius*sin(dTickAngleRad);
			pointInner[nRef].y = ROUND(dTemp);
		}
		else	// 子刻度
		{
			dTemp = m_ptMeterCenter.x + nSubTickR*cos(dTickAngleRad);
			pointInner[nRef].x = ROUND(dTemp);
			dTemp = m_ptMeterCenter.y - nSubTickR*sin(dTickAngleRad);
			pointInner[nRef].y = ROUND(dTemp);
		}

		nRef++;
	}

	// 多边形区域
	m_rgnBoundary.DeleteObject();


	//// 绘制刻度
	//penDraw.DeleteObject();
	//penDraw.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	//pPenOld = pDC->SelectObject(&penDraw);

	//for (int i = 0; i < nRef; i++)
	//{
	//	pDC->MoveTo(m_pointBoundary[i]);
	//	pDC->LineTo(pointInner[i]);
	//}

	//pDC->SelectObject(pPenOld);	
}

void CMeter::ReconstructControl()
{
	if ((m_pBitmapOldBackground) &&
		(m_bitmapBackground.GetSafeHandle()) &&
		(m_dcBackground.GetSafeHdc()))
	{
		m_dcBackground.SelectObject(m_pBitmapOldBackground);
		m_dcBackground.DeleteDC();
		m_bitmapBackground.DeleteObject();
	}

	Invalidate();
}

void CMeter::DrawNeedle(CDC *pDC)
{
	int nResult;
	double dRadPerDeg = 4.0 * atan(1.0) / 180.0;
	double dAngleDeg;
	double dAngleRad;
	double dTemp;
	CBrush brushFill;
	CBrush *pBrushOld;
	CPen penDraw;
	CPen *pPenOld;
	CPoint pointNeedle[4];	// 指针由四边形组成

	double sita[4] = { 0 };

	if (m_meterStyle == ZMETERSTYLE::N2)
	{
		CBrush currentBrush;
		CBrush *pOldBrush = NULL;

		CPen LineColor;
		CPen *pOldLineColor = NULL;

		penDraw.DeleteObject();
		LineColor.CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		pOldLineColor = pDC->SelectObject(&LineColor);

		if (m_N2IsWorking)
		{
			currentBrush.CreateSolidBrush(RGB(0, 255, 0));
			pOldBrush = pDC->SelectObject(&currentBrush);
		}
		else
		{
			currentBrush.CreateSolidBrush(RGB(0, 0, 0));
			pOldBrush = pDC->SelectObject(&currentBrush);
		}

		pDC->Ellipse((int)(m_ptMeterCenter.x - m_nRadiusFrame / 4),
			(int)(m_ptMeterCenter.y - m_nRadiusFrame / 4),
			(int)(m_ptMeterCenter.x + m_nRadiusFrame / 4),
			(int)(m_ptMeterCenter.y + m_nRadiusFrame / 4));
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldLineColor);
		currentBrush.DeleteObject();
		LineColor.DeleteObject();
	}
	switch (m_meterStyle)
	{
	case ZMETERSTYLE::N1:
	case ZMETERSTYLE::N2:
		sita[0] = m_nStartAngleDeg;
		sita[1] = m_nStartAngleDeg - (m_space3[0] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);
		sita[2] = sita[1] - (m_space3[1] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);
		sita[3] = sita[2] - (m_space3[2] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);

		// 计算角度并限定指针走的角度

		if (m_dCurrentValue <= m_valueRange[0])
		{
			dAngleDeg = m_nStartAngleDeg;
			m_colorNeedle = m_colorTable[0];

		}
		else if ((m_dCurrentValue > m_valueRange[0]) && (m_dCurrentValue <= m_valueRange[1]))
		{
			dAngleDeg = sita[0] - (sita[0] - sita[1]) *
				(m_dCurrentValue - m_valueRange[0]) / (m_valueRange[1] - m_valueRange[0]);
			m_colorNeedle = m_colorTable[0];
		}
		else if ((m_dCurrentValue > m_valueRange[1]) && (m_dCurrentValue <= m_valueRange[2]))
		{
			dAngleDeg = sita[1] - (sita[1] - sita[2]) *
				(m_dCurrentValue - m_valueRange[1]) / (m_valueRange[2] - m_valueRange[1]);
			m_colorNeedle = m_colorTable[1];
		}
		else if ((m_dCurrentValue > m_valueRange[2]) && (m_dCurrentValue <= m_valueRange[3]))
		{
			dAngleDeg = sita[2] - (sita[2] - sita[3]) *
				(m_dCurrentValue - m_valueRange[2]) / (m_valueRange[3] - m_valueRange[2]);
			m_colorNeedle = m_colorTable[2];
		}
		else
		{
			dAngleDeg = m_nEndAngleDeg;
			m_colorNeedle = m_colorTable[2];
		}
		dAngleRad = dAngleDeg*dRadPerDeg;
		break;
	case ZMETERSTYLE::T4:
		sita[0] = m_nStartAngleDeg;
		sita[1] = m_nStartAngleDeg - (m_space3[0] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);
		sita[2] = sita[1] - (m_space3[1] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);
		sita[3] = sita[2] - (m_space3[2] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);

		// 计算角度并限定指针走的角度

		if (m_dCurrentValue <= m_valueRange[0])
		{
			dAngleDeg = m_nStartAngleDeg;
			m_colorNeedle = m_colorTable[0];

		}
		else if ((m_dCurrentValue > m_valueRange[0]) && (m_dCurrentValue <= m_valueRange[1]))
		{
			dAngleDeg = sita[0] - (sita[0] - sita[1]) *
				(m_dCurrentValue - m_valueRange[0]) / (m_valueRange[1] - m_valueRange[0]);
			m_colorNeedle = m_colorTable[0];
		}
		else if ((m_dCurrentValue > m_valueRange[1]) && (m_dCurrentValue <= m_valueRange[2]))
		{
			dAngleDeg = sita[1] - (sita[1] - sita[2]) *
				(m_dCurrentValue - m_valueRange[1]) / (m_valueRange[2] - m_valueRange[1]);
			m_colorNeedle = m_colorTable[1];
		}
		else if ((m_dCurrentValue > m_valueRange[2]) && (m_dCurrentValue <= m_valueRange[3]))
		{
			dAngleDeg = sita[2] - (sita[2] - sita[3]) *
				(m_dCurrentValue - m_valueRange[2]) / (m_valueRange[3] - m_valueRange[2]);
			m_colorNeedle = m_colorTable[2];
		}
		else
		{
			dAngleDeg = m_nEndAngleDeg;
			m_colorNeedle = m_colorTable[2];
		}
		dAngleRad = dAngleDeg*dRadPerDeg;
		break;
	case ZMETERSTYLE::HYD1:
	case ZMETERSTYLE::HYD2:
		if (m_meterStyle == ZMETERSTYLE::HYD1)
		{
			sita[0] = m_nEndAngleDeg;
			sita[1] = m_nEndAngleDeg + (m_space3[2] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);
			sita[2] = sita[1] + (m_space3[1] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);
			sita[3] = sita[2] + (m_space3[0] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);

			// 计算角度并限定指针走的角度

			if (m_dCurrentValue <= m_valueRange[0])
			{
				dAngleDeg = m_nEndAngleDeg;
				m_colorNeedle = m_colorTable[2];
			}
			else if ((m_dCurrentValue > m_valueRange[0]) && (m_dCurrentValue <= m_valueRange[1]))
			{
				dAngleDeg = sita[0] + (sita[1] - sita[0]) *
					(m_dCurrentValue - m_valueRange[0]) / (m_valueRange[1] - m_valueRange[0]);
				m_colorNeedle = m_colorTable[2];
			}
			else if ((m_dCurrentValue > m_valueRange[1]) && (m_dCurrentValue <= m_valueRange[2]))
			{
				dAngleDeg = sita[1] + (sita[2] - sita[1]) *
					(m_dCurrentValue - m_valueRange[1]) / (m_valueRange[2] - m_valueRange[1]);
				m_colorNeedle = m_colorTable[1];
			}
			else if ((m_dCurrentValue > m_valueRange[2]) && (m_dCurrentValue <= m_valueRange[3]))
			{
				dAngleDeg = sita[2] + (sita[3] - sita[2]) *
					(m_dCurrentValue - m_valueRange[2]) / (m_valueRange[3] - m_valueRange[2]);
				m_colorNeedle = m_colorTable[0];
			}
			else
			{
				dAngleDeg = m_nStartAngleDeg;
				m_colorNeedle = m_colorTable[0];
			}
			dAngleRad = dAngleDeg*dRadPerDeg;
		}
		else
		{
			sita[0] = m_nStartAngleDeg;
			sita[1] = m_nStartAngleDeg - (m_space3[0] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);
			sita[2] = sita[1] - (m_space3[1] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);
			sita[3] = sita[2] - (m_space3[2] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);

			// 计算角度并限定指针走的角度

			if (m_dCurrentValue <= m_valueRange[0])
			{
				dAngleDeg = m_nStartAngleDeg;
				m_colorNeedle = m_colorTable[0];
			}
			else if ((m_dCurrentValue > m_valueRange[0]) && (m_dCurrentValue <= m_valueRange[1]))
			{
				dAngleDeg = sita[0] - (sita[0] - sita[1]) *
					(m_dCurrentValue - m_valueRange[0]) / (m_valueRange[1] - m_valueRange[0]);
				m_colorNeedle = m_colorTable[0];
			}
			else if ((m_dCurrentValue > m_valueRange[1]) && (m_dCurrentValue <= m_valueRange[2]))
			{
				dAngleDeg = sita[1] - (sita[1] - sita[2]) *
					(m_dCurrentValue - m_valueRange[1]) / (m_valueRange[2] - m_valueRange[1]);
				m_colorNeedle = m_colorTable[1];
			}
			else if ((m_dCurrentValue > m_valueRange[2]) && (m_dCurrentValue <= m_valueRange[3]))
			{
				dAngleDeg = sita[2] - (sita[2] - sita[3]) *
					(m_dCurrentValue - m_valueRange[2]) / (m_valueRange[3] - m_valueRange[2]);
				m_colorNeedle = m_colorTable[2];
			}
			else
			{
				dAngleDeg = m_nEndAngleDeg;
				m_colorNeedle = m_colorTable[2];
			}
			dAngleRad = dAngleDeg*dRadPerDeg;
		}
		break;
	case ZMETERSTYLE::LW:
	case ZMETERSTYLE::RW:
		if (m_meterStyle == ZMETERSTYLE::LW)
		{
			sita[0] = m_nEndAngleDeg;
			sita[1] = m_nEndAngleDeg + (m_space3[2] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);
			sita[2] = sita[1] + (m_space3[1] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);
			sita[3] = sita[2] + (m_space3[0] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);

			// 计算角度并限定指针走的角度

			if (m_dCurrentValue <= m_valueRange[0])
			{
				dAngleDeg = m_nEndAngleDeg;
				m_colorNeedle = m_colorTable[2];
			}
			else if ((m_dCurrentValue > m_valueRange[0]) && (m_dCurrentValue <= m_valueRange[1]))
			{
				dAngleDeg = sita[0] + (sita[1] - sita[0]) *
					(m_dCurrentValue - m_valueRange[0]) / (m_valueRange[1] - m_valueRange[0]);
				m_colorNeedle = m_colorTable[2];
			}
			else if ((m_dCurrentValue > m_valueRange[1]) && (m_dCurrentValue <= m_valueRange[2]))
			{
				dAngleDeg = sita[1] + (sita[2] - sita[1]) *
					(m_dCurrentValue - m_valueRange[1]) / (m_valueRange[2] - m_valueRange[1]);
				m_colorNeedle = m_colorTable[1];
			}
			else if ((m_dCurrentValue > m_valueRange[2]) && (m_dCurrentValue <= m_valueRange[3]))
			{
				dAngleDeg = sita[2] + (sita[3] - sita[2]) *
					(m_dCurrentValue - m_valueRange[2]) / (m_valueRange[3] - m_valueRange[2]);
				m_colorNeedle = m_colorTable[0];
			}
			else
			{
				dAngleDeg = m_nStartAngleDeg;
				m_colorNeedle = m_colorTable[0];
			}
			dAngleRad = dAngleDeg*dRadPerDeg;
		}
		else
		{
			sita[0] = m_nStartAngleDeg;
			sita[1] = m_nStartAngleDeg - (m_space3[0] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);
			sita[2] = sita[1] - (m_space3[1] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);
			sita[3] = sita[2] - (m_space3[2] * (360 + m_nStartAngleDeg - m_nEndAngleDeg) / m_TotalDots);

			// 计算角度并限定指针走的角度

			if (m_dCurrentValue <= m_valueRange[0])
			{
				dAngleDeg = m_nStartAngleDeg;
				m_colorNeedle = m_colorTable[0];

			}
			else if ((m_dCurrentValue > m_valueRange[0]) && (m_dCurrentValue <= m_valueRange[1]))
			{
				dAngleDeg = sita[0] - (sita[0] - sita[1]) *
					(m_dCurrentValue - m_valueRange[0]) / (m_valueRange[1] - m_valueRange[0]);
				m_colorNeedle = m_colorTable[0];
			}
			else if ((m_dCurrentValue > m_valueRange[1]) && (m_dCurrentValue <= m_valueRange[2]))
			{
				dAngleDeg = sita[1] - (sita[1] - sita[2]) *
					(m_dCurrentValue - m_valueRange[1]) / (m_valueRange[2] - m_valueRange[1]);
				m_colorNeedle = m_colorTable[1];
			}
			else if ((m_dCurrentValue > m_valueRange[2]) && (m_dCurrentValue <= m_valueRange[3]))
			{
				dAngleDeg = sita[2] - (sita[2] - sita[3]) *
					(m_dCurrentValue - m_valueRange[2]) / (m_valueRange[3] - m_valueRange[2]);
				m_colorNeedle = m_colorTable[2];
			}
			else
			{
				dAngleDeg = m_nEndAngleDeg;
				m_colorNeedle = m_colorTable[2];
			}
			dAngleRad = dAngleDeg*dRadPerDeg;
		}
		break;
	default:
		break;
	}

	m_colorText = m_colorNeedle;

	// 计算三角形底边两个点
	pointNeedle[0].x = m_ptMeterCenter.x - (int)(m_nCenterRadius * 10 * sin(dAngleRad) / 8);
	pointNeedle[0].y = m_ptMeterCenter.y - (int)(m_nCenterRadius * 10 * cos(dAngleRad) / 8);
	pointNeedle[2].x = m_ptMeterCenter.x + (int)(m_nCenterRadius * 10 * sin(dAngleRad) / 8);
	pointNeedle[2].y = m_ptMeterCenter.y + (int)(m_nCenterRadius * 10 * cos(dAngleRad) / 8);

	// 计算指针顶部坐标
	dTemp = m_ptMeterCenter.x + m_nRadiusFrame*cos(dAngleRad) * 95 / 100;
	pointNeedle[1].x = ROUND(dTemp);
	dTemp = m_ptMeterCenter.y - m_nRadiusFrame*sin(dAngleRad) * 95 / 100;
	pointNeedle[1].y = ROUND(dTemp);

	// 计算指针尾部坐标
	dTemp = m_ptMeterCenter.x - m_nRadiusFrame*cos(dAngleRad) / 6;
	pointNeedle[3].x = ROUND(dTemp);
	dTemp = m_ptMeterCenter.y + m_nRadiusFrame*sin(dAngleRad) / 6;
	pointNeedle[3].y = ROUND(dTemp);

	pDC->SelectClipRgn(&m_rgnBoundary);

	brushFill.CreateSolidBrush(m_colorNeedle);
	penDraw.CreatePen(PS_SOLID, 1, m_colorNeedle);

	pPenOld = pDC->SelectObject(&penDraw);
	pBrushOld = pDC->SelectObject(&brushFill);

	// 绘制指针
	pDC->Polygon(pointNeedle, 4);

	nResult = pDC->SelectClipRgn(NULL);

	pDC->SelectObject(pPenOld);
	pDC->SelectObject(pBrushOld);

	// 立体感处理
	if (dAngleDeg > 90)
	{
		penDraw.DeleteObject();
		penDraw.CreatePen(PS_SOLID, 2, m_colorShadow);
		pPenOld = pDC->SelectObject(&penDraw);
		pDC->MoveTo(pointNeedle[1]);
		pDC->LineTo(pointNeedle[0]);
		pDC->LineTo(pointNeedle[3]);
		pDC->SelectObject(pPenOld);

		penDraw.DeleteObject();
		penDraw.CreatePen(PS_SOLID, 1, m_colorHighlight);
		pPenOld = pDC->SelectObject(&penDraw);
		pDC->MoveTo(pointNeedle[1]);
		pDC->LineTo(pointNeedle[2]);
		pDC->LineTo(pointNeedle[3]);
		pDC->SelectObject(pPenOld);
	}
	else
	{
		penDraw.DeleteObject();
		penDraw.CreatePen(PS_SOLID, 2, m_colorShadow);
		pPenOld = pDC->SelectObject(&penDraw);
		pDC->MoveTo(pointNeedle[1]);
		pDC->LineTo(pointNeedle[2]);
		pDC->LineTo(pointNeedle[3]);
		pDC->SelectObject(pPenOld);

		penDraw.DeleteObject();
		penDraw.CreatePen(PS_SOLID, 1, m_colorHighlight);
		pPenOld = pDC->SelectObject(&penDraw);
		pDC->MoveTo(pointNeedle[1]);
		pDC->LineTo(pointNeedle[0]);
		pDC->LineTo(pointNeedle[3]);
		pDC->SelectObject(pPenOld);
	}
}

void CMeter::UpdateNeedle(double dValue)
{
	m_dCurrentValue = dValue;
	Invalidate();
}

void CMeter::UpdateCenterCircle(const BOOL N2IsWorking)
{
	m_N2IsWorking = N2IsWorking;
	Invalidate();

}

void CMeter::SetNeedleColor(COLORREF colorNeedle)
{
	m_colorNeedle = colorNeedle;
	ReconstructControl();
}


void CMeter::SetRange(double dMin, double dMax)
{
	m_dMaxValue = dMax;
	m_dMinValue = dMin;
	ReconstructControl();
}


void CMeter::SetScaleDecimals(int nDecimals)
{
	m_nScaleDecimals = nDecimals;
	ReconstructControl();
}

void CMeter::SetUnits(CString &strUnits)
{
	m_strUnits = strUnits;
	ReconstructControl();
}

void CMeter::SetValueDecimals(int nDecimals)
{
	m_nValueDecimals = nDecimals;
	ReconstructControl();
}


void CMeter::DrawNode(CDC *pDC)
{
	CPen penDraw, *pPenOld = NULL;
	COLORREF cEdge, cMiddle, cNode;

	cMiddle = RGB(255, 255, 255);
	cEdge = RGB(0, 0, 0);

	for (int i = m_nCenterRadius * 3 / 4; i >= 0; i--)
	{
		cNode = RGB((GetRValue(cEdge) - GetRValue(cMiddle))*((float)i) * 4 / (3 * m_nCenterRadius) + GetRValue(cMiddle),
			(GetGValue(cEdge) - GetGValue(cMiddle))*((float)i) * 4 / (3 * m_nCenterRadius) + GetGValue(cMiddle),
			(GetBValue(cEdge) - GetBValue(cMiddle))*((float)i) * 4 / (3 * m_nCenterRadius) + GetBValue(cMiddle));

		penDraw.DeleteObject();
		penDraw.CreatePen(PS_SOLID, 1, cNode);
		pPenOld = pDC->SelectObject(&penDraw);
		pDC->Arc(m_ptMeterCenter.x - i, m_ptMeterCenter.y - i, m_ptMeterCenter.x + i, m_ptMeterCenter.y + i,
			m_ptMeterCenter.x - i, m_ptMeterCenter.y, m_ptMeterCenter.x - i, m_ptMeterCenter.y);
		pDC->SelectObject(pPenOld);
	}
}

void CMeter::SetColorTick(BOOL bColorTick)
{
	m_bColorTick = bColorTick;
	ReconstructControl();
}

void CMeter::DrawValue(CDC *pDC)
{
	int nHeight = 0;
	/*CPoint pttemp;*/
	CString strtemp;
	CFont *pFontOld = NULL;

	//仪表的名称与相应数值显示
	nHeight = (int)(m_nRadiusFrame *  1.40 / 3.0);

	strtemp.Format(TEXT("%.*lf"), m_nValueDecimals, m_dCurrentValue);  //仪表数值的格式化

	m_font.DeleteObject();
	m_font.CreateFont(nHeight, 0, 0, 0, 700,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	pFontOld = pDC->SelectObject(&m_font);
	pDC->SetBkColor(m_colorButton);

	strtemp += m_meterUnit;
	switch (m_meterStyle)
	{
	case ZMETERSTYLE::N1:
	case ZMETERSTYLE::N2:
		pDC->SetTextAlign(TA_TOP | TA_CENTER);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOut(m_ptMeterCenter.x - m_nRadiusFrame, (int)(m_ptMeterCenter.y - m_nRadiusFrame * 1.15), m_strUnits);
		pDC->SetTextAlign(TA_TOP | TA_LEFT);
		pDC->SetTextColor(m_colorText);
		pDC->TextOut((int)(m_ptMeterCenter.x + m_nRadiusFrame / 6.0), (int)(m_ptMeterCenter.y + m_nRadiusFrame / 2.0), strtemp);
		break;
	case ZMETERSTYLE::T4:
		pDC->SetTextAlign(TA_TOP | TA_CENTER);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOut((int)(m_ptMeterCenter.x - m_nRadiusFrame), (int)(m_ptMeterCenter.y - m_nRadiusFrame * 1.15), m_strUnits);
		pDC->SetTextAlign(TA_TOP | TA_LEFT);
		pDC->SetTextColor(m_colorText);
		pDC->TextOut((int)(m_ptMeterCenter.x + m_nRadiusFrame / 6.0), (int)(m_ptMeterCenter.y + m_nRadiusFrame / 2.0), strtemp);
		break;
	case ZMETERSTYLE::HYD1:
	case ZMETERSTYLE::LW:
		pDC->SetTextAlign(TA_TOP | TA_CENTER);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOut((int)(m_ptMeterCenter.x + m_nRadiusFrame * 1.0 / 5.0),
			(int)(m_ptMeterCenter.y + 4.0 * m_nRadiusFrame / 5.50), m_strUnits);
		pDC->SetTextColor(m_colorText);
		pDC->TextOut((int)(m_ptMeterCenter.x + m_nRadiusFrame * 2.0 / 5.0),
			(int)(m_ptMeterCenter.y - m_nRadiusFrame * 1.20), strtemp);
		break;
	case ZMETERSTYLE::HYD2:
	case ZMETERSTYLE::RW:
		pDC->SetTextAlign(TA_TOP | TA_CENTER);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOut((int)(m_ptMeterCenter.x - m_nRadiusFrame / 5.0),
			(int)(m_ptMeterCenter.y + 4 * m_nRadiusFrame / 5.50), m_strUnits);
		pDC->SetTextColor(m_colorText);
		pDC->TextOut((int)(m_ptMeterCenter.x - m_nRadiusFrame / 2.0),
			(int)(m_ptMeterCenter.y - m_nRadiusFrame * 1.20), strtemp);
		break;
	default:
		break;
	}

	//恢复字体和背景色
	pDC->SelectObject(pFontOld);
	pDC->SetBkColor(m_colorWindow);
}

void CMeter::SetTicks(int nTicks)
{
	m_nTicks = nTicks;
	ReconstructControl();
}

void CMeter::SetSubTicks(int nSubTicks)
{
	m_nSubTicks = nSubTicks;
	ReconstructControl();
}

void CMeter::SetAngleRange(int nStartAngleDeg, int nEndAngleDeg)
{
	m_nStartAngleDeg = nStartAngleDeg;
	m_nEndAngleDeg = nEndAngleDeg;
	ReconstructControl();
}

const double CMeter::GetMaxValue() const
{
	return m_dMaxValue;
}

const double CMeter::GetMinValue() const
{
	return m_dMinValue;
}

