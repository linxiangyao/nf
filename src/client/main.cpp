#include "nfClientCgi.h"
#include "nfClient.h"
USING_NAMESPACE_S





class __ConsoleAppLogic : public IConsoleAppLogic, public NfClient::ICallback
{
public:
	__ConsoleAppLogic(int argc, char** argv)
	{
		m_client = nullptr;
		m_api = nullptr;
		m_is_matched = false;
		m_is_opened = false;
		m_update_start_time_ms = 0;

		ConsoleArgs arg(argc, argv);
		m_giver_uin = 1;
		m_receiver_uin = 3;
		auto it = arg.m_args.find("uin");
		if (it != arg.m_args.end())
		{
			m_receiver_uin = StringUtil::parseUint(arg.m_args["uin"]);
			m_giver_uin = StringUtil::parseUint(arg.m_args["uin"]);
		}

		m_role = __ERole_giver;
		it = arg.m_args.find("role");
		if (it != arg.m_args.end())
		{
			if (arg.m_args["role"] == "r")
				m_role = __ERole_receiver;
		}

	}

private:
	enum __EMsgType
	{
		__EMsgType_NfClient_recvResp_RedEnvelope_giverCreateSession = 9178161,
		__EMsgType_NfClient_recvResp_RedEnvelope_giverDeleteSession,
		__EMsgType_NfClient_recvResp_RedEnvelope_giverUpdateSession,
		__EMsgType_NfClient_recvResp_RedEnvelope_giverReportScanResult,
		__EMsgType_NfClient_recvResp_RedEnvelope_receiverCreateSession,
		__EMsgType_NfClient_recvResp_RedEnvelope_receiverDeleteSession,
		__EMsgType_NfClient_recvResp_RedEnvelope_receiverUpdateSession,
		__EMsgType_NfClient_recvResp_RedEnvelope_receiverReportScanResult,
		__EMsgType_NfClient_recvPush_RedEnvelope_matchResult,
	};

	enum __ERole
	{
		__ERole_giver,
		__ERole_receiver,
	};

	class __Msg_push_MatchResult : public Message
	{
	public:
		uint32_t m_giver_uin;
		std::string m_wx_re_id; 
		std::string m_re_qr_code;
		std::vector<uint32_t> m_recevier_uins;
	};





	virtual void onAppStartMsg(IConsoleAppApi* api) override
	{
		printf("logic:: onAppStartMsg\n");
		m_api = api;

		uint64_t cur_time = TimeUtil::getMsTime();
		m_wx_re_id = "re_" + StringUtil::toString(cur_time);
		m_re_qr_code = __genReQrCode(cur_time);
		m_is_matched = false;
		m_is_opened = false;
		m_update_start_time_ms = cur_time;
		for (uint32_t i = 0; i < 5; ++i)
		{
			m_scan_uins.push_back(i + 1);
		}

		m_scan_timer_id = m_api->getMessageLooper().createTimer(NULL);
		m_api->getMessageLooper().startTimer(m_scan_timer_id, 1, 1 * 1000);

		__restartClient();
	}

	virtual void onAppStopMsg() override
	{
		printf("logic:: onAppStopMsg\n");
		__stopClient();
		printf("logic:: onAppStopMsg end\n");
	}

	virtual void onTextMsg(const std::string& msg) override
	{
		printf("logic:: onTextMsg, text=%s\n", msg.c_str());
		if (msg == "restart\n")
		{
			__restartClient();
		}
		else if (msg == "stop\n")
		{
			__stopClient();
		}
		else if (msg == "start\n")
		{
			__restartClient();
		}
	}

	virtual void onMessage(Message* msg, bool* is_handled) override
	{
		if (msg->m_sender == this && msg->m_target == this)
		{
			*is_handled = true;
			switch (msg->m_msg_type)
			{
			case __EMsgType_NfClient_recvResp_RedEnvelope_giverCreateSession:
				__onMsg_recvResp_RedEnvelope_giverCreateSession(msg);
				break;
			case __EMsgType_NfClient_recvResp_RedEnvelope_giverDeleteSession:
				__onMsg_recvResp_RedEnvelope_giverDeleteSession(msg);
				break;
			case __EMsgType_NfClient_recvResp_RedEnvelope_giverUpdateSession:
				__onMsg_recvResp_RedEnvelope_giverUpdateSession(msg);
				break;
			case __EMsgType_NfClient_recvResp_RedEnvelope_giverReportScanResult:
				__onMsg_recvResp_RedEnvelope_giverReportScanResult(msg);
				break;

			case __EMsgType_NfClient_recvResp_RedEnvelope_receiverCreateSession:
				__onMsg_recvResp_RedEnvelope_receiverCreateSession(msg);
				break;
			case __EMsgType_NfClient_recvResp_RedEnvelope_receiverDeleteSession:
				__onMsg_recvResp_RedEnvelope_receiverDeleteSession(msg);
				break;
			case __EMsgType_NfClient_recvResp_RedEnvelope_receiverUpdateSession:
				__onMsg_recvResp_RedEnvelope_receiverUpdateSession(msg);
				break;
			case __EMsgType_NfClient_recvResp_RedEnvelope_receiverReportScanResult:
				__onMsg_recvResp_RedEnvelope_receiverReportScanResult(msg);
				break;

			case __EMsgType_NfClient_recvPush_RedEnvelope_matchResult:
				__onMsg_recvPush_RedEnvelope_matchResult(msg);
				break;

			default:
				slog_e("err msg type");
				break;
			}
		}
	}

