#pragma once




#include <deque>
#include <queue>

#include "../MemoryDC.h" 
#include "../stdafx.h"

#define  UM_TIMER_GRAPHIC_PROC  (WM_USER + 1)


// CGraphic

namespace Graphic {

	//队列中一次要显示的点数量:50个点
	const int Deque_Point_Num = 50;

	//普通点的标记
	const int Ordinary_Point = 1;

	//圆点的标记
	const int Circle_Point = 2;

	//三角形点的标记
	const int Triangle_Point = 3;

	//坐标值类型
	typedef int point_coord_inate;

	//点要显示的实际物理值类型
	typedef double point_value_type;

	//点与点之间的间隔长度类型
	typedef double point_interval_type;

	//当前点的类型，1为普通点，2为圆点，3为三角形点
	typedef int point_type;

	//点的颜色类型
	typedef COLORREF point_color;

	//时间信息结构体
	typedef struct
	{
		int hour;
		int min;
		int sec;
		int ms;
	}time_struct;

	//坐标信息结构体
	typedef struct
	{
		point_coord_inate xPos;
		point_coord_inate yPos;
	}point_struct;

	//数据范围结构体
	typedef struct
	{
		double min_scope;
		double max_scope;
		//新增数据单位
		CString data_unit;
	}data_scope_struct;

	//队列基本数据结构体，含有时间，值2种信息
	typedef struct
	{
		//当前时间信息
		time_struct arrive_time; 
		//double arrive_time;

		//当前坐标点值
		point_value_type arrive_value;
	}base_data_struct;

	//队列中点的类型结构体，圆形点，三角形点，普通点
	// 	typedef struct
	// 	{
	// 		//圆形点
	// 		bool isCricle;
	// 		//三角形点
	// 		bool isTriangle;
	// 		//普通点
	// 		bool isOrdinary;
	// 	}point_type;

	//基本点的属性结构体 
	typedef struct
	{
		//当前点的类型
		point_type type;
		//点与点之间的间隔长度
		point_interval_type interval_length;
		//点的颜色
		point_color color;

		//数据范围最小值
		double min;
		//数据范围最大值
		double max;

		//是否已经初始化这个点的数据
		bool hasInit;

	}point_attributes;


	//队列数据完整结构体
	typedef struct
	{
		//点的数据
		base_data_struct base_data;
		//点的属性
		point_attributes attributes;
	}data_struct;

	//Chart_Data为曲线队列中存储的基本数据类型
	typedef std::pair<Graphic::point_struct, Graphic::data_struct> Chart_Data;

	//Chart_Data_Queue为曲线数据队列：单向队列
	//typedef std::queue<Graphic::Chart_Data> Chart_Data_Queue;


	//Chart_Data_Queue为曲线数据队列：双端队列
	typedef std::deque<Graphic::Chart_Data> Chart_Data_Queue;


	//实时监控模式
	const int MonitoringModel = 1;
	//回放模式
	const int PlaybackModel = 0;

}

namespace GraphicColor {
	const COLORREF DARK = RGB(0, 0, 0);
	const COLORREF WHITE = RGB(255, 255, 255);
	const COLORREF RED = RGB(255, 0, 0);
	const COLORREF GREEN = RGB(0, 255, 0);
}

namespace GraphicCoordinate {
	//起始坐标的x，y的比例
	const double ordinary_x_scale = 0.05;
	const double ordinary_y_scale = 0.90;

	//x轴结束坐标的比例
	const double x_scale_of_x_end = 0.95 + 0.02;
	const double y_scale_of_x_end = 0.90;

	//y轴结束坐标的比例 
	const double x_scale_of_y_end = 0.05;
	const double y_scale_of_y_end = 0.05 - 0.02; 
	const double y_scale_of_y_end_top_line = 0.05;

	//x, y轴的刻度数量
	const int x_graduation_num = 11;
	//Y轴刻度数能够变化
	const int y_graduation_num = 24;// 16;


	//x，y轴刻度之间的偏移量
	const double x_graduation_offset = 0.09;
	//Y轴偏移量能够变化 (0.9 - 0.05) / 23 = 0.037
	const double y_graduation_offset = 0.0354;//0.0354 = (ordinary_y_scale - 0.05) / y_graduation_num;//0.046;

	//y轴第一个刻度线的相对于y轴结束的比例
	const double first_y_graduation_offset = y_graduation_offset;

