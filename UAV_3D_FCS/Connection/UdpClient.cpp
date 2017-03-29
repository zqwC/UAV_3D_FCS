/***************************************************************
* Copyright (c) 2016, 陈登龙
* All rights reserved.
*
* 文件名称：UdpClient.cpp
* 摘    要：本项目的TCP通信类的实现文件。
*
* 当前版本：1.0
* 作    者：陈登龙
* 完成日期：2016-10-25
***************************************************************/
#include "stdafx.h"
#include "UdpClient.h"


CUdpClient::CUdpClient() : m_Pump(), m_client(m_Pump)
{


}


CUdpClient::~CUdpClient()
{
}

/*! @function
********************************************************************************
* 函数名   : StartUdpConnect
* 功能     : 开启UDP连接 
* 参数     : void
* 返回值   : void
* 作者     : 陈登龙 南昌航空大学信息工程学院自动控制系
* 日期     : 2016-10-25
*******************************************************************************/
void CUdpClient::StartUdpConnect()
{ 
	m_Pump.start_service();
}


/*! @function
********************************************************************************
* 函数名   : StopUdpConnect
* 功能     : 断开UDP连接
* 参数     : void
* 返回值   : void
* 作者     : 陈登龙 南昌航空大学信息工程学院自动控制系
* 日期     : 2016-10-25
*******************************************************************************/
void CUdpClient::StopUdpConnect()
{
	if (m_Pump.is_running())
	{
		m_Pump.stop_service();
	}
	
}

/*! @function
********************************************************************************
* 函数名   : SetLocalPort
* 功能     : 设置UDP的本地端口
* 参数     : LocalPort : 本地监听的端口
* 返回值   : void
* 作者     : 陈登龙 南昌航空大学信息工程学院自动控制系
* 日期     : 2016-10-25
*******************************************************************************/
void CUdpClient::SetLocalPort(const unsigned short LocalPort)
{
	m_client.set_local_addr(LocalPort);
}

/*! @function
********************************************************************************
* 函数名   : SetUdpAddr
* 功能     : 设置连接的UDP服务器端口ip和port
* 参数     : ip : 本地监听UDP服务器ip
			 port：本地监听的UDP服务器端口
* 返回值   : void
* 作者     : 陈登龙 南昌航空大学信息工程学院自动控制系
* 日期     : 2016-10-25
*******************************************************************************/
void CUdpClient::SetUdpAddr(const std::string& ip, const unsigned short PeerPort)
{
	m_PeerAddr = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(ip), PeerPort);
}
