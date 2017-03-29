#pragma once

#include "HUD.h"
#include "../protocol/FCToPCProtocolConvertToRealData.h"

#include <mutex>


namespace HUDViewControlSpace {
	//HUD定时器ID = 1
	const int HUDTimerID = 1;
	//HUD定时时间 = 200ms
	const int HUDTimerElapse = 200;
};




// CHUDViewControl 窗体视图

class CHUDViewControl : public CFormView
{
	DECLARE_DYNCREATE(CHUDViewControl)

protected:
	CHUDViewControl();           // 动态创建所使用的受保护的构造函数
	virtual ~CHUDViewControl();

public:
	enum { IDD = IDD_HUD };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	CHUD m_HUD;			//控件
	TSPIDC m_TSPIDC;	//接收数据
	bool m_DataIsComing = false;

	std::mutex m_MutexSendReq;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//打开定时器
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//接收数据
	void UpdateTSPIDC(const TSPIDC& tspidc);

	//初始化
	virtual void OnInitialUpdate();
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


