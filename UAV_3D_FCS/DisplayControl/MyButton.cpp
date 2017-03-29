// MyButton.cpp : implementation file
//
#include "stdafx.h"
#include "../stdafx.h"
#include "MyButton.h" 
#include "../MemoryDC.h"
#include "../loadimg.h"

#include"../resource.h"
// CMyButton

IMPLEMENT_DYNAMIC(CMyButton, CButton)

CMyButton::CMyButton()
{
	m_flag=FALSE;
	LoadImageFromResource(&m_img[0], IDB_PNG_UAT_PLAY, TEXT("PNG"));
	LoadImageFromResource(&m_img[1], IDB_PNG_AT_PLAY, TEXT("PNG"));
	LoadImageFromResource(&m_img[2], IDB_PNG_AT_PLAY, TEXT("PNG"));
	LoadImageFromResource(&m_img[3], IDB_PNG_UAT_PLAY, TEXT("PNG"));
	LoadImageFromResource(&m_img[4], IDB_PNG_UAT_PLAY, TEXT("PNG"));
	CRect rect(50, 50, 150, 80);
	m_ButRect = rect;
	m_LMouseDown = FALSE;
	m_ButtonState = STATE_LEAVE;
	m_OldCursor = LoadCursor(NULL, IDC_HAND);
	m_ToolTipCtrl = NULL;
	m_ToolTipCtrl = new CMyToolTipCtrl();
	m_ToolTipCtrl->SetBack(IDB_PNG_LH_TT);
	m_ToolTipRect.left = 0;
	m_ToolTipRect.right = 80;
	m_ToolTipRect.top = 0;
	m_ToolTipRect.bottom = 20;
	m_bTextVisible = FALSE;
}

CMyButton::~CMyButton()
{
	for (int i = 0; i < 5; ++i)
	{
		if (!m_img[i].IsNull())
		{
			m_img[i].Destroy();
		}
	}
	if (m_ToolTipCtrl)
	{
		delete m_ToolTipCtrl;
		m_ToolTipCtrl = NULL;
	}
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


void CMyButton::PreSubclassWindow( )
{
	ModifyStyle( 0, BS_OWNERDRAW );        //设置按钮属性为自画式
	CRect clientRect;
	GetWindowRect(&clientRect);
	ScreenToClient(&clientRect);
	m_Rgn.CreateEllipticRgn(clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
	SetWindowRgn(m_Rgn, TRUE);
	
	CWnd *pParent = GetParent();
	if (!pParent)
	{
		return;
	}
	BOOL ret = m_ToolTipCtrl->Create(pParent, m_ToolTipRect, TEXT(""), GetDlgCtrlID(),  CMyToolTipCtrl::TOP);

	CButton::PreSubclassWindow();
}

void CMyButton::SetTextVisible(BOOL bTextVisible)
{
	m_bTextVisible = bTextVisible;
}

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	CBitmap MemBitmap;
	CRect clientRect;
	CDC *dc = CDC::FromHandle( lpDrawItemStruct->hDC );
	m_ButRect=CRect(lpDrawItemStruct->rcItem);
	GetClientRect(&clientRect);
	if(clientRect.Width() == 0 
		|| clientRect.Height() == 0)return;
	CMemoryDC memDC(dc, &clientRect);
	CBitmap *pOldBit=memDC.SelectObject(&MemBitmap);
	int old_mode;
	old_mode = memDC.SetBkMode(TRANSPARENT);
	switch(m_ButtonState)
	{
	case STATE_DISABLE:if(!m_img[4].IsNull())m_img[4].Draw(memDC.GetSafeHdc(), m_ButRect);break;
	case STATE_FOCUS:if(!m_img[1].IsNull())m_img[1].Draw(memDC.GetSafeHdc(), m_ButRect);break;
	case STATE_HOVER:if(!m_img[2].IsNull())m_img[2].Draw(memDC.GetSafeHdc(), m_ButRect);break;
	case STATE_NOR:if(!m_img[3].IsNull())m_img[3].Draw(memDC.GetSafeHdc(), m_ButRect);break;
	case STATE_LEAVE:if(!m_img[3].IsNull())m_img[3].Draw(memDC.GetSafeHdc(), m_ButRect);break;
	case STATE_SEL:if(!m_img[1].IsNull())m_img[1].Draw(memDC.GetSafeHdc(), m_ButRect);break;
	}
	if (m_bTextVisible)
	{
		//在窗口中心绘制文字
		CString window_text = TEXT("");
		CRect rect;
		GetClientRect(rect);
		GetWindowText(window_text);
		memDC.SetTextColor(RGB(255, 255, 255));
		memDC.DrawText(window_text, rect, DT_CENTER | DT_VCENTER);
	}
	memDC.SetBkMode(old_mode);
	memDC.SelectObject(pOldBit);
}

void CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_ButtonState == STATE_LEAVE)
	{
		m_ButtonState = STATE_HOVER;
		TRACKMOUSEEVENT evt = { sizeof(evt), TME_LEAVE, m_hWnd, 0 };
		TrackMouseEvent(&evt);
		OnMouseHover(0,CPoint());
	}
	else
	{
		CRect rect;
		GetWindowRect(&rect);
		CWnd *pParent = GetParent();
		if (!pParent)
		{
			return;
		}
		pParent->ScreenToClient(&rect);
		pParent->ActivateTopParent();
		CPoint point(rect.right, rect.top);
		CString text;
		if (m_ToolTipCtrl && m_ToolTipCtrl->m_hWnd)
		{
			ToolTipMove(point);
			GetWindowText(text);
			m_ToolTipCtrl->SetText(text);
			m_ToolTipCtrl->ShowWindow(SW_SHOW);
		}
		::SetCursor(LoadCursor(NULL, IDC_HAND));

	}
}


