#ifndef _NF_SERVER_CGI_H_
#define _NF_SERVER_CGI_H_
#include "nfComm.h"
USING_NAMESPACE_S



class __ServerCgiCtx
{
public:
	__ServerCgiCtx()
	{
		m_uin = 0;
		m_packer = nullptr;
		m_network = nullptr;
	}

	ServerCgi::SendPack* newSendPackAndPack(uint32_t send_cmd_type, uint32_t send_seq, const byte_t* send_body, size_t send_body_len) const
	{
		ServerCgi::SendPack* send_pack = m_network->newSendPack(m_sid, m_ssid, send_cmd_type, send_seq);

		Binary* whole_pack_bin = &(send_pack->m_send_whole_pack_bin);
		StPacker::Pack p;
		p.m_head.m_cmd_type = send_pack->m_send_cmd_type;
		p.m_head.m_err = 0;
		p.m_head.m_seq = send_pack->m_send_seq;
		p.m_head.m_uin = m_uin;
		memcpy(p.m_head.m_session_id_bin, m_ssid.m_data, 16);
		p.m_body.attach((byte_t*)send_body, send_body_len);
		m_packer->packToBin(p, whole_pack_bin);
		p.m_body.detach();

		return send_pack;
	}

	socket_id_t m_sid;
	session_id_t m_ssid;
	uint32_t m_uin;
	StPacker* m_packer;
	ServerNetwork* m_network;
	ServerCgi::ICallback* m_callback;
};











class __ServerCgi_RedEnvelope_giverCreateSession : public ServerCgi
{
public:
	static const ServerCgiInfo & s_getServerCgiInfo()
	{
		s_cgi_info.m_cgi_type = EServerCgiType_c2sReq_s2cResp;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_GiverCreateSession;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_GiverCreateSession;
		return s_cgi_info;
	}

	virtual const ServerCgiInfo& getServerCgiInfo() const override { return s_getServerCgiInfo(); }

	bool initSendPack(const __ServerCgiCtx& cgi_ctx, uint32_t err_code)
	{
		m_s2cResp_body = std::string() + "re_giver_create_session: err_code=" + StringUtil::toString(err_code) + ",";
		SendPack* send_pack = cgi_ctx.newSendPackAndPack(getServerCgiInfo().m_send_cmd_type, getRecvPack()->m_recv_seq, (const byte_t*)m_s2cResp_body.c_str(), m_s2cResp_body.size() + 1);
		setSendPack(send_pack);
		setCallback(cgi_ctx.m_callback);
		return true;
	}

	std::string m_c2sReq_body;
	std::string m_c2sReq_wx_re_id;
	uint32_t m_c2sReq_giver_uin;
	std::string m_c2sReq_re_qr_code;
	uint32_t m_c2sReq_re_count;

	std::string m_s2cResp_body;


private:
	virtual void onSetRecvPackEnd() override
	{
		StPacker::Pack* st_pack = (StPacker::Pack*)(getRecvPack()->m_recv_ext);
		m_c2sReq_body = (const char*)st_pack->m_body.getData();
		m_c2sReq_wx_re_id = StringUtil::fetchMiddle(m_c2sReq_body, "wx_re_id=", ",");
		m_c2sReq_giver_uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "giver_uin=", ","));
		m_c2sReq_re_qr_code = StringUtil::fetchMiddle(m_c2sReq_body, "re_qr_code=", ",");
		m_c2sReq_re_count = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "re_count=", ","));
	}

	static ServerCgiInfo s_cgi_info;
};
ServerCgiInfo __ServerCgi_RedEnvelope_giverCreateSession::s_cgi_info;



class __ServerCgi_RedEnvelope_giverDeleteSession : public ServerCgi
{
public:
	static const ServerCgiInfo & s_getServerCgiInfo()
	{
		s_cgi_info.m_cgi_type = EServerCgiType_c2sReq_s2cResp;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_GiverDeleteSession;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_GiverDeleteSession;
		return s_cgi_info;
	}

	virtual const ServerCgiInfo& getServerCgiInfo() const override { return s_getServerCgiInfo(); }

