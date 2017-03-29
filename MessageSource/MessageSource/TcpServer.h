#pragma once

//configuration
#undef MAX_MSG_NUM
#define MAX_MSG_NUM	   1500 
#define REUSE_OBJECT          //use objects pool
#define FORCE_TO_USE_MSG_RECV_BUFFER //force to use the msg recv buffer
#define ENHANCED_STABILITY

//use the following macro to control the type of packer and unpacker
#define PACKER_UNPACKER_TYPE	1
//1-default packer and unpacker, head(length) + body
//2-fixed length packer and unpacker
//3-prefix and suffix packer and unpacker

#if 2 == PACKER_UNPACKER_TYPE
#define DEFAULT_PACKER	fixed_legnth_packer
#define DEFAULT_UNPACKER fixed_length_unpacker
#endif
#if 3 == PACKER_UNPACKER_TYPE
#define DEFAULT_PACKER prefix_suffix_packer
#define DEFAULT_UNPACKER prefix_suffix_unpacker
#endif

#include "communication/st_asio_wrapper_server.h"

using namespace st_asio_wrapper;
 
#define QUIT_COMMAND	"quit"
#define RESTART_COMMAND	"restart"
#define LIST_ALL_CLIENT	"list_all_client"
#define LIST_STATUS		"status"
#define SUSPEND_COMMAND	"suspend"
#define RESUME_COMMAND	"resume"

class CTcpServer
{
public:
	CTcpServer();
	~CTcpServer();

private:
	typedef std::string msg_type;

public: 
	void start();
	void stop();
	void send_msg(const msg_type& p_msg); 
	void set_server_addr(const unsigned short p_port, const std::string& p_ip);
	bool is_connecting();
private: 
	class My_st_server_socket : public st_server_socket_base<>
	{
		public:
			typedef std::string msg_type;
		public:
			My_st_server_socket(i_server& server_): st_server_socket_base(server_){}  
	};

private: 
	st_service_pump m_pump;
	 
	st_server_base<My_st_server_socket> m_server;   
};

#undef FORCE_TO_USE_MSG_RECV_BUFFER
#undef MAX_MSG_NUM