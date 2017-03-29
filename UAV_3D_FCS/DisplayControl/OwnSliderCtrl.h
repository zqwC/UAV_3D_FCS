#pragma once

#include "MyToolTipCtrl.h"
#include "MirrorDlg.h"
#include "../MemoryDC.h"
#include"../resource.h"
class CTimeFormat
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
};


// COwnSliderCtrl

class COwnSliderCtrl : public CWnd
{
#define TIMER_THUMB_SHOW 0x0010
#define TIMER_THUMB_HIDE 0x0020
//滑动条位置样式
#define SLIDER 2000
	enum{
		HOVER,
		DRAG,
		LBUTTONDOWN,
		LBUTTONUP,
		RBUTTONDOWN,
		RBUTTONUP,
		CLICK,
		SELECTED,
		LEAVE
	};
//区域划分
	enum{
		AREA_BACK,
		AREA_CHANNEL,
		AREA_THUMB
	};
//滑块样式
	enum{
		TBS_VTOP = 0x0001,
		TBS_VCENTER = 0x0002,
		TBS_VBOTTOM = 0x0003,
		TBS_HLEFT = 0x0010,
		TBS_HCENTER = 0x0020,
		TBS_HRIGHT = 0x0030
	};
	DECLARE_DYNAMIC(COwnSliderCtrl)
//提示框显示物理值
public:
	//标记样式
	enum{
		SIGN_TOP,
		SIGN_BOTTOM,
		SIGN_LEFT,
		SIGN_RIGHT
	};
	enum{
		TOOLTIP_TIME = 0x0010,
		TOOLTIP_VALUE = 0x0020
	};
	COwnSliderCtrl();
	virtual ~COwnSliderCtrl();
	//重设各元素尺寸、位置
	VOID ResetAll();
	//重设客户区大小
	VOID ResetClientRect();
	//重设通道大小
	VOID ResetChannelRect();
	//重设滑块大小
	VOID ResetThumbRect();
	//滑块状态
	INT m_ThumbState;
	//凹槽状态
	INT m_ChannelState;
	//鼠标当前状态
	INT m_MouseState;
	//物理最小值
	DWORD m_MinValue;
	//物理最大值
	DWORD m_MaxValue;
	//可行区物理长度
	DWORD m_LineSize;
	//当前滑块窗口位置
	DOUBLE m_CurPos;
	//滑块透明度
	INT m_Alpha;
	//待定
	INT m_PageSize;
	//通道物理最大值
	INT m_nChannelMax;
	//通道物理最小值
	INT m_nChannelMin;
	//待定
	INT m_TickFeq;
	//待定
	INT m_NumTick;
	//提示框样式
	INT m_ToolStyle;
	//提示框窗口
	CMyToolTipCtrl *m_ToolTipCtrl;
	//提示框位置样式
	INT m_TipSide;
	//显示动画时间间隔
	UINT m_uElapse;
	//标记样式
	INT m_SignAlign;
	//样式
	DWORD m_Style;
	//滑块左边颜色
	COLORREF m_LeftColor;
	//滑块右边颜色
	COLORREF m_RightColor;
	//滑块未选中背景
	UINT m_uiThumbUnselected;
	//滑块悬浮悬浮背景
	UINT m_uiThumbHover;
	//滑块选中背景
	UINT m_uiThumbSelected;
	//鼠标离开通道背景
	UINT m_uiChannelLeave;
	//鼠标悬浮通道北京
	UINT m_uiChannelHover;
	//已播放进度条背景
	UINT m_uiPlayed;
	//禁止区背景
	UINT m_uiBeyondArea;
	//控件整体背景
	UINT m_uiBack;
	//父窗口
	CWnd* m_Parent;
	//窗体旧大小
	CSize m_OldSize;
	//旧鼠标形状
	HCURSOR m_OldCursor;
	//滑块移动函数
	VOID MoveThumb(CPoint point);
	//判断落点区域
	UINT JudgeArea(CPoint point);
	//获得当前位置的时间
	DOUBLE GetPosTime(UINT curPos);
	//获取当前像素位置的时间
	DOUBLE GetPixelTime(DOUBLE pixel);
	//放大镜
	CMirrorDlg m_LargerMirror;
	//是否开启放大镜
	BOOL m_bMirror;
	//放大镜窗体位置
	CRect m_MirrorRect;
	//最大帧数
	INT m_iMaxFrameSpeed;
	//最小帧数
	INT m_iMinFrameSpeed;
	//控件样式实现
	VOID ImplementStyle(UINT style);
	//动态创建播放条控件
	bool Create(CWnd* pParent, const CRect &rc, CString text, DWORD id = 0,
		DWORD style = WS_VISIBLE | WS_CHILD);
	//获得通道窗体客户区位置
	VOID GetChannelRect(CRect &rect);
	//设置通道客户区位置
	VOID SetChannelRect(CRect rect);
	//获得滑块客户区位置
	VOID GetThumbRect(CRect &rect);
	//设置滑块客户区位置
	VOID SetThumbRect(CRect rect);
	//设置播放信息
	VOID SetDisplayInfo(_DISPLAY_INFO DisplayInfo);
	//设置标记对其样式
	VOID SetSignAlign(INT AlignStyle);
	//初始化所有信息
	VOID Init();
	//获得播放速度
	DOUBLE GetSpeed();
	//获得播放帧速度
	INT GetFrameSpeed();
	//设置播放帧速度
	VOID SetFrameSpeed(INT frame);
	//设置单位帧速度
	//VOID SetSpeed(DOUBLE speed);
	//设置时间总值，以秒为单位
	VOID SetValue(DWORD value);
	//重置速度
	VOID ResetSpeed();
	//重置帧速
	VOID ResetFrameSpeed();
	//获得时间间隔总值
	DOUBLE GetValue();
	//获得播放区长度
	DWORD GetLineSize( ) const;
	//设置播放区
	BOOL SetLineSize( DWORD nSize );
	//设置标记线尺寸
	BOOL SetSignSize( CSize cSize );
	//待定
	INT GetPageSize( ) const;
	//待定
	INT SetPageSize( INT nSize );
	//获得物理最大值
	DWORD GetRangeMax( ) const;
	//获得物理最小值
	DWORD GetRangeMin( ) const;
	//获得值：秒/像素
	DOUBLE GetUnit();
	//设置值：秒/像素
	BOOL SetUnit(DOUBLE unit);
	//获得物理最大值、最小值
	VOID GetRange( DWORD& nMin, DWORD& nMax ) const;
	//设置物理最大值、最小值
	VOID SetRange( DWORD nMin, DWORD nMax, BOOL bRedraw = FALSE );
	//获取通道物理最大值、最小值
	VOID GetSelection( DWORD &nMin, DWORD &nMax ) const;
	VOID SetSelection( DWORD nMin, DWORD nMax );
	//设置通道窗口当前位置
	DOUBLE GetPos( ) const;
	//通过播放时刻获取位置
	DOUBLE GetPosByTime(DWORD time);
	DOUBLE GetPosByTime(DOUBLE passedTime);
	VOID InsertIntoPointLists(POINT point, INT flag);
	//设置播放位置
	VOID SetPos( DOUBLE nPos );
	//待定
	UINT GetNumTics( ) const;
	//待定
	DWORD* GetTicArray( ) const;
	//待定
	INT GetTic( INT nTic ) const;
	//待定
	INT GetTicPos( INT nTic ) const;
	//待定
	BOOL SetTic( INT nTic );
	//待定
	VOID SetTicFreq( INT nFreq );
	//待定
	CWnd* GetBuddy( BOOL fLocation = TRUE ) const;
	//待定
	CWnd* SetBuddy( CWnd* pWndBuddy, BOOL fLocation = TRUE );
	//获得当前绑定的提示框
	CMyToolTipCtrl* GetToolTips( ) const;
	//设置绑定提示框
	VOID SetToolTips( CMyToolTipCtrl* pWndTip );
	//设置提示框位置样式
	INT SetTipSide( INT nLocation );
	//设置是否允许做标记
	VOID SetSign(BOOL bSign);
	//设置是否开启放大镜
	VOID SetMirror(BOOL bMirror);
	//设置标记图片
	VOID SetSignBack(UINT utSignBack);
	//设置透明值
	VOID SetAlpha(INT alpha);
	//获得透明值
	INT	 GetAlpha();
	//设置提示框样式
	VOID SetToolTipStyle(INT style);
	//获取播放位置对应的时间
	CString GetPosValue(UINT curPos);
	//提示框移动
	VOID ToolTipMove(CPoint point);

