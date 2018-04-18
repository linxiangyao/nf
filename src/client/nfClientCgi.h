#ifndef _NF_CLIENT_CGI_H_
#define _NF_CLIENT_CGI_H_
#include "nfComm.h"
USING_NAMESPACE_S



class __ClientSendPackBuilder
{
public:
	__ClientSendPackBuilder()
	{
		m_uin = 0;

		memset(m_session_id_bin, 0, 16);
		uint64_t tick = TimeUtil::getTick();
		memcpy(m_session_id_bin, &tick, 8);
		m_session_id_bin[14] = 0x11;
		m_session_id_bin[15] = 0x12;

		m_packer = nullptr;
		m_network = nullptr;
		m_callback = nullptr;
		m_send_pack_id = 0;
		m_send_pack_seq = 0;
		m_send_body = nullptr;
		m_send_body_len = 0;
	}

	ClientCgi::SendPack* newSendPackAndPack() const
	{
		ClientCgi::SendPack* send_pack = m_network->newSendPack(m_send_pack_id, m_send_cmd_type, m_send_pack_seq);

		Binary* whole_pack_bin = &(send_pack->m_send_whole_pack_bin);
		StPacker::Pack p;
		p.m_head.m_cmd_type = send_pack->m_send_cmd_type;
		p.m_head.m_err = 0;
		p.m_head.m_seq = send_pack->m_send_seq;
		p.m_head.m_uin = m_uin;
		memcpy(p.m_head.m_session_id_bin, m_session_id_bin, 16);
		p.m_body.attach((byte_t*)m_send_body, m_send_body_len);
		m_packer->packToBin(p, whole_pack_bin);
		p.m_body.detach();

		return send_pack;
	}

	uint32_t m_uin;
	byte_t m_session_id_bin[16];
	StPacker* m_packer;
	ClientNetwork* m_network;

	ClientCgi::ICallback* m_callback;
	uint64_t m_send_pack_id;
	uint32_t m_send_pack_seq;
	uint32_t m_send_cmd_type;
	const byte_t* m_send_body;
	size_t m_send_body_len;
};






class __ClientCgi_RedEnvelope_base : public ClientCgi
{
public:
	std::string m_c2s_req_body;
	std::string m_s2c_resp_body;
	uint32_t m_s2c_resp_err_code;


protected:
	bool __initSendPack(__ClientSendPackBuilder& builder)
	{
		builder.m_send_cmd_type = getCgiInfo().m_send_cmd_type;
		builder.m_send_body = (const byte_t*)m_c2s_req_body.c_str();
		builder.m_send_body_len = m_c2s_req_body.size() + 1;
		ClientCgi::SendPack* send_pack = builder.newSendPackAndPack();

		setSendPack(send_pack);
		setCallback(builder.m_callback);
		return true;
	}
};



class __ClientCgi_RedEnvelope_giverCreateSession : public __ClientCgi_RedEnvelope_base
{
public:
	static const ClientCgiInfo & s_getCgiInfo()
	{
		s_cgi_info.m_cgi_type = EClientCgiType_c2sReq_s2cResp;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_GiverCreateSession;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_GiverCreateSession;
		return s_cgi_info;
	}

	virtual const ClientCgiInfo & getCgiInfo() const { return s_getCgiInfo(); }

	bool initSendPack(__ClientSendPackBuilder& ctx, const std::string& wx_re_id,  uint32_t giver_uin, const std::string& re_qr_code, uint32_t re_count)
	{
		m_c2s_req_body = std::string() + "re_giver_create_session: "
			+ "wx_re_id=" + wx_re_id + ","
			+ "giver_uin=" + StringUtil::toString(giver_uin) + ","
			+ "re_qr_code=" + re_qr_code + ","
			+ "re_count=" + StringUtil::toString(re_count) + ",";

		__initSendPack(ctx);
		return true;
	}


private:
	virtual void onSetRecvPackEnd()
	{
		RecvPack* recv_pack = getRecvPack();
		StPacker::Pack* st_pack = (StPacker::Pack*)recv_pack->m_recv_ext;
		m_s2c_resp_body = (const char*)st_pack->m_body.getData();
		m_s2c_resp_err_code = StringUtil::parseUint(StringUtil::fetchMiddle(m_s2c_resp_body, "err_code=", ","));
	}

	static ClientCgiInfo s_cgi_info;
};



class __ClientCgi_RedEnvelope_giverDeleteSession : public __ClientCgi_RedEnvelope_base
{
public:
	static const ClientCgiInfo & s_getCgiInfo()
	{
		s_cgi_info.m_cgi_type = EClientCgiType_c2sReq_s2cResp;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_GiverDeleteSession;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_GiverDeleteSession;
		return s_cgi_info;
	}

