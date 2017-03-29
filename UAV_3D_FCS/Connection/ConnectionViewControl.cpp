/***************************************************************
* Copyright (c) 2016, 陈登龙
* All rights reserved.
*
* 文件名称：ConnectionViewControl.cpp
* 摘    要：用于本项目的通信连接模块的视图类的cpp文件
*
* 当前版本：1.0
* 作    者：陈登龙
* 完成日期：2016-10-21
***************************************************************/

#include "stdafx.h"
#include "../stdafx.h"
#include "../UAV_3D_FCS.h"
#include "ConnectionViewControl.h"


// CConnectionViewControl

IMPLEMENT_DYNCREATE(CConnectionViewControl, CFormView)

CConnectionViewControl::CConnectionViewControl()
	: CFormView(IDD_COMMUNICATION)
{

}

CConnectionViewControl::~CConnectionViewControl()
{
 
}

void CConnectionViewControl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CConnectionViewControl, CFormView)
	ON_BN_CLICKED(IDC_BTN_RESET, &CConnectionViewControl::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_TCP_CONNECT, &CConnectionViewControl::OnBnClickedBtnTcpConnect)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CConnectionViewControl 诊断

#ifdef _DEBUG
void CConnectionViewControl::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CConnectionViewControl::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CConnectionViewControl 消息处理程序


void CConnectionViewControl::OnBnClickedBtnReset()
{
	////获取设置的IP
	CString _ip;
	GetDlgItemText(IDC_TCP_IP, _ip);
	std::string ip;
	ip = (LPCSTR)CStringA(_ip);

	//获取设置的端口
	int port = GetDlgItemInt(IDC_TCP_PORT);

	//设置ip和port
	m_TcpClient.SetConnectAddr(port, ip);

	MessageBox(_T("Reset ip and port success!"));
}


void CConnectionViewControl::OnBnClickedBtnTcpConnect()
{  
	//开启TCP连接 

	m_TcpClient.StartTcpConnect();

	if (m_TcpClient.IsTcpConnecting())
	{
		MessageBox(_T("TCP connect success!"));
	} 
	else
	{
		MessageBox(_T("TCP connect fail!"));
	}
}


void CConnectionViewControl::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	 
	/* 初始设置为192.168.16.107 : 10002 */
	SetDlgItemInt(IDC_TCP_PORT, 10002);
	GetDlgItem(IDC_TCP_IP)->SetWindowText(_T("127.0.0.1"));   
}


void CConnectionViewControl::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	ShowScrollBar(SB_BOTH, FALSE);
}


void CConnectionViewControl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CFormView::OnPaint()
	CRect  rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(0, 0, 0));
}


HBRUSH CConnectionViewControl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if ((pWnd->GetDlgCtrlID() == (IDC_STATIC_IP)) || (pWnd->GetDlgCtrlID() == (IDC_STATIC_PORT)))
	{ 
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(225, 225, 225));
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	return hbr;
}
