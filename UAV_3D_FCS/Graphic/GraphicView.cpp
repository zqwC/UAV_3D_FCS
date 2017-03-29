// GraphicView.cpp : 实现文件
//

#include "stdafx.h"
#include "../stdafx.h" 
#include "GraphicView.h"
#include "afxdialogex.h"
#include "../resource.h"


// CGraphicView 对话框

extern MessageBus g_Bus;
extern CRITICAL_SECTION g_csA;

IMPLEMENT_DYNAMIC(CGraphicView, CDialogEx)

CGraphicView::CGraphicView(CWnd* pParent /*=NULL*/)
: CDialogEx(CGraphicView::IDD, pParent),
m_current_select_form(""),
m_current_select_param(""), 
m_current_model(GraphicView::MonitoringModel),
m_cur_state(GraphicView::stop_state)//默认打开是停止状态，曲线不更新数据
{
	
	//初始化曲线数据队列数据包为0
	memset(&m_ChartDataDeque, 0, sizeof(m_ChartDataDeque));

	//初始化接收曲线的单个临时数据包
	memset(&m_rec_chart_data, 0, sizeof(m_rec_chart_data)); 
}

CGraphicView::~CGraphicView()
{  
	//先关闭定时器,防止暂停时重复关闭定时器
	//CloseEventTimer();
	 
	//再关闭消息总线
	CloseMessageBus();
}

void CGraphicView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPHIC, m_Graphic);
	//DDX_Control(pDX, IDC_SLIDER_ZOOM, m_SliderZoom);
	DDX_Control(pDX, IDC_COMBO_TABLE, m_SelectForm);
	DDX_Control(pDX, IDC_COMBO_TABLE_PARA, m_SelectFormParam);
	DDX_Control(pDX, IDC_EDIT_Y_MIN, m_y_min_edit);
	DDX_Control(pDX, IDC_EDIT_Y_MAX, m_y_max_edit);
	DDX_Control(pDX, IDC_EDIT_ARRIVE_TIME, m_edit_arrive_time);
	DDX_Control(pDX, IDC_EDIT_ARRIVE_VALUE, m_edit_arrive_value);
	DDX_Control(pDX, IDC_BTN_PAUSE, m_BtnPause);
	DDX_Control(pDX, IDC_BTN_UPDATA_PARAM, m_BtnUpdataParam);
}


BEGIN_MESSAGE_MAP(CGraphicView, CDialogEx)
	ON_MESSAGE(UM_TIMER_GRAPHIC_PROC, OnMyGriphicTimerProc)
	//ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_ZOOM, &CGraphicView::OnNMReleasedcaptureSliderZoom)
	ON_CBN_SELCHANGE(IDC_COMBO_TABLE, &CGraphicView::OnCbnSelchangeComboTable)
	ON_CBN_SELCHANGE(IDC_COMBO_TABLE_PARA, &CGraphicView::OnCbnSelchangeComboTablePara)
	ON_WM_TIMER()
	//ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CGraphicView::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BTN_UPDATA_PARAM, &CGraphicView::OnBnClickedUpdataParam)
	ON_STN_CLICKED(IDC_GRAPHIC, &CGraphicView::OnStnClickedGraphic)
END_MESSAGE_MAP()


// CGraphicView 消息处理程序

BOOL CGraphicView::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// TODO:  在此添加额外的初始化  
	  

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void _stdcall TimeGriphicProc(UINT uTimerID, UINT uMessage, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	::PostMessage((HWND)dwUser, UM_TIMER_GRAPHIC_PROC, 0, 0);
}
 

void  CGraphicView::OnMyTimerCallBack()
{
	if (isDataArriva)
	{
		isDataArriva = false;
		m_Graphic.SetCurrentData_One_Package(m_rec_chart_data);
	}
}


LRESULT CGraphicView::OnMyGriphicTimerProc(WPARAM wParam, LPARAM lParam)
{
	//std::lock_guard<std::mutex>  locker(m_mutex);
	//更新曲线，每次传输一个队列的数据，需要知道曲线控件是否更新完毕，才能发送下一个曲线队列数据

	//200ms发送给曲线控件更新
	if (isDataArriva)
	{
		isDataArriva = false;
		
		//确保当前发送的数据点结构体是正确初始化过的，如果这个结构体被正确初始化过，则结构体中的颜色属性一定不为0，必须为红色或者绿色
		//if (m_rec_chart_data.second.attributes.color != 0)
		//设置新的标记来判断当前数据点结构体是否已经被成功初始化
		if (m_rec_chart_data.second.attributes.hasInit)
		{
			//更新到达时间和到达的值
			//UpdataArriveTimeAndValue(m_rec_chart_data.second.base_data.arrive_time, m_rec_chart_data.second.base_data.arrive_value);
			//UpdataArriveTimeAndValue(m_rec_chart_data.second.base_data, m_rec_chart_data.second.base_data.arrive_value);

			//发的可能是前一个选择的数据字的数据结构体，导致前面的数据可能会乱
			//要保证选择了新的数据字后发送的数据是新的数据值的数据
			m_Graphic.SetCurrentData_One_Package(m_rec_chart_data);
		} 
	}
	
	return 0;
}

 



