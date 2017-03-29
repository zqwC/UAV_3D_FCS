#pragma once
#include"../resource.h"

// CMirrorDlg 对话框

class CMirrorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMirrorDlg)

public:
	CMirrorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMirrorDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MIRROR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
