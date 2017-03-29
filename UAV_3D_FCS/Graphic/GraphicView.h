#pragma once

#include "Graphic.h" 
#include <mutex>
#include <vector>
#include "../ComboBoxNoWheel.h"
// CGraphicView 对话框

//使用多媒体定时器要用到的库和头文件
#pragma comment(lib, "Winmm.lib")
#include <Mmsystem.h> 
#include "afxcmn.h"
#include "afxwin.h"
#include "../resource.h"


#define UM_TIMER_GRAPHIC_PROC (WM_USER + 1)


namespace GraphicView
{
	//飞行姿态
	const CString TSPI = _T("TSPI");

	//发动机1
	const CString ESD = _T("Engine");//_T("ESD");

	//发动机2
	const CString ESD2 = _T("Engine_2");//_T("ESD2");

	//告警
	const CString AMD = _T("Warning");//_T("AMD");

	//液油
	const CString LOSD = _T("HYD_Oil");//_T("LOSD");
	
	//航电
	const CString ASD = _T("AVS");

	//飞控
	const CString AFCSD = _T("FCS");

	//实时监控模式
	const int MonitoringModel = 1;
	//回放模式
	const int PlaybackModel = 0;

	//停止状态
	const int stop_state = 1;
	//播放状态
	const int play_state = 0;

	//通用定时时间ms
	const int Timer = 200;

	//回放模式模式定时时间ms
	const int PlaybackModelTimer = 50;

	//实时监控模式定时时间ms
	const int MonitoringModelTimer = 200;
}


class CGraphicView : public CDialogEx
{


	DECLARE_DYNAMIC(CGraphicView)

public:
	CGraphicView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGraphicView();

// 对话框数据
	enum { IDD = IDD_DIALOG_GRAPHIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP() 
	afx_msg LRESULT OnMyGriphicTimerProc(WPARAM wParam, LPARAM lParam);


public:
	typedef CString value_name;
	typedef Graphic::data_struct value_struct;

	typedef std::map<value_name, Graphic::data_scope_struct> data_scope_map;
	typedef std::pair<value_name, Graphic::data_scope_struct> data_scope_pair;

	//设置到曲线控件上的曲线数据队列
	Graphic::Chart_Data_Queue m_ChartDataDeque;

	//曲线控件
	CGraphic m_Graphic;

	virtual BOOL OnInitDialog();

private:
	//消息头
	MessageHeader m_MessageHeader; 

	//标记要更新的数据是否拿到
	bool isDataArriva = false;

	std::mutex m_mutex;

	//定时器ID
	UINT m_uTimerID = 0; 

	//存储单个数据点的曲线数据结构
	Graphic::Chart_Data m_rec_chart_data; 

	//存储数据范围的数据结构
	data_scope_map m_data_scope;

	//当前数据的显示模式
	int m_current_model;

	//标记曲线当前需要更新哪个数据包的数据，避免了关闭消息总线的缺点
	CString m_data_pkg_flag;
	 
private:

	//更新数据
	void UpdateTSPIDC(const TSPIDC& p_tspidc); 

private:
	//初始化曲线要更新的数据结构体
	void InitTSPIDCValueStruct(std::vector<CString>& p_param_vec, const TSPIDC& p_tspidc, value_struct& p_value_struct);
	 

private:
	//初始化数据点结构体
	void InitValueStruct(value_struct& p_value_struct, const value_name p_name, const Graphic::point_value_type& p_value);

private: 
	//根据当前选择的数据包初始化数据范围
	void InitDataScope(); 

	//初始化每个数据包范围
	void InitTSPIDataScope(); 
	 

private:
	//开启精确的定时器
	void StartEventTimer();

	//关闭精确的定时器
	void CloseEventTimer();

	//初始化消息总线
	void StartMessageBus();

	//关闭消息总线
	void CloseMessageBus();

private:
	//初始化数据包向量容器
	void InitFormVector(std::vector<CString>& p_form_vec);
	//初始化数据包变量向量容器
	void InitFormParamVector();