/*********************************************************
函数名   : UpdateTSPIDC
功能     : 从消息总线拿到飞行姿态数据
参数     :	[IN]	: bsddc 发送过来的飞控数据包引用
			[OUT]	: 无
返回值   : 无
备注     :
时间     : 2016/8/8 陈登龙
**********************************************************/
void CGraphicView::UpdateTSPIDC(const TSPIDC& p_tspidc)
{
	if (GraphicView::TSPI == m_data_pkg_flag)
	{
		std::lock_guard<std::mutex> locker(m_mutex);

		//处理飞控数据包中的一个数据, 转换成曲线数据包格式
		memset(&m_rec_chart_data, 0, sizeof(m_rec_chart_data));

		value_struct send_value;
		memset(&send_value, 0, sizeof(send_value));;

		//初始化数据点结构体
		InitTSPIDCValueStruct(m_form_param_vec, p_tspidc, send_value);

		//接收数据结构体到成员变量，等待定时器发送
		m_rec_chart_data.second = send_value;

		//数据到来
		isDataArriva = true;
	}
}


/*****************************************************************
函数名   :	InitTSPIDCValueStruct
功能     :	初始化飞行姿态数据点结构体
参数     :	[IN]	: p_param_vec 存储飞控变量名称的Vec的引用
					: p_afcsddc 从消息总线收到的飞控数据包的引用

			[OUT]	: p_value_struct 返回初始化后的点的结构体
返回值   :	无
备注     :  14个飞行姿态数据字加完
时间     :	2016/8/12 陈登龙
******************************************************************/
void CGraphicView::InitTSPIDCValueStruct(std::vector<CString>& p_param_vec, const TSPIDC& p_tspidc, value_struct& p_value_struct)
{
	//得到消息头
	m_MessageHeader = p_tspidc.m_MessageHeader;
	int prarm_vec_index = 0;
	if (m_current_select_param == p_param_vec.at(prarm_vec_index++))InitValueStruct(p_value_struct, m_current_select_param, p_tspidc.m_ID);
	if (m_current_select_param == p_param_vec.at(prarm_vec_index++))InitValueStruct(p_value_struct, m_current_select_param, p_tspidc.m_DID);

	if (m_current_select_param == p_param_vec.at(prarm_vec_index++))InitValueStruct(p_value_struct, m_current_select_param, p_tspidc.m_LOTD);//即时经度
	if (m_current_select_param == p_param_vec.at(prarm_vec_index++))InitValueStruct(p_value_struct, m_current_select_param, p_tspidc.m_LATD);//即时纬度

	if (m_current_select_param == p_param_vec.at(prarm_vec_index++))InitValueStruct(p_value_struct, m_current_select_param, p_tspidc.m_SPH);
	if (m_current_select_param == p_param_vec.at(prarm_vec_index++))InitValueStruct(p_value_struct, m_current_select_param, p_tspidc.m_PRH);
	if (m_current_select_param == p_param_vec.at(prarm_vec_index++))InitValueStruct(p_value_struct, m_current_select_param, p_tspidc.m_RH);
	if (m_current_select_param == p_param_vec.at(prarm_vec_index++))InitValueStruct(p_value_struct, m_current_select_param, p_tspidc.m_VS);
	if (m_current_select_param == p_param_vec.at(prarm_vec_index++))InitValueStruct(p_value_struct, m_current_select_param, p_tspidc.m_IVS);
	if (m_current_select_param == p_param_vec.at(prarm_vec_index++))InitValueStruct(p_value_struct, m_current_select_param, p_tspidc.m_TH);
	if (m_current_select_param == p_param_vec.at(prarm_vec_index++))InitValueStruct(p_value_struct, m_current_select_param, p_tspidc.m_ANGP);
	if (m_current_select_param == p_param_vec.at(prarm_vec_index++))InitValueStruct(p_value_struct, m_current_select_param, p_tspidc.m_ANGR);
	if (m_current_select_param == p_param_vec.at(prarm_vec_index++))InitValueStruct(p_value_struct, m_current_select_param, p_tspidc.m_ANGPS);
	if (m_current_select_param == p_param_vec.at(prarm_vec_index++))InitValueStruct(p_value_struct, m_current_select_param, p_tspidc.m_ANGRS);



}
       


