#pragma once
/***************************************************************
* Copyright (c) 2016, 陈登龙
* All rights reserved.
*
* 文件名称：ConnectionViewControl.h
* 摘    要：用于本项目的通信连接模块的视图类头文件
*
* 当前版本：1.0
* 作    者：陈登龙
* 完成日期：2016-10-21
***************************************************************/



#include "TcpClient.h" 
#include "../resource.h"

// CConnectionViewControl 窗体视图

class CConnectionViewControl : public CFormView
{
	DECLARE_DYNCREATE(CConnectionViewControl)

protected:
	CConnectionViewControl();           // 动态创建所使用的受保护的构造函数
	virtual ~CConnectionViewControl();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMUNICATION };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	/* TCP通信客户端对象 */
	CTcpClient m_TcpClient; 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnTcpConnect();
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


