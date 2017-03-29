#pragma once

#include "..\stdafx.h"
// CMyToolTipCtrl
#include"../resource.h"
class CMyToolTipCtrl : public CWnd
{
	//SetWindowRgn(),CombineRgn(),SelectClipRgn()
public:
	enum{
		TOP = 0x0010,
		RIGHT = 0x0020,
		LEFT = 0x0040,
		BOTTOM = 0x0080
	};
	enum{
		TIME = 0x0001,
		VALUE = 0x0002
	};
	const static DWORD TOOLTIP = 3000;
protected:
	DECLARE_DYNAMIC(CMyToolTipCtrl)

public:
	CMyToolTipCtrl();
	virtual ~CMyToolTipCtrl();

	UINT m_Dir;
	DWORD m_AttachId;

    void Init();
    void  OnPaint(HWND hwnd);
	int m_Style;
    int MakeTransparent(CDC *pDC, CDC *pDCImage, COLORREF transparentColor);
    HWND RegAndCreateWindow();
    COLORREF m_colTrans;
    DWORD m_dwWidth;
    DWORD m_dwHeight;
    DWORD m_dwWidth2;
    DWORD m_dwHeight2;
    void FreeResources();
    HBITMAP m_hBitmap;
	HBITMAP m_hBitmap2;
    LPCTSTR m_lpszClassName;
	CWnd *m_Parent;
	CRect m_ClientRect;
	CRect m_WindowRect;
	CPoint m_OffPos;
	CString m_strText;
	UINT m_uiBack;
	CRgn m_Rgn;
	void DrawBack(CDC *pDC);
	void SetBack(UINT back);
	void _DrawText(CDC *pDC);
	void SetText(CString text);
	bool Create(CWnd* pParent, CRect rc, CString text, 
		DWORD attachId, UINT dir = TOP,
		DWORD id = 0,DWORD style =	WS_POPUP);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnClose();
};


