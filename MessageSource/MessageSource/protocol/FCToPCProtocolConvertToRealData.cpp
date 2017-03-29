/***************************************************************
* Copyright (c) 2016, 陈登龙
* All rights reserved.
*
* 文件名称：StructOfProtocolsConvertToRealData.cpp
* 摘    要：为本项目定义的通信协议实际物理值的实现文件。
*
* 当前版本：1.0
* 作    者：陈登龙
* 完成日期：2016-10-15
***************************************************************/

#include "stdafx.h"
#include "FCToPCProtocolConvertToRealData.h"

static double GetRandom(double min, double max)
{
	int steps = 5000;
	static double t = 0;
	static int dir = 1;
	static double origin = min;
	double result = t * dir * (max - min) / steps + origin;
	if (t >= steps)
	{
		dir = -1 * dir;
		origin = (max + min - (max - min) * dir) / 2;
		t = 0;
	}
	t++;
	return result;
}

static void DecNumToString(double num, CString& str)
{
	str.Format(TEXT("%.2f"), num);
}

static void FormatString(CString name, CString value, CString& tar_str)
{
	tar_str = name + TEXT(":") + value + TEXT(";");
}


/*! @function
************************************************************************
* 函数名   : TSPIToTSPIDC
* 功能     : 将协议值TSPI转换成实际物理值
* 参数     :	tspi 协议结构体
				isToEngUint	是否需要转换成英制
* 返回值   : void
* 作者     : 陈登龙 南昌航空大学信息工程学院自动控制系
* 日期     : 2016-10-15
************************************************************************/
void TSPIDC::TSPIToTSPIDC(const TSPI& tspi, const bool isToEngUint)
{
	m_MessageHeader = tspi.m_MessageHeader;       //消息头
	m_ID = tspi.m_ID;
	m_DID = tspi.m_DID;
	m_LOTD = (double)(((boost::int32_t)tspi.m_LLOTD + (boost::int32_t)(tspi.m_HLOTD << 16)) * 90.0 / 1073741824.0);
	m_LATD = (double)(((boost::int32_t)tspi.m_LLATD + (boost::int32_t)(tspi.m_HLATD << 16)) * 45.0 / 1073741824.0);
	m_SPH = (double)tspi.m_SPH;
	m_PRH = (double)tspi.m_PRH;
	m_RH = (double)tspi.m_RH;
	m_VS = (double)tspi.m_VS / 8;
	m_IVS = (double)tspi.m_IVS / 8;
	m_TH = (double)tspi.m_TH * 90.0 / 16384.0;
	m_ANGP = (double)tspi.m_ANGP * 45.0 / 16384.0;
	m_ANGR = (double)tspi.m_ANGR * 90.0 / 16384.0;
	m_ANGPS = (double)tspi.m_ANGPS * 90.0 / 16384.0;
	m_ANGRS = (double)tspi.m_ANGRS * 180.0 / 16384.0;

	for (; !((m_TH > -0.000001) && (m_TH < 360.000001));)
	{
		m_TH = (m_TH < -0.000001) ? (m_TH + 360.00000000) : (m_TH - 360.00000000);
	}

	if (isToEngUint)
		ConvertToEnglishUnits();

}


void TSPIDC::TSPIDCToTSPI(TSPI& tspi, const bool isToEngUint)
{
	if (isToEngUint)
		ConvertToMetricUnit();
	tspi.m_ID = m_ID;
	tspi.m_DID = m_DID;
	tspi.m_LLOTD = (m_LOTD / 90.0 * 1073741824.0);
	tspi.m_HLOTD = (((boost::int32_t)(m_LOTD / 90.0 * 1073741824.0)) & 0xFFFF0000) >> 16;
	tspi.m_LLATD = (m_LATD / 45.0 * 1073741824.0);
	tspi.m_HLATD = (((boost::int32_t)(m_LATD / 45.0 * 1073741824.0)) & 0xFFFF0000) >> 16;
	tspi.m_SPH = m_SPH;
	tspi.m_PRH = m_PRH;
	tspi.m_RH = m_RH;
	tspi.m_VS = m_VS * 8;
	tspi.m_IVS = m_IVS * 8;
	tspi.m_TH = m_TH / 90.0 * 16384.0;
	tspi.m_ANGP = m_ANGP / 45.0 * 16384.0;
	tspi.m_ANGR = m_ANGR / 90.0 * 16384.0;
	tspi.m_ANGPS = m_ANGPS / 90.0 * 16384.0;
	tspi.m_ANGRS = m_ANGRS / 180.0 * 16384.0;
}

