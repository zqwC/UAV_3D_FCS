/***************************************************************
* Copyright (c) 2016, 陈登龙
* All rights reserved.
*
* 文件名称：MySplitterWnd.h
* 摘    要：本文件是为一款基于MFC的自定义窗口分割类头文件。
* 第一版本完成了屏蔽鼠标左键按下、鼠标移动消息的功能。
*
* 当前版本：1.0
* 作    者：陈登龙
* 完成日期：2016年10月14日
***************************************************************/
#pragma once
#include "afxext.h"// Graphic.cpp : 实现文件


class CMySplitterWnd :
	public CSplitterWnd
{
public:
	CMySplitterWnd(void);
	~CMySplitterWnd(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