	bool initSendPack(const __ServerCgiCtx& cgi_ctx, uint32_t err_code)
	{
		m_s2cResp_body = std::string() + "re_giver_delete_session: err_code=" + StringUtil::toString(err_code) + ",";
		SendPack* send_pack = cgi_ctx.newSendPackAndPack(getServerCgiInfo().m_send_cmd_type, getRecvPack()->m_recv_seq, (const byte_t*)m_s2cResp_body.c_str(), m_s2cResp_body.size() + 1);
		setSendPack(send_pack);
		setCallback(cgi_ctx.m_callback);
		return true;
	}

	std::string m_c2sReq_body;
	std::string m_c2sReq_wx_re_id;
	uint32_t m_c2sReq_giver_uin;
	std::string m_s2cResp_body;


private:
	virtual void onSetRecvPackEnd() override
	{
		StPacker::Pack* st_pack = (StPacker::Pack*)(getRecvPack()->m_recv_ext);
		m_c2sReq_body = (const char*)st_pack->m_body.getData();
		m_c2sReq_wx_re_id = StringUtil::fetchMiddle(m_c2sReq_body, "wx_re_id=", ",");
		m_c2sReq_giver_uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "giver_uin=", ","));
	}

	static ServerCgiInfo s_cgi_info;
};
ServerCgiInfo __ServerCgi_RedEnvelope_giverDeleteSession::s_cgi_info;



class __ServerCgi_RedEnvelope_giverUpdateSession : public ServerCgi
{
public:
	static const ServerCgiInfo & s_getServerCgiInfo()
	{
		s_cgi_info.m_cgi_type = EServerCgiType_c2sReq_s2cResp;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_GiverUpdateSession;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_GiverUpdateSession;
		return s_cgi_info;
	}

	virtual const ServerCgiInfo& getServerCgiInfo() const override { return s_getServerCgiInfo(); }

	bool initSendPack(const __ServerCgiCtx& cgi_ctx, uint32_t err_code)
	{
		m_s2cResp_body = std::string() + "re_giver_update_session: err_code=" + StringUtil::toString(err_code) + ",";
		SendPack* send_pack = cgi_ctx.newSendPackAndPack(getServerCgiInfo().m_send_cmd_type, getRecvPack()->m_recv_seq, (const byte_t*)m_s2cResp_body.c_str(), m_s2cResp_body.size() + 1);
		setSendPack(send_pack);
		setCallback(cgi_ctx.m_callback);
		return true;
	}

	std::string m_c2sReq_body;
	std::string m_c2sReq_wx_re_id;
	uint32_t m_c2sReq_giver_uin;
	std::string m_c2sReq_re_qr_code;

	std::string m_s2cResp_body;


private:
	virtual void onSetRecvPackEnd() override
	{
		StPacker::Pack* st_pack = (StPacker::Pack*)(getRecvPack()->m_recv_ext);
		m_c2sReq_body = (const char*)st_pack->m_body.getData();
		m_c2sReq_wx_re_id = StringUtil::fetchMiddle(m_c2sReq_body, "wx_re_id=", ",");
		m_c2sReq_giver_uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "giver_uin=", ","));
		m_c2sReq_re_qr_code = StringUtil::fetchMiddle(m_c2sReq_body, "re_qr_code=", ",");
	}

	static ServerCgiInfo s_cgi_info;
};
ServerCgiInfo __ServerCgi_RedEnvelope_giverUpdateSession::s_cgi_info;



class __ServerCgi_RedEnvelope_giverReportScanResult : public ServerCgi
{
public:
	static const ServerCgiInfo & s_getServerCgiInfo()
	{
		s_cgi_info.m_cgi_type = EServerCgiType_c2sReq_s2cResp;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_GiverReportScanResult;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_GiverReportScanResult;
		return s_cgi_info;
	}

	virtual const ServerCgiInfo& getServerCgiInfo() const override { return s_getServerCgiInfo(); }

	bool initSendPack(const __ServerCgiCtx& cgi_ctx, uint32_t err_code)
	{
		m_s2cResp_body = std::string() + "re_giver_report_scan: err_code=" + StringUtil::toString(err_code) + ",";
		SendPack* send_pack = cgi_ctx.newSendPackAndPack(getServerCgiInfo().m_send_cmd_type, getRecvPack()->m_recv_seq, (const byte_t*)m_s2cResp_body.c_str(), m_s2cResp_body.size() + 1);
		setSendPack(send_pack);
		setCallback(cgi_ctx.m_callback);
		return true;
	}