/***************************************************************
函数名   : StartMessageBus
功能     : 初始化消息总线，拿到数据
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   : 无
备注     : 应该在选择了组合框中的选项时才接收数据,
		   每次只更新当前标记m_data_pkg_flag的数据包中的数据
时间     : 2016/8/17 陈登龙
****************************************************************/
void CGraphicView::StartMessageBus()
{    
	if (GraphicView::TSPI == m_current_select_form) g_Bus.Attach([this](const TSPIDC& tspidc){ UpdateTSPIDC(tspidc); }, "GraphicTSPIDC");
	 
}



/*********************************************************
函数名   : CloseMessageBus
功能     : 关闭消息总线
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   :	无
备注     : 在关闭曲线对话框时，应该禁止曲线对话框接收数据
时间     : 2016/8/17 陈登龙
**********************************************************/
void CGraphicView::CloseMessageBus()
{  
	g_Bus.Remove<void, const TSPIDC&>("GraphicTSPIDC"); 
}




/*********************************************************
函数名   :	StartEventTimer
功能     :	开启精确的200ms定时器
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   : 无
备注     :  
时间     : 2016/8/8 陈登龙
**********************************************************/
void CGraphicView::StartEventTimer()
{
	//开启200ms定时器
	if (timeBeginPeriod(20) == TIMERR_NOERROR)
	{ 
		//使用消息队列
		//m_uTimerID = timeSetEvent(200, 1, (LPTIMECALLBACK)TimeGriphicProc, (DWORD)GetSafeHwnd(), TIME_PERIODIC);
		
		//使用回调函数
		//m_uTimerID = timeSetEvent(200, 1, (LPTIMECALLBACK)TimerCallBack, (DWORD)GetSafeHwnd(), TIME_PERIODIC);
		//m_uTimerID = timeSetEvent(200, 1, (LPTIMECALLBACK)TimerCallBack, (DWORD)this, TIME_PERIODIC);
	}

	//普通定时器
	SetTimer(m_uTimerID, GraphicView::Timer, nullptr);


	//SetTimer(m_uTimerID, GraphicView::PlaybackModelTimer, nullptr);

	//SetTimer(m_uTimerID, GraphicView::MonitoringModelTimer, nullptr);
}

/*********************************************************
函数名   :	CloseEventTimer
功能     :	关闭精确的200ms定时器
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   : 无
备注     :
时间     : 2016/8/17 陈登龙
**********************************************************/
void CGraphicView::CloseEventTimer()
{
	//timeKillEvent(m_uTimerID);
	KillTimer(m_uTimerID);
}



/*********************************************************
函数名   :	OnNMReleasedcaptureSliderZoom
功能     :	滑动条事件处理
参数     :	[IN]	: 无
			[OUT]	:	pNMHDR 
						pResult
返回值   :	无
备注     :	滑动滑动条时调用
时间     :	2016/8/12 陈登龙
**********************************************************/
void CGraphicView::OnNMReleasedcaptureSliderZoom(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;

	 
	
}




/*********************************************************
函数名   :	OnCbnSelchangeComboTable
功能     :	数据包列表框事件处理
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   :	无
备注     :	选择具体数据包时调用
			增加数据到来标记重置，防止数据显示错乱
时间     :	2016/8/20 陈登龙
修改	 :	2016/11/24 选择了下拉框条目后进行背景的重绘
**********************************************************/
void CGraphicView::OnCbnSelchangeComboTable()
{
	// TODO:  在此添加控件通知处理程序代码 

	//在更新新的数据之前，设置数据到达标记为Flase，表示当前选择的数据字的数据没有到达，否则如果数据到达标记仍然为TRUE，会显示上一个数据字的数据
	isDataArriva = false;
	
	
	m_SelectForm.GetLBText(m_SelectForm.GetCurSel(), m_current_select_form);   


	//更新变量列表框
	UpdataFormParamCombo();

	//更新后得到选择的第一个变量名称
	m_SelectFormParam.GetLBText(m_SelectFormParam.GetCurSel(), m_current_select_param);

	//初始化所选数据包的数据范围
	InitDataScope();

	//变量下拉列表框重绘
	m_SelectFormParam.Invalidate();

	//重新初始化两个按钮
	InitTwoBtn();
	 

	//更新Y轴范围
	UpdateYScopeEdit();

	//更新曲线
	UpdateGraphic();

	//需要重绘背景
	m_Graphic.ReDraw();
}

