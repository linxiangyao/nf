#ifndef NF_CLIENT_H
#define NF_CLIENT_H
#include "nfComm.h"
#include "nfClientCgi.h"
USING_NAMESPACE_S




void __initNfClientLog();





class NfClient
{
public:
	class ICallback
	{
	public:
		virtual ~ICallback() {}
		virtual void onNfClient_recvResp_RedEnvelope_giverCreateSession(uint64_t cgi_id, int err_code) = 0;
		virtual void onNfClient_recvResp_RedEnvelope_giverDeleteSession(uint64_t cgi_id, int err_code) = 0;
		virtual void onNfClient_recvResp_RedEnvelope_giverUpdateSession(uint64_t cgi_id, int err_code) = 0;
		virtual void onNfClient_recvResp_RedEnvelope_giverReportScanResult(uint64_t cgi_id, int err_code) = 0;
		virtual void onNfClient_recvResp_RedEnvelope_receiverCreateSession(uint64_t cgi_id, int err_code) = 0;
		virtual void onNfClient_recvResp_RedEnvelope_receiverDeleteSession(uint64_t cgi_id, int err_code) = 0;
		virtual void onNfClient_recvResp_RedEnvelope_receiverUpdateSession(uint64_t cgi_id, int err_code) = 0;
		virtual void onNfClient_recvResp_RedEnvelope_receiverReportScanResult(uint64_t cgi_id, int err_code) = 0;
		virtual void onNfClient_recvResp_RedEnvelope_reportStatisticZishi(uint64_t cgi_id, int err_code) = 0;
		virtual void onNfClient_recvPush_RedEnvelope_matchResult(uint32_t giver_uin, const std::string& wx_re_id, const std::string& re_qr_code, const std::vector<uint32_t>& recevier_uins) = 0;
		virtual void onNfClient_recvResp_AddFriend_reportUserInfo(uint64_t cgi_id, int err_code) = 0;
		virtual void onNfClient_recvResp_AddFriend_queryUserInfo(uint64_t cgi_id, int err_code, uint32_t uin, const std::string& user_name) = 0;
	};


	NfClient(ICallback* callback);
	~NfClient();

	bool startClient();
	void stopClient();
	uint64_t sendReq_RedEnvelope_giverCreateSession(const std::string& wx_re_id, uint32_t giver_uin, const std::string& re_qr_code, uint32_t re_count);
	uint64_t sendReq_RedEnvelope_giverDeleteSession(const std::string& wx_re_id, uint32_t giver_uin);
	uint64_t sendReq_RedEnvelope_giverUpdateSession(const std::string& wx_re_id, uint32_t giver_uin, const std::string& re_qr_code);
	uint64_t sendReq_RedEnvelope_giverReportScanResult(const std::string& wx_re_id, uint32_t giver_uin, const std::vector<uint32_t>& recevier_uins);
	uint64_t sendReq_RedEnvelope_receiverCreateSession(uint32_t receiver_uin);
	uint64_t sendReq_RedEnvelope_receiverDeleteSession(uint32_t receiver_uin);
	uint64_t sendReq_RedEnvelope_receiverUpdateSession(uint32_t receiver_uin, const std::string& wx_re_id, uint32_t giver_uin, bool is_re_opened);
	uint64_t sendReq_RedEnvelope_receiverReportScanResult(uint32_t receiver_uin, const std::vector<uint32_t>& giver_uins);
	uint64_t sendReq_RedEnvelope_reportStatistic_Zisi(uint32_t uin, bool is_sender, int16_t angle, int16_t ax, int16_t ay, int16_t az);
	uint64_t sendReq_AddFriend_reportUserInfo(uint32_t uin, const std::string& user_name);
	uint64_t sendReq_AddFriend_queryUserInfo(uint32_t uin);




private:
	class __Handler;


	void __postMessage(Message* msg);



	ICallback* m_callback;
	MessageLoopThread* m_thread;
	__Handler * m_handler;
	__ClientSendPackBuilder m_cgi_ctx;
	uint64_t m_send_pack_id_seed;
	uint32_t m_send_pack_seq_seed;
};






#endif