	std::string m_c2sReq_body;
	std::string m_c2sReq_wx_re_id;
	uint32_t m_c2sReq_giver_uin;
	std::vector<uint32_t> m_c2sReq_scan_uins;

	std::string m_s2cResp_body;


private:
	virtual void onSetRecvPackEnd() override
	{
		StPacker::Pack* st_pack = (StPacker::Pack*)(getRecvPack()->m_recv_ext);
		m_c2sReq_body = (const char*)st_pack->m_body.getData();
		m_c2sReq_giver_uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "giver_uin=", ","));
		m_c2sReq_wx_re_id = StringUtil::fetchMiddle(m_c2sReq_body, "wx_re_id=", ",");
		uint32_t c2sReq_uin_count = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "scan_uin_count=", ","));
		for (size_t i = 0; i < c2sReq_uin_count; ++i)
		{
			std::string uin_tag = std::string() + "scan_uin" + StringUtil::toString(i + 1) + "=" ;
			uint32_t uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, uin_tag, ","));
			m_c2sReq_scan_uins.push_back(uin);
		}
	}

	static ServerCgiInfo s_cgi_info;
};
ServerCgiInfo __ServerCgi_RedEnvelope_giverReportScanResult::s_cgi_info;



class __ServerCgi_RedEnvelope_receiverCreateSession : public ServerCgi
{
public:
	static const ServerCgiInfo & s_getServerCgiInfo()
	{
		s_cgi_info.m_cgi_type = EServerCgiType_c2sReq_s2cResp;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_ReceiverCreateSession;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_ReceiverCreateSession;
		return s_cgi_info;
	}

	virtual const ServerCgiInfo& getServerCgiInfo() const override { return s_getServerCgiInfo(); }

	bool initSendPack(const __ServerCgiCtx& cgi_ctx, uint32_t err_code)
	{
		m_s2cResp_body = std::string() + "re_receiver_create_session: err_code=" + StringUtil::toString(err_code) + ",";
		SendPack* send_pack = cgi_ctx.newSendPackAndPack(getServerCgiInfo().m_send_cmd_type, getRecvPack()->m_recv_seq, (const byte_t*)m_s2cResp_body.c_str(), m_s2cResp_body.size() + 1);
		setSendPack(send_pack);
		setCallback(cgi_ctx.m_callback);
		return true;
	}

	std::string m_c2sReq_body;
	uint32_t m_c2sReq_receiver_uin;

	std::string m_s2cResp_body;


private:
	virtual void onSetRecvPackEnd() override
	{
		StPacker::Pack* st_pack = (StPacker::Pack*)(getRecvPack()->m_recv_ext);
		m_c2sReq_body = (const char*)st_pack->m_body.getData();
		m_c2sReq_receiver_uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "receiver_uin=", ","));
	}

	static ServerCgiInfo s_cgi_info;
};
ServerCgiInfo __ServerCgi_RedEnvelope_receiverCreateSession::s_cgi_info;



class __ServerCgi_RedEnvelope_receiverDeleteSession : public ServerCgi
{
public:
	static const ServerCgiInfo & s_getServerCgiInfo()
	{
		s_cgi_info.m_cgi_type = EServerCgiType_c2sReq_s2cResp;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_ReceiverDeleteSession;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_ReceiverDeleteSession;
		return s_cgi_info;
	}

	virtual const ServerCgiInfo& getServerCgiInfo() const override { return s_getServerCgiInfo(); }

	bool initSendPack(const __ServerCgiCtx& cgi_ctx, uint32_t err_code)
	{
		m_s2cResp_body = std::string() + "re_receiver_delete_session: err_code=" + StringUtil::toString(err_code) + ",";
		SendPack* send_pack = cgi_ctx.newSendPackAndPack(getServerCgiInfo().m_send_cmd_type, getRecvPack()->m_recv_seq, (const byte_t*)m_s2cResp_body.c_str(), m_s2cResp_body.size() + 1);
		setSendPack(send_pack);
		setCallback(cgi_ctx.m_callback);
		return true;
	}

