// SplashWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "UAV_3D_FCS.h"
#include "SplashWnd.h"

#include "resource.h"


// CSplashWnd

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000
#endif

IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

CSplashWnd::CSplashWnd()
{
	Init();
}

CSplashWnd::CSplashWnd(UINT dwFileName, UINT dwFile2Name, COLORREF colTrans)
{
	m_SplashBack = dwFileName;
	m_SplaskBack2 = dwFile2Name;
	m_colTrans = colTrans;
}
CSplashWnd::~CSplashWnd()
{
	if (m_bitmap.GetSafeHandle())
	{
		m_bitmap.DeleteObject();
	}
	if (m_bitmap2.GetSafeHandle())
	{
		m_bitmap2.DeleteObject();
	}
}


BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


CSplashWnd* CSplashWnd::c_pSplashWnd;
// CSplashWnd 消息处理程序
void CSplashWnd::ShowSplashScreen(CWnd* pParentWnd)

{
	c_pSplashWnd = new CSplashWnd;
	if (!c_pSplashWnd->Create(pParentWnd))
		delete c_pSplashWnd;
	else
		c_pSplashWnd->UpdateWindow();

}
BOOL CSplashWnd::Create(CWnd* pParentWnd)

{
	m_bitmap.LoadBitmap(IDB_BITMAP_SPLASH);
	m_bitmap2.LoadBitmap(IDB_BITMAP_SPLASH2);
	BITMAP bm;
	m_bitmap.GetBitmap(&bm);
	CreateEx(WS_EX_TOPMOST,
	AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_WAIT)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, NULL, NULL);
	return MakeTransparent();
}
int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)

{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  在此添加您专用的创建代码
	// Center the window.
	CenterWindow();
	// Set a timer to destroy the splash screen.
	SetTimer(0, 200, NULL);
	SetTimer(1, 5000, NULL);
	return 0;

}
void CSplashWnd::OnPaint()

{
	if (!m_bitmap.GetSafeHandle() || !m_bitmap2.GetSafeHandle())
	{
		return;
	}
	CPaintDC dc(this); // device context for painting

	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(&dc))
		return;
	BITMAP bm;
	m_bitmap.GetBitmap(&bm);
	// Paint the image.
	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
	DOUBLE process = 1 - (DOUBLE)m_Process / 100.0;
	dc.BitBlt(0, 0, bm.bmWidth, (int)(bm.bmHeight * process), &dcImage, 0, 0, SRCCOPY | SRCERASE);
	dcImage.SelectObject(&m_bitmap2);
	dc.BitBlt(0, (int)(bm.bmHeight * process), bm.bmWidth, bm.bmHeight, &dcImage, 0, (int)(bm.bmHeight * process), SRCCOPY | SRCERASE);
	dcImage.SelectObject(pOldBitmap);
	dcImage.DeleteDC();
	

	TEXTMETRIC tm;
	CFont font, *pOldFont = NULL;
	INT text_width = 0, text_height = 0;
	CRect textRect, rect, textRext;
	::GetClientRect(this->m_hWnd, &rect);
	::GetTextMetrics(dc, &tm);
	text_width = tm.tmAveCharWidth * 1.5;
	text_height = tm.tmHeight * 1.5;
	VERIFY(font.CreateFont(
		text_height,             // nHeight
		text_width,              // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		TEXT("楷体")));            // lpszFacename
	textRect.left = textRect.top = 0;
	textRect.bottom = text_height;
	textRect.right = text_width + m_Text.GetLength() * 15;
	textRext.left = ((rect.right - rect.left) - textRect.Width()) / 2;
	textRext.right = textRext.left + textRect.Width();
	textRext.top = ((rect.bottom - rect.top) - textRect.Height()) / 2;
	textRext.bottom = textRext.top + textRect.Height();
	SetBkMode(dc, TRANSPARENT);
	if (m_Process < 50)
	{
		SetTextColor(dc, RGB(106, 150, 248));
	}
	else
	{
		SetTextColor(dc, RGB(0, 0, 0));
	}
	pOldFont = (CFont*)SelectObject(dc, font);
	DrawText(dc, m_Text, m_Text.GetLength(), &textRext, DT_CENTER | DT_VCENTER);
	SelectObject(dc, pOldFont);
	font.DeleteObject();
}
void CSplashWnd::OnTimer(UINT_PTR nIDEvent)

{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 0)
	{
		//不断更新
		if (m_Process >= 100)
		{
			KillTimer(0);
			m_Process = 100;
		}
		else
		{
			m_Process += 5;
		}
		m_Text.Format(TEXT("%u%%"), m_Process);
		Invalidate();
		UpdateWindow();
	}
	else
	{
		DestroyWindow();
		AfxGetMainWnd()->ShowWindow(SW_SHOWMAXIMIZED);
		AfxGetMainWnd()->UpdateWindow();
	}
	CWnd::OnTimer(nIDEvent);
}

bool CSplashWnd::MakeTransparent()
{
	//  =======================================================================
	//  Set the layered window style and make the required color transparent
	//  =======================================================================
	if (this->m_hWnd && g_pSetLayeredWindowAttributes && m_colTrans)
	{
		//  set layered style for the window
		SetWindowLong(this->m_hWnd, GWL_EXSTYLE, GetWindowLong(this->m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		//  call it with 0 alpha for the given color
		g_pSetLayeredWindowAttributes(this->m_hWnd, m_colTrans, 0, LWA_COLORKEY);
	}
	return TRUE;
}
void CSplashWnd::Init()
{
	//  =======================================================================
	//  Initialize the variables
	//  =======================================================================
	m_colTrans = RGB(255, 255, 255);
	m_Process = 0;
	m_Text = TEXT("0%");
	//  =======================================================================
	//  Keep the function pointer for the SetLayeredWindowAttributes function
	//  in User32.dll ready
	//  =======================================================================
	HMODULE hUser32 = GetModuleHandle(TEXT("USER32.DLL"));

	g_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)
		GetProcAddress(hUser32, "SetLayeredWindowAttributes");
}