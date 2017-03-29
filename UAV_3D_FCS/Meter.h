#if !defined(AFX_METER_H__D5802279_6502_4453_BE21_58604877AD39__INCLUDED_)
#define AFX_METER_H__D5802279_6502_4453_BE21_58604877AD39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Meter.h : header file
//

#ifndef ROUND
#define ROUND(x) (int)((x) + 0.5 - (double)((x) < 0))
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeter window

enum class ZMETERSTYLE
{
	N1,
	N2,
	T4,
	HYD1,
	HYD2,
	LW,
	RW
};

//仪表属性结构体
struct MeterAttribute
{
	int StartAngleDeg;  //起始角度
	int EndAngleDeg;	//终止角度
	int Ticks;			//表盘刻度
	int SubTicks;		//小格刻度
	double MaxValue;	//最大值
	double MinValue;	//最小值
	double CurrentValue;//当前值

	int ScaleDecimals;	//
	int ValueDecimals;	//数值精度
	COLORREF colorNeedle;//指针的颜色
	CString strUnits;	//表盘标识
	BOOL bColorTick;	//
	CString meterUnit;	//数值单位

	COLORREF colorTable[3];	//表盘弧的三段颜色
	int space_3[3];			//	

	ZMETERSTYLE meterStyle;		//表盘类型
	double valueRange[4];		//valueRange[4]={X1,X2,X3,X4}
};


class CMeter : public CWnd
{
	// Construction
public:
	explicit CMeter(void);
	virtual ~CMeter();

	// Attributes
public:
	void SetMeterAttribute(const MeterAttribute &meterValue);

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeter)
protected:
	//}}AFX_VIRTUAL

	// Implementation
public:
	const double GetMaxValue(void) const;  //获取仪表最大值
	const double GetMinValue(void) const;  //获取仪表最小值
	void SetAngleRange(int nStartAngleDeg, int nEndAngleDeg);
	void SetSubTicks(int nSubTicks);
	void SetTicks(int nTicks);
	void DrawValue(CDC *pDC);
	void SetColorTick(BOOL bColorTick = FALSE);
	void DrawNode(CDC *pDC);
	void SetValueDecimals(int nDecimals);	//动态设置数值精度
	void SetUnits(CString &strUnits);  //设置仪表名称
	void SetScaleDecimals(int nDecimals);
	void SetRange(double dMin, double dMax);	//设置数值的范围
	void SetNeedleColor(COLORREF colorNeedle);	//设置指针的颜色
	void UpdateNeedle(double dValue);	//设置当前值
	void UpdateCenterCircle(const BOOL tAFCSD9);


	void DrawNeedle(CDC *pDC);
	void ReconstructControl();
	void DrawMeterBackground(CDC *pDC, CRect &rect);

	void GetBaseLineLength(const CRect& rect);




	// Generated message map functions
private:
	int m_nTabNum;
	BOOL m_bColorTick;
	COLORREF m_colorNeedle;
	double m_dCurrentValue;
	BOOL m_N2IsWorking = FALSE;
	double m_dMaxValue;  //仪表最大值
	double m_dMinValue;  //仪表最小值
	int m_nScaleDecimals;
	CString m_strUnits;
	int m_nValueDecimals;  //仪表数值的精度

	enum { TABNUM = 3 };   //色彩刻度数，这可以自由增加减少
	COLORREF m_colorTable[3];

	int m_space3[3];
	int m_TotalDots;

	ZMETERSTYLE m_meterStyle;		//表盘类型
	double m_valueRange[4];

	int m_nStartAngleDeg;	// 仪表盘圆弧起始角度
	int m_nEndAngleDeg;		// 仪表盘圆弧终止角度
	int m_nTicks;			// 刻度数
	int m_nSubTicks;		// 分刻度数


	double m_dLeftAngleRad;
	double m_dRightAngleRad;
	int    m_nCenterRadius;


	CRect m_rectCtrl;			// 控件区域
	CRect m_rectValue;			// 显示数值区域
	CRgn m_rgnBoundary;

	CBitmap *m_pBitmapOldBackground;
	CBitmap m_bitmapBackground;
	CDC m_dcBackground;

	CPoint m_ptMeterCenter;		// 仪表中心

	enum { BOUNDARY_POINTS = 200 };
	CPoint m_pointBoundary[BOUNDARY_POINTS]; // 边界点，用于绘制刻度

	CFont m_font;			// 显示文字字体

	COLORREF m_colorWindow;		// 背景色
	COLORREF m_colorHighlight;
	COLORREF m_colorShadow;
	COLORREF m_colorButton;
	COLORREF m_colorText = RGB(0, 255, 0);		// 显示文本颜色
	int m_nRadiusFrame;			                // 仪表盘边框半径

	CString m_meterUnit;
	//{{AFX_MSG(CMeter)
	afx_msg void OnPaint();
	//afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_METER_H__D5802279_6502_4453_BE21_58604877AD39__INCLUDED_)
