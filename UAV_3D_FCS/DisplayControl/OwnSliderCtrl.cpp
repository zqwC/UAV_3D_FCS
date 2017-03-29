// OwnSliderCtrl.cpp : 实现文件
//
//
/***********************************************************************
* Copyright (c) 2014, 郑迪威
* All rights reserved.
* 
* 文件名称：OwnSliderCtrl.cpp
* 摘    要：本项目是为一款基于MFC的播放条控制
* 
* 当前版本：1.0
* 作    者：郑迪威
* 完成日期：2014年12月
*
* 当前版本：1.1
* 修 改 人：郑迪威
* 完成日期：2015年11月23日
* 修改内容：修改一系列函数
*
* 当前版本：1.2
* 修 改 人：郑迪威
* 完成日期：2015年12月13日
* 修改内容：增加系统时间结构体的处理函数
*	--------------------------------------------------------------------------------
* 备注     :  
* 典型用法 :
*	--------------------------------------------------------------------------------
* 作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
* 时间     : 2015/11/22
**************************************************************************************/
#include "stdafx.h"
//#include "../DisPlayTerminal.h"
#include "OwnSliderCtrl.h"
#include "../MemoryDC.h"
#include "../loadimg.h"

extern DWORD gStartTime;
extern DWORD gCurTime;
extern DOUBLE gPixel;
extern BOOL gResetID;

CTimeFormat::CTimeFormat(void)
{
}

CTimeFormat::~CTimeFormat(void)
{
}

DWORD CTimeFormat::HMSToSecond(DWORD hour, DWORD minute, DWORD second)
{
	return hour * 3600 + minute * 60 + second;
}

DWORD CTimeFormat::HMSToSecond(Time time)
{
	return time.Hour * 3600 + time.Minute * 60 + time.Second;
}

void CTimeFormat::SecondToHMS(DWORD &hour, DWORD &minute, DWORD &second, DWORD srcsecond)
{
	hour = srcsecond / 3600;
	srcsecond -= hour * 3600;
	minute = srcsecond / 60;
	srcsecond -= minute * 60;
	second = srcsecond;
}

DWORD CTimeFormat::HMSToSecond(DOUBLE hour, DOUBLE minute, DOUBLE second)
{
	return (DWORD)(hour * 3600 + minute * 60 + second);
}

void CTimeFormat::SecondToHMS(DWORD &hour, DWORD &minute, DWORD &second, DOUBLE srcsecond)
{
	hour = (DWORD)(srcsecond / 3600 + 10E-8);
	srcsecond -= hour * 3600;
	minute = (DWORD)(srcsecond / 60 + 10E-8);
	srcsecond -= minute * 60;
	second = (DWORD)(srcsecond + 10E-8);
}

void CTimeFormat::SecondToHMS(Time &time, DOUBLE srcsecond)
{
	time.Hour = (DWORD)(srcsecond / 3600 + 10E-8);
	srcsecond -= time.Hour * 3600;
	time.Minute = (DWORD)(srcsecond / 60 + 10E-8);
	srcsecond -= time.Minute * 60;
	time.Second = (DWORD)(srcsecond + 10E-8);
}

void CTimeFormat::SecondToHMS(Time &time, DWORD srcsecond)
{
	time.Hour = srcsecond / 3600;
	srcsecond -= time.Hour * 3600;
	time.Minute = srcsecond / 60;
	srcsecond -= time.Minute * 60;
	time.Second = srcsecond;
}

Time CTimeFormat::HMSAddHMS(Time t1, Time t2)
{
	Time t3;
	t3.Second = (t1.Second + t2.Second) % 60;
	t3.Minute = (t1.Minute + t2.Minute + (t1.Second + t2.Second) / 60) % 60;
	t3.Hour = t1.Hour + t2.Hour + (t1.Minute + t2.Minute) / 60 + (t1.Second + t2.Second) / 3600;
	return t3;
}
Time CTimeFormat::HMSSubHMS(Time t1, Time t2)
{
	DWORD s1, s2;
	Time t3;
	s1 = HMSToSecond(t1);
	s2 = HMSToSecond(t2);
	SecondToHMS(t3, s1 < s2 ? s2 - s1 : s1 - s2);
	return t3;
}


