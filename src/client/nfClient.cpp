#include "nfClient.h"
ClientCgiInfo __ClientCgi_RedEnvelope_giverCreateSession::s_cgi_info;
ClientCgiInfo __ClientCgi_RedEnvelope_giverDeleteSession::s_cgi_info;
ClientCgiInfo __ClientCgi_RedEnvelope_giverUpdateSession::s_cgi_info;
ClientCgiInfo __ClientCgi_RedEnvelope_giverReportScanResult::s_cgi_info;
ClientCgiInfo __ClientCgi_RedEnvelope_receiverCreateSession::s_cgi_info;
ClientCgiInfo __ClientCgi_RedEnvelope_receiverDeleteSession::s_cgi_info;
ClientCgiInfo __ClientCgi_RedEnvelope_receiverUpdateSession::s_cgi_info;
ClientCgiInfo __ClientCgi_RedEnvelope_receiverReportScanResult::s_cgi_info;
ClientCgiInfo __ClientCgi_RedEnvelope_reportStatisticZishi::s_cgi_info;
ClientCgiInfo __ClientCgi_RedEnvelope_matchResult::s_cgi_info;


std::map<uint32_t, ClientCgiInfo> __getCgiInfoMap()
{
	std::map<uint32_t, ClientCgiInfo> cgi_infos;
	cgi_infos[__ClientCgi_RedEnvelope_giverCreateSession::s_getCgiInfo().m_send_cmd_type] = __ClientCgi_RedEnvelope_giverCreateSession::s_getCgiInfo();
	cgi_infos[__ClientCgi_RedEnvelope_giverDeleteSession::s_getCgiInfo().m_send_cmd_type] = __ClientCgi_RedEnvelope_giverDeleteSession::s_getCgiInfo();
	cgi_infos[__ClientCgi_RedEnvelope_giverUpdateSession::s_getCgiInfo().m_send_cmd_type] = __ClientCgi_RedEnvelope_giverUpdateSession::s_getCgiInfo();
	cgi_infos[__ClientCgi_RedEnvelope_giverReportScanResult::s_getCgiInfo().m_send_cmd_type] = __ClientCgi_RedEnvelope_giverReportScanResult::s_getCgiInfo();
	cgi_infos[__ClientCgi_RedEnvelope_receiverCreateSession::s_getCgiInfo().m_send_cmd_type] = __ClientCgi_RedEnvelope_receiverCreateSession::s_getCgiInfo();
	cgi_infos[__ClientCgi_RedEnvelope_receiverDeleteSession::s_getCgiInfo().m_send_cmd_type] = __ClientCgi_RedEnvelope_receiverDeleteSession::s_getCgiInfo();
	cgi_infos[__ClientCgi_RedEnvelope_receiverUpdateSession::s_getCgiInfo().m_send_cmd_type] = __ClientCgi_RedEnvelope_receiverUpdateSession::s_getCgiInfo();
	cgi_infos[__ClientCgi_RedEnvelope_receiverReportScanResult::s_getCgiInfo().m_send_cmd_type] = __ClientCgi_RedEnvelope_receiverReportScanResult::s_getCgiInfo();
	cgi_infos[__ClientCgi_RedEnvelope_reportStatisticZishi::s_getCgiInfo().m_send_cmd_type] = __ClientCgi_RedEnvelope_reportStatisticZishi::s_getCgiInfo();
	cgi_infos[__ClientCgi_RedEnvelope_matchResult::s_getCgiInfo().m_send_cmd_type] = __ClientCgi_RedEnvelope_matchResult::s_getCgiInfo();
	return cgi_infos;
}


#define __MSG_TYPE_NfClient_sendReq_RedEnvelope_giverCreateSession 9126181
#define __MSG_TYPE_NfClient_sendReq_RedEnvelope_giverDeleteSession 9126182
#define __MSG_TYPE_NfClient_sendReq_RedEnvelope_giverUpdateSession 9126183
#define __MSG_TYPE_NfClient_sendReq_RedEnvelope_giverReportScanResult 9126184
#define __MSG_TYPE_NfClient_sendReq_RedEnvelope_receiverCreateSession 9126185
#define __MSG_TYPE_NfClient_sendReq_RedEnvelope_receiverDeleteSession 9126186
#define __MSG_TYPE_NfClient_sendReq_RedEnvelope_receiverUpdateSession 9126187
#define __MSG_TYPE_NfClient_sendReq_RedEnvelope_receiverReportScanResult 9126188
#define __MSG_TYPE_NfClient_sendReq_RedEnvelope_reportStatistic_zisi 9126189




class __Msg_sendReq_RedEnvelope_giverCreateSession : public Message
{
public:
	__Msg_sendReq_RedEnvelope_giverCreateSession() { m_msg_type = __MSG_TYPE_NfClient_sendReq_RedEnvelope_giverCreateSession; m_send_pack_id = 0; m_send_pack_seq = 0; m_giver_uin = 0; m_re_count = 0; }

	uint64_t m_send_pack_id;
	uint32_t m_send_pack_seq;
	std::string m_wx_re_id;
	uint32_t m_giver_uin;
	std::string m_re_qr_code;
	uint32_t m_re_count;
};

class __Msg_sendReq_RedEnvelope_giverDeleteSession : public Message
{
public:
	__Msg_sendReq_RedEnvelope_giverDeleteSession() { m_msg_type = __MSG_TYPE_NfClient_sendReq_RedEnvelope_giverDeleteSession; m_send_pack_id = 0; m_send_pack_seq = 0; m_giver_uin = 0; }

