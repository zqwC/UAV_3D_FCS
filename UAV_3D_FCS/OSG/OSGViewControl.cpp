/***************************************************************
* Copyright (c) 2016, 陈登龙
* All rights reserved.
*
* 文件名称：OSGViewControl.cpp
* 摘    要：3D模块的控制器。
*
* 当前版本：1.0
* 作    者：陈登龙
* 完成日期：2016年10月日
***************************************************************/
// OSGViewControl.cpp : 实现文件
//

#include "stdafx.h"
#include "../UAV_3D_FCS.h"
#include "../stdafx.h"
#include "OSGViewControl.h"


// COSGViewControl

IMPLEMENT_DYNCREATE(COSGViewControl, CFormView)

COSGViewControl::COSGViewControl()
	: CFormView(IDD_BASE_VIEW_CONTROL)
{

}

COSGViewControl::~COSGViewControl()
{
}

void COSGViewControl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COSGViewControl, CFormView)
END_MESSAGE_MAP()


// COSGViewControl 诊断

#ifdef _DEBUG
void COSGViewControl::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void COSGViewControl::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// COSGViewControl 消息处理程序
