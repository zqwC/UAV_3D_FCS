// PlayBackViewControl.cpp : 实现文件
//

#include "stdafx.h"
#include "../stdafx.h"
#include "../UAV_3D_FCS.h"
#include "PlayBackViewControl.h"


// CPlayBackViewControl

IMPLEMENT_DYNCREATE(CPlayBackViewControl, CFormView)

CPlayBackViewControl::CPlayBackViewControl()
	: CFormView(IDD_PLAY_BACK)
{
	/* 创建曲线控件 */
	m_GraphicView.Create(IDD_DIALOG_GRAPHIC);
	m_GraphicView.ShowWindow(SW_HIDE);
}

CPlayBackViewControl::~CPlayBackViewControl()
{
}

void CPlayBackViewControl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPlayBackViewControl, CFormView)
	ON_BN_CLICKED(IDC_BTN_GRAPHIC, &CPlayBackViewControl::OnBnClickedBtnGraphic)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPlayBackViewControl 诊断

#ifdef _DEBUG
void CPlayBackViewControl::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPlayBackViewControl::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPlayBackViewControl 消息处理程序


void CPlayBackViewControl::OnBnClickedBtnGraphic()
{
	/* 显示曲线控件 */
	m_GraphicView.ShowWindow(SW_SHOW);
	m_GraphicView.InitGraphic();
}


void CPlayBackViewControl::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	ShowScrollBar(SB_BOTH, FALSE);
}


void CPlayBackViewControl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CFormView::OnPaint()

	CRect  rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(0, 0, 0));
}