	uint64_t m_send_pack_id;
	uint32_t m_send_pack_seq;
	std::string m_wx_re_id;
	uint32_t m_giver_uin;
};

class __Msg_sendReq_RedEnvelope_giverUpdateSession : public Message
{
public:
	__Msg_sendReq_RedEnvelope_giverUpdateSession() { m_msg_type = __MSG_TYPE_NfClient_sendReq_RedEnvelope_giverUpdateSession; m_send_pack_id = 0; m_send_pack_seq = 0; m_giver_uin = 0; }

	uint64_t m_send_pack_id;
	uint32_t m_send_pack_seq;
	std::string m_wx_re_id;
	uint32_t m_giver_uin;
	std::string m_re_qr_code;
};

class __Msg_sendReq_RedEnvelope_giverReportScanResult : public Message
{
public:
	__Msg_sendReq_RedEnvelope_giverReportScanResult() { m_msg_type = __MSG_TYPE_NfClient_sendReq_RedEnvelope_giverReportScanResult; m_send_pack_id = 0; m_send_pack_seq = 0; m_giver_uin = 0; }

	uint64_t m_send_pack_id;
	uint32_t m_send_pack_seq;
	std::string m_wx_re_id;
	uint32_t m_giver_uin;
	std::vector<uint32_t> m_recevier_uins;
};

class __Msg_sendReq_RedEnvelope_receiverCreateSession : public Message
{
public:
	__Msg_sendReq_RedEnvelope_receiverCreateSession() { m_msg_type = __MSG_TYPE_NfClient_sendReq_RedEnvelope_receiverCreateSession; m_send_pack_id = 0; m_send_pack_seq = 0; m_receiver_uin = 0; }

	uint64_t m_send_pack_id;
	uint32_t m_send_pack_seq;
	uint32_t m_receiver_uin;
};

class __Msg_sendReq_RedEnvelope_receiverDeleteSession : public Message
{
public:
	__Msg_sendReq_RedEnvelope_receiverDeleteSession() { m_msg_type = __MSG_TYPE_NfClient_sendReq_RedEnvelope_receiverDeleteSession; m_send_pack_id = 0; m_send_pack_seq = 0; m_receiver_uin = 0; }

	uint64_t m_send_pack_id;
	uint32_t m_send_pack_seq;
	uint32_t m_receiver_uin;
};

class __Msg_sendReq_RedEnvelope_receiverUpdateSession : public Message
{
public:
	__Msg_sendReq_RedEnvelope_receiverUpdateSession() { m_msg_type = __MSG_TYPE_NfClient_sendReq_RedEnvelope_receiverUpdateSession; m_send_pack_id = 0; m_send_pack_seq = 0; m_receiver_uin = 0; m_giver_uin = 0; m_is_re_opened = false; }

	uint64_t m_send_pack_id;
	uint32_t m_send_pack_seq;
	uint32_t m_receiver_uin;
	std::string m_wx_re_id;
	uint32_t m_giver_uin;
	bool m_is_re_opened;
};

class __Msg_sendReq_RedEnvelope_receiverReportScanResult : public Message
{
public:
	__Msg_sendReq_RedEnvelope_receiverReportScanResult() { m_msg_type = __MSG_TYPE_NfClient_sendReq_RedEnvelope_receiverReportScanResult; m_send_pack_id = 0; m_send_pack_seq = 0; m_receiver_uin = 0; }

	uint64_t m_send_pack_id;
	uint32_t m_send_pack_seq;
	uint32_t m_receiver_uin;
	std::vector<uint32_t> m_giver_uins;
};

class __Msg_sendReq_RedEnvelope_reportStatistic_zishi : public Message
{
public:
	__Msg_sendReq_RedEnvelope_reportStatistic_zishi()
	{
		m_msg_type = __MSG_TYPE_NfClient_sendReq_RedEnvelope_reportStatistic_zisi;
		m_send_pack_id = 0; 
		m_send_pack_seq = 0; 
		m_uin = 0;
		m_is_sender = false;
		m_angle = 0;
		m_ax = 0;
		m_ay = 0;
		m_az = 0;
	}

	uint64_t m_send_pack_id;
	uint32_t m_send_pack_seq;
	uint32_t m_uin;
	bool m_is_sender;
	int16_t m_angle;
	int16_t m_ax;
	int16_t m_ay;
	int16_t m_az;
};









class NfClient::__Handler : public ClientNetworkMsgLooperHandler
{
public:
	class InitParam
	{
	public:
		InitParam() { m_looper = nullptr; m_callback = nullptr; }
		MessageLooper* m_looper;
		NfClient::ICallback* m_callback;
	};

	__Handler()
	{
	}

	~__Handler()
	{
	}