BOOL CTimeFormat::IsLeapYear(INT year)
{
	if (year % 100)
	{
		if (year % 4 == 0)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (year % 400 == 0)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

DWORD CTimeFormat::YMDHMSToStartDay(SYSTEMTIME time)
{
	////到1月1号0时0分0秒
	//平年各月份天数
	INT month_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	INT dseconds = 0;
	if (IsLeapYear(time.wYear))
	{
		month_days[1] = 29;
	}

	//月
	for (size_t i = 1; i < time.wMonth; i++)
	{
		dseconds += month_days[i - 1] * 24 * 3600;
	}
	//日
	dseconds += (time.wDay - 1) * 24 * 3600;
	//时
	dseconds += time.wHour * 3600;
	//分
	dseconds += time.wMinute * 60;
	//秒
	dseconds += time.wSecond;
	return dseconds;
}

DWORD CTimeFormat::YMDHMSToEndDay(SYSTEMTIME time)
{
	//到12月31号23时59分59秒
	INT month_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	DWORD dseconds = 0;
	if (IsLeapYear(time.wYear))
	{
		month_days[1] = 29;
	}
	//计算到23时59分59秒总秒数
	dseconds += (23 - time.wHour) * 3600 + (59 - time.wMinute) * 60 + (59 - time.wSecond);
	//计算到本月末尾日期23时59分59秒的总秒数
	dseconds += (month_days[time.wMonth - 1] - time.wDay) * 24 * 3600;
	//计算到12月末31号59分59秒的秒数差
	for (size_t i = time.wMonth + 1; i <= 12; i++)
	{
		dseconds += month_days[i - 1] * 24 * 3600;
	}
	return dseconds;
}

DWORD CTimeFormat::YEndToYStart(INT year_start, INT year_end)
{
	//计算某一年12月31号23时59分59秒到某一年1月1号0时0分0秒的总秒数差
	
	if (year_start == year_end)
	{
		return 0;
	}
	else
	{
		if (year_start > year_end)
		{
			INT year_temp = year_end;
			year_end = year_start;
			year_start = year_temp;
		}
		DWORD dseconds = 0;
		dseconds += 1;
		year_start += 1;
		for (INT i = year_start; i < year_end; i++)
		{
			if (IsLeapYear((INT)i))
			{
				dseconds += 366 * 24 * 3600;
			}
			else
			{
				dseconds += 365 * 24 * 3600;
			}
		}
		return dseconds;
	}
}
DWORD CTimeFormat::YMDHMSToSecond(SYSTEMTIME start_time, SYSTEMTIME end_time)
{
	if (end_time.wYear > start_time.wYear)
	{
		return YMDHMSToEndDay(start_time)
			+ YEndToYStart(start_time.wYear, end_time.wYear)
			+ YMDHMSToStartDay(end_time);
	}
	else if (end_time.wYear == start_time.wYear)
	{
		return YMDHMSToStartDay(end_time) - YMDHMSToStartDay(start_time);
	}
	else
	{
		return 0;
	}
}


void CTimeFormat::MillSecondToHMSMS(DWORD &hour, DWORD &minute, DWORD &second, DWORD &mill_second, DWORD src_mill_second)
{
	SecondToHMS(hour, minute, second, src_mill_second / 1000);
	mill_second = src_mill_second % 1000;
}

void CTimeFormat::MillSecondToHMSMS(Time & time, DWORD srcmill_second)
{
	SecondToHMS(time, srcmill_second / 1000);
	time.MillSecond = srcmill_second % 1000;
}

void CTimeFormat::MillSecondToHMSMS(DWORD &hour, DWORD &minute, DWORD &second, DWORD &mill_second, DOUBLE src_mill_second)
{
	SecondToHMS(hour, minute, second, src_mill_second / 1000);
	mill_second = (DWORD)(src_mill_second + 10E-10) % 1000;
}

void CTimeFormat::MillSecondToHMSMS(Time & time, DOUBLE src_mill_second)
{
	SecondToHMS(time, src_mill_second / 1000);
	time.MillSecond = (DWORD)(src_mill_second + 10E-10) % 1000;
}

void CTimeFormat::HMSMSToMillSecond(DWORD &mill_sercond, Time time)
{
	mill_sercond = HMSToSecond(time) * 1000 + time.MillSecond;
}
void CTimeFormat::HMSMSToMillSecond(DWORD &tar_mill_second, DWORD hour, DWORD minute, DWORD second, DWORD mill_second)
{
	tar_mill_second = HMSToSecond(hour, minute, second) * 1000 + mill_second;
}

DWORD CTimeFormat::HMSMSToMillSecond(Time time)
{
	DWORD tar_mill_second = 0;
	HMSMSToMillSecond(tar_mill_second, time);
	return tar_mill_second;
}

DWORD CTimeFormat::HMSMSToMillSecond(DWORD hour, DWORD minute, DWORD second, DWORD mill_second)
{
	DWORD tar_mill_second = 0;
	HMSMSToMillSecond(tar_mill_second, hour, minute, second, mill_second);
	return tar_mill_second;
}
// COwnSliderCtrl

IMPLEMENT_DYNAMIC(COwnSliderCtrl, CWnd)

COwnSliderCtrl::COwnSliderCtrl()
{
	m_uiThumbSelected = IDB_PNG_AT_THUMB;
	m_uiThumbUnselected = IDB_PNG_UAT_THUMB;
	m_uiChannelLeave = IDB_PNG_CHANNEL;
	m_uiChannelHover = IDB_PNG_CHANNEL;
	m_uiPlayed = IDB_PNG_PLAYED_CHANNEL;
	m_uiBack = IDB_PNG_BACK;
	m_uiBeyondArea = IDB_PNG_BEYOND_CHANNEL;

	m_ThumbState = LEAVE;
	m_ChannelState = LEAVE;
	m_MouseState = LEAVE;

	m_ThumbRect.left = m_ThumbRect.top = 0;
	m_ThumbRect.right = 15;
	m_ThumbRect.bottom = 9;

	
	m_ChannelRect.left = m_ChannelRect.top = 0;
	m_ChannelRect.right = 99;
	m_ChannelRect.bottom = 5;


	m_ToolTipRect.left = 0;
	m_ToolTipRect.top = 0;
	m_ToolTipRect.right = 90;
	m_ToolTipRect.bottom = 29;

	m_MirrorRect.left = 0;
	m_MirrorRect.top = 0;
	m_MirrorRect.right = 99;
	m_MirrorRect.bottom = 49;

	m_CurPos = 0;
	m_uElapse = 1;
	m_Alpha = 0;

	m_ToolTipCtrl = NULL;
	m_OldCursor = LoadCursor(NULL, IDC_HAND);
	m_ToolTipCtrl = new CMyToolTipCtrl();
	m_bSign = FALSE;


	if (m_saPointRecords.GetCount())
	{
		m_saPointRecords.RemoveAll();
	}

	m_strFileName = TEXT("posRecords.txt");

	//注册控件类
	m_szClassName = AfxRegisterWndClass(0);
	m_uiSignBack = IDB_PNG_BLUE_SIGN;// TEXT("./res/mediaPlayer/sign_back_horz.png");
	//默认提示框为时间值
	m_ToolStyle = TOOLTIP_TIME;
	m_bMirror = FALSE;

	Time begin, end, rangeBegin, rangeEnd;
	CTimeFormat::SecondToHMS(begin, (DWORD)0);
	CTimeFormat::SecondToHMS(end, (DWORD)100);
	rangeBegin = begin;
	rangeEnd = end;

	m_DisplayInfo.Begin = begin;
	m_DisplayInfo.End = end;
	m_DisplayInfo.RangeBegin = rangeBegin;
	m_DisplayInfo.RangeEnd = rangeEnd;
	//最小帧数1
	m_iMinFrameSpeed = 1;
	//最大帧数10
	m_iMaxFrameSpeed = 10;
}

COwnSliderCtrl::~COwnSliderCtrl()
{
	if (m_ToolTipCtrl)
	{
		delete m_ToolTipCtrl;
		m_ToolTipCtrl = NULL;
	}
}

BEGIN_MESSAGE_MAP(COwnSliderCtrl, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()  
	ON_WM_MOUSEHOVER()	// 此消息系统并不会给我们发送
	ON_WM_MOUSELEAVE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// COwnSliderCtrl 消息处理程序

bool COwnSliderCtrl::Create(CWnd* pParent, const CRect &rc, CString text,
							DWORD id /* = 0 */,DWORD style /* = WS_VISIBLE|WS_CHILD */)
{
	if (pParent == NULL)return FALSE;
	// 动态创建控件
	BOOL ret = CWnd::CreateEx(0, m_szClassName, text, style, rc, pParent, id + SLIDER), 
		retu = m_ToolTipCtrl->Create(pParent, m_ToolTipRect, TEXT(""), id + SLIDER, style & TBS_VERT ? CMyToolTipCtrl::RIGHT : CMyToolTipCtrl::TOP);
	CRect newChannelRect, newThumbRect;
	m_Parent = pParent;
	m_ClientRect.left = m_ChannelRect.top = 0;
	m_ClientRect.right = rc.Width();
	m_ClientRect.bottom = rc.Height();
	m_Style = style;
	//根据样式设置元素位置和尺寸
	Init();
	//默认底部
	m_SignAlign = SIGN_BOTTOM;
	//默认每秒1帧
	m_iFrame = 1;
	//可行区物理长度
	m_LineSize = (DWORD)((m_MaxValue - m_MinValue + 1) / m_Unit);
	//记录初始窗口尺寸
	m_OldSize.cx = rc.Width();
	m_OldSize.cy = rc.Height();
	//设置标记线尺寸
	SetSignSize(CSize(2, 15));
	//创建放大镜
	m_LargerMirror.Create(IDD_DIALOG_MIRROR);
	return ret && retu? true : false;
}

//初始化所有信息
VOID COwnSliderCtrl::Init()
{
	//物理值设置为100ms级
	m_Value = ((CTimeFormat::HMSMSToMillSecond(m_DisplayInfo.End) - CTimeFormat::HMSMSToMillSecond(m_DisplayInfo.Begin))) / 1;/*毫秒*/
	//播放区域物理最小值
	m_MinValue = CTimeFormat::HMSMSToMillSecond(m_DisplayInfo.RangeBegin) / 1;
	//播放区域物理最大值
	m_MaxValue = CTimeFormat::HMSMSToMillSecond(m_DisplayInfo.RangeEnd) / 1;
	//每像素播放几个100毫秒
	m_Unit = m_Value / (DOUBLE)(m_ClientRect.Width());
	//每个100毫秒多少个像素
	if(abs(m_Unit) > 10E-5)m_Speed = 1 / m_Unit;
	else m_Speed = 1.0;
	//当前时间位置
	if (abs(m_Unit) > 10E-5)m_CurPos = m_MinValue / m_Unit;
	else m_CurPos = 0.0;
	//初始化标记点
	m_saPointRecords.RemoveAll();
	m_PointBack.clear();
	m_PointStyle.clear();
	//根据样式设置元素位置和尺寸
	ImplementStyle(m_Style);
}

/*! @function
********************************************************************************
<PRE>
函数名   : SetSignSize
功能     : 设置标记线尺寸
参数     : [IN]	 参数1 : 逻辑尺寸
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
BOOL COwnSliderCtrl::SetSignSize( CSize cSize )
{
	if (m_Style & TBS_VERT)
	{
		m_SignSize.cx = cSize.cy;
		m_SignSize.cy = cSize.cx;
	}
	else
	{
		m_SignSize.cy = cSize.cy;
		m_SignSize.cx = cSize.cx;
	}
	return TRUE;
}

/*! @function
********************************************************************************
<PRE>
函数名   : PostClickEvent
功能     : 该函数用来向父窗口发送单击消息
参数     : [IN]		   : 无
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::PostClickEvent()
{
	CWnd* parent = GetParent();
	if(parent != NULL)
	{
		WPARAM wp = MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED);
		LPARAM lp = (LPARAM) m_hWnd;
		parent->PostMessage(WM_COMMAND, wp, lp);
	}
}


VOID COwnSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_ChannelState = HOVER;
	//IN THE AREA
	if (JudgeArea(point) != AREA_BACK )
	{
		CRect rect;
		GetChannelRect(rect);
		//IN THE RANGE
		if (m_Style & TBS_VERT)
		{
			if (point.y < GetPosByTime(m_MinValue) + rect.top)
			{
				point.y = GetPosByTime(m_MinValue) + rect.top;
			}
			if (point.y > GetPosByTime(m_MaxValue) + rect.top)
			{
				point.y = GetPosByTime(m_MaxValue) + rect.top;
			}
			m_CurPos = point.y;
		}
		else
		{
			if (point.x < GetPosByTime(m_MinValue) + rect.left)
			{
				point.x = GetPosByTime(m_MinValue) + rect.left;
			}
			if (point.x > GetPosByTime(m_MaxValue) + rect.left)
			{
				point.x = GetPosByTime(m_MaxValue) + rect.left;
			}
			m_CurPos = point.x;
		}
		gPixel = 0.0;
		m_ThumbState = LBUTTONDOWN;
		gResetID = TRUE;
		ToolTipMove(point);
		MoveThumb(point);
		Invalidate();
		if(m_Alpha < 250)
		{
			m_Alpha = 250;
		}
	}
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);
	point.x += rect.left;
	point.y += rect.top;
}

// 鼠标进入和鼠标移出消息需要我们自己监听
VOID COwnSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// 只处理鼠标第一次进入时的情况
	CRect rect, channel_rect;
	GetChannelRect(rect);
	m_Parent->ActivateTopParent();
	ToolTipMove(point);
	if (m_ToolTipCtrl->m_hWnd 
		&& !m_ToolTipCtrl->IsWindowVisible()
		&& rect.PtInRect(point))
	{
		m_ToolTipCtrl->ShowWindow(SW_NORMAL);
		this->SetActiveWindow();
	}
	if(m_MouseState == LEAVE)
	{
		m_MouseState = HOVER;
		m_ThumbState = LBUTTONUP;
		TRACKMOUSEEVENT evt = { sizeof(evt), TME_LEAVE, m_hWnd, 0 };
		TrackMouseEvent(&evt);
		OnMouseHover(0, point);
	}
	else
	{
		if (m_ThumbState == LBUTTONDOWN)
		{
			//拖拽滑块
			MoveThumb(point);
			if (m_Style & TBS_VERT)
			{
				if (point.y > GetPosByTime(m_MaxValue) + m_ChannelRect.top)
				{
					point.y = GetPosByTime(m_MaxValue) + m_ChannelRect.top;
				}
				if (point.y < GetPosByTime(m_MinValue) + m_ChannelRect.top)
				{
					point.y = GetPosByTime(m_MinValue) + m_ChannelRect.top;
				}
				m_CurPos = point.y;
			}
			else
			{
				if (point.x > GetPosByTime(m_MaxValue) + m_ChannelRect.left)
				{
					point.x = GetPosByTime(m_MaxValue) + m_ChannelRect.left;
				}
				if (point.x < GetPosByTime(m_MinValue) + m_ChannelRect.left)
				{
					point.x = GetPosByTime(m_MinValue) + m_ChannelRect.left;
				}
				m_CurPos = point.x;
			}
		}
		if (m_bSign && m_ChannelRect.PtInRect(point) && m_ChannelState == RBUTTONDOWN)
		{
			m_SignLine = point;
		}
		Invalidate();
	}
}

/*! @function
********************************************************************************
<PRE>
函数名   : ToolTipMove
功能     : 提示框移动
参数     : [IN]	 参数1 : 逻辑位置
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::ToolTipMove(CPoint point)
{
	CRect rect, channel_rect, toolTipRect, mirrorRect;
	INT tempPoint;
	//获得当前控件所在父窗口的位置
	GetWindowRect(&rect);
	GetChannelRect(channel_rect);
	//m_Parent->ScreenToClient(&rect);
	ClientToScreen(channel_rect);
	if (m_Style & TBS_VERT)
	{
		tempPoint = point.y < m_ChannelRect.top ? m_ChannelRect.top : point.y;
		/*垂直滑动,右端显示*/
		toolTipRect.top = rect.top + point.y - m_ToolTipRect.Height() / 2;
		toolTipRect.bottom = toolTipRect.top + m_ToolTipRect.Height() - 1;
		toolTipRect.left = channel_rect.right + 2;
		toolTipRect.right = toolTipRect.left +  m_ToolTipRect.Width();

		if (m_ToolTipCtrl->m_hWnd)
		{
			m_ToolTipCtrl->MoveWindow(&toolTipRect);
		}
	}
	else
	{
		tempPoint = point.x < m_ChannelRect.left ? m_ChannelRect.left : point.x;
		/*水平滑动,上端显示*/
		toolTipRect.left = rect.left + point.x - m_ToolTipRect.Width() / 2;
		toolTipRect.right = toolTipRect.left + m_ToolTipRect.Width() - 1;
		toolTipRect.bottom = channel_rect.top - 2;
		toolTipRect.top = toolTipRect.bottom - m_ToolTipRect.Height();
		if (m_ToolTipCtrl->m_hWnd)
		{
			m_ToolTipCtrl->MoveWindow(&toolTipRect);
		}
	}
	if (m_ToolStyle == TOOLTIP_VALUE)
	{
		if (m_ToolTipCtrl->m_hWnd)m_ToolTipCtrl->SetText(GetPosValue(tempPoint));
	}
	else
	{
		Time curTime;
		CString formatTime;
		CTimeFormat::MillSecondToHMSMS(curTime, GetPosTime(tempPoint - m_ChannelRect.left) + CTimeFormat::HMSMSToMillSecond(m_DisplayInfo.Origin));
		formatTime.Format(TEXT("%d:%d:%d.%d"), curTime.Hour, curTime.Minute, curTime.Second, curTime.MillSecond);
		if (m_ToolTipCtrl->m_hWnd)m_ToolTipCtrl->SetText(formatTime);
	}
}

/*! @function
********************************************************************************
<PRE>
函数名   : SetToolTipStyle
功能     : 设置提示框样式
参数     : [IN]	 参数1 : 位置样式
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::SetToolTipStyle(INT style)
{
	m_ToolStyle = style;
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetPosValue
功能     : 获得当前位置对应的物理值
参数     : [IN]	 参数1 : 客户区位置
           [OUT]	   : 无
           
返回值   : 对应的物理值
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
CString COwnSliderCtrl::GetPosValue(UINT curPos)
{
	CString temp = TEXT("");
	temp.Format(TEXT("%d"), (INT)(curPos * m_Unit + CTimeFormat::HMSMSToMillSecond(m_DisplayInfo.Origin)));
	return temp;
}


/*! @function
********************************************************************************
<PRE>
函数名   : GetPosTime
功能     : 获得当前位置对应的物理值
参数     : [IN]	 参数1 : 客户区位置
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
DOUBLE  COwnSliderCtrl::GetPosTime(UINT curPos)
{
	return curPos * m_Unit;
}



/*! @function
********************************************************************************
<PRE>
函数名   : GetPosByTime
功能     : 获得对应时间的客户区位置
参数     : [IN]	 参数1 : 消已播放时间,以秒做单位
           [OUT]	   : 无
           
返回值   : 对应的物理值
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  客户区位置
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
DOUBLE  COwnSliderCtrl::GetPosByTime(DOUBLE passedTime)
{
	return passedTime / m_Unit;
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetPixelTime
功能     : 获得对应时间的像素位置
参数     : [IN]	 参数1 : 已播放时间,以秒做单位
           [OUT]	   : 无
           
返回值   : 对应的物理值
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  像素位置
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
DOUBLE  COwnSliderCtrl::GetPixelTime(DOUBLE pixel)
{
	if(m_ChannelRect.Width() == 0 
		|| m_ChannelRect.Height() == 0)return 0.0;
	return pixel * m_Unit;
}

VOID COwnSliderCtrl::OnMouseHover(UINT nFlags, CPoint point)
{
	// 鼠标进入
	if (SetTimer(TIMER_THUMB_SHOW, m_uElapse, NULL) == 0)
	{
		MessageBox(NULL, L"SetTimer failed!" ,NULL);
		return ;
	}
	m_OldCursor = ::SetCursor(LoadCursor(NULL, IDC_HAND));
}

VOID COwnSliderCtrl::OnMouseLeave()
{
	m_MouseState = LEAVE;
	m_ChannelState = LEAVE;
	KillTimer(TIMER_THUMB_SHOW);
	if (m_ToolTipCtrl->m_hWnd && m_ToolTipCtrl->IsWindowVisible())
	{
		m_ToolTipCtrl->ShowWindow(SW_HIDE);
	}
	m_OldCursor = ::SetCursor(m_OldCursor);
	m_Alpha = 0;
	Invalidate();
}

VOID COwnSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetThumbRect(rect);
	m_ThumbState = LBUTTONUP;
	if (m_Style & TBS_VERT)
	{
		rect.top += m_ChannelRect.top;
		rect.bottom += m_ChannelRect.top;
	}
	else
	{
		rect.left += m_ChannelRect.left;
		rect.right += m_ChannelRect.left;
	}
	InvalidateRect(rect);
	CWnd::OnLButtonUp(nFlags, point);
}

/*! @function
********************************************************************************
<PRE>
函数名   : MoveBorder
功能     : 清除窗口边框
参数     : [IN]	 参数1 : 无
           [OUT]	   : 无
           
返回值   : 对应的物理值
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::MoveBorder()
{
	CRect rect;
	CRgn rgn;
	GetClientRect(&rect);
	rgn.CreateRectRgn(1, 1, rect.Width(), rect.Height());
	SetWindowRgn((HRGN)rgn.GetSafeHandle(), TRUE);
}

VOID COwnSliderCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	if (!dc)
	{
		return;
	}
	CRect rect;
	GetClientRect(&rect);
	CMemoryDC memDC(&dc, &rect);
	//绘制背景
	DrawBack(&memDC);
	//绘制凹槽
	DrawChannel(&memDC);
	//绘制已完结播放条
	DrawPlayed(&memDC);
	//绘制区域外区域
	DrawByeondRange(&memDC);
	//绘制滑块
	DrawThumb(&memDC);
	if (m_bSign)
	{
		//绘制标记
		DrawSign(&memDC);
	}
	if (m_ChannelState == RBUTTONDOWN)
	{
		//右键按下即绘制
		DrawSignLine(&memDC, m_SignLine);
		if (m_bMirror)
		{
			DrawLargerMirror(&memDC, m_SignLine);
		}
	}
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetUnit
功能     : 获得值：秒/像素
参数     : [IN]	 参数1 : 无
           [OUT]	   : 无
           
返回值   : 对应的值：秒/像素
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
DOUBLE COwnSliderCtrl::GetUnit()
{
	m_Unit = m_Value / (DOUBLE)(m_ClientRect.Width() - m_ChannelRect.left);
	return m_Unit;
}


/*! @function
********************************************************************************
<PRE>
函数名   : GetUnit
功能     : 设置值：秒/像素
参数     : [IN]	 参数1 : 秒/像素参量
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
BOOL COwnSliderCtrl::SetUnit(DOUBLE unit)
{
	m_Unit = unit;
	Invalidate();
	return TRUE;
}

/*! @function
********************************************************************************
<PRE>
函数名   : SetMirror
功能     : 是否开启放大镜
参数     : [IN]	 参数1 : TRUE表示开启，FALSE表示关闭
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::SetMirror(BOOL bMirror)
{
	m_bMirror = bMirror;
}

/*! @function
********************************************************************************
<PRE>
函数名   : DrawByeondRange
功能     : 绘制禁止区背景
参数     : [IN]	 参数1 : CD设备指针
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::DrawByeondRange(CDC *pDC)
{
	CRect rect1, rect2;
	CImage img;
	GetChannelRect(rect1);
	rect2 = rect1;
	if(!img.IsNull())img.Destroy();
	if (m_Style & TBS_VERT)
	{
		rect1.bottom = GetPosByTime(m_MaxValue) + m_ChannelRect.top;
		rect2.top = GetPosByTime(m_MinValue) + m_ChannelRect.top;
	}
	else
	{
		rect1.right = GetPosByTime(m_MinValue) + m_ChannelRect.left;
		rect2.left = GetPosByTime(m_MaxValue) + m_ChannelRect.left;//UNIT0.16155
	}
	LoadImageFromResource(&img, m_uiBeyondArea, TEXT("PNG"));
	if (rect1.Width() > 0 && rect1.Height() > 0)
	{
		pDC->SetStretchBltMode(HALFTONE);
		img.Draw(pDC->GetSafeHdc(), rect1);
	}
	if (rect2.Width() > 1 && rect2.Height() > 1)
	{
		pDC->SetStretchBltMode(HALFTONE);
		img.Draw(pDC->GetSafeHdc(), rect2);
	}
	if (!img.IsNull())
	{
		img.Destroy();
	}
}

/*! @function
********************************************************************************
<PRE>
函数名   : DrawPlayed
功能     : 绘制禁止区背景
参数     : [IN]	 参数1 : CD设备指针
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  根据通道位置和当前位置获取已播放位置信息
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::DrawPlayed(CDC *pDC)
{
	//1、获得通道位置信息
	//2、获得当前窗口位置信息
	CRect rect;
	CImage img;
	GetChannelRect(rect);
	if(rect.Height() == 0 
		|| rect.Width() == 0) return;
	if(!img.IsNull())img.Destroy();
	if (m_Style & TBS_VERT)
	{
		rect.bottom = (INT)m_CurPos + m_ChannelRect.top;
		if (rect.Height() < m_ChannelRect.top)
		{
			rect.top = m_ChannelRect.top;
		}
	}
	else
	{
		rect.right = (INT)m_CurPos + m_ChannelRect.left;
		if (rect.Width() < m_ChannelRect.left)
		{
			rect.left = m_ChannelRect.left;
		}
	}
	if (rect.Width() > 0 && rect.Height() > 0)
	{
		pDC->SetStretchBltMode(HALFTONE);
		LoadImageFromResource(&img, m_uiPlayed, TEXT("PNG"));
		img.Draw(pDC->GetSafeHdc(), rect);
		
	}
	if (!img.IsNull())
	{
		img.Destroy();
	}
}


//设置播放信息
VOID COwnSliderCtrl::SetDisplayInfo(_DISPLAY_INFO DisplayInfo)
{
	m_DisplayInfo = DisplayInfo;
	//重新初始化
	Init();
	Invalidate();
}

BOOL COwnSliderCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;	// 阻止擦除背景，防止闪烁
}

VOID COwnSliderCtrl::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_ThumbState = LEAVE;
	m_ChannelState = LEAVE;
	m_MouseState = LEAVE;
	m_ThumbRect.left = m_ThumbRect.top = 0;
	m_ThumbRect.right = 15;
	m_ThumbRect.bottom = 9;
	m_ChannelRect.left = m_ChannelRect.top = 0;
	m_ChannelRect.right = 99;
	m_ChannelRect.bottom = 5;
}

/*! @function
********************************************************************************
<PRE>
函数名   : DrawThumb
功能     : 绘制滑块
参数     : [IN]	 参数1 : CD设备指针
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::DrawThumb(CDC *pDC)
{
	CRect rect, srcRect;
	UINT backImg;
	CImage img;
	GetThumbRect(rect);
	if (m_ThumbState == LBUTTONUP)
	{
		backImg = m_uiThumbUnselected;
	}
	else
	{
		backImg = m_uiThumbSelected;
	}
	LoadImageFromResource(&img, backImg, TEXT("PNG"));
	srcRect.top = 0;
	srcRect.left = 0;
	srcRect.right = img.GetWidth();
	srcRect.bottom = img.GetHeight();
	if (!img.IsNull())
	{
		img.AlphaBlend(pDC->GetSafeHdc(), rect, srcRect, m_Alpha, AC_SRC_OVER);
		img.Destroy();
	}
}

/*! @function
********************************************************************************
<PRE>
函数名   : DrawChannel
功能     : 绘制通道背景
参数     : [IN]	 参数1 : CD设备指针
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::DrawChannel(CDC *pDC)
{
	CRect rect;
	CImage img;
	UINT backImg;
	GetChannelRect(rect);
	if(rect.Height() == 0 
		|| rect.Width() == 0) return;
	if(!img.IsNull())img.Destroy();
	pDC->SetStretchBltMode(HALFTONE);
	switch (m_ChannelState)
	{
	case HOVER:
	case RBUTTONDOWN:
		backImg = m_uiChannelHover;
		break;
	default:
		backImg = m_uiChannelLeave;
		break;
	}
	LoadImageFromResource(&img, backImg, TEXT("PNG"));
	img.Draw(pDC->GetSafeHdc(), rect);
	img.Destroy();
}

/*! @function
********************************************************************************
<PRE>
函数名   : DrawBack
功能     : 绘制客户区背景
参数     : [IN]	 参数1 : CD设备指针
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::DrawBack(CDC *memDC)
{
	CRect rect;
	CImage img;
	GetClientRect(&rect);
	if(rect.Height() == 0 
		|| rect.Width() == 0)return;
	if(!img.IsNull())img.Destroy();
	memDC->SetStretchBltMode(HALFTONE);
	LoadImageFromResource(&img, m_uiBack, TEXT("PNG"));
	img.Draw(memDC->GetSafeHdc(), rect);
	img.Destroy();
}

/*! @function
********************************************************************************
<PRE>
函数名   : DrawSignLine
功能     : 绘制标记线
参数     : [IN]	 参数1 : CD设备指针
		   [IN]	 参数2 : 客户区位置
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::DrawSignLine(CDC *pDC, CPoint pos)
{
	CRect rect;
	CBrush brush(RGB(0, 0, 255));//蓝色
	rect = m_ChannelRect;
	if (m_Style & TBS_VERT)
	{
		rect.top = pos.y;
		rect.bottom = rect.top + 1;
	}
	else
	{
		rect.left = pos.x;
		rect.right = rect.left + 1;
	}
	pDC->FillRect(&rect, &brush);
	brush.DeleteObject();
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetChannelRect
功能     : 获得通道客户区位置
参数     : [IN]		   : 无
           [OUT] 参数1 : 矩形类
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::GetChannelRect(CRect &rect)
{
	rect = m_ChannelRect;
}

/*! @function
********************************************************************************
<PRE>
函数名   : SetChannelRect
功能     : 设置通道客户区位置
参数     : [IN]	 参数1 : 矩形位置
           [OUT]       : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::SetChannelRect(CRect rect)
{
	if (m_Style & TBS_VERT)
	{
		if (m_ThumbRect.Height() >= rect.Height())
		{
			return;
		}
	}
	else
	{
		if (m_ThumbRect.Width() >= rect.Width())
		{
			return ;
		}
	}
	m_ChannelRect = rect;
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetThumbRect
功能     : 获得滑块客户区位置
参数     : [IN]	       : 无
           [OUT] 参数1 : 矩形位置
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::GetThumbRect(CRect &rect)
{
	rect = m_ThumbRect;
}

/*! @function
********************************************************************************
<PRE>
函数名   : SetThumbRect
功能     : 获得滑块客户区位置
参数     : [IN]	 参数1 : 矩形位置
           [OUT]       : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  滑块仅在播放区运动
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::SetThumbRect(CRect rect)
{
	if (m_Style & TBS_VERT)
	{
		if (rect.top < GetPosByTime(m_MinValue))
		{
			rect.top = GetPosByTime(m_MinValue);
			rect.bottom = rect.top + m_ThumbRect.Height();
		}
		if (rect.bottom > GetPosByTime(m_MaxValue))
		{
			rect.bottom = GetPosByTime(m_MaxValue);
			rect.top = rect.bottom - m_ThumbRect.Height();
		}
		if (rect.top <= 0)
		{
			rect.top = 0;
			rect.bottom = rect.top + m_ThumbRect.Height();
		}
		if (rect.bottom >= m_ChannelRect.Height())
		{
			rect.bottom = m_ChannelRect.Height();
			rect.top = rect.bottom - m_ThumbRect.Height();
		}
	}
	else
	{
		if (rect.left < GetPosByTime(m_MinValue))
		{
			rect.left = GetPosByTime(m_MinValue);
			rect.right = rect.left + m_ThumbRect.Width();
		}
		if (rect.right > GetPosByTime(m_MaxValue))
		{
			rect.right = GetPosByTime(m_MaxValue);
			rect.left = rect.right - m_ThumbRect.Width();
		}
		if (rect.left <= 0)
		{
			rect.left = 0;
			rect.right = rect.left + m_ThumbRect.Width();
		}
		if (rect.right >= m_ChannelRect.Width())
		{
			rect.right = m_ChannelRect.Width();
			rect.left = rect.right - m_ThumbRect.Width();
		}
	}
	m_ThumbRect = rect;
}

/*! @function
********************************************************************************
<PRE>
函数名   : MoveThumb
功能     : 移动滑块客户区位置
参数     : [IN]	 参数1 : 客户区位置
           [OUT]       : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  滑块仅在播放区运动
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::MoveThumb(CPoint point)
{
	CRect newThumb = m_ThumbRect;
	
	if (m_Style & TBS_VERT)
	{
		if (point.y > GetPosByTime(m_MaxValue) + m_ChannelRect.top)
		{
			point.y = GetPosByTime(m_MaxValue) + m_ChannelRect.top;
		}
		if (point.y < GetPosByTime(m_MinValue) + m_ChannelRect.top)
		{
			point.y = GetPosByTime(m_MinValue) + m_ChannelRect.top;
		}
		int height = m_ThumbRect.Height();
		newThumb.top = (point.y - height / 2);
		newThumb.bottom = newThumb.top + height;
	}
	else
	{	
		if (point.x > GetPosByTime(m_MaxValue) + m_ChannelRect.left)
		{
			point.x = GetPosByTime(m_MaxValue) + m_ChannelRect.left;
		}
		if (point.x < GetPosByTime(m_MinValue) + m_ChannelRect.left)
		{
			point.x = GetPosByTime(m_MinValue) + m_ChannelRect.left;
		}
		int width = m_ThumbRect.Width();
		newThumb.left = (point.x - width / 2);
		newThumb.right = newThumb.left + width;
	}
	SetThumbRect(newThumb);
}
/*! @function
********************************************************************************
<PRE>
函数名   : JudgeArea
功能     : 判断滑块所在区域是否超过客户区
参数     : [IN]	 参数1 : 客户区位置
           [OUT]       : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  不允许超出空间范围
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
UINT COwnSliderCtrl::JudgeArea(CPoint point)
{
	if (point.x >= m_ThumbRect.left 
		&& point.x <= m_ThumbRect.right
		&& point.y >= m_ThumbRect.top
		&& point.y <= m_ThumbRect.bottom)
	{
		return AREA_THUMB;
	}
	else if (point.x >= m_ChannelRect.left 
		&& point.x <= m_ChannelRect.right
		&& point.y >= m_ChannelRect.top
		&& point.y <= m_ChannelRect.bottom)
	{
		return AREA_CHANNEL;
	}
	else
	{
		return AREA_BACK;
	}
}

VOID COwnSliderCtrl::OnTimer(UINT_PTR nIDEvent)
{
	CRect rect;
	GetThumbRect(rect);
	switch (nIDEvent)
	{
	case TIMER_THUMB_SHOW:
		if (m_Alpha > 250)
		{
			m_Alpha = 250;
			KillTimer(TIMER_THUMB_SHOW);
		}
		else
		{
			m_Alpha += 30;
		}
		break;
	case TIMER_THUMB_HIDE:
		m_Alpha = 0;
		KillTimer(TIMER_THUMB_HIDE);
		break;
	}
}


/*! @function
********************************************************************************
<PRE>
函数名   : ImplementStyle
功能     : 根据样式设置通道和滑块的位置
参数     : [IN]	 参数1 : 客户区位置
           [OUT]       : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::ImplementStyle(UINT style)
{
	INT centerY, centerX;
	CRect newChannelRect, newThumbRect, ClientRect;
	GetClientRect(&ClientRect);
	centerY = ClientRect.Height() / 2;
	centerX = ClientRect.Width() / 2;
	/*凹槽和滑块均居中*/
	if (style & TBS_VERT)
	{
		/*垂直中*/
		newChannelRect.top = 0;
		newChannelRect.bottom = ClientRect.Height();
		newChannelRect.left = centerX - m_ChannelRect.Width() / 2;
		newChannelRect.right = centerX + m_ChannelRect.Width() / 2;
		if (style & TBS_HLEFT)
		{
			/*水平左*/
			newThumbRect.top = m_CurPos;
			newThumbRect.bottom = m_CurPos + m_ThumbRect.Height();
			newThumbRect.left = m_ChannelRect.left;
			newThumbRect.right = newThumbRect.left + m_ThumbRect.Width();
		}
		else if (style & TBS_HRIGHT)
		{
			/*水平右*/
			newThumbRect.top = m_CurPos;
			newThumbRect.bottom = m_CurPos + m_ThumbRect.Height();
			newThumbRect.right = m_ChannelRect.right;
			newThumbRect.left = newThumbRect.right - m_ThumbRect.Width();
		}
		else
		{
			/*水平中*/
			newThumbRect.top = m_CurPos;
			newThumbRect.bottom = m_CurPos + m_ThumbRect.Height();
			newThumbRect.left = centerX - m_ThumbRect.Width() / 2;
			newThumbRect.right = centerX + m_ThumbRect.Width() / 2;
		}
	}
	else
	{
		/*水平居中*/
		newChannelRect.left = 0;
		newChannelRect.right = ClientRect.Width();
		newChannelRect.top = centerY - m_ChannelRect.Height() / 2;
		newChannelRect.bottom = centerY + m_ChannelRect.Height() / 2;
		if (style & TBS_VTOP)
		{
			/*垂直上*/
			newThumbRect.left = m_CurPos;
			newThumbRect.right = m_CurPos + m_ThumbRect.Width();
			newThumbRect.top = m_ChannelRect.top;
			newThumbRect.bottom = newThumbRect.top + m_ThumbRect.Height();
		}
		else if (style & TBS_VBOTTOM)
		{
			/*垂直下*/
			newThumbRect.left = m_CurPos;
			newThumbRect.right = m_CurPos + m_ThumbRect.Width();
			newThumbRect.bottom = m_ChannelRect.bottom;
			newThumbRect.top = newThumbRect.bottom - m_ChannelRect.Height();
		}
		else
		{
			/*垂直中*/
			newThumbRect.left = m_CurPos;
			newThumbRect.right = m_CurPos + m_ThumbRect.Width();
			newThumbRect.top = centerY - m_ThumbRect.Height() / 2;
			newThumbRect.bottom = centerY + m_ThumbRect.Height() / 2;
		}
	}
	SetChannelRect(newChannelRect);
	SetThumbRect(newThumbRect);
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetValue
功能     : 获得总的时间间隔
参数     : [IN]	       : 无
           [OUT]       : 无
           
返回值   : 总时间
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  总时间以秒做单位
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
DOUBLE COwnSliderCtrl::GetValue()
{
	return m_Value;
}

/*! @function
********************************************************************************
<PRE>
函数名   : SetValue
功能     : 设置总的时间间隔
参数     : [IN]	       : 无
           [OUT]       : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::SetValue(DWORD value)
{
	CRect rect;
	GetClientRect(rect);
	m_Value = value;
	//重新设置
	m_Unit = value / (DOUBLE)rect.Width();
	SetLineSize((DWORD)((m_MaxValue - m_MinValue) / m_Unit));
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetLineSize
功能     : 获得播放区物理值大小
参数     : [IN]	       : 无
           [OUT]       : 无
           
返回值   : 物理值
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
DWORD COwnSliderCtrl::GetLineSize( ) const
{
	return m_LineSize;
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetPosByTime
功能     : 根据对应物理值获得客户区位置
参数     : [IN]	       : 时间
           [OUT]       : 无
           
返回值   : 客户区像素位置
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
DOUBLE COwnSliderCtrl::GetPosByTime(DWORD time)
{
	return abs(m_Unit) > 10E-5 ? time / m_Unit : 0.0;
}

/*! @function
********************************************************************************
<PRE>
函数名   : SetLineSize
功能     : 设置播放域物理值大小
参数     : [IN]	 参数1 : 时间
           [OUT]       : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  未完全实现
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
BOOL COwnSliderCtrl::SetLineSize( DWORD nSize )
{
	m_LineSize = nSize;
	return TRUE;
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetPageSize
功能     : 获得绝对时间间隔
参数     : [IN]	       : 无
           [OUT]       : 无
           
返回值   : 绝对时间大小
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  未完全实现
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
INT COwnSliderCtrl::GetPageSize( ) const
{
	return m_PageSize;
}

/*! @function
********************************************************************************
<PRE>
函数名   : SetPageSize
功能     : 设置绝对时间间隔
参数     : [IN]	 参数1 : 时间间隔
           [OUT]       : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  未完全实现
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
INT COwnSliderCtrl::SetPageSize( INT nSize )
{
	m_PageSize = nSize;
	return TRUE;
}
/*! @function
********************************************************************************
<PRE>
函数名   : GetRangeMax
功能     : 获得播放域物理值最大值
参数     : [IN]	       : 无
           [OUT]       : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
DWORD COwnSliderCtrl::GetRangeMax( ) const
{
	return m_MaxValue;
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetRangeMin
功能     : 获得播放域物理值最小值
参数     : [IN]	       : 无
           [OUT]       : 无
           
返回值   : 播放区物理最小值
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
DWORD COwnSliderCtrl::GetRangeMin( ) const
{
	return m_MinValue;
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetRange
功能     : 获得播放域物理值最小值、最大值
参数     : [IN]	       : 无
           [OUT] 参数1 : 最小值
           [OUT] 参数2 : 最大值
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::GetRange( DWORD& nMin, DWORD& nMax ) const
{
	nMin = m_MinValue;
	nMax = m_MaxValue;
}

/*! @function
********************************************************************************
<PRE>
函数名   : SetRange
功能     : 设置播放域物理值最小值、最大值
参数     : [IN] 参数1 : 最小值
           [IN] 参数2 : 最大值
           [OUT]      : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::SetRange( DWORD nMin, DWORD nMax, BOOL bRedraw)
{
	if (nMin < 0 || nMin > m_Value
		|| nMax < 0 || nMax > m_Value
		|| nMin >= nMax)
	{
		return;
	}
	m_MinValue = nMin;
	m_MaxValue = nMax;
	SetLineSize(DWORD((m_MaxValue - m_MinValue) / m_Unit));
	if (m_CurPos < GetPosByTime(m_MinValue))
	{
		SetPos(GetPosByTime(m_MinValue));
	}
	Invalidate();
}

/*! @function
********************************************************************************
<PRE>
函数名   : DrawLargerMirror
功能     : 绘制放大镜
参数     : [IN] 参数1 : DC设备指针
           [IN] 参数2 : 起点位置
           [OUT]      : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::DrawLargerMirror(CDC *pDC, CPoint pos)
{
	CDC *tdc = m_LargerMirror.GetDC();
	CRect mrect, srect, rect;
	GetWindowRect(rect);
	m_LargerMirror.GetClientRect(&mrect);
	srect = m_ChannelRect;
	if (m_Style & TBS_VERT)
	{
		srect.top = pos.y - m_ThumbRect.Height() / 2;
		srect.bottom = srect.top + m_ThumbRect.Height() - 1;
		srect.left = m_ChannelRect.left;
		srect.right = rect.Width() - 1;
		if (srect.top < 0)
		{
			srect.top = 0;
			srect.bottom = srect.top + m_ThumbRect.Height() - 1;
		}
		if (srect.bottom > rect.Height() - 1)
		{
			srect.bottom = rect.Height() - 1;
			srect.top = srect.bottom - m_ThumbRect.Height() + 1;
		}

		pos.y -= mrect.Width() / 2;
	}
	else
	{
		srect.left = pos.x - m_ThumbRect.Width() / 2;
		srect.right = srect.left + m_ThumbRect.Width() - 1;
		srect.top = m_ChannelRect.top;
		srect.bottom = rect.Height() - 1;
		if (srect.left < 0)
		{
			srect.left = 0;
			srect.right = srect.right + m_ThumbRect.Width() - 1;
		}
		if (srect.right > rect.Width() - 1)
		{
			srect.right = rect.Width() - 1;
			srect.left = srect.right - m_ThumbRect.Width() + 1;
		}
		pos.x -= mrect.Width() / 2;
	}
	tdc->StretchBlt(1, 1, mrect.Width() - 2, mrect.Height() - 2, 
		pDC, srect.left, srect.top, srect.Width(), srect.Height(), SRCCOPY);
	ReleaseDC(tdc);
	pos.x += rect.left;
	pos.y += rect.top + 20;
	m_LargerMirror.MoveWindow(pos.x, pos.y, mrect.Width(), mrect.Height());
	m_LargerMirror.ShowWindow(SW_SHOW);
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetSelection
功能     : 获取通道绝对时间最小值、最大值
参数     : [IN]	       : 无
           [OUT] 参数1 : 物理最小值
           [OUT] 参数2 : 物理最大值
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  未实现
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::GetSelection( DWORD& nMin, DWORD& nMax ) const
{
	if (nMin < 0 || nMin > m_Value
		|| nMax < 0 || nMax > m_Value
		|| nMax <= nMin)
	{
		return;
	}
	nMax = m_nChannelMax ;
	nMin = m_nChannelMin ;
}

/*! @function
********************************************************************************
<PRE>
函数名   : SetSelection
功能     : 设置通道绝对时间最小值、最大值
参数     : [IN]  参数1 : 物理最小值
           [IN]  参数2 : 物理最大值
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  未实现
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::SetSelection( DWORD nMin, DWORD nMax )
{
	m_nChannelMin = nMin;
	m_nChannelMax = nMax;
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetPos
功能     : 获取当前滑块客户区位置
参数     : [IN]        : 无
           [OUT]	   : 无
           
返回值   : 客户区位置
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
DOUBLE COwnSliderCtrl::GetPos( ) const
{
	return m_CurPos;
}

/*! @function
********************************************************************************
<PRE>
函数名   : SetPos
功能     : 设置当前滑块客户区位置
参数     : [IN]  参数1 : 客户区位置
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::SetPos( DOUBLE nPos )
{
	CRect rect;
	GetClientRect(rect);
	if (nPos < 0)
	{
		return;
	}
	else if (nPos > rect.Width())
	{
		nPos = rect.Width();
	}
	m_CurPos = nPos;
	MoveThumb(m_CurPos);
	Invalidate();
}

//*************************待实现函数**************************************
UINT COwnSliderCtrl::GetNumTics( ) const
{
	return m_NumTick;
}

DWORD* COwnSliderCtrl::GetTicArray( ) const
{
	DWORD* Tic = new DWORD(m_NumTick);
	INT i;
	for (i = 0; i < m_NumTick; i++)
	{
		Tic[i] = i * m_TickFeq;
	}
	return Tic;
}

INT COwnSliderCtrl::GetTic( INT nTic ) const
{
	return nTic * m_TickFeq;
}

INT COwnSliderCtrl::GetTicPos( INT nTic ) const
{
	return m_nChannelMin + nTic * m_TickFeq;
}

BOOL COwnSliderCtrl::SetTic( INT nTic )
{
	//DrawTic;
	return 0;
}

VOID COwnSliderCtrl::SetTicFreq( INT nFreq )
{
	m_TickFeq = nFreq;
}

CWnd* COwnSliderCtrl::GetBuddy( BOOL fLocation ) const
{
	CWnd *pcwnd = new CWnd();
	pcwnd->Attach(GetSafeHwnd());
	return pcwnd;
}

CWnd* COwnSliderCtrl::SetBuddy( CWnd* pWndBuddy, BOOL fLocation )
{
	pWndBuddy->Attach(GetSafeHwnd());
	return GetBuddy();
}

CMyToolTipCtrl* COwnSliderCtrl::GetToolTips( ) const
{
	return m_ToolTipCtrl;
}

VOID COwnSliderCtrl::SetToolTips( CMyToolTipCtrl* pWndTip )
{
	m_ToolTipCtrl = pWndTip;
}

INT COwnSliderCtrl::SetTipSide( INT nLocation )
{
	m_TipSide = nLocation;
	return TRUE;
}
//*************************待实现函数***********************************************

BOOL COwnSliderCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CWnd::PreTranslateMessage(pMsg);
}

/*! @function
********************************************************************************
<PRE>
函数名   : OnMyPaint
功能     : 设置各元素背景图
参数     : [IN]  参数1 : 滑块左边颜色
		   [IN]  参数2 : 滑块右边颜色
		   [IN]  参数3 : 滑块正常状态背景
		   [IN]  参数4 : 滑块激活状态背景
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::OnMyPaint(COLORREF left/*☜颜色*/, COLORREF right/*→_→颜色*/, 
	UINT thumbUnselected/*未按下*/, UINT thumbSelected/*按下*/)
{
	VERIFY(left);
	VERIFY(right);
	m_LeftColor = left;
	m_RightColor = right;
	m_uiThumbUnselected = thumbUnselected;
	m_uiThumbHover = thumbUnselected;
	m_uiThumbSelected = thumbSelected;
}

