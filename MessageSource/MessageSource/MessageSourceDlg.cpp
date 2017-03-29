
// MessageSourceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MessageSource.h"
#include "MessageSourceDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMessageSourceDlg 对话框



CMessageSourceDlg::CMessageSourceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessageSourceDlg::IDD, pParent)
, m_ID(0)					  //飞机编号  3、4
, m_DID(0)					  //目的编号  5、6
, m_LOTD(0)                   //即时经度  7、8、9、10
, m_LATD(0)                   //即时纬度  11、12、13、14
, m_SPH(0)                    //标准气压高度  15、16
, m_PRH(0)                    //气压修正高度  17、18
, m_RH(0)                  //无线电高度  19、20
, m_VS(0)                     //真空速  21、22
, m_IVS(0)                    //指示空速  23、24
, m_TH(0)                     //真航向  25、26
, m_ANGP(0)                   //俯仰角  27、28
, m_ANGR(0)                   //横滚角  29、30
, m_ANGPS(0)                  //俯仰角速率  31、32
, m_ANGRS(0)                  //横滚角速率  33、34
, m_bState(FALSE)
, m_strMsg(_T(""))
, m_dwTimInterval(20)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMessageSourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_ID);
	DDX_Text(pDX, IDC_EDIT_DID, m_DID);
	DDX_Text(pDX, IDC_EDIT_LOTD, m_LOTD);
	DDX_Text(pDX, IDC_EDIT_LATD, m_LATD);
	DDX_Text(pDX, IDC_EDIT_SPH, m_SPH);
	DDX_Text(pDX, IDC_EDIT_PRH, m_PRH);
	DDX_Text(pDX, IDC_EDIT_RH, m_RH);
	DDX_Text(pDX, IDC_EDIT_VS, m_VS);
	DDX_Text(pDX, IDC_EDIT_IVS, m_IVS);
	DDX_Text(pDX, IDC_EDIT_TH, m_TH);
	DDX_Text(pDX, IDC_EDIT_ANGP, m_ANGP);
	DDX_Text(pDX, IDC_EDIT_ANGR, m_ANGR);
	DDX_Text(pDX, IDC_EDIT_ANGPS, m_ANGPS);
	DDX_Text(pDX, IDC_EDIT_ANGRS, m_ANGRS);
	DDX_Text(pDX, IDC_EDIT_DATA, m_strMsg);
	DDX_Text(pDX, IDC_EDIT_TIME_INTERVAL, m_dwTimInterval);
}

BEGIN_MESSAGE_MAP(CMessageSourceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND_SPECIFIED_MSG, &CMessageSourceDlg::OnBnClickedButtonSendSpecifiedMsg)
	ON_BN_CLICKED(IDC_BUTTON_SEND_GENERATE_MSG, &CMessageSourceDlg::OnBnClickedButtonSendGenerateMsg)
	ON_BN_CLICKED(IDC_BUTTON_START, &CMessageSourceDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMessageSourceDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_STATUS, &CMessageSourceDlg::OnBnClickedButtonStatus)
	ON_BN_CLICKED(IDC_BUTTON_SET_IP_PORT, &CMessageSourceDlg::OnBnClickedButtonSetIpPort)
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_EDIT_TIME_INTERVAL, &CMessageSourceDlg::OnEnKillfocusEditTimeInterval)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CMessageSourceDlg 消息处理程序