	std::string m_c2sReq_body;
	uint32_t m_c2sReq_receiver_uin;
	std::string m_s2cResp_body;


private:
	virtual void onSetRecvPackEnd() override
	{
		StPacker::Pack* st_pack = (StPacker::Pack*)(getRecvPack()->m_recv_ext);
		m_c2sReq_body = (const char*)st_pack->m_body.getData();
		m_c2sReq_receiver_uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "receiver_uin=", ","));
	}

	static ServerCgiInfo s_cgi_info;
};
ServerCgiInfo __ServerCgi_RedEnvelope_receiverDeleteSession::s_cgi_info;



class __ServerCgi_RedEnvelope_receiverUpdateSession : public ServerCgi
{
public:
	static const ServerCgiInfo & s_getServerCgiInfo()
	{
		s_cgi_info.m_cgi_type = EServerCgiType_c2sReq_s2cResp;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_ReceiverUpdateSession;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_ReceiverUpdateSession;
		return s_cgi_info;
	}

	virtual const ServerCgiInfo& getServerCgiInfo() const override { return s_getServerCgiInfo(); }

	bool initSendPack(const __ServerCgiCtx& cgi_ctx, uint32_t err_code)
	{
		m_s2cResp_body = std::string() + "re_receiver_update_session: err_code=" + StringUtil::toString(err_code) + ",";
		SendPack* send_pack = cgi_ctx.newSendPackAndPack(getServerCgiInfo().m_send_cmd_type, getRecvPack()->m_recv_seq, (const byte_t*)m_s2cResp_body.c_str(), m_s2cResp_body.size() + 1);
		setSendPack(send_pack);
		setCallback(cgi_ctx.m_callback);
		return true;
	}

	std::string m_c2sReq_body;
	uint32_t m_c2sReq_giver_uin;
	std::string m_c2sReq_wx_re_id;
	uint32_t m_c2sReq_receiver_uin;
	bool m_c2sReq_is_envelope_opend;

	std::string m_s2cResp_body;


private:
	virtual void onSetRecvPackEnd() override
	{
		StPacker::Pack* st_pack = (StPacker::Pack*)(getRecvPack()->m_recv_ext);
		m_c2sReq_body = (const char*)st_pack->m_body.getData();
		m_c2sReq_wx_re_id = StringUtil::fetchMiddle(m_c2sReq_body, "wx_re_id=", ",");
		m_c2sReq_giver_uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "giver_uin=", ","));
		m_c2sReq_receiver_uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "receiver_uin=", ","));
		m_c2sReq_is_envelope_opend = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "is_envelope_opened=", ","));
	}

	static ServerCgiInfo s_cgi_info;
};
ServerCgiInfo __ServerCgi_RedEnvelope_receiverUpdateSession::s_cgi_info;



class __ServerCgi_RedEnvelope_receiverReportScanResult : public ServerCgi
{
public:
	static const ServerCgiInfo & s_getServerCgiInfo()
	{
		s_cgi_info.m_cgi_type = EServerCgiType_c2sReq_s2cResp;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_ReceiverReportScanResult;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_ReceiverReportScanResult;
		return s_cgi_info;
	}

	virtual const ServerCgiInfo& getServerCgiInfo() const override { return s_getServerCgiInfo(); }

	bool initSendPack(const __ServerCgiCtx& cgi_ctx, uint32_t err_code)
	{
		m_s2cResp_body = std::string() + "re_receiver_report_scan: err_code=" + StringUtil::toString(err_code) + ",";
		SendPack* send_pack = cgi_ctx.newSendPackAndPack(getServerCgiInfo().m_send_cmd_type, getRecvPack()->m_recv_seq, (const byte_t*)m_s2cResp_body.c_str(), m_s2cResp_body.size() + 1);
		setSendPack(send_pack);
		setCallback(cgi_ctx.m_callback);
		return true;
	}

	std::string m_c2sReq_body;
	uint32_t m_c2sReq_receiver_uin;
	std::vector<uint32_t> m_c2sReq_scan_uins;
	std::string m_s2cResp_body;


private:
	virtual void onSetRecvPackEnd() override
	{
		StPacker::Pack* st_pack = (StPacker::Pack*)(getRecvPack()->m_recv_ext);
		m_c2sReq_body = (const char*)st_pack->m_body.getData();
		m_c2sReq_receiver_uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "receiver_uin=", ","));
		uint32_t c2sReq_uin_count = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "scan_uin_count=", ","));
		for (size_t i = 0; i < c2sReq_uin_count; ++i)
		{
			std::string uin_tag = std::string() + "scan_uin"  + StringUtil::toString(i + 1) + "=";
			uint32_t uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, uin_tag, ","));
			m_c2sReq_scan_uins.push_back(uin);
		}
	}

	static ServerCgiInfo s_cgi_info;
};
ServerCgiInfo __ServerCgi_RedEnvelope_receiverReportScanResult::s_cgi_info;



