#pragma once

#include <cmath>

#include "afxwin.h"
#include "protocol/FCToPCProtocolConvertToRealData.h"
#include "MemoryDC.h"


namespace UavViewPace {
	const COLORREF DARK = RGB(0, 0, 0);
	const COLORREF WHITE = RGB(255, 255, 255);
	const COLORREF RED = RGB(255, 0, 0);
	const COLORREF GREEN = RGB(0, 255, 0);

	//文本的字体
	const CString font_type = _T("楷体");

	//数据名称字体大小
	const int font_size = 120;
};

class CUavView :
	public CWnd
{
public:
	CUavView();
	~CUavView();

protected:
	TSPIDC m_TSPIDC;			//数据包

	//用于存放控件区域矩形大小的变量
	CRect       m_rectCtrl;

	//缓冲区内绘制背景的虚拟DC
	CDC			m_dcPlate;

	//在缓冲区中将图形绘制到此位图中
	CBitmap		m_bitmapPlate;

	//旧的位置指针
	CBitmap     *m_pbitmapOldPlate;

	//是否需要重绘静态背景
	bool m_reqRedrawStaticBgd = true; 
	
	//为绘制控件做准备
	void DrawStaticPrepare(CDC* pDC);
	
	//绘制静态背景,子类必须重写
	virtual void DrawStaticBackground(CDC *pDC) = 0;
	
	//绘制动态背景,子类必须重写
	virtual void DrawDynamicBackground(CDC *pDC) = 0;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

public:
	//更新控件数据
	void SetCurrentData(const TSPIDC& tspidc);

	//画文本
	void DrawUavText(CDC* p_dc, const CString& p_show_text, int p_start_x, int p_start_y, CString p_font_type, int p_font_size, COLORREF p_color = UavViewPace::WHITE);

	//画圆
	void DrawCricle(CDC* p_dc, POINT point_up_left, POINT point_low_right, COLORREF p_color = UavViewPace::WHITE, int p_line_width = 1, int p_line_type = PS_SOLID);
	 
};