BOOL CMessageSourceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//ShowWindow(SW_MAXIMIZE);

	// TODO:  在此添加额外的初始化代码
	GetDlgItem(IDC_IPADDRESS)->SetWindowText(_T("127.0.0.1"));
	GetDlgItem(IDC_PORT)->SetWindowText(_T("10002"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMessageSourceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMessageSourceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMessageSourceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMessageSourceDlg::OnBnClickedButtonSendSpecifiedMsg()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	std::string strHex;
	CString strTable;
	m_TSPIDC.m_ID = m_ID;
	m_TSPIDC.m_DID = m_DID;
	m_TSPIDC.m_LOTD = m_LOTD;
	m_TSPIDC.m_LATD = m_LATD;
	m_TSPIDC.m_SPH = m_SPH;
	m_TSPIDC.m_PRH = m_PRH;
	m_TSPIDC.m_RH = m_RH;
	m_TSPIDC.m_VS = m_VS;
	m_TSPIDC.m_IVS = m_IVS;
	m_TSPIDC.m_TH = m_TH;
	m_TSPIDC.m_ANGP = m_ANGP;
	m_TSPIDC.m_ANGR = m_ANGR;
	m_TSPIDC.m_ANGPS = m_ANGPS;
	m_TSPIDC.m_ANGRS = m_ANGRS;

	/* 实际物理值转换成协议值 */
	m_TSPIDC.TSPIDCToTSPI(m_TSPI, true);
	
	/* 为消息头赋值时间 */
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	m_TSPI.m_MessageHeader.hour = systime.wHour;
	m_TSPI.m_MessageHeader.minute = systime.wMinute;
	m_TSPI.m_MessageHeader.second = systime.wSecond;
	m_TSPI.m_MessageHeader.millSecond = systime.wMilliseconds;

	m_TSPIDC.ConvertToTable(strTable);
	m_strMsg += strTable;
	m_strMsg += TEXT("\r\n"); 

	if (m_TcpServer.is_connecting())
	{ 
		//序列化成JSON字符串
		m_Sr.Serialize(m_TSPI, "TSPI");
		std::string Json_str = m_Sr.GetString();

		/* 直接发送协议值的JSON字符串 */
		m_TcpServer.send_msg(Json_str);

		m_strMsg += strHex.c_str();
		m_strMsg += TEXT("\r\n");
	}


	UpdateData(FALSE);
}


void CMessageSourceDlg::OnBnClickedButtonSendGenerateMsg()
{
	if (!m_bState)
	{
		SetDlgItemText(IDC_BUTTON_SEND_GENERATE_MSG, _T("暂停发送"));
		SetTimer(TIMER_SEND, m_dwTimInterval, NULL);
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_SEND_GENERATE_MSG, _T("发送连续数据"));
		KillTimer(TIMER_SEND);
	}
	m_bState = !m_bState;
}


void CMessageSourceDlg::OnBnClickedButtonStart()
{
 	m_TcpServer.start();
	MessageBox(_T("TCP Server has start!"));
}


void CMessageSourceDlg::OnBnClickedButtonStop()
{
	m_TcpServer.stop();
	MessageBox(_T("TCP Server has stop!"));
}


void CMessageSourceDlg::OnBnClickedButtonStatus()
{
	if (m_TcpServer.is_connecting())
	{
		MessageBox(_T("TCP Server is running!"));
	}
	else
	{
		MessageBox(_T("TCP Server is stop!"));
	}
}


void CMessageSourceDlg::OnBnClickedButtonSetIpPort()
{
 
	//获取设置的IP
	CString _ip;
	GetDlgItemText(IDC_IPADDRESS, _ip);
	std::string ip;
	ip = (LPCSTR)CStringA(_ip);

	//获取设置的端口
	int port = GetDlgItemInt(IDC_PORT);
 
	m_TcpServer.set_server_addr(port, ip);

	MessageBox(_T("Set IP And Port Ok!"));
}


