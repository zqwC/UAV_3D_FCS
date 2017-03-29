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