	bool init(const InitParam& param)
	{
		m_init_param = param;

		ClientNetworkMsgLooperHandler::ClientNetworkMsgLooperHandlerInitParam p;
		p.m_looper = param.m_looper;
		p.m_svr_ip_or_name = "127.0.0.1";
		//p.m_svr_ip = "120.78.58.61";
		p.m_svr_port = 12306;
		p.m_cgi_infos = __getCgiInfoMap();
		if (!ClientNetworkMsgLooperHandler::init(p))
			return false;

		return true;
	}





private:
	virtual void onMessage(Message* msg, bool* is_handled) override
	{
		ClientNetworkMsgLooperHandler::onMessage(msg, is_handled);
		if (*is_handled)
			return;

		if (msg->m_target == this)
		{
			switch (msg->m_msg_type)
			{
			case __MSG_TYPE_NfClient_sendReq_RedEnvelope_giverCreateSession:
				__onMsg_sendReq_RedEnvelope_giverCreateSession(msg);
				*is_handled = true;
				break;
			case __MSG_TYPE_NfClient_sendReq_RedEnvelope_giverDeleteSession:
				__onMsg_sendReq_RedEnvelope_giverDeleteSession(msg);
				*is_handled = true;
				break;
			case __MSG_TYPE_NfClient_sendReq_RedEnvelope_giverUpdateSession:
				__onMsg_sendReq_RedEnvelope_giverUpdateSession(msg);
				*is_handled = true;
				break;
			case __MSG_TYPE_NfClient_sendReq_RedEnvelope_giverReportScanResult:
				__onMsg_sendReq_RedEnvelope_giverReportScanResult(msg);
				*is_handled = true;
				break;
			case __MSG_TYPE_NfClient_sendReq_RedEnvelope_receiverCreateSession:
				__onMsg_sendReq_RedEnvelope_receiverCreateSession(msg);
				*is_handled = true;
				break;
			case __MSG_TYPE_NfClient_sendReq_RedEnvelope_receiverDeleteSession:
				__onMsg_sendReq_RedEnvelope_receiverDeleteSession(msg);
				*is_handled = true;
				break;
			case __MSG_TYPE_NfClient_sendReq_RedEnvelope_receiverUpdateSession:
				__onMsg_sendReq_RedEnvelope_receiverUpdateSession(msg);
				*is_handled = true;
				break;
			case __MSG_TYPE_NfClient_sendReq_RedEnvelope_receiverReportScanResult:
				__onMsg_sendReq_RedEnvelope_receiverReportScanResult(msg);
				*is_handled = true;
				break;
			case __MSG_TYPE_NfClient_sendReq_RedEnvelope_reportStatistic_zisi:
				__onMsg_sendReq_RedEnvelope_reportStatisticZisi(msg);
				*is_handled = true;
				break;
			default:
				slog_e("NfClient::__Handler: unkonw msg type");
				*is_handled = true;
				break;
			}
		}
		
	}

	virtual void onMsg_startCmd_end(Message* msg) override
	{
		m_cgi_ctx.m_network = getNetwork();
		m_cgi_ctx.m_packer = getPacker();
	}

	virtual void onMessageTimerTick(uint64_t timer_id, void* user_data) override
	{
		ClientNetworkMsgLooperHandler::onMessageTimerTick(timer_id, user_data);
	}

	virtual void onClientNetwork_cgiDone(ClientNetwork* network, ClientCgi * cgi) override
	{
		ClientNetworkMsgLooperHandler::onClientNetwork_cgiDone(network, cgi);
		slog_d("cgi done");

		switch (cgi->getCgiInfo().m_send_cmd_type)
		{
		case __ECgiCmdType_c2sReq_RedEnvelope_GiverCreateSession:
			__onClientCgi_cgiDone_RedEnvelope_giverCreateSession(cgi);
			break;
		case __ECgiCmdType_c2sReq_RedEnvelope_GiverDeleteSession:
			__onClientCgi_cgiDone_RedEnvelope_giverDeleteSession(cgi);
			break;
		case __ECgiCmdType_c2sReq_RedEnvelope_GiverUpdateSession:
			__onClientCgi_cgiDone_RedEnvelope_giverUpdateSession(cgi);
			break;
		case __ECgiCmdType_c2sReq_RedEnvelope_GiverReportScanResult:
			__onClientCgi_cgiDone_RedEnvelope_giverReportScanResult(cgi);
			break;
		case __ECgiCmdType_c2sReq_RedEnvelope_ReceiverCreateSession:
			__onClientCgi_cgiDone_RedEnvelope_receiverCreateSession(cgi);
			break;
		case __ECgiCmdType_c2sReq_RedEnvelope_ReceiverDeleteSession:
			__onClientCgi_cgiDone_RedEnvelope_receiverDeleteSession(cgi);
			break;
		case __ECgiCmdType_c2sReq_RedEnvelope_ReceiverUpdateSession:
			__onClientCgi_cgiDone_RedEnvelope_receiverUpdateSession(cgi);
			break;
		case __ECgiCmdType_c2sReq_RedEnvelope_ReceiverReportScanResult:
			__onClientCgi_cgiDone_RedEnvelope_receiverReportScanResult(cgi);
			break;
		case __ECgiCmdType_c2sReq_RedEnvelope_ReportStatisticZishi:
			__onClientCgi_cgiDone_RedEnvelope_reportStatisticZishi(cgi);
			break;

		default:
			slog_e("unknow cgi done!");
			break;
		}
		delete cgi;
	}

	virtual void onClientNetwork_recvS2cPushPack(ClientNetwork* network, std::unique_ptr<ClientNetwork::RecvPack>* recv_pack) override
	{
		ClientNetwork::RecvPack* p = recv_pack->get();
		if (p->m_recv_cmd_type == __ECgiCmdType_s2cPush_RedEnvelope_MatchResult)
		{
			__ClientCgi_RedEnvelope_matchResult cgi;
			cgi.setRecvPack(recv_pack->release());
			slog_i("push= %0", cgi.m_s2c_push_body);
			m_init_param.m_callback->onNfClient_recvPush_RedEnvelope_matchResult(
				cgi.m_s2c_push_giver_uin, cgi.m_s2c_push_wx_re_id, cgi.m_s2c_push_re_qr_code, cgi.m_s2c_push_receiver_uins);
		}
	}

	virtual void onClientNetwork_recvS2cReqPack(ClientNetwork* network, std::unique_ptr<ClientNetwork::RecvPack>* recv_pack) override
	{
		slog_d("recv s2c_req pack\n");
	}