	//x, y轴刻度线的长度比例
	const double x_graduation_length_scale = 0.03;
	const double y_graduation_length_scale = 0.01;

	

	//右侧y轴起始的x，y比例
	const double right_y_start_x_scale = 0.93 + 0.02;
	const double right_y_start_y_scale = 0.9;

	//右侧y轴结束的x，y比例
	const double right_y_end_x_scale = 0.93 + 0.02;
	const double right_y_end_y_scale = 0.05 - 0.02;

	//X轴箭头的x，y比例
	const double x_arrow_end_x_scale = 0.935 + 0.02;
	const double x_arrow_start_y_scale_top = 0.89;
	const double x_arrow_start_y_scale_bot = 0.91;
		   
	//左侧Y轴箭头的x，y比例
	const double y_arrow_end_y_scale = 0.08 - 0.02;
	const double y_arrow_end_x_scale_left = 0.045;
	const double y_arrow_end_x_scale_right = 0.055;

	//红色线条相对于一个刻度的y轴偏移
	const double red_line_y_offset = 0.005; 

	//曲线上画的点的大小
	//const int point_size = 100;

	//x轴每两个数据点之间的x间隔比例
	const double x_offset = (right_y_start_x_scale - 0.01 - ordinary_x_scale) / Graphic::Deque_Point_Num;

	//数据点的半径
	const int point_radius = 2;

	//用于滑动条来调节Y轴刻度时使用的调节比例
	const double adjust_y_graduation_scale = 0.1; 

	//测试使用的值
	const double temp_red_line_scale = 2;

	
////////////////////////////////字体和字体大小//////////////////////////////////////////

	//y轴刻度字体大小
	const int y_scale_font_size = 70;

	//Y轴单位的字体大小
	const int y_unit_font_size = 120;

	//X轴单位的字体大小
	const int x_unit_font_size = 120;

	//坐标系中文本的字体
	const CString font_type = _T("楷体");

	//Y轴数据名称字体大小
	const int y_data_name_font_size = 120;

	//坐标系当前显示的时间 
	const int coordinate_cur_time_font_size = 100;
}





class CGraphic : public CWnd
{
	DECLARE_DYNAMIC(CGraphic)

public:
	CGraphic();
	virtual ~CGraphic();

private: 

	//用于存放控件区域矩形大小的变量
	CRect       m_rectCtrl;				

	//缓冲区内绘制背景的虚拟DC
	CDC			m_dcPlate;					

	//在缓冲区中将图形绘制到此位图中
	CBitmap		m_bitmapPlate;	

	//旧的位置指针
	CBitmap     *m_pbitmapOldPlate; 

	//存储当前点的时间的结构体
	Graphic::time_struct m_time_struct; 

	//存储当前点的坐标的结构体
	Graphic::point_struct m_point_struct; 

	//存储x轴点的比例容器
	std::vector<double> m_x_scale_vec;
private:
	bool m_requiresRedraw = true;

	//滑动条的值
	int m_slider_zoom;

	//滑动条的比例
	double m_slider_scale;

	//y轴坐标最大刻度
	double m_y_max;

	//y轴坐标最小刻度
	double m_y_min;

	//当前曲线显示的值范围的最小值
	double m_value_min;

	//当前曲线显示的值范围的最大值
	double m_value_max; 

	//Y轴刻度数量
	int m_y_graduation_num;

	//Y轴刻度间隔比例
	double m_y_graduation_offset;

	//当前曲线显示模式
	int m_cur_model;

	//当前Y轴数据单位
	CString m_cur_y_unit;

	//当前曲线显示的数据的名称
	CString m_cur_data_name;

	//当前曲线左下角最新时间
	CString m_cur_time;

	//当前是否暂停的文本
	//CString m_cur_btn_pause_text;
private:
	//曲线图核心数据结构 
	Graphic::Chart_Data_Queue m_ChartDataDeque;

public:
	//控件重绘
	void ReconstructControl() { Invalidate(); }

	//曲线数据接口1，每次接收一个数据包(一个点)的数据
	void SetCurrentData_One_Package(Graphic::Chart_Data& chart_data);

	//曲线数据接口2，每次传输一个队列的数据-暂时不用
	void SetCurrentData_One_Deque(const Graphic::Chart_Data_Queue& chart_data_deque);

public:
	//用于初始化清空队列中数据
	void CleanQueueData() { m_ChartDataDeque.clear(); } 