	virtual void onMessageTimerTick(uint64_t timer_id, void* user_data) override
	{
		if (timer_id != m_scan_timer_id)
			return;
		if (m_client == nullptr)
			return;

		if (m_role == __ERole_giver)
		{
			m_client->sendReq_RedEnvelope_giverReportScanResult(m_wx_re_id, m_giver_uin, m_scan_uins);

			// 每30秒更新一次qrCode
			uint64_t cur_time = TimeUtil::getMsTime();
			if (cur_time - m_update_start_time_ms > 30 * 1000)
			{
				m_update_start_time_ms = cur_time;
				m_re_qr_code = __genReQrCode(cur_time);
				m_client->sendReq_RedEnvelope_giverUpdateSession(m_wx_re_id, m_giver_uin, m_re_qr_code);
			}
		}
		else
		{
			if (!m_is_matched)
			{
				m_client->sendReq_RedEnvelope_receiverReportScanResult(m_receiver_uin, m_scan_uins);
			}

			// 收到红包后，5秒后拆开红包，并删除session
			if (m_is_matched && !m_is_opened && TimeUtil::getMsTime() - m_update_start_time_ms > 5 * 1000)
			{
				m_is_opened = true;
				m_client->sendReq_RedEnvelope_receiverUpdateSession(m_receiver_uin, m_wx_re_id, m_giver_uin, true);
				m_client->sendReq_RedEnvelope_receiverDeleteSession(m_receiver_uin);
			}
		}
	}
	
	virtual void onNfClient_recvResp_RedEnvelope_giverCreateSession(uint64_t cgi_id, int err_code) override
	{
		__postMsgToSelf(__EMsgType_NfClient_recvResp_RedEnvelope_giverCreateSession, cgi_id, err_code);
	}

	virtual void onNfClient_recvResp_RedEnvelope_giverDeleteSession(uint64_t cgi_id, int err_code) override
	{
		__postMsgToSelf(__EMsgType_NfClient_recvResp_RedEnvelope_giverDeleteSession, cgi_id, err_code);
	}

	virtual void onNfClient_recvResp_RedEnvelope_giverUpdateSession(uint64_t cgi_id, int err_code) override
	{
		__postMsgToSelf(__EMsgType_NfClient_recvResp_RedEnvelope_giverUpdateSession, cgi_id, err_code);
	}

	virtual void onNfClient_recvResp_RedEnvelope_giverReportScanResult(uint64_t cgi_id, int err_code) override
	{
		__postMsgToSelf(__EMsgType_NfClient_recvResp_RedEnvelope_giverReportScanResult, cgi_id, err_code);
	}

	virtual void onNfClient_recvResp_RedEnvelope_receiverCreateSession(uint64_t cgi_id, int err_code) override
	{
		__postMsgToSelf(__EMsgType_NfClient_recvResp_RedEnvelope_receiverCreateSession, cgi_id, err_code);
	}

	virtual void onNfClient_recvResp_RedEnvelope_receiverDeleteSession(uint64_t cgi_id, int err_code) override
	{
		__postMsgToSelf(__EMsgType_NfClient_recvResp_RedEnvelope_receiverDeleteSession, cgi_id, err_code);
	}

	virtual void onNfClient_recvResp_RedEnvelope_receiverUpdateSession(uint64_t cgi_id, int err_code) override
	{
		__postMsgToSelf(__EMsgType_NfClient_recvResp_RedEnvelope_receiverUpdateSession, cgi_id, err_code);
	}

	virtual void onNfClient_recvResp_RedEnvelope_receiverReportScanResult(uint64_t cgi_id, int err_code) override
	{
		__postMsgToSelf(__EMsgType_NfClient_recvResp_RedEnvelope_receiverReportScanResult, cgi_id, err_code);
	}

	virtual void onNfClient_recvResp_RedEnvelope_reportStatisticZishi(uint64_t cgi_id, int err_code) override
	{

	}