	void __onMsg_sendReq_RedEnvelope_giverCreateSession(Message* msg)
	{
		__Msg_sendReq_RedEnvelope_giverCreateSession* m = (__Msg_sendReq_RedEnvelope_giverCreateSession*)msg;
		__startCgi_RedEnvelope_giverCreateSession(m->m_send_pack_id, m->m_send_pack_seq
			, m->m_wx_re_id, m->m_giver_uin, m->m_re_qr_code, m->m_re_count);
	}

	void __onMsg_sendReq_RedEnvelope_giverDeleteSession(Message* msg)
	{
		__Msg_sendReq_RedEnvelope_giverDeleteSession* m = (__Msg_sendReq_RedEnvelope_giverDeleteSession*)msg;
		__startCgi_RedEnvelope_giverDeleteSession(m->m_send_pack_id, m->m_send_pack_seq
			, m->m_wx_re_id, m->m_giver_uin);
	}

	void __onMsg_sendReq_RedEnvelope_giverUpdateSession(Message* msg)
	{
		__Msg_sendReq_RedEnvelope_giverUpdateSession* m = (__Msg_sendReq_RedEnvelope_giverUpdateSession*)msg;
		__startCgi_RedEnvelope_giverUpdateSession(m->m_send_pack_id, m->m_send_pack_seq
			, m->m_wx_re_id, m->m_giver_uin, m->m_re_qr_code);
	}

	void __onMsg_sendReq_RedEnvelope_giverReportScanResult(Message* msg)
	{
		__Msg_sendReq_RedEnvelope_giverReportScanResult* m = (__Msg_sendReq_RedEnvelope_giverReportScanResult*)msg;
		__startCgi_RedEnvelope_giverReportScanResult(m->m_send_pack_id, m->m_send_pack_seq
			, m->m_wx_re_id, m->m_giver_uin, m->m_recevier_uins);
	}

	void __onMsg_sendReq_RedEnvelope_receiverCreateSession(Message* msg)
	{
		__Msg_sendReq_RedEnvelope_receiverCreateSession* m = (__Msg_sendReq_RedEnvelope_receiverCreateSession*)msg;
		__startCgi_RedEnvelope_receiverCreateSession(m->m_send_pack_id, m->m_send_pack_seq
			, m->m_receiver_uin);
	}

	void __onMsg_sendReq_RedEnvelope_receiverDeleteSession(Message* msg)
	{
		__Msg_sendReq_RedEnvelope_receiverDeleteSession* m = (__Msg_sendReq_RedEnvelope_receiverDeleteSession*)msg;
		__startCgi_RedEnvelope_receiverDeleteSession(m->m_send_pack_id, m->m_send_pack_seq
			, m->m_receiver_uin);
	}

	void __onMsg_sendReq_RedEnvelope_receiverUpdateSession(Message* msg)
	{
		__Msg_sendReq_RedEnvelope_receiverUpdateSession* m = (__Msg_sendReq_RedEnvelope_receiverUpdateSession*)msg;
		__startCgi_RedEnvelope_receiverUpdateSession(m->m_send_pack_id, m->m_send_pack_seq
			, m->m_receiver_uin, m->m_wx_re_id, m->m_giver_uin, m->m_is_re_opened);
	}

	void __onMsg_sendReq_RedEnvelope_receiverReportScanResult(Message* msg)
	{
		__Msg_sendReq_RedEnvelope_receiverReportScanResult* m = (__Msg_sendReq_RedEnvelope_receiverReportScanResult*)msg;
		__startCgi_RedEnvelope_receiverReportScanResult(m->m_send_pack_id, m->m_send_pack_seq
			, m->m_receiver_uin, m->m_giver_uins);
	}

	void __onMsg_sendReq_RedEnvelope_reportStatisticZisi(Message* msg)
	{
		__Msg_sendReq_RedEnvelope_reportStatistic_zishi* m = (__Msg_sendReq_RedEnvelope_reportStatistic_zishi*)msg;
		__startCgi_RedEnvelope_reportStatisticZisi(m->m_send_pack_id, m->m_send_pack_seq
			, m->m_uin, m->m_is_sender, m->m_angle, m->m_ax, m->m_ay, m->m_az);
	}

	void __onClientCgi_cgiDone_RedEnvelope_giverCreateSession(ClientCgi* cgi)
	{
		__ClientCgi_RedEnvelope_giverCreateSession* c = (__ClientCgi_RedEnvelope_giverCreateSession*)cgi;
		if (!cgi->getIsSuccess())
			slog_e("cgi fail, seq:%0", cgi->getSendPack()->m_send_seq);
		else
			slog_i("resp= seq:%0, %1", c->getRecvPack()->m_recv_seq, c->m_s2c_resp_body.c_str());

		int err_code = c->getIsSuccess() ? c->m_s2c_resp_err_code : -1;
		m_init_param.m_callback->onNfClient_recvResp_RedEnvelope_giverCreateSession(cgi->getSendPack()->m_send_pack_id, err_code);
	}

	void __onClientCgi_cgiDone_RedEnvelope_giverDeleteSession(ClientCgi* cgi)
	{
		__ClientCgi_RedEnvelope_giverDeleteSession* c = (__ClientCgi_RedEnvelope_giverDeleteSession*)cgi;
		if (!cgi->getIsSuccess())
			slog_e("cgi fail, seq:%0", cgi->getSendPack()->m_send_seq);
		else
			slog_i("resp= seq:%0, %1", c->getRecvPack()->m_recv_seq, c->m_s2c_resp_body.c_str());

		int err_code = c->getIsSuccess() ? c->m_s2c_resp_err_code : -1;
		m_init_param.m_callback->onNfClient_recvResp_RedEnvelope_giverDeleteSession(cgi->getSendPack()->m_send_pack_id, err_code);
	}