	//将数据包添加到组合框中
	void AddFormVecToComboForm(std::vector<CString>& m_form_param_vec);
	//将数据包中的变量添加到变量列表框中
	void AddFormParamVecToComboFormParam(std::vector<CString>& p_form_param_vec); 

	//更新数据包列表框
	void UpdataFormCombo();  
	//更新数据包变量列表框
	void UpdataFormParamCombo(); 

private:
	//向变量容器中添加变量名称
	void AddTSPIParamToParamVec();  
	 

private:
	//更新曲线图
	void UpdateGraphic();

	//更新Y轴范围编辑框
	void UpdateYScopeEdit();

	//更新到达的时间和值
	void UpdataArriveTimeAndValue(double p_cur_time, double p_cur_value);

	//更新到达的时间和值的重载
	void CGraphicView::UpdataArriveTimeAndValue(Graphic::time_struct arrive_time, double p_cur_value);

private:
	//得到最小范围
	double GetMinScope(const CString p_current_select_param) { return (*m_data_scope.find(p_current_select_param)).second.min_scope; }

	//得到最大范围
	double GetMaxScope(const CString p_current_select_param) { return (*m_data_scope.find(p_current_select_param)).second.max_scope; }

	//得到当前数据单位
	CString GetCurDataUnit(const CString p_current_select_param) { return (*m_data_scope.find(p_current_select_param)).second.data_unit; }


public:
	//初始化曲线图
	void InitGraphic(); 

	//初始化两个按钮
	void InitTwoBtn();

	//设置当前显示模式
	void SetCurentModel(const int p_model) { m_current_model = p_model; }

	//设置当前到达时间和到达值，用于回放暂停时鼠标的移动消息
	void SetCurrentTimeAndValue(double p_cur_time, double p_cur_value) { UpdataArriveTimeAndValue(p_cur_time, p_cur_value); }

	//得到当前曲线图的数据显示模式
	int GetCurrentModel() { return m_current_model; }

	//设置当前显示状态
	void SetCurShowState(const int p_cur_state){ m_cur_state = p_cur_state; }
	 
private:
	// 滑动条
	//CSliderCtrl m_SliderZoom;

	// 暂停按钮
	CButton m_BtnPause;

	// 用于回放模式更新min和max范围
	CButton m_BtnUpdataParam;

	// 数据包选择框
	CComboBoxNoWheel m_SelectForm;

	// 数据包中具体变量选择框
	CComboBoxNoWheel m_SelectFormParam; 

	// Y轴最大范围
	CEdit m_y_max_edit;

	// Y轴最小范围
	CEdit m_y_min_edit; 

	// 显示到达时间的编辑框
	CEdit m_edit_arrive_time;

	// 显示到达值的编辑框
	CEdit m_edit_arrive_value;

	//存储当前选择的数据包的名称
	CString m_current_select_form;

	//存储当前选择的数据包中的参数
	CString m_current_select_param;
	
	//存储数据包名称的向量容器
	std::vector<CString> m_form_vec;

	//存储数据包中变量名称的向量容器
	std::vector<CString> m_form_param_vec; 

	//存储数据包中具体变量类型和对应值的结构体的容器
	std::map<value_name, value_struct> m_form_param_map;
	 
	//当前数据回放的状态，停止还是继续
	int m_cur_state;
protected:
	//使用了多种定时器，暂时不使用这种回调
	static void CALLBACK TimerCallBack(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
	{
		CGraphicView* pThis = (CGraphicView*)dwUser;
		pThis->OnMyTimerCallBack();
	}
public:
	void OnMyTimerCallBack();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedPause(); 
	afx_msg void OnBnClickedUpdataParam();
	afx_msg void OnCbnSelchangeComboTable();
	afx_msg void OnCbnSelchangeComboTablePara();
	afx_msg void OnNMReleasedcaptureSliderZoom(NMHDR *pNMHDR, LRESULT *pResult);  
public: 
	afx_msg void OnStnClickedGraphic();
};
void _stdcall TimeGriphicProc(UINT uTimerID, UINT uMessage, DWORD dwUser, DWORD dw1, DWORD dw2);

 