void CMyButton::ToolTipMove(CPoint point)
{
	CRect toolTipRect, rect;
	GetWindowRect(&rect);
	CWnd *pParent = GetParent();
	if (!pParent)
	{
		return;
	}
	//pParent->ScreenToClient(&rect);
	/*右端显示*/
	/*toolTipRect.top = point.y;
	toolTipRect.bottom = toolTipRect.top + m_ToolTipRect.Height();
	toolTipRect.left = point.x;
	toolTipRect.right = toolTipRect.left + m_ToolTipRect.Width();*/
	toolTipRect.left = rect.right;
	toolTipRect.right = toolTipRect.left + m_ToolTipRect.Width();
	toolTipRect.top = rect.top;
	toolTipRect.bottom = toolTipRect.top + m_ToolTipRect.Height();


	if (m_ToolTipCtrl->m_hWnd)
	{
		m_ToolTipCtrl->MoveWindow(&toolTipRect);
	}
}

void CMyButton::OnMouseHover(UINT nFlags, CPoint point)
{
	// 鼠标进入
	Invalidate();
	//m_OldCursor = ::SetCursor(LoadCursor(NULL, IDC_HAND));
}

void CMyButton::OnMouseLeave()
{
	m_ButtonState = STATE_LEAVE;
	if (m_ToolTipCtrl->m_hWnd)
	{
		m_ToolTipCtrl->ShowWindow(SW_HIDE);
	}
	Invalidate();
}

void CMyButton::OnMyPaint(CString pNor,CString pHover,CString pSel,CString pFocus,CString pDisable)
{
	for (int i = 0; i < 5; ++i)
	{
		if (!m_img[i].IsNull())
		{
			m_img[i].Destroy();
		}
	}
	m_img[0].Load(pNor);
	m_img[1].Load(pHover);
	m_img[2].Load(pSel);
	m_img[3].Load(pFocus);
	m_img[4].Load(pDisable);
}
void CMyButton::OnMyPaint(UINT pNor, UINT pHover, UINT pSel, UINT pFocus, UINT pDisable)
{
	for (int i = 0; i < 5; ++i)
	{
		if (!m_img[i].IsNull())
		{
			m_img[i].Destroy();
		}
	}
	LoadImageFromResource(&m_img[0], pNor, TEXT("PNG"));
	LoadImageFromResource(&m_img[1], pHover, TEXT("PNG"));
	LoadImageFromResource(&m_img[2], pSel, TEXT("PNG"));
	LoadImageFromResource(&m_img[3], pFocus, TEXT("PNG"));
	LoadImageFromResource(&m_img[4], pDisable, TEXT("PNG"));
}
BOOL CMyButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
//	return CButton::OnEraseBkgnd(pDC);
}

void CMyButton::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CButton::OnShowWindow(bShow, nStatus);
	
	// TODO: 在此处添加消息处理程序代码	CPoint point;
}


int CMyButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  在此添加您专用的创建代码
	return 0;
}


void CMyButton::OnDestroy()
{
	CButton::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_Rgn.DeleteObject();
}


void CMyButton::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;

	GetClientRect(&rect);
	m_Rgn.DeleteObject();
	m_Rgn.CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom);
	SetWindowRgn(m_Rgn, FALSE);
}


void CMyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CButton::OnLButtonDown(nFlags, point);
}