#pragma once

#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "GridCtrl/GridCtrl.h"
#include "GridCtrl/CMyGridCtrl.h"
#include "Server/ServerSQLOperation.h"
#include "DisplayControl/OwnSliderCtrl.h"
#include "ProcessDlg.h"

// CTimesDlg 对话框

using namespace boost::xpressive;
/*class CTimeFormat
{
public:
	CTimeFormat(void);
	~CTimeFormat(void);
	static BOOL IsLeapYear(INT year);
	static DWORD YMDHMSToStartDay(SYSTEMTIME time);
	static DWORD YMDHMSToEndDay(SYSTEMTIME time);
	static DWORD YEndToYStart(INT year_start, INT year_end);
	static Time HMSAddHMS(Time t1, Time t2);
	static Time HMSSubHMS(Time t1, Time t2);
	static void YMDHMSToTime(Time &time, SYSTEMTIME start_time, SYSTEMTIME end_time);
	static DWORD YMDHMSToSecond(SYSTEMTIME start_time, SYSTEMTIME end_time);
	static DWORD HMSToSecond(DWORD hour, DWORD minute, DWORD second);
	static DWORD HMSToSecond(Time time);
	static void SecondToHMS(DWORD &hour, DWORD &minute, DWORD &second, DWORD srcsecond);
	static void SecondToHMS(Time &time, DWORD src_second);

	static void MillSecondToHMSMS(DWORD &hour, DWORD &minute, DWORD &second, DWORD &mill_second, DWORD src_mill_second);
	static void MillSecondToHMSMS(Time & time, DWORD src_mill_second);
	static void HMSMSToMillSecond(DWORD &mill_sercond, Time time);
	static void HMSMSToMillSecond(DWORD &mill_sercond, DWORD hour, DWORD minute, DWORD second, DWORD mill_second);
	static DWORD HMSMSToMillSecond(Time time);
	static DWORD HMSMSToMillSecond(DWORD hour, DWORD minute, DWORD second, DWORD mill_second);

	static DWORD HMSToSecond(DOUBLE hour, DOUBLE minute, DOUBLE second);
	static void SecondToHMS(DWORD &hour, DWORD &minute, DWORD &second, DOUBLE srcsecond);
	static void SecondToHMS(Time &time, DOUBLE srcsecond);
	static void MillSecondToHMSMS(DWORD &hour, DWORD &minute, DWORD &second, DWORD &mill_second, DOUBLE src_mill_second);
	static void MillSecondToHMSMS(Time & time, DOUBLE src_mill_second);
};*/
class CTimesDlg : public CDialogEx
{
#define START_DATE_YEAR		2
#define START_DATE_MONTH	3
#define START_DATE_DAY		4
#define START_TIME_HOUR		5
#define START_TIME_MINUTE	6
#define START_TIME_SECOND	7

#define END_DATE_YEAR		8
#define END_DATE_MONTH		9
#define END_DATE_DAY		10
#define END_TIME_HOUR		11
#define END_TIME_MINUTE		12
#define END_TIME_SECOND		13
	DECLARE_DYNAMIC(CTimesDlg)

public:
	CTimesDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTimesDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TIMES_VIEW };


	/*********************接口函数**************************/
	//设置标题
	VOID SetTitle(vector<CString> titles);
	//设置连接字符串
	VOID SetConnectionString(CString strConnection);
	//设置数据库账号
	VOID SetUserString(CString strUser);
	//设置数据库密码
	VOID SetPasswordString(CString strPassword);
	//设置数据库名称
	VOID SetDatabaseString(CString strDatabase);
	//获取数据库名称
	VOID GetDatabaseString(CString &strDatabase);
	CString GetDatabaseString();
	//设置远程数据库IP
	VOID SetDatabaseIPString(CString strDatabaseIP);
	//设置远程数据库端口
	VOID SetDatabasePortString(CString strDatabasePort);
	//获得连接字符串
	CString GetConnectinString();
	//获得数据库账号
	CString GetUserStirng();
	//获得数据库密码
	CString GetPasswordString();
	//设置文本内容
	VOID SetTexts(vector<CString> texts);
	//获得选择的飞机编号
	CString GetSelectedID();
	//获得选择的表格名
	CString GetSelectedTable();
	CString GetDatabaseIPString();
	CString GetDatabasePortString();
	//获得数据库中的数据
	void GetTableDatas(vector<CString> &datas);
	//获得选定的播放时间段
	DISPLAY_INFO GetDisplayInfo();
	//获得选定的表格的具体播放时间段
	DISPLAY_INFO GetRealDisplayInfo();
	//获得选定的表格的具体播放时间
	VOID GetRealRecordTime(string &tar_start_time, string &tar_end_time, CString table_name);
	VOID GetRealRecordTime(Time &tar_start_time, Time &tar_end_time, CString table_name);
	VOID GetRealRecordTime(DWORD &tar_start_time, DWORD &tar_end_time, CString table_name);
	//判断是否有选择
	BOOL HasSelected();
	VOID SetSlelected(BOOL bSelected);
	//解析数据
	VOID AnalyseSqlData(Time &time, string strJson);
	//初始化时间
	VOID InitTime(Time &time);
	/********************************************************/
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//数据库名称
	CString m_strDatabase;
	//远程数据库IP地址
	CString m_strDatabaseIP;
	//远程数据库端口
	CString m_strDatabasePort;
	//数据库连接字符串
	CString m_strConnection;
	//数据库账号
	CString m_strUser;
	//数据库密码
	CString m_strPassword;
	//列表标题
	vector<CString> m_Titles;
	//表名列标题
	vector<CString> m_TablesTitles;
	//列表行数
	UINT m_uRowNums;
	//表名列表行数
	UINT m_uTableRowNums;
	//行高
	UINT m_RowHeight;
	//标题背景色
	COLORREF m_TitleBkColor;
	//日期动态列表
	CGridCtrl *m_Grid;
	//表名动态列表
	CMyGridCtrl *m_GridCtrl;
	//数据库操作对象
	SQLOperation m_Sql;
	//原始表名
	vector<CString> m_TableNames;
	//非重复时间
	vector<CString> m_UniqueDateTimes;
	//显示的最终文本
	vector<CString> m_Texts;
	//选择的回放表名
	CString m_SelectedTableName;
	//选择的回放飞机编号
	CString m_SelectedID;
	//表格名匹配
	cregex m_TableMatch;
	//是否有选择
	BOOL m_bSelected;
	//选择的日期时间编号
	INT m_SelectedDateTimeIndex;
	//具体时间
	vector<SYSTEMTIME> m_StartTimes;
	vector<SYSTEMTIME> m_EndTimes;
	//GRID颜色
	COLORREF m_GridBkColor;

	CProcessDlg m_ProcessDlg;

	//初始化列表
	BOOL InitTimesGrid();
	//初始化表名列表
	BOOL InitTablesGrid();
	//获取所有表格并存入容器
	BOOL GetTableNamesFromSql(vector<CString> &table_names);
	//单表名到单日期时间
	BOOL TableNameToDateTime(CString str_table_name, SYSTEMTIME &start_time, SYSTEMTIME &end_time);
	//单日期到单时间字符串
	VOID DateTimeToString(SYSTEMTIME start_time, SYSTEMTIME end_time, CString &tar_str);
	//多日期到多时间字符串
	VOID DateTimesToStrings(vector<SYSTEMTIME> start_times, vector<SYSTEMTIME> end_times, vector<CString> &tar_strs);
	//单时间字符串到单日期
	BOOL StringToDateTime(CString src_str, SYSTEMTIME &start_time, SYSTEMTIME &end_time);
	//多时间字符串到多日期
	BOOL StringsToDateTimes(vector<CString> src_strs, vector<SYSTEMTIME> &start_times, vector<SYSTEMTIME> &end_times);
	//多表名到多日期时间
	VOID TableNamesToDateTimes(vector<CString> str_table_names, vector<SYSTEMTIME> &start_times, vector<SYSTEMTIME> &end_times);
	//时间字符串去重函数
	VOID DeleteSameStrings(vector<CString> src_strs, vector<CString> &tar_strs);
	//表名提取时间去重
	BOOL DeleteSameTimeInTableNames(vector<CString> src_str_table_names, vector<CString> &tar_strs);
	//获得单表格名的日期部分
	BOOL GetDateTimeInTableName(CString str_table_name, CString &str_date_time);
	//获得指定日期时间的表名
	VOID GetSpecifiedTableNames(vector<CString> src_table_names, CString src_date_time, vector<CString> &tar_table_names);
	//将日期转换为最终显示文本
	VOID DateTimesToTexts(vector<SYSTEMTIME> start_times, vector<SYSTEMTIME> end_times, vector<CString> &texts);
	//将显示文本插入到时间指定列
	VOID InsertIntoSpecifiedCol(INT index);
	//获得指定时间的所有表格名
	VOID GetTableNamesByDateTimes(SYSTEMTIME start_time, SYSTEMTIME end_time, vector<CString> &tar_strs);
	//获得指定时间的所有ID号
	VOID GetTableNamesByDateTimes(CString date_time, vector<CString> &tar_ids);
	//将显示文本插入到表格
	VOID InsertIntoSpecifiedTableCol(vector<CString> str_ids, CString str_date_times);
	VOID InsertIntoSpecifiedTableCol(vector<CString> texts);
	//判断表是否存在
	BOOL IsTableExist(CString table_name, CString database);
	//判断列名是否存在
	BOOL IsColExist(CString col_name, CString table_name, CString database);
	//已知飞机编号和日期时间求表名
	VOID GetTableNameByIDAndDateTime(vector<CString> &tar_table_names, vector<CString> src_ids, CString src_date_time);
	//获得选择的项
	VOID GetSelectedItem();
	//主更新函数
	BOOL Update();
	//表名显示格式化
	VOID FormatTableName(CString str_table_name, CString &str_format_table_name);
	DECLARE_MESSAGE_MAP()
public:
	//数据库索引名
	vector<CString> m_SqlColNames;
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};