/*********************************************************
函数名   :	OnCbnSelchangeComboTablePara
功能     :	变量列表框事件处理
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   :	无
备注     :	选择具体变量时调用
			增加数据到来标记重置，防止数据显示错乱
时间     :	2016/8/20 陈登龙
修改	 :	2016/11/24 选择了下拉框条目后进行背景的重绘
**********************************************************/
void CGraphicView::OnCbnSelchangeComboTablePara()
{
	// TODO:  在此添加控件通知处理程序代码 
	//在更新新的数据之前，设置数据到达标记为Flas
	//表示当前选择的数据字的数据没有的到达，否则如果数据到达标记仍然为TRUE
	//定时器一直开启,会显示上一个数据字的数据
	isDataArriva = false;

	//得到选择的具体变量名称
	m_SelectFormParam.GetLBText(m_SelectFormParam.GetCurSel(), m_current_select_param); 

	//更新Y轴范围
	UpdateYScopeEdit();
	
	//重新初始化两个按钮
	InitTwoBtn(); 

	//更新曲线
	UpdateGraphic(); 

	//需要重绘背景
	m_Graphic.ReDraw();
}



/***********************************************************************
函数名   :	InitFormVector
功能     :	初始化存储数据包名称的vec
参数     :	[IN]	: p_form_vec&	存储数据包名称的vec
			[OUT]	: 无
返回值   :	无
备注     :	加上了全部的7个数据包名，调整了包名的顺序，与协议保持一致
时间     :	2016/8/20 陈登龙
************************************************************************/
void CGraphicView::InitFormVector(std::vector<CString>& p_form_vec)
{
	p_form_vec.clear();
	p_form_vec.reserve(10);

	//添加7个包名到第一个数据包下拉列表框，按顺序添加，下拉列表框取消了自动排序
	p_form_vec.push_back(GraphicView::TSPI);  
	 

}


/*********************************************************
函数名   :	AddFormVecToComboForm
功能     :	添加数据包名称到m_SelectForm下拉列表框中
参数     :	[IN]	: p_form_vec&	存储数据包名称的vec
			[OUT]	: 无
返回值   :	无
备注     :
时间     :	2016/8/13 陈登龙
**********************************************************/
void CGraphicView::AddFormVecToComboForm(std::vector<CString>& p_form_vec)
{
	//添加包名到Form列表框
	m_SelectForm.ResetContent();
	for (auto pkg : p_form_vec)
	{
		m_SelectForm.AddString(pkg);
	}
}
 

/*********************************************************
函数名   :	InitFormParamVector
功能     :	初始化变量列表框
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   :	无
备注     :	在此函数中确定当前曲线要更新的数据所属的包名
时间     :  2016/8/17 陈登龙
**********************************************************/
void CGraphicView::InitFormParamVector()
{ 
	if (GraphicView::TSPI == m_current_select_form) 
	{
		m_data_pkg_flag = GraphicView::TSPI;
		AddTSPIParamToParamVec();
	} 
}



/**********************************************************
函数名   :	AddFormParamVecToComboFormParam
功能     :	添加变量名称到m_SelectFormParam下拉列表框中
参数     :	[IN]	: p_form_param_vec&	存储变量名称的vec
			[OUT]	: 无
返回值   :	无
备注     :	 
时间     :	2016/8/13 陈登龙
**********************************************************/
void CGraphicView::AddFormParamVecToComboFormParam(std::vector<CString>& p_form_param_vec)
{ 
	//添加前先清空 
	m_SelectFormParam.ResetContent();
	//添加包名到Form列表框
	for (auto param_name : p_form_param_vec)
	{
		m_SelectFormParam.AddString(param_name);
	}
}
 


/*************************************************************************
函数名   :	InitGraphic
功能     :	初始化曲线
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   :	无
备注     :	用于启动曲线图时的控件重绘和基本初始化，按钮初始置为Pause
时间     : 2016/8/12 陈登龙
**************************************************************************/
void CGraphicView::InitGraphic()
{   
	//清除上次退出的残余曲线图,并重绘
	//m_Graphic.CleanQueueData(); 
	//m_Graphic.ReconstructControl();

	UpdataFormCombo();

	//得到数据包列表框中第一个数据包名称
	m_SelectForm.GetLBText(m_SelectForm.GetCurSel(), m_current_select_form);

	UpdataFormParamCombo();

	//得到变量列表框中第一个变量名称
	m_SelectFormParam.GetLBText(m_SelectFormParam.GetCurSel(), m_current_select_param); 

	//初始化两个按钮
	InitTwoBtn();


	//初始化第一个数据包中的所有数据范围
	InitDataScope();  

	//更新Y轴范围
	UpdateYScopeEdit();

	//开启定时器
	StartEventTimer();  

	//初始更新一次曲线
	UpdateGraphic();

	
}