void TSPIDC::ConvertToTable(CString& str)
{
	CString tstr, sstr;
	DecNumToString(m_ID, tstr);
	FormatString(TEXT("飞机编号"), tstr, sstr);
	str += sstr;
	DecNumToString(m_DID, tstr);
	FormatString(TEXT("目的编号"), tstr, sstr);
	str += sstr;
	DecNumToString(m_LOTD, tstr);
	FormatString(TEXT("即时经度"), tstr, sstr);
	str += sstr;
	DecNumToString(m_LATD, tstr);
	FormatString(TEXT("即时纬度"), tstr, sstr);
	str += sstr;
	DecNumToString(m_SPH, tstr);
	FormatString(TEXT("标准气压高度"), tstr, sstr);
	str += sstr;
	DecNumToString(m_PRH, tstr);
	FormatString(TEXT("气压修正高度"), tstr, sstr);
	str += sstr;
	DecNumToString(m_RH, tstr);
	FormatString(TEXT("无线电高度"), tstr, sstr);
	str += sstr;
	DecNumToString(m_VS, tstr);
	FormatString(TEXT("真空速"), tstr, sstr);
	str += sstr;
	DecNumToString(m_IVS, tstr);
	FormatString(TEXT("指示空速"), tstr, sstr);
	str += sstr;
	DecNumToString(m_TH, tstr);
	FormatString(TEXT("真航向"), tstr, sstr);
	str += sstr;
	DecNumToString(m_ANGP, tstr);
	FormatString(TEXT("俯仰角"), tstr, sstr);
	str += sstr;
	DecNumToString(m_ANGR, tstr);
	FormatString(TEXT("横滚角"), tstr, sstr);
	str += sstr;
	DecNumToString(m_ANGPS, tstr);
	FormatString(TEXT("俯仰角速率"), tstr, sstr);
	str += sstr;
	DecNumToString(m_ANGRS, tstr);
	FormatString(TEXT("横滚角速率"), tstr, sstr);
}

/*! @function
************************************************************************
* 函数名   : ConvertToEnglishUnits
* 功能     : 将需要转换的协议值TSPI转换成英制
* 参数     : void
* 返回值   : void
* 作者     : 陈登龙 南昌航空大学信息工程学院自动控制系
* 日期     : 2016-10-15
************************************************************************/
void TSPIDC::ConvertToEnglishUnits()
{
	m_SPH /= 0.3048;
	m_PRH /= 0.3048;
	m_RH  /= 0.3048;
	m_VS  /= 1.852;
	m_IVS /= 1.852;
}


void TSPIDC::ConvertToMetricUnit()
{
	m_SPH *= 0.3048;
	m_PRH *= 0.3048;
	m_RH *= 0.3048;
	m_VS *= 1.852;
	m_IVS *= 1.852;
}


void TSPIDC::CreateRandomTSPIDC()
{
	m_ID = 0;
	m_DID = 0;
	m_LOTD = GetRandom(-180, 180);
	m_LATD = GetRandom(21, 24);
	m_SPH = GetRandom(-1476.377952755906, 65616.7979002624);
	m_PRH = GetRandom(-1476.377952755906, 65616.7979002624);
	m_RH = GetRandom(0, 2000);
	m_VS = GetRandom(0, 1079.91360691144);
	m_IVS = GetRandom(0, 1079.91360691144);
	m_TH = GetRandom(0, 360);
	m_ANGP = GetRandom(-20, 20);
	m_ANGR = GetRandom(-180, 180);
	m_ANGPS = GetRandom(-180, 180);
	m_ANGRS = GetRandom(-360, 360);
}