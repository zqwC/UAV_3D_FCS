#pragma once
/***************************************************************
* Copyright (c) 2016, 陈登龙
* All rights reserved.
*
* 文件名称：UdpClient.h
* 摘    要：用于本项目的UDP通信类。
*
* 当前版本：1.0
* 作    者：陈登龙
* 完成日期：2016-10-25
***************************************************************/

#include "communication/st_asio_wrapper_base.h"
#include "communication/st_asio_wrapper_udp_socket.h"
#include "communication/st_asio_wrapper_udp_client.h"
#include "communication/st_asio_wrapper_client.h"

using namespace st_asio_wrapper;


/*! @class
********************************************************************************
* 类名称   : CUdpClient
* 功能     : CUdpClient提供基于Boost.Asio完全异步加多线程的网络通信接口。
* 作者     : 陈登龙 南昌航空大学信息工程学院自动控制系
* 当前版本 ：1.0
* 作    者 ：陈登龙
* 完成日期 ：2016-10-16
*******************************************************************************/
class CUdpClient
{
public:
	CUdpClient();
	~CUdpClient();

public:
	/* 开启UDP连接 */
	void StartUdpConnect();

	/* 断开UDP连接 */
	void StopUdpConnect();
	
	/* 设置UDP的本地端口 */
	void SetLocalPort(const unsigned short LocalPort);
	
	/* 设置连接的UDP服务器端口ip和port */
	void SetUdpAddr(const std::string& ip,  const unsigned short PeerPort);

private:
	/*! @class
	********************************************************************************
	* 类名称   : MyUdpConnector
	* 功能     : MyUdpConnector是基于Boost.Asio的st_udp_socket拓展的usp-socket，
				 作为类型传入st_sclient<MyUdpConnector> m_client,
				 作为本项目的UDP通信客户端对象。
	* 作者     : 陈登龙 南昌航空大学信息工程学院自动控制系
	* 当前版本 ：1.0
	* 作    者 ：陈登龙
	* 完成日期 ：2016-10-17
	*******************************************************************************/
	class MyUdpConnector : public st_udp_socket
	{
		typedef std::string msg_type;

		public:
			MyUdpConnector(boost::asio::io_service& io_service_): st_udp_socket(io_service_) {}
		protected:
			virtual bool on_msg(msg_type& msg)
			{
				//自定义处理消息
				AfxMessageBox(_T("Test"));
				return true;
			}
	};

private:
	/* UDP服务泵 */
	st_service_pump m_Pump;

	/* UDP客户端对象 */
	st_sclient<MyUdpConnector> m_client;

	/* UDP监听的地址 */
	boost::asio::ip::udp::endpoint m_PeerAddr;
};