	void __onClientCgi_cgiDone_RedEnvelope_giverUpdateSession(ClientCgi* cgi)
	{
		__ClientCgi_RedEnvelope_giverUpdateSession* c = (__ClientCgi_RedEnvelope_giverUpdateSession*)cgi;
		if (!cgi->getIsSuccess())
			slog_e("cgi fail, seq:%0", cgi->getSendPack()->m_send_seq);
		else
			slog_i("resp= seq:%0, %1", c->getRecvPack()->m_recv_seq, c->m_s2c_resp_body.c_str());

		int err_code = c->getIsSuccess() ? c->m_s2c_resp_err_code : -1;
		m_init_param.m_callback->onNfClient_recvResp_RedEnvelope_giverUpdateSession(cgi->getSendPack()->m_send_pack_id, err_code);
	}

	void __onClientCgi_cgiDone_RedEnvelope_giverReportScanResult(ClientCgi* cgi)
	{
		__ClientCgi_RedEnvelope_giverReportScanResult* c = (__ClientCgi_RedEnvelope_giverReportScanResult*)cgi;
		if (!cgi->getIsSuccess())
			slog_e("cgi fail, seq:%0", cgi->getSendPack()->m_send_seq);
		else
			slog_i("resp= seq:%0, %1", c->getRecvPack()->m_recv_seq, c->m_s2c_resp_body.c_str());

		int err_code = c->getIsSuccess() ? c->m_s2c_resp_err_code : -1;
		m_init_param.m_callback->onNfClient_recvResp_RedEnvelope_giverReportScanResult(cgi->getSendPack()->m_send_pack_id, err_code);
	}

	void __onClientCgi_cgiDone_RedEnvelope_receiverCreateSession(ClientCgi* cgi)
	{
		__ClientCgi_RedEnvelope_receiverCreateSession* c = (__ClientCgi_RedEnvelope_receiverCreateSession*)cgi;
		if (!cgi->getIsSuccess())
			slog_e("cgi fail, seq:%0", cgi->getSendPack()->m_send_seq);
		else
			slog_i("resp= seq:%0, %1", c->getRecvPack()->m_recv_seq, c->m_s2c_resp_body.c_str());

		int err_code = c->getIsSuccess() ? c->m_s2c_resp_err_code : -1;
		m_init_param.m_callback->onNfClient_recvResp_RedEnvelope_receiverCreateSession(cgi->getSendPack()->m_send_pack_id, err_code);
	}

	void __onClientCgi_cgiDone_RedEnvelope_receiverDeleteSession(ClientCgi* cgi)
	{
		__ClientCgi_RedEnvelope_receiverDeleteSession* c = (__ClientCgi_RedEnvelope_receiverDeleteSession*)cgi;
		if (!cgi->getIsSuccess())
			slog_e("cgi fail, seq:%0", cgi->getSendPack()->m_send_seq);
		else
			slog_i("resp= seq:%0, %1", c->getRecvPack()->m_recv_seq, c->m_s2c_resp_body.c_str());

		int err_code = c->getIsSuccess() ? c->m_s2c_resp_err_code : -1;
		m_init_param.m_callback->onNfClient_recvResp_RedEnvelope_receiverDeleteSession(cgi->getSendPack()->m_send_pack_id, err_code);
	}

	void __onClientCgi_cgiDone_RedEnvelope_receiverUpdateSession(ClientCgi* cgi)
	{
		__ClientCgi_RedEnvelope_receiverUpdateSession* c = (__ClientCgi_RedEnvelope_receiverUpdateSession*)cgi;
		if (!cgi->getIsSuccess())
			slog_e("cgi fail, seq:%0", cgi->getSendPack()->m_send_seq);
		else
			slog_i("resp= seq:%0, %1", c->getRecvPack()->m_recv_seq, c->m_s2c_resp_body.c_str());

		int err_code = c->getIsSuccess() ? c->m_s2c_resp_err_code : -1;
		m_init_param.m_callback->onNfClient_recvResp_RedEnvelope_receiverUpdateSession(cgi->getSendPack()->m_send_pack_id, err_code);
	}

	void __onClientCgi_cgiDone_RedEnvelope_receiverReportScanResult(ClientCgi* cgi)
	{
		__ClientCgi_RedEnvelope_receiverReportScanResult* c = (__ClientCgi_RedEnvelope_receiverReportScanResult*)cgi;
		if (!cgi->getIsSuccess())
			slog_e("cgi fail, seq:%0", cgi->getSendPack()->m_send_seq);
		else
			slog_i("resp= seq:%0, %1", c->getRecvPack()->m_recv_seq, c->m_s2c_resp_body.c_str());

		int err_code = c->getIsSuccess() ? c->m_s2c_resp_err_code : -1;
		m_init_param.m_callback->onNfClient_recvResp_RedEnvelope_receiverReportScanResult(cgi->getSendPack()->m_send_pack_id, err_code);
	}

	void __onClientCgi_cgiDone_RedEnvelope_reportStatisticZishi(ClientCgi* cgi)
	{
		__ClientCgi_RedEnvelope_reportStatisticZishi* c = (__ClientCgi_RedEnvelope_reportStatisticZishi*)cgi;
		if (!cgi->getIsSuccess())
			slog_e("cgi fail, seq:%0", cgi->getSendPack()->m_send_seq);
		else
			slog_i("resp= seq:%0, %1", c->getRecvPack()->m_recv_seq, c->m_s2c_resp_body.c_str());

		int err_code = c->getIsSuccess() ? c->m_s2c_resp_err_code : -1;
		m_init_param.m_callback->onNfClient_recvResp_RedEnvelope_reportStatisticZishi(cgi->getSendPack()->m_send_pack_id, err_code);
	}
	





