
// MessageSourceDlg.h : 头文件
//

#pragma once
#include "protocol/FCToPCProtocolConvertToRealData.h"
#include "protocol/FCToPCProtocolStruct.hpp"
#include "kapok/Serializer.hpp"
#include "TcpServer.h"


#define TIMER_SEND	0

// CMessageSourceDlg 对话框
class CMessageSourceDlg : public CDialogEx
{
// 构造
public:
	CMessageSourceDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MESSAGESOURCE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	unsigned int m_ID;				 //飞机编号  3、4
	unsigned int m_DID;				 //目的编号  5、6
	double m_LOTD;                   //即时经度  7、8、9、10
	double m_LATD;                   //即时纬度  11、12、13、14
	double m_SPH;                    //标准气压高度  15、16
	double m_PRH;                    //气压修正高度  17、18
	double m_RH;                     //无线电高度  19、20
	double m_VS;                     //真空速  21、22
	double m_IVS;                    //指示空速  23、24
	double m_TH;                     //真航向  25、26
	double m_ANGP;                   //俯仰角  27、28
	double m_ANGR;                   //横滚角  29、30
	double m_ANGPS;                  //俯仰角速率  31、32
	double m_ANGRS;                  //横滚角速率  33、34

	CString m_strMsg;
	DWORD m_dwTimInterval;

	TSPI m_TSPI;
	TSPIDC m_TSPIDC;

	CTcpServer m_TcpServer;
	Serializer m_Sr;

	bool m_bState;

	int	m_ixoldpos;
	int m_iyoldpos;
// 实现
protected:
	HICON m_hIcon;
	void SetScroll(CRect page, CRect range);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSendSpecifiedMsg();
	afx_msg void OnBnClickedButtonSendGenerateMsg();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonStatus();
	afx_msg void OnBnClickedButtonSetIpPort();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnKillfocusEditTimeInterval();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
public:
	void SendTSPIMessage();
};
