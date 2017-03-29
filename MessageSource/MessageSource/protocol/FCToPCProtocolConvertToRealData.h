#pragma once
/***************************************************************
* Copyright (c) 2016, 陈登龙
* All rights reserved.
*
* 文件名称：StructOfProtocolsConvertToRealData.h
* 摘    要：为本项目定义的通信协议实际物理值的文件。
*
* 当前版本：1.0
* 作    者：陈登龙
* 完成日期：2016-10-15
***************************************************************/


#include "FCToPCProtocolStruct.hpp"


/*! @struct
***************************************************************
* 名称  : BaseRealData
* 功能	: BaseRealData是为协议提供基类实际物理值的结构体。
* 作者  : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间  : 2016-10-15
***************************************************************/
struct BaseDataDC
{
	virtual ~BaseDataDC() {}
};


/*! @struct
***************************************************************
* 名称  : TSPIDC
* 功能	: TSPIDC是为协议提供实际飞行姿态物理值的结构体。
* 作者  : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间  : 2016-10-15
***************************************************************/
struct TSPIDC : public BaseDataDC
{
	MessageHeader   m_MessageHeader;     //消息头
	boost::uint16_t m_ID = 0;              //飞机编号  3、4
	boost::uint16_t m_DID = 0;             //目的编号  5、6
	double m_LOTD = 0.0;                   //即时经度  7、8、9、10
	double m_LATD = 0.0;                   //即时纬度  11、12、13、14
	double m_SPH = 0.0;                    //标准气压高度  15、16
	double m_PRH = 0.0;                    //气压修正高度  17、18
	double m_RH = 0.0;                     //无线电高度  19、20
	double m_VS = 0.0;                     //真空速  21、22
	double m_IVS = 0.0;                    //指示空速  23、24
	double m_TH = 0.0;                     //真航向  25、26
	double m_ANGP = 0.0;                   //俯仰角  27、28
	double m_ANGR = 0.0;                   //横滚角  29、30
	double m_ANGPS = 0.0;                  //俯仰角速率  31、32
	double m_ANGRS = 0.0;                  //横滚角速率  33、34

	TSPIDC& operator= (const TSPIDC& obj)
	{
		m_MessageHeader = obj.m_MessageHeader;
		m_ID = obj.m_ID;
		m_DID = obj.m_DID;
		m_LOTD = obj.m_LOTD;
		m_LATD = obj.m_LATD;
		m_SPH = obj.m_SPH;
		m_PRH = obj.m_PRH;
		m_RH = obj.m_RH;
		m_VS = obj.m_VS;
		m_IVS = obj.m_IVS;
		m_TH = obj.m_TH;
		m_ANGP = obj.m_ANGP;
		m_ANGR = obj.m_ANGR;
		m_ANGPS = obj.m_ANGPS;
		m_ANGRS = obj.m_ANGRS;

		return *this;
	}
	  
	void ConvertToTable(CString& str);
	void TSPIToTSPIDC(const TSPI& tspi, const bool isToEngUint);
	void TSPIDCToTSPI(TSPI& tspi, const bool isToEngUint);
	void CreateRandomTSPIDC();
private:
	void ConvertToEnglishUnits();
	void ConvertToMetricUnit();
};