/*************************************************************************
函数名   :	InitTwoBtn
功能     :	初始化两个按钮
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   :	无
备注     :	两个按钮：回放模式可用，实时监控模式按钮不可用
时间     : 2016/8/17 陈登龙
**************************************************************************/
void CGraphicView::InitTwoBtn()
{
	CString str("");
	if ((GetDlgItemText(IDC_BTN_PAUSE, str), str) == _T("Resume"))
	{
		//上一次是暂停状态就重启定时器
		StartEventTimer();
	}  

	//不管是回放模式还是实时监控模式，初始按钮显示Pause
	SetDlgItemText(IDC_BTN_PAUSE, _T("Pause"));

	if (GraphicView::PlaybackModel == m_current_model)
	{
		//回放模式显示暂停按钮
		//m_BtnPause.ShowWindow(SW_SHOW);
		m_BtnPause.EnableWindow(TRUE);
		m_BtnPause.Invalidate();

		//回放模式可以更改min和max范围
		//m_BtnUpdataParam.ShowWindow(SW_SHOW);
		m_BtnUpdataParam.EnableWindow(TRUE);
		m_BtnUpdataParam.Invalidate();
	}
	else
	{
		//实时监控模式不可以暂停-直接不显示暂停按钮
		//m_BtnPause.ShowWindow(SW_HIDE);
		//实时监控模式不可以暂停-直接禁用暂停按钮
		m_BtnPause.EnableWindow(FALSE);
		m_BtnPause.Invalidate();

		//实时监控不可以更改min和max范围-直接不显示这个按钮
		//m_BtnUpdataParam.ShowWindow(SW_HIDE);

		//实时监控不可以更改min和max范围-直接禁用这个按钮
		m_BtnUpdataParam.EnableWindow(FALSE);
		m_BtnUpdataParam.Invalidate();
	}


	
}


/***************************************************************************************
函数名   :	InitValueStruct
功能     :	初始化曲线上存储每个点数据的结构体
参数     :	[IN]	:	p_name			要初始化的变量名称，该变量是具体数据包中的变量
						p_value			要初始化的变量的值  
			[OUT]	:	p_value_struct	要初始化数据点结构体的引用
返回值   :	无
备注     :	时间没有换成实际发送时间，测试使用本地时间
时间     :	2016/8/12 陈登龙

修改时间 : 2016/11/18
****************************************************************************************/
void CGraphicView::InitValueStruct(value_struct& p_value_struct, const value_name p_name, const Graphic::point_value_type& p_value)
{ 
	//初始化值
	p_value_struct.base_data.arrive_value = p_value; 

	//修改数据源时间
	//p_value_struct.base_data.arrive_time = m_MessageHeader.hour * 3600.0 + m_MessageHeader.minute * 60.0 + m_MessageHeader.second + m_MessageHeader.millSecond / 1000.0; 
	p_value_struct.base_data.arrive_time.hour = m_MessageHeader.hour;
	p_value_struct.base_data.arrive_time.min = m_MessageHeader.minute;
	p_value_struct.base_data.arrive_time.sec = m_MessageHeader.second;
	p_value_struct.base_data.arrive_time.ms = m_MessageHeader.millSecond;
	
 


	//设置曲线左下角最新时间

	CString cur_time(""); 
	cur_time.Format(_T("%2u:%2u:%2u:%3u"), m_MessageHeader.hour, m_MessageHeader.minute, m_MessageHeader.second, m_MessageHeader.millSecond);
	m_Graphic.SetCurTime(cur_time);  



	//查找p_name变量对应的数据范围
	auto it_find = m_data_scope.find(p_name); 

	//最小范围
	p_value_struct.attributes.min = (*it_find).second.min_scope;

	//最大范围
	p_value_struct.attributes.max = (*it_find).second.max_scope; 

	//数据值在数据范围内，数据点显示绿色否则显示红色
	if ((p_value_struct.base_data.arrive_value >= p_value_struct.attributes.min) && (p_value_struct.base_data.arrive_value <= p_value_struct.attributes.max))
	{
		p_value_struct.attributes.color = GraphicColor::GREEN;
	}
	else
	{
		p_value_struct.attributes.color = GraphicColor::RED;
	}

	//已经初始化完毕这个数据点结构体
	p_value_struct.attributes.hasInit = true;

}