	virtual const ClientCgiInfo & getCgiInfo() const { return s_getCgiInfo(); }

	bool initSendPack(__ClientSendPackBuilder& ctx, const std::string& wx_re_id, uint32_t giver_uin)
	{
		m_c2s_req_body = std::string() + "re_giver_delete_session: "
			+ "wx_re_id=" + wx_re_id + ","
			+ "giver_uin=" + StringUtil::toString(giver_uin) + ",";

		__initSendPack(ctx);
		return true;
	}
	

private:
	virtual void onSetRecvPackEnd()
	{
		RecvPack* recv_pack = getRecvPack();
		StPacker::Pack* st_pack = (StPacker::Pack*)recv_pack->m_recv_ext;
		m_s2c_resp_body = (const char*)st_pack->m_body.getData();
		m_s2c_resp_err_code = StringUtil::parseUint(StringUtil::fetchMiddle(m_s2c_resp_body, "err_code=", ","));
	}

	static ClientCgiInfo s_cgi_info;
};



class __ClientCgi_RedEnvelope_giverUpdateSession : public __ClientCgi_RedEnvelope_base
{
public:
	static const ClientCgiInfo & s_getCgiInfo()
	{
		s_cgi_info.m_cgi_type = EClientCgiType_c2sReq_s2cResp;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_GiverUpdateSession;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_GiverUpdateSession;
		return s_cgi_info;
	}

	virtual const ClientCgiInfo & getCgiInfo() const { return s_getCgiInfo(); }

	bool initSendPack(__ClientSendPackBuilder& ctx, const std::string& wx_re_id, const uint32_t giver_uin, const std::string& re_qr_code)
	{
		m_c2s_req_body = std::string() + "re_giver_update_session: "
			+ "wx_re_id=" + wx_re_id + ","
			+ "giver_uin=" + StringUtil::toString(giver_uin) + ","
			+ "re_qr_code=" + re_qr_code + ",";

		__initSendPack(ctx);
		return true;
	}


private:
	virtual void onSetRecvPackEnd()
	{
		RecvPack* recv_pack = getRecvPack();
		StPacker::Pack* st_pack = (StPacker::Pack*)recv_pack->m_recv_ext;
		m_s2c_resp_body = (const char*)st_pack->m_body.getData();
		m_s2c_resp_err_code = StringUtil::parseUint(StringUtil::fetchMiddle(m_s2c_resp_body, "err_code=", ","));
	}

	static ClientCgiInfo s_cgi_info;
};



class __ClientCgi_RedEnvelope_giverReportScanResult : public __ClientCgi_RedEnvelope_base
{
public:
	static const ClientCgiInfo & s_getCgiInfo()
	{
		s_cgi_info.m_cgi_type = EClientCgiType_c2sReq_s2cResp;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_GiverReportScanResult;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_GiverReportScanResult;
		return s_cgi_info;
	}

	virtual const ClientCgiInfo & getCgiInfo() const { return s_getCgiInfo(); }

	bool initSendPack(__ClientSendPackBuilder& ctx, const std::string& wx_re_id, uint32_t giver_uin, const std::vector<uint32_t>& scan_uins)
	{
		m_c2s_req_body = std::string() + "re_giver_report_scan: "
			+ "wx_re_id=" + wx_re_id + ","
			+ "giver_uin=" + StringUtil::toString(giver_uin) + ","
			+ "scan_uin_count=" + StringUtil::toString(scan_uins.size()) + ",";
		for (size_t i = 0; i < scan_uins.size(); ++i)
		{
			m_c2s_req_body = m_c2s_req_body + "scan_uin" + StringUtil::toString(i + 1) + "=" + StringUtil::toString(scan_uins[i]) + ",";
		}


		__initSendPack(ctx);
		return true;
	}
	

private:
	virtual void onSetRecvPackEnd()
	{
		RecvPack* recv_pack = getRecvPack();
		StPacker::Pack* st_pack = (StPacker::Pack*)recv_pack->m_recv_ext;
		m_s2c_resp_body = (const char*)st_pack->m_body.getData();
		m_s2c_resp_err_code = StringUtil::parseUint(StringUtil::fetchMiddle(m_s2c_resp_body, "err_code=", ","));
	}

	static ClientCgiInfo s_cgi_info;
};














class __ClientCgi_RedEnvelope_receiverCreateSession : public __ClientCgi_RedEnvelope_base
{
public:
	static const ClientCgiInfo & s_getCgiInfo()
	{
		s_cgi_info.m_cgi_type = EClientCgiType_c2sReq_s2cResp;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_ReceiverCreateSession;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_ReceiverCreateSession;
		return s_cgi_info;
	}

	virtual const ClientCgiInfo & getCgiInfo() const { return s_getCgiInfo(); }