	//得到当前曲线数据队列中p_index处的点的属性
	Graphic::Chart_Data GetIndexPoint(const int p_index) { return m_ChartDataDeque[p_index]; } 

	//得到当前队列大小
	int GetCurrentQueueSize() { return m_ChartDataDeque.size(); }

public:
	//设置y轴min和max刻度值
	void SetYMinAndMaxScale(const double p_y_min, const double p_y_max) {m_y_min = p_y_min; m_y_max = p_y_max;}

	//设置y轴min和max刻度值
	void SetYValueMinAndMax(const double p_value_min, const double p_value_max) {m_value_max = p_value_max; m_value_min = p_value_min;}

	//设置曲线当前的y轴刻度数量
	void SetCurrentYGraduationNum(const int p_y_graduation_num) { m_y_graduation_num = p_y_graduation_num; }

	//设置曲线当前显示数据的模式
	void SetCurModel(const int p_model) { m_cur_model = p_model; }

	//设置当前Y轴刻度的间隔比例
	void SetCurrentYGraduationOffset(const double p_y_graduation_offset) { m_y_graduation_offset = p_y_graduation_offset; }

	//设置当前Y轴数据单位
	void SetYDataUnit(const CString p_cur_unit) { m_cur_y_unit = p_cur_unit; } 

	//设置滑动条的大小
	void SetSliderZoom(const int p_slider_pos) { m_slider_zoom = p_slider_pos; } 

	//设置当前显示的数据名称
	void SetCurDataName(const CString p_data_name) { m_cur_data_name = p_data_name; }

	//设置当前最新时间,增加时间不为0的判断
	void SetCurTime(const CString p_cur_time) { if (_T("00:00:00:000") == p_cur_time){ return; } m_cur_time = p_cur_time; }

	//提供给需要重绘背景逻辑部分的接口
	void ReDraw() { m_requiresRedraw = true; Invalidate(); };
	
	//得到暂停按钮文本
	//void SetCurBtnPauseText(const CString p_cur_btn_pause_text) { m_cur_btn_pause_text = p_cur_btn_pause_text; }
private:
	//绘制曲线静态背景
	void DrawStaticBackground(CDC * dc);	

	//画静态坐标轴
	void DrawStaticAxis(CDC* CDC, const CString p_x_unit, const CString p_y_unit);
 	 
	//画动态曲线
	void DrawDynamicCurve(CMemoryDC* MemDC, Graphic::Chart_Data_Queue &DisplayData);

	//根据对话框大小和相应的坐标比例获取实际的坐标位置
	inline POINT GetRealPoint(CRect rect, double x_scale, double y_scale);  

	//画直线
	void DrawLine(CDC* p_dc, POINT p_start, POINT p_end, COLORREF p_color = GraphicColor::WHITE, int p_line_type = PS_SOLID, int p_line_width = 1);

	//画点
	void DrawPoint(CDC* p_dc, POINT point, COLORREF p_color = GraphicColor::WHITE, int p_line_width = 1, int p_line_type = PS_SOLID);

	//画圆
	void DrawCricle(CDC* p_dc, POINT point_up_left, POINT point_low_right, COLORREF p_color = GraphicColor::WHITE, int p_line_width = 1, int p_line_type = PS_SOLID);

	//画文本
	void DrawGraphicText(CDC* p_dc, const CString& p_show_text, int p_start_x, int p_start_y, CString p_font_type, int p_font_size, COLORREF p_color = GraphicColor::WHITE);
	
	//根据当前选择的Y轴的范围动态绘制Y坐标的刻度
	void DrawDynamicYScale(CDC* p_dc, const double p_y_min, const double p_y_max);

	//用于在OnPaint中绘制静态的内容
	void DrawStaticInOnPaint(CDC* p_dc); 

private: 
	//为m_ChartDataDeque中的数据点添加坐标
	void AddCoordinatePointForChartData(Graphic::Chart_Data& chart_data);

	//计算到达数据点的x坐标比例
	inline double GetArriveXScale(Graphic::Chart_Data& arrive_data);

	//计算到达数据点的y坐标比例
	inline double GetArriveYScale(Graphic::Chart_Data& arrive_data);


public:

	//重新调整队列中元素的坐标
	void AdjustQueuePoint();


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point); 
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


