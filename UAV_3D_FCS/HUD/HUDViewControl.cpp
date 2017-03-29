// HUDViewControl.cpp : 实现文件
//

#include "stdafx.h"
#include "../stdafx.h"
#include "../UAV_3D_FCS.h"
#include "HUDViewControl.h"


// CHUDViewControl


extern MessageBus g_Bus;

IMPLEMENT_DYNCREATE(CHUDViewControl, CFormView)

CHUDViewControl::CHUDViewControl()
	: CFormView(CHUDViewControl::IDD)
{
	/* 接收g_Bus上的数据包 */
	g_Bus.Attach([this](const TSPIDC& tspidc){ UpdateTSPIDC(tspidc); }, "TSPIDC");

	memset(&m_TSPIDC, 0, sizeof(m_TSPIDC));
}

CHUDViewControl::~CHUDViewControl()
{
	if (m_DataIsComing)
	{
		//如果数据到来，则肯定开启了定时器，这里需要释放掉
		KillTimer(HUDViewControlSpace::HUDTimerID);
	}
	
	/* 释放g_Bus上的数据包 */
	g_Bus.Remove<void, const TSPIDC&>("TSPIDC");
}

void CHUDViewControl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX); 
	DDX_Control(pDX, IDC_HUD, m_HUD);
}

BEGIN_MESSAGE_MAP(CHUDViewControl, CFormView)
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CHUDViewControl 诊断

#ifdef _DEBUG
void CHUDViewControl::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CHUDViewControl::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHUDViewControl 消息处理程序


void CHUDViewControl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	//数据更新
	m_HUD.SetCurrentData(m_TSPIDC);


	//打开定时器
	CFormView::OnTimer(nIDEvent);
}


/*! @function
********************************************************************************
* 函数名   : UpdateTSPIDC
* 功能     : 接收消息总线上的TSPIDC数据
* 参数     : tspidc 接收到的TSPIDC数据包
* 返回值   : void
* 作者     : 陈登龙 南昌航空大学信息工程学院自动控制系
* 日期     : 2017-03-04
*******************************************************************************/
void CHUDViewControl::UpdateTSPIDC(const TSPIDC& tspidc)
{
	if (!m_DataIsComing)
	{
		/* 不使用回调函数 */
		SetTimer(HUDViewControlSpace::HUDTimerID, HUDViewControlSpace::HUDTimerElapse, NULL);

		//数据已经到来，使用这个标记防止重复开启定时器
		m_DataIsComing = true;
	}
	

	{
		std::lock_guard<std::mutex>  locker(m_MutexSendReq);
		m_TSPIDC = tspidc; 
	}
}



void CHUDViewControl::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO:  在此添加专用代码和/或调用基类
	 
}


void CHUDViewControl::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	ShowScrollBar(SB_BOTH, FALSE);
}
