
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "UAV_3D_FCS.h"

#include "MainFrm.h"
#include "SplashWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(SHOW_MAINWINDOW, OnShowMainWindow)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("未能创建工具栏\n");
	//	return -1;      // 未能创建
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("未能创建状态栏\n");
	//	return -1;      // 未能创建
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	/*m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);*/
	CSplashWnd::ShowSplashScreen();


	/*去掉菜单栏*/
	SetMenu(NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	//修改项目名称
	cs.lpszName = _T("UAV_3D_FCS");
	cs.style &= ~FWS_ADDTOTITLE;

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int	cy = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE);

	CRect rect;
	GetClientRect(&rect);
	
	/*将主窗口分成4个区域*/
	if (!m_SplitterWnd.CreateStatic(this, 2, 2, WS_CHILD | WS_VISIBLE | WS_BORDER))
	{
		return FALSE;
	}  

	if (!m_SplitterWnd1.CreateStatic(&m_SplitterWnd, 2, 1, WS_CHILD | WS_VISIBLE, m_SplitterWnd.IdFromRowCol(0, 1)))
	{
		return FALSE;
	}

	/*左上角用于3D模块*/
	if (!m_SplitterWnd.CreateView(0, 0, RUNTIME_CLASS(COSGViewControl), CSize((int)rect.Width() * 0.8, (int)(rect.Height() * 0.8)), pContext))
	{
		return FALSE;
	} 
 

	/*右上角用于飞行控制和HUD模块*/ 
	if (!m_SplitterWnd1.CreateView(0, 0, RUNTIME_CLASS(CHUDViewControl), CSize((int)rect.Width() * 0.2, (int)(rect.Height() * 0.8 * 0.5)), pContext))
	{
		return FALSE;
	}

	if (!m_SplitterWnd1.CreateView(1, 0, RUNTIME_CLASS(CFlightSysViewControl), CSize((int)rect.Width() * 0.2, (int)(rect.Height() * 0.8 * 0.5)), pContext))
	{
		return FALSE;
	} 

	/*左下角用于数据回放模块*/
	if (!m_SplitterWnd.CreateView(1, 0, RUNTIME_CLASS(CPlayBackViewControl), CSize((int)rect.Width() * 0.8, (int)(rect.Height() * 0.2)), pContext))
	{
		return FALSE;
	}

	/*右下角用于通信连接模块*/
	if (!m_SplitterWnd.CreateView(1, 1, RUNTIME_CLASS(CConnectionViewControl), CSize((int)rect.Width() * 0.3, (int)(rect.Height() * 0.2)), pContext))
	{
		return FALSE;
	}  

	return true;
}

void CMainFrame::ActivateFrame(int nCmdShow)
{
	// TODO:  在此添加专用代码和/或调用基类
	nCmdShow = SW_HIDE;

	CFrameWnd::ActivateFrame(nCmdShow);
}

LRESULT CMainFrame::OnShowMainWindow(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_SHOW);
	UpdateWindow();

	return 0;
}