/***************************************************************************
函数名   :	InitDataScope
功能     :	初始化数据包中变量的数据范围
参数     :	[IN]	:	无	  
			[OUT]	:	无
返回值   :	无
备注     :	添加所有的7包数据范围
时间     :	2016/8/17 陈登龙
****************************************************************************/
void CGraphicView::InitDataScope()
{  
	if (GraphicView::TSPI == m_current_select_form)	InitTSPIDataScope();   
}



/*********************************************************
函数名   :	UpdataFormCombo
功能     :	更新数据包列表框
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   :	无
备注     :	更新数据包列表框
时间     :	2016/8/13 陈登龙
**********************************************************/
void CGraphicView::UpdataFormCombo()
{
	//初始化数据包列表框
	InitFormVector(m_form_vec);

	//添加数据包字符串到数据包列表框
	AddFormVecToComboForm(m_form_vec);

	//默认都选择第一项
	m_SelectForm.SetCurSel(0);
}




/*********************************************************
函数名   :	UpdataFormParamCombo
功能     :	更新数据包变量列表框
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   :	无
备注     :	更新变量列表框
时间     :	2016/8/13 陈登龙
**********************************************************/
void CGraphicView::UpdataFormParamCombo()
{
	//初始化变量列表框
	InitFormParamVector();

	//添加数据包中的变量到变量列表框
	AddFormParamVecToComboFormParam(m_form_param_vec);

	//默认都选择第一项
	m_SelectFormParam.SetCurSel(0);
}




/*********************************************************
函数名   :	UpdateGraphic
功能     :	更新曲线
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   :	无
备注     :	用于在选择数据包列表框或者变量的列表框时调用
时间     :	2016/8/13 陈登龙
**********************************************************/
void CGraphicView::UpdateGraphic()
{
	//清空曲线数据队列中的数据
	m_Graphic.CleanQueueData();

	//设置y轴刻度范围默认为数据范围
	m_Graphic.SetYMinAndMaxScale(GetMinScope(m_current_select_param), GetMaxScope(m_current_select_param));
	
	//设置数据范围
	m_Graphic.SetYValueMinAndMax(GetMinScope(m_current_select_param), GetMaxScope(m_current_select_param));

	//更新Y轴单位
	m_Graphic.SetYDataUnit(GetCurDataUnit(m_current_select_param));

	//设置当前曲线显示的数据名称和范围
	m_Graphic.SetCurDataName(m_current_select_param);


	//设置当前曲线显示模式
	m_Graphic.SetCurModel(m_current_model);

	//重绘曲线控件
	m_Graphic.ReconstructControl();  

	//关闭所有的消息总线
	//CloseMessageBus();

	//分类开启消息总线
	StartMessageBus(); 
}
 


/******************************************
函数名   :	AddTSPIParamToParamVec
功能     :	向变量容器中添加飞行姿态变量名称
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   :  无
备注     :  14个飞行姿态数据字加完
时间     :  2016/8/17 陈登龙
*******************************************/
void CGraphicView::AddTSPIParamToParamVec()
{
	//先清空
	m_form_param_vec.clear();
	m_form_param_vec.reserve(40);

	m_form_param_vec.push_back(_T("True_Plane_No"));
	m_form_param_vec.push_back(_T("Traget_Plane_No"));

	m_form_param_vec.push_back(_T("Immediate_LON"));
	m_form_param_vec.push_back(_T("Immediate_LAT"));

	m_form_param_vec.push_back(_T("STD_PRE_ALT"));
	m_form_param_vec.push_back(_T("PRE_CORR_ALT"));
	m_form_param_vec.push_back(_T("Radio_ALT"));
	m_form_param_vec.push_back(_T("T_Airspeed"));
	m_form_param_vec.push_back(_T("I_Airspeed"));
	m_form_param_vec.push_back(_T("T_Course"));
	m_form_param_vec.push_back(_T("Pitch_ANG"));
	m_form_param_vec.push_back(_T("Roll_ANG"));
	m_form_param_vec.push_back(_T("Pitch_Rate"));
	m_form_param_vec.push_back(_T("Roll_Rate"));
}

 


