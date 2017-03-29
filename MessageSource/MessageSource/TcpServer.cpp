#include "stdafx.h"
#include "TcpServer.h"


CTcpServer::CTcpServer() : m_pump(), m_server(m_pump)
{
}


CTcpServer::~CTcpServer()
{
	if (m_pump.is_running())
	{
		m_pump.stop_service();
	}
}


void CTcpServer::start()
{ 
	m_pump.start_service(); 
}


void CTcpServer::stop()
{
	if (m_pump.is_running())
	{
		m_pump.stop_service();
	}
}


void CTcpServer::send_msg(const msg_type& p_msg)
{ 
	m_server.broadcast_msg(p_msg);  
}

 

void CTcpServer::set_server_addr(const unsigned short p_port, const std::string& p_ip)
{
	m_server.set_server_addr(p_port, p_ip);
}

bool CTcpServer::is_connecting()
{
	if (m_server.is_listening())
	{
		return true;
	}
	else
	{
		return false;
	}
}