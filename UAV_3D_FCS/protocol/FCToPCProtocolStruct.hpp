#pragma once
/***************************************************************
* Copyright (c) 2016, 陈登龙
* All rights reserved.
*
* 文件名称：StructOfProtocols.h
* 摘    要：为本项目定义的通信协议。
*
* 当前版本：1.0
* 作    者：陈登龙
* 完成日期：2016-10-15
***************************************************************/

#include "../kapok/Kapok.hpp"
#include "boost/cstdint.hpp" 


/*! @struct
*************************************************************** 
* 名称  : BaseData
* 功能	: BaseData是为协议提供基类的结构体。
* 作者  : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间  : 2016-10-15
***************************************************************/
struct BaseData 
{
	virtual ~BaseData() {};
};

/*! @struct
***************************************************************
* 名称  : MessageHeader
* 功能	: MessageHeader是为协议提供消息头的结构体。
* 作者  : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间  : 2016-10-15
***************************************************************/
struct MessageHeader
{
	boost::uint16_t MESSAGE_CODE = 0;         //消息代码：3002
	boost::uint16_t MSG_Length = 0;           //报文长度
	boost::uint16_t MESSAGE_COUNTER = 0;      //备用
	boost::uint16_t LogicSendID = 0;          //发方设备ID：网管服务器，取值501
	boost::uint16_t LogicReceID = 0;          //收方设备ID：地面应用系统，取值401
	boost::uint8_t  MESSAGE_LENGTH = 0;       //消息体长度
	boost::uint16_t MESSAGE_NUMBER = 0;       //备用
	boost::uint8_t  COMM_SOURCE_ADDR = 0;     //发送平台地址（飞机编号）
	boost::uint8_t  COMM_TARGET_ADDR = 0;     //接收平台地址
	boost::uint8_t  date = 0;                 //是否跨零点的日期
	boost::uint8_t  hour = 0;                 //24小时制的时
	boost::uint8_t  minute = 0;               //分
	boost::uint8_t  second = 0;               //秒
	boost::uint16_t millSecond = 0;           //毫秒
	boost::uint16_t PID = 0;                  //飞机编号

	META(MESSAGE_CODE, MSG_Length, MESSAGE_COUNTER, LogicSendID,
		LogicReceID, MESSAGE_LENGTH, MESSAGE_NUMBER,
		COMM_SOURCE_ADDR, COMM_TARGET_ADDR,
		date, hour, minute, second, millSecond,
		PID)

		MessageHeader& operator=(const MessageHeader&obj)
	{
		MESSAGE_CODE = obj.MESSAGE_CODE;
		MSG_Length = obj.MSG_Length;
		MESSAGE_COUNTER = obj.MESSAGE_COUNTER;
		LogicSendID = obj.LogicSendID;
		LogicReceID = obj.LogicReceID;
		MESSAGE_LENGTH = obj.MESSAGE_LENGTH;
		MESSAGE_NUMBER = obj.MESSAGE_NUMBER;
		COMM_SOURCE_ADDR = obj.COMM_SOURCE_ADDR;
		COMM_TARGET_ADDR = obj.COMM_TARGET_ADDR;
		date = obj.date;
		hour = obj.hour;
		minute = obj.minute;
		second = obj.second;
		millSecond = obj.millSecond;
		PID = obj.PID;

		return *this;
	}

	bool operator!=(const MessageHeader&obj)
	{
		if ((date != obj.date) || (hour != obj.hour) || (minute != obj.minute) ||
			(second != obj.second) || (millSecond != obj.millSecond))
			return true;
		else
			return false;
	}
};


/*! @struct
***************************************************************
* 名称  : TSPI
* 功能	: TSPI是定义无人机飞行姿态的协议数据的结构体。
* 作者  : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间  : 2016-10-15
* 备注  :  
***************************************************************/
struct TSPI : public BaseData
{
	MessageHeader   m_MessageHeader;     //消息头
	boost::uint16_t m_ID = 0;            //飞机编号  3、4
	boost::uint16_t m_DID = 0;           //目的编号  5、6

	boost::uint16_t m_LLOTD = 0;         //即时经度低位  7、8
	boost::uint16_t m_HLOTD = 0;         //即时经度高位  9、10
	boost::uint16_t m_LLATD = 0;         //即时纬度低位  11、12
	boost::uint16_t m_HLATD = 0;         //即时纬度高位  13、14
	boost::int16_t m_SPH = 0;            //标准气压高度  15、16
	boost::int16_t m_PRH = 0;            //气压修正高度  17、18

	boost::int16_t m_RH = 0;             //无线电高度  19、20
	boost::int16_t m_VS = 0;             //真空速  21、22
	boost::int16_t m_IVS = 0;            //指示空速  23、24
	boost::int16_t m_TH = 0;             //真航向  25、26
	boost::int16_t m_ANGP = 0;           //俯仰角  27、28
	boost::int16_t m_ANGR = 0;           //横滚角  29、30
	boost::int16_t m_ANGPS = 0;          //俯仰角速率  31、32
	boost::int16_t m_ANGRS = 0;          //横滚角速率  33、34

	META(m_MessageHeader, m_ID, m_DID, m_LLOTD, m_HLOTD,
		m_LLATD, m_HLATD, m_SPH, m_PRH,
		m_RH, m_VS, m_IVS, m_TH,
		m_ANGP, m_ANGR, m_ANGPS, m_ANGRS)

		TSPI& operator= (const TSPI& obj)
	{
		m_MessageHeader = obj.m_MessageHeader;
		m_ID = obj.m_ID;
		m_DID = obj.m_DID;
		m_LLOTD = obj.m_LLOTD;
		m_HLOTD = obj.m_HLOTD;
		m_LLATD = obj.m_LLATD;
		m_HLATD = obj.m_HLATD;
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
};

/*! @struct
***************************************************************
* 名称  : ProtocolData
* 功能	: ProtocolData是定义无人机完整飞行姿态的协议数据的结构体。
* 作者  : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间  : 2016-10-15
* 备注  : 无人机协议需要修正。
***************************************************************/
struct ProtocolData
{ 
	TSPI m_TSPI;

	ProtocolData& operator=(const ProtocolData& obj)
	{
		m_TSPI = obj.m_TSPI; 
	} 

	META(m_TSPI)
};