	bool initSendPack(__ClientSendPackBuilder& ctx, uint32_t receiver_uin)
	{
		m_c2s_req_body = std::string() + "re_receiver_create_session: "
			+ "receiver_uin=" + StringUtil::toString(receiver_uin) + ",";

		__initSendPack(ctx);
		return true;
	}


private:
	virtual void onSetRecvPackEnd()
	{
		RecvPack* recv_pack = getRecvPack();
		StPacker::Pack* st_pack = (StPacker::Pack*)recv_pack->m_recv_ext;
		m_s2c_resp_body = (const char*)st_pack->m_body.getData();
		m_s2c_resp_err_code = StringUtil::parseUint(StringUtil::fetchMiddle(m_s2c_resp_body, "err_code=", ","));
	}

	static ClientCgiInfo s_cgi_info;
};



class __ClientCgi_RedEnvelope_receiverDeleteSession : public __ClientCgi_RedEnvelope_base
{
public:
	static const ClientCgiInfo & s_getCgiInfo()
	{
		s_cgi_info.m_cgi_type = EClientCgiType_c2sReq_s2cResp;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_ReceiverDeleteSession;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_ReceiverDeleteSession;
		return s_cgi_info;
	}

	virtual const ClientCgiInfo & getCgiInfo() const { return s_getCgiInfo(); }

	bool initSendPack(__ClientSendPackBuilder& ctx, uint32_t receiver_uin)
	{
		m_c2s_req_body = std::string() + "re_receiver_delete_session: "
			+ "receiver_uin=" + StringUtil::toString(receiver_uin) + ",";

		__initSendPack(ctx);
		return true;
	}



private:
	virtual void onSetRecvPackEnd()
	{
		RecvPack* recv_pack = getRecvPack();
		StPacker::Pack* st_pack = (StPacker::Pack*)recv_pack->m_recv_ext;
		m_s2c_resp_body = (const char*)st_pack->m_body.getData();
		m_s2c_resp_err_code = StringUtil::parseUint(StringUtil::fetchMiddle(m_s2c_resp_body, "err_code=", ","));
	}

	static ClientCgiInfo s_cgi_info;
};



class __ClientCgi_RedEnvelope_receiverUpdateSession : public __ClientCgi_RedEnvelope_base
{
public:
	static const ClientCgiInfo & s_getCgiInfo()
	{
		s_cgi_info.m_cgi_type = EClientCgiType_c2sReq_s2cResp;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_ReceiverUpdateSession;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_ReceiverUpdateSession;
		return s_cgi_info;
	}

	virtual const ClientCgiInfo & getCgiInfo() const { return s_getCgiInfo(); }

	bool initSendPack(__ClientSendPackBuilder& ctx, const std::string& wx_re_id, uint32_t giver_uin, uint32_t receiver_uin, bool is_opened)
	{
		m_c2s_req_body = std::string() + "re_recevier_update_session: "
			+ "wx_re_id=" + wx_re_id + ","
			+ "giver_uin=" + StringUtil::toString(giver_uin) + ","
			+ "receiver_uin=" + StringUtil::toString(receiver_uin) + ","
			+ "is_envelope_opened=" + StringUtil::toString(is_opened) + ",";

		__initSendPack(ctx);
		return true;
	}



private:
	virtual void onSetRecvPackEnd()
	{
		RecvPack* recv_pack = getRecvPack();
		StPacker::Pack* st_pack = (StPacker::Pack*)recv_pack->m_recv_ext;
		m_s2c_resp_body = (const char*)st_pack->m_body.getData();
		m_s2c_resp_err_code = StringUtil::parseUint(StringUtil::fetchMiddle(m_s2c_resp_body, "err_code=", ","));
	}

	static ClientCgiInfo s_cgi_info;
};



class __ClientCgi_RedEnvelope_receiverReportScanResult : public __ClientCgi_RedEnvelope_base
{
public:
	static const ClientCgiInfo & s_getCgiInfo()
	{
		s_cgi_info.m_cgi_type = EClientCgiType_c2sReq_s2cResp;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_ReceiverReportScanResult;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_ReceiverReportScanResult;
		return s_cgi_info;
	}

	virtual const ClientCgiInfo & getCgiInfo() const { return s_getCgiInfo(); }

