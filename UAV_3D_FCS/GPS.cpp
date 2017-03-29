#include "stdafx.h"
#include "GPS.h"


CGPS::CGPS()
{
}


CGPS::~CGPS()
{
}

 

/*! @function
********************************************************************************
* 函数名   : DrawStaticBackground
* 功能     : 绘制GPS的静态背景
* 参数     : pDC 绘图指针
* 返回值   : void
* 备注     : 使用pDC指针来绘制，显示下面的五个数据的静态文字部分
			 m_LOTD                 //即时经度(°) 
			 m_LATD                 //即时纬度(°)
			 m_SPH                  //标准气压高度(ft)
			 m_PRH                  //气压修正高度(ft)
			 m_RH                   //无线电高度(ft)
			 显示格式：
				变量名称：变量值
			 控件为黑色背景
*******************************************************************************/
void CGPS::DrawStaticBackground(CDC *pDC)
{ 

	GetClientRect(&m_rectCtrl);
	CString show_str("GPS"); 
	int start_x = m_rectCtrl.left;
	int start_y = m_rectCtrl.top; 

	//Show "GPS"
	DrawUavText(pDC, show_str, start_x + GPSPace::title_x_offset, start_y, UavViewPace::font_type, UavViewPace::font_size, UavViewPace::WHITE);

	show_str = "Lat:";
	DrawUavText(pDC, show_str, start_x + GPSPace::x_offset, start_y + 20, UavViewPace::font_type, UavViewPace::font_size, UavViewPace::GREEN);

	show_str = "Lot:";
	DrawUavText(pDC, show_str, start_x + GPSPace::x_offset, start_y + 35, UavViewPace::font_type, UavViewPace::font_size, UavViewPace::GREEN);

	show_str = "SPH:";
	DrawUavText(pDC, show_str, start_x + GPSPace::x_offset, start_y + 50, UavViewPace::font_type, UavViewPace::font_size, UavViewPace::GREEN);

	show_str = "PRH:";
	DrawUavText(pDC, show_str, start_x + GPSPace::x_offset, start_y + 65, UavViewPace::font_type, UavViewPace::font_size, UavViewPace::GREEN);

	show_str = " RH:";
	DrawUavText(pDC, show_str, start_x + GPSPace::x_offset, start_y + 80, UavViewPace::font_type, UavViewPace::font_size, UavViewPace::GREEN);

}



/*! @function
********************************************************************************
* 函数名   : DrawDynamicBackground
* 功能     : 绘制GPS的动态背景
* 参数     : pDC 绘图指针
* 返回值   : void
* 备注     : 使用pDC指针来绘制，显示下面的五个数据的动态变量部分
			m_LOTD                 //即时经度(°)
			m_LATD                 //即时纬度(°)
			m_SPH                  //标准气压高度(ft)
			m_PRH                  //气压修正高度(ft)
			m_RH                   //无线电高度(ft)
			显示格式：
				变量名称：变量值
			控件为黑色背景
*******************************************************************************/
void CGPS::DrawDynamicBackground(CDC *pDC)
{ 
	GetClientRect(&m_rectCtrl);
	CString show_str("");
	int start_x = m_rectCtrl.left;
	int start_y = m_rectCtrl.top;

	//Show m_LATD
	show_str.Format(_T("%5.2f"), m_TSPIDC.m_LATD);  
	DrawUavText(pDC, show_str, start_x + GPSPace::var_x_offset, start_y + 20, UavViewPace::font_type, UavViewPace::font_size, UavViewPace::GREEN);

	show_str.Format(_T("%5.2f"), m_TSPIDC.m_LOTD);
	DrawUavText(pDC, show_str, start_x + GPSPace::var_x_offset, start_y + 35, UavViewPace::font_type, UavViewPace::font_size, UavViewPace::GREEN);

	show_str.Format(_T("%5.2f"), m_TSPIDC.m_SPH);
	DrawUavText(pDC, show_str, start_x + GPSPace::var_x_offset, start_y + 50, UavViewPace::font_type, UavViewPace::font_size, UavViewPace::GREEN);
	
	show_str.Format(_T("%5.2f"), m_TSPIDC.m_PRH);
	DrawUavText(pDC, show_str, start_x + GPSPace::var_x_offset, start_y + 65, UavViewPace::font_type, UavViewPace::font_size, UavViewPace::GREEN);

	show_str.Format(_T("%5.2f"), m_TSPIDC.m_RH);
	DrawUavText(pDC, show_str, start_x + GPSPace::var_x_offset, start_y + 80, UavViewPace::font_type, UavViewPace::font_size, UavViewPace::GREEN);

}