class __ServerCgi_RedEnvelope_reportStatisticZishi : public ServerCgi
{
public:
	static const ServerCgiInfo & s_getServerCgiInfo()
	{
		s_cgi_info.m_cgi_type = EServerCgiType_c2sReq_s2cResp;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_ReportStatisticZishi;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_ReportStatisticZishi;
		return s_cgi_info;
	}

	virtual const ServerCgiInfo& getServerCgiInfo() const override { return s_getServerCgiInfo(); }

	bool initSendPack(const __ServerCgiCtx& cgi_ctx, uint32_t err_code)
	{
		m_s2cResp_body = std::string() + "re_report_statistic_zisi: err_code=" + StringUtil::toString(err_code) + ",";
		SendPack* send_pack = cgi_ctx.newSendPackAndPack(getServerCgiInfo().m_send_cmd_type, getRecvPack()->m_recv_seq, (const byte_t*)m_s2cResp_body.c_str(), m_s2cResp_body.size() + 1);
		setSendPack(send_pack);
		setCallback(cgi_ctx.m_callback);
		return true;
	}

	std::string m_c2sReq_body;
	uint32_t m_c2sReq_uin;
	bool m_c2sReq_is_sender;
	int16_t m_c2sReq_angle;
	int16_t m_c2sReq_ax;
	int16_t m_c2sReq_ay;
	int16_t m_c2sReq_az;
	std::string m_s2cResp_body;


private:
	virtual void onSetRecvPackEnd() override
	{
		StPacker::Pack* st_pack = (StPacker::Pack*)(getRecvPack()->m_recv_ext);
		m_c2sReq_body = (const char*)st_pack->m_body.getData();
		m_c2sReq_uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "uin=", ","));
		m_c2sReq_is_sender = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "is_sender=", ","));
		m_c2sReq_angle = StringUtil::parseInt(StringUtil::fetchMiddle(m_c2sReq_body, "angle=", ","));
		m_c2sReq_ax = StringUtil::parseInt(StringUtil::fetchMiddle(m_c2sReq_body, "ax=", ","));
		m_c2sReq_ay = StringUtil::parseInt(StringUtil::fetchMiddle(m_c2sReq_body, "ay=", ","));
		m_c2sReq_az = StringUtil::parseInt(StringUtil::fetchMiddle(m_c2sReq_body, "az=", ","));
	}

	static ServerCgiInfo s_cgi_info;
};
ServerCgiInfo __ServerCgi_RedEnvelope_reportStatisticZishi::s_cgi_info;


class __ServerCgi_redEnvelopeMatchResult : public ServerCgi
{
public:
	static const ServerCgiInfo & s_getServerCgiInfo()
	{
		s_cgi_info.m_cgi_type = EServerCgiType_s2cPush;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_s2cPush_RedEnvelope_MatchResult;
		return s_cgi_info;
	}

	virtual const ServerCgiInfo& getServerCgiInfo() const override { return s_getServerCgiInfo(); }

	virtual void initSendPack(const __ServerCgiCtx& cgi_ctx, const std::string& wx_re_id, uint32_t giver_uin, std::vector<uint32_t> receiver_uins, const std::string& re_qr_code)
	{
		m_s2c_push_body = std::string() + "re_matchs: "
			+ "wx_re_id=" + wx_re_id + ","
			+ "giver_uin=" + StringUtil::toString(giver_uin) + ","
			+ "re_qr_code=" + re_qr_code + ","
			+ "receiver_uin_count=" + StringUtil::toString(receiver_uins.size()) + ",";
		for (size_t i = 0; i < receiver_uins.size(); ++i)
		{
			m_s2c_push_body = m_s2c_push_body + "receiver_uin" + StringUtil::toString(i + 1) + "=" + StringUtil::toString(receiver_uins[i]) + ",";
		}
		SendPack* send_pack = cgi_ctx.newSendPackAndPack(getServerCgiInfo().m_send_cmd_type, 0, (const byte_t*)m_s2c_push_body.c_str(), m_s2c_push_body.size() + 1);
		setSendPack(send_pack);
		setCallback(cgi_ctx.m_callback);
	}