/*************************************************************
函数名   :	InitTSPIDataScope
功能     :	初始化飞行姿态数据范围
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   : 无
备注     : 14个飞行姿态数据字范围加完
2016-10-13修改：修改需要转换成英制的单位的数据范围
时间     : 2016/8/17 陈登龙
**************************************************************/
void CGraphicView::InitTSPIDataScope()
{
	//先清空
	m_data_scope.clear();
	int vec_name_index = 0;
	m_data_scope.insert(data_scope_pair(m_form_param_vec.at(vec_name_index++), Graphic::data_scope_struct{ 0.0, 100.0 }));
	m_data_scope.insert(data_scope_pair(m_form_param_vec.at(vec_name_index++), Graphic::data_scope_struct{ 0.0, 100.0 }));

	m_data_scope.insert(data_scope_pair(m_form_param_vec.at(vec_name_index++), Graphic::data_scope_struct{ -180.0, 180.0, _T("°") }));
	m_data_scope.insert(data_scope_pair(m_form_param_vec.at(vec_name_index++), Graphic::data_scope_struct{ -90.0, 90.0, _T("°") }));

	m_data_scope.insert(data_scope_pair(m_form_param_vec.at(vec_name_index++), Graphic::data_scope_struct{ -1476.0, 65617.0, _T("ft") }));
	m_data_scope.insert(data_scope_pair(m_form_param_vec.at(vec_name_index++), Graphic::data_scope_struct{ -1476.0, 65617.0, _T("ft") }));
	m_data_scope.insert(data_scope_pair(m_form_param_vec.at(vec_name_index++), Graphic::data_scope_struct{ 0, 4921.0, _T("ft") }));
	m_data_scope.insert(data_scope_pair(m_form_param_vec.at(vec_name_index++), Graphic::data_scope_struct{ 0, 1080.0, _T ("kn") }));
	m_data_scope.insert(data_scope_pair(m_form_param_vec.at(vec_name_index++), Graphic::data_scope_struct{ 0, 864.0, _T("kn") }));
	m_data_scope.insert(data_scope_pair(m_form_param_vec.at(vec_name_index++), Graphic::data_scope_struct{ -180.0, 180.0, _T("°") }));
	m_data_scope.insert(data_scope_pair(m_form_param_vec.at(vec_name_index++), Graphic::data_scope_struct{ -90, 90, _T("°") }));
	m_data_scope.insert(data_scope_pair(m_form_param_vec.at(vec_name_index++), Graphic::data_scope_struct{ -180.0, 180.0, _T("°") }));
	m_data_scope.insert(data_scope_pair(m_form_param_vec.at(vec_name_index++), Graphic::data_scope_struct{ -3.1416, 3.1416, _T("rad/s") }));
	m_data_scope.insert(data_scope_pair(m_form_param_vec.at(vec_name_index++), Graphic::data_scope_struct{ -3.1416 * 2, 3.1416 * 2, _T("rad/s") }));



} 

void CGraphicView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
// 	if (nIDEvent == m_uTimerID)
// 	{
// 		//std::lock_guard<std::mutex>  locker(m_mutex);
// 		//更新曲线，每次传输一个队列的数据，需要知道曲线控件是否更新完毕，才能发送下一个曲线队列数据
// 
// 		//200ms发送给曲线控件更新,一起发数据会卡顿
// 		if (isDataArriva)
// 		{
// 			isDataArriva = false;
// 			//更新到达时间和到达的值
// 			UpdataArriveTimeAndValue(m_rec_chart_data.second.base_data.arrive_time, m_rec_chart_data.second.base_data.arrive_value);
// 			m_Graphic.SetCurrentData_One_Package(m_rec_chart_data);
// 		}
// 
// 	}
	//进入临界区
	EnterCriticalSection(&g_csA);
	//200ms发送给曲线控件更新
	if (isDataArriva)
	{
		isDataArriva = false;

		//确保当前发送的数据点结构体是正确初始化过的，如果这个结构体被正确初始化过，则结构体中的颜色属性一定不为0，必须为红色或者绿色
		//if (m_rec_chart_data.second.attributes.color != 0)
		//设置新的标记来判断当前数据点结构体是否已经被成功初始化
		if (m_rec_chart_data.second.attributes.hasInit)
		{
			//更新到达时间和到达的值
			//UpdataArriveTimeAndValue(m_rec_chart_data.second.base_data.arrive_time, m_rec_chart_data.second.base_data.arrive_value);
			
			//改变到达时间-重载函数
			UpdataArriveTimeAndValue(m_rec_chart_data.second.base_data.arrive_time, m_rec_chart_data.second.base_data.arrive_value);
		
			//发的可能是前一个选择的数据字的数据结构体，导致前面的数据可能会乱
			//要保证选择了新的数据字后发送的数据是新的数据值的数据
			m_Graphic.SetCurrentData_One_Package(m_rec_chart_data);
		}
	}
	//离开临界区
	LeaveCriticalSection(&g_csA);
	CDialogEx::OnTimer(nIDEvent);
}

