/***************************************************************
* Copyright (c) 2016, 陈登龙
* All rights reserved.
*
* 文件名称：TcpClient.cpp
* 摘    要：本项目的TCP通信类的实现文件。
*
* 当前版本：1.0
* 作    者：陈登龙
* 完成日期：2016-10-16
***************************************************************/
#include "stdafx.h"
#include "../stdafx.h"
#include "TcpClient.h"


CTcpClient::CTcpClient() : m_pump(), m_client(m_pump)
{

}


CTcpClient::~CTcpClient()
{
	if (m_pump.is_running())
	{
		m_pump.stop_service();
	}
}


/*! @function
********************************************************************************
* 函数名   : SetConnectAddr
* 功能     : 设置TCP的连接地址
* 参数     : p_port 连接的端口
			 p_ip 连接的ip
* 返回值   : void
* 作者     : 陈登龙 南昌航空大学信息工程学院自动控制系
* 日期     : 2016-10-16
*******************************************************************************/
void CTcpClient::SetConnectAddr(unsigned short port, const std::string& ip)
{
	m_client.set_server_addr(port, ip);
}

/*! @function
********************************************************************************
* 函数名   : StartTcpConnect
* 功能     : 开启TCP连接
* 参数     : void
* 返回值   : void
* 作者     : 陈登龙 南昌航空大学信息工程学院自动控制系
* 日期     : 2016-10-16
*******************************************************************************/
void CTcpClient::StartTcpConnect()
{
	m_pump.start_service();
}

/*! @function
********************************************************************************
* 函数名   : StopTcpConnect
* 功能     : 停止TCP连接
* 参数     : void
* 返回值   : void
* 作者     : 陈登龙 南昌航空大学信息工程学院自动控制系
* 日期     : 2016-10-16
*******************************************************************************/
void CTcpClient::StopTcpConnect()
{
	if (m_pump.is_running())
	{
		m_pump.stop();
	}
}

/*! @function
********************************************************************************
* 函数名   : IsTcpConnecting
* 功能     : 判断TCP当前是否已经连接
* 参数     : void
* 返回值   : 如果TCP已经连接返回true，否则返回false
* 作者     : 陈登龙 南昌航空大学信息工程学院自动控制系
* 日期     : 2016-10-16
*******************************************************************************/
bool CTcpClient::IsTcpConnecting()
{
	return m_client.is_connected();
}