	std::string m_s2c_push_body;


private:
	static ServerCgiInfo s_cgi_info;
};
ServerCgiInfo __ServerCgi_redEnvelopeMatchResult::s_cgi_info;






class __ServerCgi_AddFriend_reportUserInfo : public ServerCgi
{
public:
	static const ServerCgiInfo & s_getServerCgiInfo()
	{
		s_cgi_info.m_cgi_type = EServerCgiType_c2sReq_s2cResp;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_c2sReq_AddFriend_ReportUserInfo;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_s2cResp_AddFriend_ReportUserInfo;
		return s_cgi_info;
	}

	virtual const ServerCgiInfo& getServerCgiInfo() const override { return s_getServerCgiInfo(); }

	bool initSendPack(const __ServerCgiCtx& cgi_ctx, uint32_t err_code)
	{
		m_s2cResp_body = std::string() + "af_report_user_info: err_code=" + StringUtil::toString(err_code) + ",";
		SendPack* send_pack = cgi_ctx.newSendPackAndPack(getServerCgiInfo().m_send_cmd_type, getRecvPack()->m_recv_seq, (const byte_t*)m_s2cResp_body.c_str(), m_s2cResp_body.size() + 1);
		setSendPack(send_pack);
		setCallback(cgi_ctx.m_callback);
		return true;
	}

	std::string m_c2sReq_body;
	uint32_t m_c2sReq_uin;
	std::string m_c2sReq_user_name;

	std::string m_s2cResp_body;


private:
	virtual void onSetRecvPackEnd() override
	{
		StPacker::Pack* st_pack = (StPacker::Pack*)(getRecvPack()->m_recv_ext);
		m_c2sReq_body = (const char*)st_pack->m_body.getData();
		m_c2sReq_uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "uin=", ","));
		m_c2sReq_user_name = StringUtil::fetchMiddle(m_c2sReq_body, "user_name=", ",");
	}

	static ServerCgiInfo s_cgi_info;
};
ServerCgiInfo __ServerCgi_AddFriend_reportUserInfo::s_cgi_info;




class __ServerCgi_AddFriend_queryUserInfo : public ServerCgi
{
public:
	static const ServerCgiInfo & s_getServerCgiInfo()
	{
		s_cgi_info.m_cgi_type = EServerCgiType_c2sReq_s2cResp;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_c2sReq_AddFriend_QueryUserInfo;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_s2cResp_AddFriend_QueryUserInfo;
		return s_cgi_info;
	}

	virtual const ServerCgiInfo& getServerCgiInfo() const override { return s_getServerCgiInfo(); }

	bool initSendPack(const __ServerCgiCtx& cgi_ctx, uint32_t err_code, uint32_t uin, const std::string& user_name)
	{
		m_s2cResp_body = std::string() + "af_query_user_info: err_code=" + StringUtil::toString(err_code) + ", "
			+ "uin=" + StringUtil::toString(uin) + ", "
			+ "user_name=" + user_name + ", "
			;
		SendPack* send_pack = cgi_ctx.newSendPackAndPack(getServerCgiInfo().m_send_cmd_type, getRecvPack()->m_recv_seq, (const byte_t*)m_s2cResp_body.c_str(), m_s2cResp_body.size() + 1);
		setSendPack(send_pack);
		setCallback(cgi_ctx.m_callback);
		return true;
	}

	std::string m_c2sReq_body;
	uint32_t m_c2sReq_uin;

	std::string m_s2cResp_body;


private:
	virtual void onSetRecvPackEnd() override
	{
		StPacker::Pack* st_pack = (StPacker::Pack*)(getRecvPack()->m_recv_ext);
		m_c2sReq_body = (const char*)st_pack->m_body.getData();
		m_c2sReq_uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_c2sReq_body, "uin=", ","));
	}

	static ServerCgiInfo s_cgi_info;
};
ServerCgiInfo __ServerCgi_AddFriend_queryUserInfo::s_cgi_info;




#endif
