/***************************************************************
* Copyright (c) 2016, 陈登龙
* All rights reserved.
*
* 文件名称：MySplitterWnd.cpp
* 摘    要：本文件是为一款基于MFC的自定义窗口分割类的实现文件。
*
* 当前版本：1.0
* 作    者：陈登龙
* 完成日期：2016年10月14日
***************************************************************/
#include "stdafx.h"
#include "MySplitterWnd.h"

CMySplitterWnd::CMySplitterWnd(void)
{
}


CMySplitterWnd::~CMySplitterWnd(void)
{
}

BEGIN_MESSAGE_MAP(CMySplitterWnd, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void CMySplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//屏蔽左键按下消息
	CWnd::OnLButtonDown(nFlags, point);
}


void CMySplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//屏蔽鼠标移动消息
	CWnd::OnMouseMove(nFlags, point);
}
