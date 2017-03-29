
// UAV_3D_FCS.h : UAV_3D_FCS 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CUAV_3D_FCSApp:
// 有关此类的实现，请参阅 UAV_3D_FCS.cpp
//

class CUAV_3D_FCSApp : public CWinApp
{
public:
	CUAV_3D_FCSApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CUAV_3D_FCSApp theApp;