	virtual void onNfClient_recvPush_RedEnvelope_matchResult(uint32_t giver_uin, const std::string& wx_re_id, const std::string & re_qr_code, const std::vector<uint32_t>& recevier_uins) override
	{
		__Msg_push_MatchResult* msg = new __Msg_push_MatchResult();
		msg->m_msg_type = __EMsgType_NfClient_recvPush_RedEnvelope_matchResult;
		msg->m_sender = this;
		msg->m_target = this;
		msg->m_giver_uin = giver_uin;
		msg->m_wx_re_id = wx_re_id;
		msg->m_re_qr_code = re_qr_code;
		msg->m_recevier_uins = recevier_uins;
		m_api->getMessageLooper().postMessage(msg);
	}

	virtual void onNfClient_recvResp_AddFriend_reportUserInfo(uint64_t cgi_id, int err_code) override
	{

	}

	virtual void onNfClient_recvResp_AddFriend_queryUserInfo(uint64_t cgi_id, int err_code, uint32_t uin, const std::string& user_name) override
	{

	}

	void __onMsg_recvResp_RedEnvelope_giverCreateSession(Message* msg)
	{

	}

	void __onMsg_recvResp_RedEnvelope_giverDeleteSession(Message* msg)
	{

	}

	void __onMsg_recvResp_RedEnvelope_giverUpdateSession(Message* msg)
	{

	}

	void __onMsg_recvResp_RedEnvelope_giverReportScanResult(Message* msg)
	{

	}

	void __onMsg_recvResp_RedEnvelope_receiverCreateSession(Message* msg)
	{

	}

	void __onMsg_recvResp_RedEnvelope_receiverDeleteSession(Message* msg)
	{

	}

	void __onMsg_recvResp_RedEnvelope_receiverUpdateSession(Message* msg)
	{

	}

	void __onMsg_recvResp_RedEnvelope_receiverReportScanResult(Message* msg)
	{

	}

	void __onMsg_recvPush_RedEnvelope_matchResult(Message* msg)
	{
		if (m_role == __ERole_receiver)
		{
			m_is_matched = true;
			__Msg_push_MatchResult* m = (__Msg_push_MatchResult*)msg;
			m_giver_uin = m->m_giver_uin;
			m_wx_re_id = m->m_wx_re_id;
		}
	}








	void __stopClient()
	{
		if (m_client != nullptr)
		{
			m_client->stopClient();
			delete m_client;
			m_client = nullptr;
		}
	}

	void __restartClient()
	{
		__stopClient();

		m_client = new NfClient(this);
		m_client->startClient();
		if (m_role == __ERole_giver)
		{
			m_client->sendReq_RedEnvelope_giverCreateSession(m_wx_re_id, m_giver_uin, m_re_qr_code, 10);
		}
		else
		{
			m_client->sendReq_RedEnvelope_receiverCreateSession(m_receiver_uin);
		}

		m_client->sendReq_RedEnvelope_reportStatistic_Zisi(1024, true, 180, 100, 200, -300);
		m_client->sendReq_AddFriend_reportUserInfo(1, "ryan");
		m_client->sendReq_AddFriend_reportUserInfo(2, "lin");
		m_client->sendReq_AddFriend_queryUserInfo(1);
		m_client->sendReq_AddFriend_queryUserInfo(2);
	}

	void __postMsgToSelf(__EMsgType msg_type, uint64_t cgi_id, int err_code)
	{
		Message* msg = new Message();
		msg->m_sender = this;
		msg->m_target = this;
		msg->m_msg_type = msg_type;
		msg->m_args.setUint64("cgi_id", cgi_id);
		msg->m_args.setInt32("err_code", err_code);
		m_api->getMessageLooper().postMessage(msg);
	}

	std::string __genReQrCode(uint64_t t)
	{
		return std::string() + "qrcode_" + StringUtil::toString(t);
	}




	IConsoleAppApi* m_api;
	uint64_t m_scan_timer_id;
	NfClient* m_client;
	__ERole m_role;
	uint32_t m_giver_uin;
	uint32_t m_receiver_uin;
	std::string m_wx_re_id;
	std::string m_re_qr_code;
	std::vector<uint32_t> m_scan_uins;
	bool m_is_matched;
	bool m_is_opened;
	uint64_t m_update_start_time_ms;
};














int main(int argc, char** argv)
{
	__initNfClientLog();
	slog_i("NfClient ---------------------------------------------------------");
	std::string args;
	for (int i = 0; i < argc; ++i)
	{
		args = args + argv[i] + " ";
	}
	slog_i("args=%0", args);

	ConsoleApp* app = new ConsoleApp();
	__ConsoleAppLogic* logic = new __ConsoleAppLogic(argc, argv);
	app->run(logic);
	delete logic;
	delete app;
    return 0;
}

