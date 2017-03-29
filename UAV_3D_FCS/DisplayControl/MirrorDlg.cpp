// MirrorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../UAV_3D_FCS.h"
#include "MirrorDlg.h"
#include "afxdialogex.h"


// CMirrorDlg 对话框

IMPLEMENT_DYNAMIC(CMirrorDlg, CDialogEx)

CMirrorDlg::CMirrorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMirrorDlg::IDD, pParent)
{

}

CMirrorDlg::~CMirrorDlg()
{
}

void CMirrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMirrorDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMirrorDlg 消息处理程序


void CMirrorDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}