/*********************************************************
函数名   :	UpdateYScopeEdit
功能     :	更新Y轴范围编辑框
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   :	无
备注     :
时间     : 2016/8/14 陈登龙
**********************************************************/
void CGraphicView::UpdateYScopeEdit()
{
	//设置数据到控件上
	CString temp_text("");
	temp_text.Format(_T("%.2f"), GetMinScope(m_current_select_param)); 
	m_y_min_edit.SetWindowText(temp_text);

	temp_text.Format(_T("%.2f"), GetMaxScope(m_current_select_param));
	m_y_max_edit.SetWindowText(temp_text);

	m_y_min_edit.Invalidate();
	m_y_max_edit.Invalidate();
}


void CGraphicView::UpdataArriveTimeAndValue(double p_cur_time, double p_cur_value)
{
	//设置数据到控件上
	CString temp_text("");
	//temp_text.Format(_T("%.2f"), m_rec_chart_data.second.base_data.arrive_time);
	temp_text.Format(_T("%.2f"), p_cur_time);
	m_edit_arrive_time.SetWindowText(temp_text);

	//temp_text.Format(_T("%.2f"), m_rec_chart_data.second.base_data.arrive_value);
	temp_text.Format(_T("%.2f"), p_cur_value);
	m_edit_arrive_value.SetWindowText(temp_text);

	m_edit_arrive_time.Invalidate();
	m_edit_arrive_value.Invalidate();
}





/*********************************************************
函数名   :	UpdataArriveTimeAndValue
功能     :	更新时间和值得重载
参数     :	[IN]	: 无
			[OUT]	: 无
返回值   :	无
备注     :
时间     : 2016/8/14 陈登龙
修改时间 : 2016/11/18
**********************************************************/
void CGraphicView::UpdataArriveTimeAndValue(Graphic::time_struct arrive_time, double p_cur_value)
{
	//设置数据到控件上
	CString temp_text("");
	temp_text.Format(_T("%2u:%2u:%2u:%3u"), arrive_time.hour, arrive_time.min, arrive_time.sec, arrive_time.ms);
	m_edit_arrive_time.SetWindowText(temp_text);




 	temp_text.Format(_T("%.2f"), p_cur_value);
	m_edit_arrive_value.SetWindowText(temp_text);

	m_edit_arrive_time.Invalidate();
	m_edit_arrive_value.Invalidate();
}






//关闭重启曲线图，Pause和Resume会乱，没有修复
void CGraphicView::OnBnClickedPause()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str("");
	GetDlgItemText(IDC_BTN_PAUSE, str);
	if (_T("Pause") == str)
	{
		//关闭定时器
		CloseEventTimer();
		//取消关闭消息总线,防止按下一个按钮，4幅曲线全部暂停
		//CloseMessageBus();

		SetDlgItemText(IDC_BTN_PAUSE, _T("Resume"));
	}
	else
	{
		//开启定时器
		StartEventTimer();
		//开启消息总线
		//StartMessageBus();

		SetDlgItemText(IDC_BTN_PAUSE, _T("Pause"));
	}

	
}


void CGraphicView::OnBnClickedUpdataParam()
{
	// TODO:  在此添加控件通知处理程序代码

	//更新min和max范围
	 
	CString str("");
	//获取设置的最小值
	GetDlgItemText(IDC_EDIT_Y_MIN, str);
	USES_CONVERSION; 
	char* min_str = W2A(str); 
	double y_min = atof(min_str);


	//获取设置的最大值 
	GetDlgItemText(IDC_EDIT_Y_MAX, str); 
	char* max_str = W2A(str);
	double y_max = atof(max_str);

	
	//重新设置曲线min和max范围
	m_Graphic.SetYMinAndMaxScale(y_min, y_max); 

	/*在数据回放完毕之后或者当前是暂停，
	更新参数后仍需要调整坐标，否则界面不更新，
	从回放模块得到当前是否回放完毕，如果回放模块已经完全回放完毕，就在这里调整一次曲线控件的坐标
	获得暂停按钮文本，为Resume时更新曲线控件坐标*/
	CString btn_str("");
	GetDlgItemText(IDC_BTN_PAUSE, btn_str);
	if ((GraphicView::stop_state == m_cur_state || _T("Resume") == btn_str) && (m_Graphic.GetCurrentQueueSize() > 0))
	{
		m_Graphic.AdjustQueuePoint();
	}
	 

	//重绘一次控件
	//m_Graphic.ReconstructControl();

	//需要重绘背景
	m_Graphic.ReDraw();
}
 

void CGraphicView::OnStnClickedGraphic()
{
	// TODO:  在此添加控件通知处理程序代码
}
