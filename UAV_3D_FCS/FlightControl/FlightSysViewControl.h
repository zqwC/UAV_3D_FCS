#pragma once

#include <mutex>
#include "../GPS.h"
#include "../Meter.h"
#include "C:\Users\God-is-fool\Documents\Visual Studio 2013\Projects\UAV_3D_FCS\UAV_3D_FCS\UAV_3D_FCS\Meter.h"
#include "C:\Users\God-is-fool\Documents\Visual Studio 2013\Projects\UAV_3D_FCS\UAV_3D_FCS\UAV_3D_FCS\Compass.h"
//#include "C:\Users\Administrator\Desktop\Work\UAV_3D_FCS\UAV_3D_FCS\UAV_3D_FCS\Meter.h"
//#include "C:\Users\Administrator\Desktop\Work\UAV_3D_FCS\UAV_3D_FCS\UAV_3D_FCS\Compass.h"

/***************************************************************
* Copyright (c) 2016, 陈登龙
* All rights reserved.
*
* 文件名称：FlightSysViewControl.h
* 摘    要：用于本项目的飞行控制模块的视图类的头文件
*
* 当前版本：1.0
* 作    者：陈登龙
* 完成日期：2016-10-21
***************************************************************/


// CFlightSysViewControl 窗体视图
namespace FlightSysViewControlSpace {
	//HUD定时器ID = 1
	const int HUDTimerID = 1;
	//HUD定时时间 = 200ms
	const int HUDTimerElapse = 200;
};

class CFlightSysViewControl : public CFormView
{
	DECLARE_DYNCREATE(CFlightSysViewControl)

protected:
	CFlightSysViewControl();           // 动态创建所使用的受保护的构造函数
	virtual ~CFlightSysViewControl();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FCS };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
private:
	//GPS控件
	CGPS m_GPS;
	//真空速表盘
	CMeter m_VSMeter;
	//指示空速表盘
	CMeter m_IVSMeter;
	//指南针控件
	CCompass m_Compass;

	TSPIDC m_TSPIDC;	//接收数据
	bool m_DataIsComing = false;

	std::mutex m_MutexSendReq;

private:
	//初始化表盘
	void InitMeter();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	 
	//接收数据
	void UpdateTSPIDC(const TSPIDC& tspidc);
	//初始化
	//virtual void OnInitialUpdate();

	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	virtual void OnInitialUpdate(); 
	afx_msg void OnPaint(); 
};