VOID COwnSliderCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_bSign && m_ChannelRect.PtInRect(point))
	{
		//调整位置
		if (m_Style & TBS_VERT)
		{
			point.x = m_ChannelRect.Width() / 2;
		}
		else
		{
			point.y = m_ChannelRect.Height() / 2;
		}
		m_ChannelState = RBUTTONDOWN;
		m_SignLine = point;
		Invalidate();
	}
	CWnd::OnRButtonDown(nFlags, point);
}

/*! @function
********************************************************************************
<PRE>
函数名   : DrawSign
功能     : 绘制标记线
参数     : [IN]  参数1 : DC设置指针
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::DrawSign(CDC *pDC)
{
	CImage img;
	INT pos;
	INT_PTR length;
	CRect rect, destRect;
	CPoint signLine(0, 0);
	CWnd *pWnd = GetParent();
	CDC *pParentDc = NULL;
	GetClientRect(rect);
	length = m_saPointRecords.GetCount();
	if (pWnd)
	{
		pParentDc = pWnd->GetDC();
	}
	for (INT i = 0; i < length; i++)
	{
		if (i >= (INT)m_PointStyle.size() || i >= (INT)m_PointBack.size())
		{
			continue;
		}
		INT sign_align = m_PointStyle[i];
		UINT sign_path = m_PointBack[i];

		LoadImageFromResource(&img, sign_path, TEXT("PNG"));
		m_SignSize.cx = img.GetWidth();
		m_SignSize.cy = img.GetHeight();
		if (m_Style & TBS_VERT)
		{
			if (sign_align == SIGN_RIGHT)
			{
				//垂直进度条右方
				pos = (INT)(_wtof(m_saPointRecords.GetAt(i)) * rect.Height() + 0.5);
				destRect.left = m_ChannelRect.right;
				destRect.right = destRect.left + m_SignSize.cx;
				destRect.top = pos - m_SignSize.cy / 2;
				destRect.bottom = pos + m_SignSize.cy / 2;
				signLine.y = pos;
			}
			else
			{
				//垂直进度条左方
				pos = (INT)(_wtof(m_saPointRecords.GetAt(i)) * rect.Height() + 0.5);
				destRect.right = m_ChannelRect.right;
				destRect.left = destRect.right - m_SignSize.cx;
				destRect.top = pos - m_SignSize.cy / 2;
				destRect.bottom = pos + m_SignSize.cy / 2;
				signLine.y = pos;
			}
		}
		else
		{
			if (sign_align == SIGN_BOTTOM)
			{
				//水平进度条下方
				pos = (INT)(_wtof(m_saPointRecords.GetAt(i)) * rect.Width() + 0.5);
				destRect.left = pos - m_SignSize.cx / 2;
				destRect.right = pos + m_SignSize.cx / 2;
				destRect.top = m_ChannelRect.bottom;
				destRect.bottom = destRect.top + m_SignSize.cy;
				signLine.x = pos;
			}
			else
			{
				//在水平进度条上方
				pos = (INT)(_wtof(m_saPointRecords.GetAt(i)) * rect.Width() + 0.5);
				destRect.left = pos - m_SignSize.cx / 2;
				destRect.right = pos + m_SignSize.cx / 2;
				destRect.bottom = m_ChannelRect.top;
				destRect.top = destRect.bottom - m_SignSize.cy;
				signLine.x = pos;
			}
		}
		DrawSignLine(pDC, signLine);
		if (!img.IsNull())
		{
			if (destRect.left < 0)
			{
				destRect.right = destRect.right - destRect.left;
				destRect.left = 0;
			}
			img.Draw(pDC->GetSafeHdc(), destRect);
			img.Destroy();
		}
	}
	if (pParentDc)
	{
		ReleaseDC(pParentDc);
	}
}

//将当前点加入标记点
VOID COwnSliderCtrl::AddCurPointToSign()
{
	CPoint cur_point;
	if (m_Style & TBS_VERT)
	{
		cur_point.y = GetPos() + m_ChannelRect.top;
		cur_point.x = (m_ChannelRect.left + m_ChannelRect.right) / 2;
	}
	else
	{
		cur_point.x = GetPos() + m_ChannelRect.left;
		cur_point.y = (m_ChannelRect.top + m_ChannelRect.bottom) / 2;
	}

	InsertIntoPointLists(cur_point, 1);
}

VOID COwnSliderCtrl::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CWnd::OnRButtonDblClk(nFlags, point);
}

/*! @function
********************************************************************************
<PRE>
函数名   : SetSign
功能     : 是否绘制标记线
参数     : [IN]  参数1 : TRUE表示是，FALSE表示否 
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::SetSign(BOOL bSign)
{
	m_bSign = bSign;
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetSpeed
功能     : 获得当前播放速度
参数     : [IN]  参数1 : 无
           [OUT]	   : 无
           
返回值   : 播放速度
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
DOUBLE COwnSliderCtrl::GetSpeed()	
{
	return m_Speed;
}


//获得播放帧速度
INT COwnSliderCtrl::GetFrameSpeed()
{
	return m_iFrame;
}

//设置播放帧速度
VOID COwnSliderCtrl::SetFrameSpeed(INT frame)
{
	if (frame < m_iMinFrameSpeed)
	{
		frame = m_iMinFrameSpeed;
	}
	if (frame > m_iMaxFrameSpeed)
	{
		frame = m_iMaxFrameSpeed;
	}
	m_iFrame = frame;
}

VOID COwnSliderCtrl::OnSize(UINT nType, INT cx, INT cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//ResetAll();
	CRect rect;
	GetClientRect(rect);
	if (rect.Width() > 0 
		&& rect.Height() > 0
		&& m_OldSize.cx > 0
		&& m_OldSize.cy > 0)
	{
		if (m_Style & TBS_VERT)
		{
			m_Unit = m_Value / rect.Height();
			m_Speed = 1 / m_Unit;
			m_CurPos = m_CurPos * rect.Height() / (DOUBLE)m_OldSize.cy;
		}
		else
		{
			m_Unit = m_Value / (DOUBLE)rect.Width();
			m_Speed = 1 / m_Unit;
			m_CurPos = m_CurPos * rect.Width() / (DOUBLE)m_OldSize.cx;
		}
		m_OldSize.cx = rect.Width();
		m_OldSize.cy = rect.Height();
	}
}

VOID COwnSliderCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bSign && m_ChannelRect.PtInRect(point))
	{
		InsertIntoPointLists(point, 0);
	}
	else
	{
		CRect rect;
		rect = m_ChannelRect;
		if (m_Style & TBS_VERT)
		{
			rect.top = point.y;
			rect.bottom = rect.top + 1;
		}
		else
		{
			rect.left = point.x;
			rect.right = rect.left + 1;
		}
		InvalidateRect(rect);
	}
	m_ChannelState = RBUTTONUP;
	m_LargerMirror.ShowWindow(SW_HIDE);
	CWnd::OnRButtonUp(nFlags, point);
}

VOID COwnSliderCtrl::InsertIntoPointLists(POINT point, INT flag)
{
	//在ChannelRect中右击
	CString pos = TEXT("");
	CRect rect;
	CRect destRect;
	INT_PTR length, finalLength;
	GetClientRect(rect);
	if (m_Style & TBS_VERT)
	{
		//只需要Y坐标与长度的比例
		pos.Format(TEXT("%f"), ((DOUBLE)(point.y)) / rect.Height());
		destRect.left = m_ChannelRect.right;
		destRect.right = destRect.left + m_SignSize.cx;
		destRect.top = point.y - m_SignSize.cy / 2;
		destRect.bottom = point.y + m_SignSize.cy / 2;
		m_SignLine.y = (LONG)(((DOUBLE)(point.y)) / rect.Height());
	}
	else
	{
		//只需要X坐标与宽度的比例
		pos.Format(TEXT("%f"), ((DOUBLE)(point.x)) / rect.Width());
		destRect.left = point.x - m_SignSize.cx / 2;
		destRect.right = point.x + m_SignSize.cx / 2;
		destRect.top = m_ChannelRect.bottom;
		destRect.bottom = destRect.top + m_SignSize.cy;
		m_SignLine.x = (LONG)(((DOUBLE)(point.x)) / rect.Width());
	}
	//判断是否该点已标记过
	//0号代表右键产生
	//1号代表数据产生
	finalLength = length = m_saPointRecords.GetCount();
	for (INT i = 0; i < finalLength; i++)
	{
		if (m_saPointRecords.GetAt(i).Compare(pos) == 0)
		{
			//判断是否为右键产生
			if (m_PointFlag.at(i) == 1)
			{
				//该点数据产生，不删除
				return;
			}
			//说明右键产生的点已存在,剔除该点,并更新长度
			//添加数据产生的同一位置点
			m_saPointRecords.RemoveAt(i);
			finalLength = m_saPointRecords.GetCount();

			//删除指定位置的样式和图片路径
			vector<INT>::iterator viit;
			vector<UINT>::iterator vsit;
			for (viit = m_PointStyle.begin(); viit != m_PointStyle.end();)
			{
				if (viit - m_PointStyle.begin() == i)
				{
					viit = m_PointStyle.erase(viit);
				}
				else
				{
					viit++;
				}
			}
			for (viit = m_PointFlag.begin(); viit != m_PointFlag.end();)
			{
				if (viit - m_PointFlag.begin() == i)
				{
					viit = m_PointFlag.erase(viit);
				}
				else
				{
					viit++;
				}
			}
			for (vsit = m_PointBack.begin(); vsit != m_PointBack.end();)
			{
				if (vsit - m_PointBack.begin() == i)
				{
					vsit = m_PointBack.erase(vsit);
				}
				else
				{
					vsit++;
				}
			}
		}
	}
	if (finalLength == length)
	{
		m_saPointRecords.Add(pos);
		m_PointStyle.push_back(m_SignAlign);
		m_PointBack.push_back(m_uiSignBack);
		m_PointFlag.push_back(flag);
	}
	Invalidate();
	destRect.InflateRect(1, 1, 1, 1);
	InvalidateRect(destRect);
}
//重置播放速度
VOID COwnSliderCtrl::ResetSpeed()
{
	m_Speed = 1;
}

//重置帧速
VOID COwnSliderCtrl::ResetFrameSpeed()
{
	m_iFrame = 1;
}

/*! @function
********************************************************************************
<PRE>
函数名   : SetAlpha
功能     : 设置滑块透明度
参数     : [IN]  参数1 : 透明度
           [OUT]	   : 无
           
返回值   : 无
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
VOID COwnSliderCtrl::SetAlpha(INT alpha)
{
	m_Alpha = alpha;
}

/*! @function
********************************************************************************
<PRE>
函数名   : GetAlpha
功能     : 获得滑块透明度
参数     : [IN]		   : 无
           [OUT]	   : 无
           
返回值   : 透明度
抛出异常 : 无
           
--------------------------------------------------------------------------------
备注     :  
典型用法 : 
--------------------------------------------------------------------------------
作者     : 郑迪威 南昌航空大学 信息工程学院 网络工程系 
时间     : 2015/11/23
*******************************************************************************/
INT COwnSliderCtrl::GetAlpha()
{
	return m_Alpha;
}


VOID COwnSliderCtrl::SetSignBack(UINT utSignBack)
{
	m_uiSignBack = utSignBack;
}
//设置标记位置样式
VOID COwnSliderCtrl::SetSignAlign(INT AlignStyle)
{
	m_SignAlign = AlignStyle;
}