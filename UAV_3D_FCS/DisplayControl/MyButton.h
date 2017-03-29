#pragma once

typedef enum BUTTON_STATE{
	STATE_DISABLE,
	STATE_FOCUS,
	STATE_HOVER,
	STATE_NOR,
	STATE_SEL,
	STATE_LEAVE
};

// CMyButton
#include "atlimage.h"
#include "MyToolTipCtrl.h"
class CMyButton : public CButton
{
	DECLARE_DYNAMIC(CMyButton)
	
	enum{
		TBS_VTOP = 0x0001,
		TBS_VCENTER = 0x0002,
		TBS_VBOTTOM = 0x0003,
		TBS_HLEFT = 0x0010,
		TBS_HCENTER = 0x0020,
		TBS_HRIGHT = 0x0030
	};
public:
	CMyButton();
	virtual ~CMyButton();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct );
	virtual void PreSubclassWindow( );
	void OnMyPaint(CString pNor,CString pHover,CString pSel,CString pFocus,CString pDisable);
	void OnMyPaint(UINT pNor, UINT pHover, UINT pSel, UINT pFocus, UINT pDisable);
	//void SetImage(LPCWSTR pNor, LPCWSTR pHover, LPCWSTR pSel, LPCWSTR pFocus, LPCWSTR pDisable);
public:

	//int m_state = NOR HOR SEL FOCUS DISABLE;
	void ToolTipMove(CPoint point);
	BUTTON_STATE m_ButtonState;
	HCURSOR m_OldCursor;
	CMyToolTipCtrl *m_ToolTipCtrl;
	CRect m_ToolTipRect;
	bool m_LMouseDown;//鼠标左键是否被按下 
	bool m_flag;
	CRect m_ButRect;
	CRgn m_Rgn;
	CImage m_img[5];//0 display, 1 focus,2 hover 3 nor 4 sel
	BOOL m_bTextVisible;
	void OnMouseHover(UINT nFlags, CPoint point);
	void OnMouseLeave();
	void SetTextVisible(BOOL bTextVisible);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};