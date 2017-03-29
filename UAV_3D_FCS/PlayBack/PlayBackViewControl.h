#pragma once


#include "../Graphic/GraphicView.h"
// CPlayBackViewControl 窗体视图

class CPlayBackViewControl : public CFormView
{
	DECLARE_DYNCREATE(CPlayBackViewControl)

protected:
	CPlayBackViewControl();           // 动态创建所使用的受保护的构造函数
	virtual ~CPlayBackViewControl();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLAY_BACK };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	CGraphicView m_GraphicView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnGraphic();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
};