void CMessageSourceDlg::OnTimer(UINT_PTR nIDEvent)
{
	
	if (nIDEvent == TIMER_SEND)
	{
		SendTSPIMessage();
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMessageSourceDlg::SendTSPIMessage()
{
	UpdateData();
	std::string strHex;
	CString strTable;

	m_TSPIDC.CreateRandomTSPIDC();

	m_ID = m_TSPIDC.m_ID;
	m_DID = m_TSPIDC.m_DID;
	m_LOTD = m_TSPIDC.m_LOTD;
	m_LATD = m_TSPIDC.m_LATD;
	m_SPH = m_TSPIDC.m_SPH;
	m_PRH = m_TSPIDC.m_PRH;
	m_RH = m_TSPIDC.m_RH;
	m_VS = m_TSPIDC.m_VS;
	m_IVS = m_TSPIDC.m_IVS;
	m_TH = m_TSPIDC.m_TH;
	m_ANGP = m_TSPIDC.m_ANGP;
	m_ANGR = m_TSPIDC.m_ANGR;
	m_ANGPS = m_TSPIDC.m_ANGPS;
	m_ANGRS = m_TSPIDC.m_ANGRS;

	m_TSPIDC.TSPIDCToTSPI(m_TSPI, TRUE);
	m_TSPIDC.ConvertToTable(strTable);
	m_strMsg = strTable;
	m_strMsg += TEXT("\r\n");
	m_TSPI.ConvertToHexString(strHex);

	if (m_TcpServer.is_connecting())
	{
		std::string strAscii;
		m_TSPI.HexStrToAsciiStr(strAscii, strHex);

		//序列化成JSON字符串
		m_Sr.Serialize(m_TSPI, "TSPI");
		std::string Json_str = m_Sr.GetString();

		m_TcpServer.send_msg(Json_str);

		m_strMsg += strHex.c_str();
		m_strMsg += TEXT("\r\n");
	}


	UpdateData(FALSE);
}

void CMessageSourceDlg::OnEnKillfocusEditTimeInterval()
{ 
	UpdateData();
	KillTimer(TIMER_SEND);
	SetTimer(TIMER_SEND, m_dwTimInterval, NULL);
	m_bState = !m_bState;
}


void CMessageSourceDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	int minpos = 0,
		maxpos = 0;
	GetScrollRange(SB_HORZ, &minpos, &maxpos);
	maxpos = GetScrollLimit(SB_HORZ);
	int curpos = GetScrollPos(SB_HORZ);
	switch (nSBCode)
	{
	case SB_LEFT:
		curpos = minpos;
		break;
	case SB_RIGHT:
		curpos = maxpos;
		break;
	case SB_ENDSCROLL:
		break;
	case SB_LINELEFT:
		if (curpos > minpos)
			curpos--;
		break;
	case SB_LINERIGHT:
		if (curpos < maxpos)
			curpos++;
		break;
	case SB_PAGELEFT:
	{
						SCROLLINFO   info;
						GetScrollInfo(SB_HORZ, &info, SIF_ALL);
						if (curpos > minpos)
							curpos = max(minpos, curpos - (int)info.nPage);
	}
		break;
	case SB_PAGERIGHT:
	{
						 SCROLLINFO   info;
						 GetScrollInfo(SB_HORZ, &info, SIF_ALL);
						 if (curpos < maxpos)
							 curpos = min(maxpos, curpos + (int)info.nPage);
	}
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		curpos = nPos;
		break;
	}
	SetScrollPos(SB_HORZ, curpos);
	ScrollWindow(m_ixoldpos - curpos, 0);
	m_ixoldpos = curpos;
	UpdateWindow();
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMessageSourceDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	int minpos = 0,
		maxpos = 0;
	GetScrollRange(SB_VERT, &minpos, &maxpos);
	maxpos = GetScrollLimit(SB_VERT);
	int curpos = GetScrollPos(SB_VERT);
	switch (nSBCode)
	{
	case SB_TOP:
		curpos = minpos;
		break;
	case SB_BOTTOM:
		curpos = maxpos;
		break;
	case SB_ENDSCROLL:
		break;
	case SB_LINEUP:
		if (curpos > minpos)
			curpos--;
		break;
	case SB_LINEDOWN:
		if (curpos < maxpos)
			curpos++;
		break;
	case SB_PAGEUP:
	{
					  SCROLLINFO   info;
					  GetScrollInfo(SB_VERT, &info, SIF_ALL);
					  if (curpos > minpos)
						  curpos = max(minpos, curpos - (int)info.nPage);
	}
		break;
	case SB_PAGEDOWN:
	{
						SCROLLINFO   info;
						GetScrollInfo(SB_VERT, &info, SIF_ALL);
						if (curpos < maxpos)
							curpos = min(maxpos, curpos + (int)info.nPage);
	}
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		curpos = nPos;
		break;
	}
	SetScrollPos(SB_VERT, curpos);
	ScrollWindow(0, m_iyoldpos - curpos);
	m_iyoldpos = curpos;
	UpdateWindow();
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMessageSourceDlg::SetScroll(CRect page, CRect range)
{
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = range.Width();
	si.nPage = page.Width();
	SetScrollInfo(SB_HORZ, &si, TRUE);
	si.nMax = range.Height();
	si.nPage = page.Height();
	SetScrollInfo(SB_VERT, &si, TRUE);
	int icurxpos = GetScrollPos(SB_HORZ);
	int icurypos = GetScrollPos(SB_VERT);
	if (icurxpos < m_ixoldpos || icurypos < m_iyoldpos)
	{
		ScrollWindow(m_ixoldpos - icurxpos, 0);
		ScrollWindow(0, m_iyoldpos - icurypos);
	}
	m_ixoldpos = icurxpos;
	m_iyoldpos = icurypos;
	Invalidate(TRUE);
}