	bool initSendPack(__ClientSendPackBuilder& ctx, uint32_t receiver_uin, const std::vector<uint32_t>& scan_uins)
	{
		m_c2s_req_body = std::string() + "re_receiver_report_scan: "
			+ "receiver_uin=" + StringUtil::toString(receiver_uin) + ","
			+ "scan_uin_count=" + StringUtil::toString(scan_uins.size()) + ",";
		for (size_t i = 0; i < scan_uins.size(); ++i)
		{
			m_c2s_req_body = m_c2s_req_body + "scan_uin" + StringUtil::toString(i + 1) + "=" + StringUtil::toString(scan_uins[i]) + ",";
		}

		__initSendPack(ctx);
		return true;
	}

private:
	virtual void onSetRecvPackEnd()
	{
		RecvPack* recv_pack = getRecvPack();
		StPacker::Pack* st_pack = (StPacker::Pack*)recv_pack->m_recv_ext;
		m_s2c_resp_body = (const char*)st_pack->m_body.getData();
		m_s2c_resp_err_code = StringUtil::parseUint(StringUtil::fetchMiddle(m_s2c_resp_body, "err_code=", ","));
	}

	static ClientCgiInfo s_cgi_info;
};






class __ClientCgi_RedEnvelope_reportStatisticZishi : public __ClientCgi_RedEnvelope_base
{
public:
	static const ClientCgiInfo & s_getCgiInfo()
	{
		s_cgi_info.m_cgi_type = EClientCgiType_c2sReq_s2cResp;
		s_cgi_info.m_send_cmd_type = __ECgiCmdType_c2sReq_RedEnvelope_ReportStatisticZishi;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_s2cResp_RedEnvelope_ReportStatisticZishi;
		return s_cgi_info;
	}

	virtual const ClientCgiInfo & getCgiInfo() const { return s_getCgiInfo(); }

	bool initSendPack(__ClientSendPackBuilder& ctx, uint32_t uin, bool is_sender, int16_t angle, int16_t ax, int16_t ay, int16_t az)
	{
		m_c2s_req_body = std::string() + "re_report_staticstic_zishi: "
			+ "uin=" + StringUtil::toString(uin) + ","
			+ "is_sender=" + StringUtil::toString(is_sender) + ","
			+ "angle=" + StringUtil::toString(angle) + ","
			+ "ax=" + StringUtil::toString(ax) + ","
			+ "ay=" + StringUtil::toString(ay) + ","
			+ "az=" + StringUtil::toString(az) + ",";

		__initSendPack(ctx);
		return true;
	}

private:
	virtual void onSetRecvPackEnd()
	{
		RecvPack* recv_pack = getRecvPack();
		StPacker::Pack* st_pack = (StPacker::Pack*)recv_pack->m_recv_ext;
		m_s2c_resp_body = (const char*)st_pack->m_body.getData();
		m_s2c_resp_err_code = StringUtil::parseUint(StringUtil::fetchMiddle(m_s2c_resp_body, "err_code=", ","));
	}

	static ClientCgiInfo s_cgi_info;
};












class __ClientCgi_RedEnvelope_matchResult : public ClientCgi
{
public:
	static const ClientCgiInfo & s_getCgiInfo()
	{
		s_cgi_info.m_cgi_type = EClientCgiType_s2cPush;
		s_cgi_info.m_recv_cmd_type = __ECgiCmdType_s2cPush_RedEnvelope_MatchResult;
		return s_cgi_info;
	}

	virtual const ClientCgiInfo & getCgiInfo() const { return s_getCgiInfo(); }

	std::string m_s2c_push_body;
	std::string m_s2c_push_wx_re_id;
	uint32_t m_s2c_push_giver_uin;
	std::vector<uint32_t> m_s2c_push_receiver_uins;
	std::string m_s2c_push_re_qr_code;


private:
	virtual void onSetRecvPackEnd()
	{
		RecvPack* recv_pack = getRecvPack();
		StPacker::Pack* st_pack = (StPacker::Pack*)recv_pack->m_recv_ext;
		m_s2c_push_body = (const char*)st_pack->m_body.getData();
		m_s2c_push_wx_re_id = StringUtil::fetchMiddle(m_s2c_push_body, "wx_re_id=", ",");
		m_s2c_push_giver_uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_s2c_push_body, "giver_uin=", ","));
		m_s2c_push_re_qr_code = StringUtil::fetchMiddle(m_s2c_push_body, "re_qr_code=", ",");

		uint32_t receiver_uin_count = StringUtil::parseUint(StringUtil::fetchMiddle(m_s2c_push_body, "receiver_uin_count=", ","));
		for (size_t i = 0; i < receiver_uin_count; ++i)
		{
			std::string uin_tag = std::string() + "receiver_uin=" + StringUtil::toString(i + 1);
			uint32_t uin = StringUtil::parseUint(StringUtil::fetchMiddle(m_s2c_push_body, uin_tag, ","));
			m_s2c_push_receiver_uins.push_back(uin);
		}
	
	}

	static ClientCgiInfo s_cgi_info;
};


std::map<uint32_t, ClientCgiInfo> __getCgiInfoMap();



#endif