	VOID OnMyPaint(COLORREF left, COLORREF right, 
		UINT thumbUnselected, UINT thumbSelected);
	//播放条信息
	DISPLAY_INFO m_DisplayInfo;
	//将当前点键入标记点
	VOID AddCurPointToSign();
protected:
	//通道客户端位置
	CRect m_ChannelRect;
	//滑块客户端位置
	CRect m_ThumbRect;
	//控件客户区位置
	CRect m_ClientRect;
	//提示框客户区位置
	CRect m_ToolTipRect;
	//标记尺寸
	CSize m_SignSize;
	//待定
	CPoint m_SignLine;
	//记录标记位置信息
	CStringArray m_saPointRecords;
	//记录标记
	vector<INT> m_PointStyle;
	vector<UINT> m_PointBack;
	vector<INT> m_PointFlag;

	//硬盘存储位置
	CString m_strFileName;
	//类名
	CString m_szClassName;
	//标记背景
	UINT m_uiSignBack;
	//是否允许标记
	BOOL m_bSign;
	//每隔1毫秒前进几像素---即为1帧
	DOUBLE m_Speed;
	//每毫秒几帧
	INT m_iFrame;
	//每像素播放几毫秒
	DOUBLE m_Unit;
	//总共几毫秒
	DOUBLE m_Value;
	//清除背景
	VOID ClearBack(CMemoryDC *pDC);
	//待定
	VOID MoveBorder();
	/************************绘制函数************************/
	//绘制滑块
	VOID DrawThumb(CDC *pDC);
	//绘制通道
	VOID DrawChannel(CDC *pDC);
	//绘制禁止区
	VOID DrawByeondRange(CDC *pDC);
	//绘制已播放区
	VOID DrawPlayed(CDC *pDC);
	//绘制通道背景
	VOID DrawBack(CDC *memDC);
	//绘制标记线
	VOID DrawSignLine(CDC *pDC, CPoint pos);
	//绘制标记
	VOID DrawSign(CDC *pDC);
	//绘制放大镜
	VOID DrawLargerMirror(CDC *pDC, CPoint pos);
	/************************绘制函数************************/
	DECLARE_MESSAGE_MAP()
public:
	VOID PostClickEvent();
	/***************************消息*************************/
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg VOID OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg VOID OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg VOID OnMouseMove(UINT nFlags, CPoint point);
	afx_msg VOID OnMouseHover(UINT nFlags, CPoint point);
	afx_msg VOID OnMouseLeave();
	afx_msg VOID OnPaint();
	afx_msg VOID OnDestroy();
	afx_msg VOID OnTimer(UINT_PTR nIDEvent);/*滑块渐隐渐显*/
	afx_msg VOID OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg VOID OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg VOID OnSize(UINT nType, INT cx, INT cy);
	afx_msg VOID OnRButtonUp(UINT nFlags, CPoint point);
	/****************消息*************************************/
};