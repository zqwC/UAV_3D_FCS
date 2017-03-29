// MyToolTip.cpp : 实现文件
//

#include "stdafx.h"
//#include "../DisPlayTerminal.h"
#include "MyToolTipCtrl.h"
#include "../MemoryDC.h"
#include "../loadimg.h"


// CMyToolTipCtrl

IMPLEMENT_DYNAMIC(CMyToolTipCtrl, CWnd)

CMyToolTipCtrl::CMyToolTipCtrl()
{
	m_uiBack = IDB_PNG_BH_TT;
	m_OffPos.x = m_OffPos.y = 0;
	m_ClientRect.left = m_ClientRect.top;
	m_ClientRect.right = 50;
	m_ClientRect.bottom = 50;
	m_strText = TEXT("");
	m_Dir = TOP;
	ZeroMemory(m_Rgn, 0);
	//注册控件类
	m_lpszClassName = AfxRegisterWndClass(0);
}

CMyToolTipCtrl::~CMyToolTipCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyToolTipCtrl, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMyToolTipCtrl 消息处理程序
bool CMyToolTipCtrl::Create(CWnd* pParent, CRect rc, CString text, DWORD attachId, UINT dir /* = RIGHT*/,
							DWORD id /* = 0 */,DWORD style)
{
	// 动态创建控件
	BOOL ret;
	CRect rect;
	pParent->GetDlgItem(attachId)->GetWindowRect(&rect);

	m_AttachId = attachId;
	m_Parent = pParent;//关联控件父窗口
	m_Dir = dir;

	m_WindowRect.left = rect.right;
	m_WindowRect.bottom = rect.top;
	m_WindowRect.top = m_WindowRect.bottom - rc.Height() + 1;
	m_WindowRect.right = m_WindowRect.left + rc.Width() - 1;

	pParent->ScreenToClient(&rect);
	if (dir == RIGHT)
	{
		//垂直滑动
		m_ClientRect.left = rect.right;
		m_ClientRect.top = rect.top;
		m_ClientRect.right = m_ClientRect.left + rc.Width() - 1;
		m_ClientRect.bottom = m_ClientRect.top + rc.Height() - 1;
	}
	else
	{
		//水平滑动条
		m_ClientRect.left = rect.left;
		m_ClientRect.bottom = rect.top;
		m_ClientRect.right = m_ClientRect.left + rc.Width() - 1;
		m_ClientRect.top = m_ClientRect.bottom - rc.Height() + 1;
	}

	ret = CWnd::CreateEx(WS_EX_TOPMOST | WS_EX_NOACTIVATE, m_lpszClassName, text, WS_POPUP, m_ClientRect, NULL, 0);
	return ret ? true : false;
}

int CMyToolTipCtrl::MakeTransparent(CDC *pDC, CDC *pDCImage, COLORREF transparentColor)
{
	CRect rect;
	CRgn rgnResult, delRgn, tempRgn;
	COLORREF curColor;
	int i, j, ret, red, green, blue, 
		transRed = GetRValue(transparentColor), 
		transGreen = GetGValue(transparentColor), 
		transBlue = GetBValue(transparentColor);
	GetClientRect(&rect);
	rgnResult.CreateRectRgn(0, 0, rect.Width() - 1, rect.Height() - 1);
	tempRgn.CreateRectRgn(0, 0, 1, 1);
	delRgn.CreateRectRgn(0, 0, 0, 0);
	for (i = 0; i < rect.Height() * 2; i++)
	{
		for (j = 0; j < rect.Width(); j++)
		{
			curColor = GetPixel(pDCImage->GetSafeHdc(), i, j);
			red = GetRValue(curColor);
			green = GetGValue(curColor);
			blue = GetBValue(curColor);
			if(curColor == transparentColor)
			{
				tempRgn.SetRectRgn(i, j, i + 1, j + 1);
				delRgn.CombineRgn(&delRgn, &tempRgn, RGN_OR);
			}
		}
	}
	rgnResult.CombineRgn(&rgnResult, &delRgn, RGN_DIFF);
	ret = SetWindowRgn((HRGN)rgnResult.GetSafeHandle(), TRUE);
	rgnResult.DeleteObject();
	delRgn.DeleteObject();
	tempRgn.DeleteObject();
	return ret;
}

void CMyToolTipCtrl::SetBack(UINT back)
{
	m_uiBack = back;
}

void CMyToolTipCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CRect rect;
	GetClientRect(&rect);
	CMemoryDC memDC(&dc, &rect);
	/*绘制关联父窗口对应位置的背景,使得透明*/
	if (m_Parent)
	{
		CRect trect;
		GetWindowRect(&trect);
		m_Parent->ScreenToClient(&trect);
		CDC *parent_dc = m_Parent->GetDC();
		if (parent_dc)
		{
			memDC.BitBlt(0, 0, rect.Width(), rect.Height(), parent_dc, trect.left, trect.top, SRCCOPY);
			ReleaseDC(parent_dc);
		}
	}
	/*绘制背景*/
	DrawBack(memDC);
	/*绘制文字*/
	_DrawText(memDC);
}

void CMyToolTipCtrl::DrawBack(CDC *pDC)
{
	CImage img;
	CRect rect;
	GetClientRect(&rect);
	if (!img.IsNull())
	{
		img.Destroy();
	}
	LoadImageFromResource(&img, m_uiBack, TEXT("PNG"));
	img.Draw(pDC->GetSafeHdc(), rect);
	img.Destroy();
}

void CMyToolTipCtrl::_DrawText(CDC *pDC)
{
	CRect destRect;
	INT txtLength = 0;
	for (INT i = 0; i < m_strText.GetLength(); i++)
	{
		if (m_strText[i] > 255)
		{
			txtLength += 2;
		}
		else
		{
			txtLength += 1;
		}
	}

	pDC->SetBkMode(TRANSPARENT);
	GetClientRect(destRect);
	/*居中显示*/
	pDC->DrawText(m_strText, destRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}
BOOL CMyToolTipCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMyToolTipCtrl::SetText(CString text)
{
	CDC *pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	CMemoryDC memDC(pDC, &rect);
	m_strText = text;
	/*绘制关联父窗口对应位置的背景,使得透明*/
	if (m_Parent)
	{
		CRect trect;
		GetWindowRect(&trect);
		m_Parent->ScreenToClient(&trect);
		CDC *parent_dc = m_Parent->GetDC();
		if (parent_dc)
		{
			memDC.BitBlt(0, 0, rect.Width(), rect.Height(), parent_dc, trect.left, trect.top, SRCCOPY);
			ReleaseDC(parent_dc);
		}
	}
	DrawBack(memDC);
	_DrawText(memDC);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);
	ReleaseDC(pDC);
}

void CMyToolTipCtrl::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_Parent)
	{
		m_Parent->SendMessage(WM_CLOSE);
	}
	CWnd::OnClose();
}
