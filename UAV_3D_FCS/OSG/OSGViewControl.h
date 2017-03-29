/***************************************************************
* Copyright (c) 2016, 陈登龙
* All rights reserved.
*
* 文件名称：OSGViewControl.h
* 摘    要：3D模块控制器头文件。
*
* 当前版本：1.0
* 作    者：陈登龙
* 完成日期：2016年10月日
***************************************************************/
#pragma once



// COSGViewControl 窗体视图

class COSGViewControl : public CFormView
{
	DECLARE_DYNCREATE(COSGViewControl)

protected:
	COSGViewControl();           // 动态创建所使用的受保护的构造函数
	virtual ~COSGViewControl();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OSG };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