	void __startCgi_RedEnvelope_giverCreateSession(uint64_t send_pack_id, uint32_t send_pack_seq, const std::string& wx_re_id, uint32_t giver_uin, const std::string& re_qr_code, uint32_t re_count)
	{
		m_cgi_ctx.m_send_pack_id = send_pack_id;
		m_cgi_ctx.m_send_pack_seq = send_pack_seq;

		__ClientCgi_RedEnvelope_giverCreateSession* cgi = new __ClientCgi_RedEnvelope_giverCreateSession();
		if (!cgi->initSendPack(m_cgi_ctx, wx_re_id, giver_uin, re_qr_code, re_count))
		{
			slog_e("fail to init cgi");
			return;
		}
		slog_i("req = seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_c2s_req_body.c_str());

		if (!getNetwork()->startCgi(cgi))
		{
			slog_e("fail to startCgi");
			return;
		}
	}

	void __startCgi_RedEnvelope_giverDeleteSession(uint64_t send_pack_id, uint32_t send_pack_seq, const std::string& wx_re_id, uint32_t giver_uin)
	{
		m_cgi_ctx.m_send_pack_id = send_pack_id;
		m_cgi_ctx.m_send_pack_seq = send_pack_seq;

		__ClientCgi_RedEnvelope_giverDeleteSession* cgi = new __ClientCgi_RedEnvelope_giverDeleteSession();
		if (!cgi->initSendPack(m_cgi_ctx, wx_re_id, giver_uin))
		{
			slog_e("fail to init cgi");
			return;
		}
		slog_i("req = seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_c2s_req_body.c_str());

		if (!getNetwork()->startCgi(cgi))
		{
			slog_e("fail to startCgi");
			return;
		}
	}

	void __startCgi_RedEnvelope_giverUpdateSession(uint64_t send_pack_id, uint32_t send_pack_seq, const std::string& wx_re_id, uint32_t giver_uin, const std::string& re_qr_code)
	{
		m_cgi_ctx.m_send_pack_id = send_pack_id;
		m_cgi_ctx.m_send_pack_seq = send_pack_seq;

		__ClientCgi_RedEnvelope_giverUpdateSession* cgi = new __ClientCgi_RedEnvelope_giverUpdateSession();
		if (!cgi->initSendPack(m_cgi_ctx, wx_re_id, giver_uin, re_qr_code))
		{
			slog_e("fail to init cgi");
			return;
		}
		slog_i("req = seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_c2s_req_body.c_str());

		if (!getNetwork()->startCgi(cgi))
		{
			slog_e("fail to startCgi");
			return;
		}
	}

	void __startCgi_RedEnvelope_giverReportScanResult(uint64_t send_pack_id, uint32_t send_pack_seq, const std::string& wx_re_id, uint32_t giver_uin, const std::vector<uint32_t> scan_uins)
	{
		m_cgi_ctx.m_send_pack_id = send_pack_id;
		m_cgi_ctx.m_send_pack_seq = send_pack_seq;

		__ClientCgi_RedEnvelope_giverReportScanResult* cgi = new __ClientCgi_RedEnvelope_giverReportScanResult();
		if (!cgi->initSendPack(m_cgi_ctx, wx_re_id, giver_uin, scan_uins))
		{
			slog_e("fail to init cgi");
			return;
		}
		slog_i("req = seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_c2s_req_body.c_str());

		if (!getNetwork()->startCgi(cgi))
		{
			slog_e("fail to startCgi");
			return;
		}
	}

	void __startCgi_RedEnvelope_receiverCreateSession(uint64_t send_pack_id, uint32_t send_pack_seq, uint32_t receiver_uin)
	{
		m_cgi_ctx.m_send_pack_id = send_pack_id;
		m_cgi_ctx.m_send_pack_seq = send_pack_seq;

		__ClientCgi_RedEnvelope_receiverCreateSession* cgi = new __ClientCgi_RedEnvelope_receiverCreateSession();
		if (!cgi->initSendPack(m_cgi_ctx, receiver_uin))
		{
			slog_e("fail to init cgi");
			return;
		}
		slog_i("req = seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_c2s_req_body.c_str());

		if (!getNetwork()->startCgi(cgi))
		{
			slog_e("fail to startCgi");
			return;
		}
	}

	void __startCgi_RedEnvelope_receiverDeleteSession(uint64_t send_pack_id, uint32_t send_pack_seq, uint32_t receiver_uin)
	{
		m_cgi_ctx.m_send_pack_id = send_pack_id;
		m_cgi_ctx.m_send_pack_seq = send_pack_seq;

		__ClientCgi_RedEnvelope_receiverDeleteSession* cgi = new __ClientCgi_RedEnvelope_receiverDeleteSession();
		if (!cgi->initSendPack(m_cgi_ctx, receiver_uin))
		{
			slog_e("fail to init cgi");
			return;
		}
		slog_i("req = seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_c2s_req_body.c_str());

		if (!getNetwork()->startCgi(cgi))
		{
			slog_e("fail to startCgi");
			return;
		}
	}

	void __startCgi_RedEnvelope_receiverUpdateSession(uint64_t send_pack_id, uint32_t send_pack_seq, uint32_t receiver_uin, const std::string& wx_re_id, uint32_t giver_uin, bool is_opened)
	{
		m_cgi_ctx.m_send_pack_id = send_pack_id;
		m_cgi_ctx.m_send_pack_seq = send_pack_seq;

		__ClientCgi_RedEnvelope_receiverUpdateSession* cgi = new __ClientCgi_RedEnvelope_receiverUpdateSession();
		if (!cgi->initSendPack(m_cgi_ctx, wx_re_id, giver_uin, receiver_uin, is_opened))
		{
			slog_e("fail to init cgi");
			return;
		}
		slog_i("req = seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_c2s_req_body.c_str());

		if (!getNetwork()->startCgi(cgi))
		{
			slog_e("fail to startCgi");
			return;
		}
	}

	void __startCgi_RedEnvelope_receiverReportScanResult(uint64_t send_pack_id, uint32_t send_pack_seq, uint32_t receiver_uin, const std::vector<uint32_t> scan_uins)
	{
		m_cgi_ctx.m_send_pack_id = send_pack_id;
		m_cgi_ctx.m_send_pack_seq = send_pack_seq;

		__ClientCgi_RedEnvelope_receiverReportScanResult* cgi = new __ClientCgi_RedEnvelope_receiverReportScanResult();
		if (!cgi->initSendPack(m_cgi_ctx, receiver_uin, scan_uins))
		{
			slog_e("fail to init cgi");
			return;
		}
		slog_i("req = seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_c2s_req_body.c_str());

		if (!getNetwork()->startCgi(cgi))
		{
			slog_e("fail to startCgi");
			return;
		}
	}

	void __startCgi_RedEnvelope_reportStatisticZisi(uint64_t send_pack_id, uint32_t send_pack_seq, uint32_t uin, bool is_sender, int16_t angle, int16_t ax, int16_t ay, int16_t az)
	{
		m_cgi_ctx.m_send_pack_id = send_pack_id;
		m_cgi_ctx.m_send_pack_seq = send_pack_seq;

		__ClientCgi_RedEnvelope_reportStatisticZishi* cgi = new __ClientCgi_RedEnvelope_reportStatisticZishi();
		if (!cgi->initSendPack(m_cgi_ctx, uin, is_sender, angle, ax, ay, az))
		{
			slog_e("fail to init cgi");
			return;
		}
		slog_i("req = seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_c2s_req_body.c_str());

		if (!getNetwork()->startCgi(cgi))
		{
			slog_e("fail to startCgi");
			return;
		}
	}



	InitParam m_init_param;
	__ClientSendPackBuilder m_cgi_ctx;
};




















NfClient::NfClient(ICallback * callback)
{
	__initNfClientLog();
	m_send_pack_id_seed = 0;
	m_send_pack_seq_seed = 0;
	m_callback = callback;
	m_handler = nullptr;
	m_thread = nullptr;
}

NfClient::~NfClient()
{
	stopClient();
}

bool NfClient::startClient()
{
	if (m_thread != nullptr)
		return true;
	m_handler = new __Handler();
	m_thread = new MessageLoopThread(m_handler);

	__Handler::InitParam p;
	p.m_callback = m_callback;
	p.m_looper = m_thread->getLooper();
	m_handler->init(p);

	m_thread->start();

	m_handler->sendMsg_startCmd();
	return true;
}

void NfClient::stopClient()
{
	if (m_thread == nullptr)
		return;
	m_handler->sendMsg_stopCmd();
	m_thread->join();
	delete m_thread;
	m_thread = nullptr;
}

uint64_t NfClient::sendReq_RedEnvelope_giverCreateSession(const std::string & wx_re_id, uint32_t giver_uin, const std::string & re_qr_code, uint32_t re_count)
{
	if (m_thread == nullptr)
		return 0;

	__Msg_sendReq_RedEnvelope_giverCreateSession* msg = new __Msg_sendReq_RedEnvelope_giverCreateSession();
	msg->m_send_pack_id = ++m_send_pack_id_seed;
	msg->m_send_pack_seq = ++m_send_pack_seq_seed;
	msg->m_wx_re_id = wx_re_id;
	msg->m_giver_uin = giver_uin;
	msg->m_re_qr_code = re_qr_code;
	msg->m_re_count = re_count;
	__postMessage(msg);

	return msg->m_send_pack_id;
}

uint64_t NfClient::sendReq_RedEnvelope_giverDeleteSession(const std::string & wx_re_id, uint32_t giver_uin)
{
	if (m_thread == nullptr)
		return 0;

	__Msg_sendReq_RedEnvelope_giverDeleteSession* msg = new __Msg_sendReq_RedEnvelope_giverDeleteSession();
	msg->m_send_pack_id = ++m_send_pack_id_seed;
	msg->m_send_pack_seq = ++m_send_pack_seq_seed;
	msg->m_wx_re_id = wx_re_id;
	msg->m_giver_uin = giver_uin;
	__postMessage(msg);

	return msg->m_send_pack_id;
}

uint64_t NfClient::sendReq_RedEnvelope_giverUpdateSession(const std::string & wx_re_id, uint32_t giver_uin, const std::string & re_qr_code)
{
	if (m_thread == nullptr)
		return 0;

	__Msg_sendReq_RedEnvelope_giverUpdateSession* msg = new __Msg_sendReq_RedEnvelope_giverUpdateSession();
	msg->m_send_pack_id = ++m_send_pack_id_seed;
	msg->m_send_pack_seq = ++m_send_pack_seq_seed;
	msg->m_wx_re_id = wx_re_id;
	msg->m_giver_uin = giver_uin;
	msg->m_re_qr_code = re_qr_code;
	__postMessage(msg);

	return msg->m_send_pack_id;
}

uint64_t NfClient::sendReq_RedEnvelope_giverReportScanResult(const std::string & wx_re_id, uint32_t giver_uin, const std::vector<uint32_t>& recevier_uins)
{
	if (m_thread == nullptr)
		return 0;

	__Msg_sendReq_RedEnvelope_giverReportScanResult* msg = new __Msg_sendReq_RedEnvelope_giverReportScanResult();
	msg->m_send_pack_id = ++m_send_pack_id_seed;
	msg->m_send_pack_seq = ++m_send_pack_seq_seed;
	msg->m_wx_re_id = wx_re_id;
	msg->m_giver_uin = giver_uin;
	msg->m_recevier_uins = recevier_uins;
	__postMessage(msg);

	return msg->m_send_pack_id;
}

uint64_t NfClient::sendReq_RedEnvelope_receiverCreateSession(uint32_t receiver_uin)
{
	if (m_thread == nullptr)
		return 0;

	__Msg_sendReq_RedEnvelope_receiverCreateSession* msg = new __Msg_sendReq_RedEnvelope_receiverCreateSession();
	msg->m_send_pack_id = ++m_send_pack_id_seed;
	msg->m_send_pack_seq = ++m_send_pack_seq_seed;
	msg->m_receiver_uin = receiver_uin;
	__postMessage(msg);

	return msg->m_send_pack_id;
}

uint64_t NfClient::sendReq_RedEnvelope_receiverDeleteSession(uint32_t receiver_uin)
{
	if (m_thread == nullptr)
		return 0;

	__Msg_sendReq_RedEnvelope_receiverDeleteSession* msg = new __Msg_sendReq_RedEnvelope_receiverDeleteSession();
	msg->m_send_pack_id = ++m_send_pack_id_seed;
	msg->m_send_pack_seq = ++m_send_pack_seq_seed;
	msg->m_receiver_uin = receiver_uin;
	__postMessage(msg);

	return msg->m_send_pack_id;
}

uint64_t NfClient::sendReq_RedEnvelope_receiverUpdateSession(uint32_t receiver_uin, const std::string & wx_re_id, uint32_t giver_uin, bool is_re_opened)
{
	if (m_thread == nullptr)
		return 0;

	__Msg_sendReq_RedEnvelope_receiverUpdateSession* msg = new __Msg_sendReq_RedEnvelope_receiverUpdateSession();
	msg->m_send_pack_id = ++m_send_pack_id_seed;
	msg->m_send_pack_seq = ++m_send_pack_seq_seed;
	msg->m_receiver_uin = receiver_uin;
	msg->m_wx_re_id = wx_re_id;
	msg->m_giver_uin = giver_uin;
	msg->m_is_re_opened = is_re_opened;
	__postMessage(msg);

	return msg->m_send_pack_id;
}

uint64_t NfClient::sendReq_RedEnvelope_receiverReportScanResult(uint32_t receiver_uin, const std::vector<uint32_t>& giver_uins)
{
	if (m_thread == nullptr)
		return 0;

	__Msg_sendReq_RedEnvelope_receiverReportScanResult* msg = new __Msg_sendReq_RedEnvelope_receiverReportScanResult();
	msg->m_send_pack_id = ++m_send_pack_id_seed;
	msg->m_send_pack_seq = ++m_send_pack_seq_seed;
	msg->m_receiver_uin = receiver_uin;
	msg->m_giver_uins = giver_uins;
	__postMessage(msg);

	return msg->m_send_pack_id;
}

uint64_t NfClient::sendReq_RedEnvelope_reportStatistic_Zisi(uint32_t uin, bool is_sender, int16_t angle, int16_t ax, int16_t ay, int16_t az)
{
	if (m_thread == nullptr)
		return 0; 
	
	__Msg_sendReq_RedEnvelope_reportStatistic_zishi* msg = new __Msg_sendReq_RedEnvelope_reportStatistic_zishi();
	msg->m_send_pack_id = ++m_send_pack_id_seed;
	msg->m_send_pack_seq = ++m_send_pack_seq_seed;
	msg->m_uin = uin;
	msg->m_is_sender = is_sender;
	msg->m_angle = angle;
	msg->m_ax = ax;
	msg->m_ay = ay;
	msg->m_az = az;
	__postMessage(msg);

	return msg->m_send_pack_id;
}

void NfClient::__postMessage(Message * msg)
{
	msg->m_target = m_handler;
	msg->m_sender = this;
	m_thread->getLooper()->postMessage(msg);
}










void __initNfClientLog()
{
	Log::releaseInstance();
#ifdef S_OS_WIN
	Log::instance().setEnableLogLevel(ELogLevel_debug);
#else
	Log::instance().setEnableLogLevel(ELogLevel_debug);
#endif // S_OS_WIN

	{
		ConsoleAppender* ca = new ConsoleAppender();
		FormatInfo formatInfo;
		formatInfo.m_isShowFunction = false;
		formatInfo.m_isShowFileName = false;
		formatInfo.m_isShowLineNum = false;
		ca->getFormator()->setFormatInfo(formatInfo);
		Log::instance().addAppender(ca);
	}
	{
		FileAppender* fa = new FileAppender();
		fa->init(".", "nfclient");
		FormatInfo formatInfo;
		formatInfo.m_isShowFunction = false;
		formatInfo.m_isShowFileName = false;
		formatInfo.m_isShowLineNum = false;
		fa->getFormator()->setFormatInfo(formatInfo);
		Log::instance().addAppender(fa);
	}
}
