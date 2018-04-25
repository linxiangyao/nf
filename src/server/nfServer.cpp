#include "nfComm.h"
#include "nfServerCgi.h"
#include "nfServerDb.h"




class __NfServerNetworkLogic : public ServerNetworkMsgLooperHandler
{
public:
	__NfServerNetworkLogic()
	{
		m_packer = nullptr;
	}

	~__NfServerNetworkLogic() 
	{
		delete m_packer;
		delete_and_erase_collection_elements(&m_receiver_sessions);
		delete_and_erase_collection_elements(&m_giver_sessions);
	}

	bool init(MessageLooper* looper)
	{
		m_cgi_infos.push_back(__ServerCgi_RedEnvelope_giverCreateSession::s_getServerCgiInfo());
		m_cgi_infos.push_back(__ServerCgi_RedEnvelope_giverDeleteSession::s_getServerCgiInfo());
		m_cgi_infos.push_back(__ServerCgi_RedEnvelope_giverUpdateSession::s_getServerCgiInfo());
		m_cgi_infos.push_back(__ServerCgi_RedEnvelope_giverReportScanResult::s_getServerCgiInfo());
		m_cgi_infos.push_back(__ServerCgi_RedEnvelope_receiverCreateSession::s_getServerCgiInfo());
		m_cgi_infos.push_back(__ServerCgi_RedEnvelope_receiverDeleteSession::s_getServerCgiInfo());
		m_cgi_infos.push_back(__ServerCgi_RedEnvelope_receiverUpdateSession::s_getServerCgiInfo());
		m_cgi_infos.push_back(__ServerCgi_RedEnvelope_receiverReportScanResult::s_getServerCgiInfo());
		m_cgi_infos.push_back(__ServerCgi_RedEnvelope_reportStatisticZishi::s_getServerCgiInfo());
		m_cgi_infos.push_back(__ServerCgi_redEnvelopeMatchResult::s_getServerCgiInfo());
		m_cgi_infos.push_back(__ServerCgi_AddFriend_reportUserInfo::s_getServerCgiInfo());
		m_cgi_infos.push_back(__ServerCgi_AddFriend_queryUserInfo::s_getServerCgiInfo());
		m_cgi_infos.push_back(__ServerCgi_AddFriend_matchResult::s_getServerCgiInfo());
		

		m_packer = new StPacker();

		return ServerNetworkMsgLooperHandler::init(looper, "0.0.0.0", 12306, m_cgi_infos, m_packer);
	}




private:
#define __ReSessionTimeOutSecond (1 * 60)
#define __ScanMatchTimeMs (60 * 60 * 1000)

	class __ScanRecord
	{
	public:
		__ScanRecord() { m_uin = 0; m_time = 0; }
		__ScanRecord(uint32_t uin, uint64_t time) { m_uin = uin; m_time = time; }
		uint32_t m_uin;
		uint64_t m_time;
	};

	class __ScanTable
	{
	public:
		void addScanRecord(const __ScanRecord& r)
		{
			if (hasScanUin(r.m_uin))
				return;
			m_scan_records.push_back(r);
		}

		void refreshScanRecord(uint32_t uin)
		{
			int index = getScanRecordIndexByUin(uin);
			if (index < 0)
				return;
			m_scan_records[index].m_time = TimeUtil::getMsTime();
		}

		void checkAndDeleteTimeOutRecord()
		{
			uint64_t cur_time = TimeUtil::getMsTime();
			for (int i = 0; i < m_scan_records.size(); ++i)
			{
				if (cur_time - m_scan_records[i].m_time >= __ScanMatchTimeMs)
				{
					slog_d("will delete record? uin=%0, time=%1, cur_time=%2", m_scan_records[i].m_uin, m_scan_records[i].m_time, cur_time);
					//m_scan_records.erase(m_scan_records.begin() + i);
					//--i;
				}
			}
		}

		bool hasScanUin(uint32_t uin)
		{
			return getScanRecordIndexByUin(uin) >= 0;
		}

		int getScanRecordIndexByUin(uint32_t uin)
		{
			for (int i = 0; i < m_scan_records.size(); ++i)
			{
				if (m_scan_records[i].m_uin == uin)
					return i;
			}
			return -1;
		}

		void clear()
		{
			m_scan_records.clear();
		}

		std::string toString() const
		{
			std::string str = "(";
			for (size_t i = 0; i < m_scan_records.size(); ++i)
			{
				if (i > 0)
					str += "|";
				str = str + StringUtil::toString(m_scan_records[i].m_uin);
			}

			str += ")";
			return str;
		}



		uint32_t m_uin;
		std::vector<__ScanRecord> m_scan_records;
	};

	class __RedEnvelopeGiverSession
	{
	public:
		__RedEnvelopeGiverSession() { m_sid = 0; m_session_create_time_ms = 0; m_session_live_start_time_ms = 0; m_giver_uin = 0; }

		~__RedEnvelopeGiverSession() { slog_i("delete giver_session, giver_uin=%0", m_giver_uin); }

		void refreshSession(socket_id_t sid, session_id_t ssid)
		{
			m_sid = sid;
			m_ssid = ssid;
			m_session_live_start_time_ms = TimeUtil::getMsTime();
		}

		bool isSessionTimeout()
		{
			return TimeUtil::getMsTime() - m_session_live_start_time_ms > __ReSessionTimeOutSecond * 1000;
		}

		std::string toString() const
		{
			std::string str = "(";

			str = str + "giver_uin=" + StringUtil::toString(m_giver_uin)
				+ ", sid=" + StringUtil::toString(m_sid)
				+ ", ssid=" + m_ssid.toString()
				+ ", scan_table=" + m_scan_table.toString();

			str += ")";
			return str;
		}
	

		socket_id_t m_sid;
		session_id_t m_ssid;
		uint64_t m_session_create_time_ms;
		uint64_t m_session_live_start_time_ms;
		uint32_t m_giver_uin;
		__ScanTable m_scan_table;
	};

	class __RedEnvelopeReceiverSession
	{
	public:
		__RedEnvelopeReceiverSession() { m_sid = 0; m_session_create_time_ms = 0; m_session_create_time_ms = 0; m_receiver_uin = 0; }
		~__RedEnvelopeReceiverSession() { slog_i("delete receiver_session, receiver_uin=%0", m_receiver_uin); }

		void refreshSession(socket_id_t sid, session_id_t ssid)
		{
			m_sid = sid;
			m_ssid = ssid;
			m_session_live_start_time_ms = TimeUtil::getMsTime();
		}

		bool isSessionTimeout()
		{
			return TimeUtil::getMsTime() - m_session_live_start_time_ms > __ReSessionTimeOutSecond * 1000;
		}

		std::string toString() const
		{
			std::string str = "(";

			str = str + "receiver_uin=" + StringUtil::toString(m_receiver_uin)
				+ ", sid=" + StringUtil::toString(m_sid)
				+ ", ssid=" + m_ssid.toString()
				+ ", scan_table=" + m_scan_table.toString();

			str += ")";
			return str;
		}

		socket_id_t m_sid;
		session_id_t m_ssid;
		uint64_t m_session_create_time_ms;
		uint64_t m_session_live_start_time_ms;
		uint32_t m_receiver_uin;
		__ScanTable m_scan_table;
	};

	class __AddFriendSession
	{
	public:
		__AddFriendSession()
		{
			m_sid = 0;
			m_session_create_time_ms = 0;
			m_session_live_start_time_ms = 0;
			m_uin = 0;
			m_scaned_uin = 0;
		}

		~__AddFriendSession()
		{
			slog_i("delete add_friend_session, uin=%0", m_uin);
		}

		void refreshSession(socket_id_t sid, session_id_t ssid)
		{
			m_sid = sid;
			m_ssid = ssid;
			m_session_live_start_time_ms = TimeUtil::getMsTime();
		}

		bool isSessionTimeout()
		{
			return TimeUtil::getMsTime() - m_session_live_start_time_ms > 60 * 1000;
		}

		socket_id_t m_sid;
		session_id_t m_ssid;
		uint64_t m_session_create_time_ms;
		uint64_t m_session_live_start_time_ms;
		uint32_t m_uin;
		std::string m_user_name;
		uint32_t m_scaned_uin;
	};

	class __AddFriendCtx
	{
	public:
		__AddFriendCtx()
		{
		}

		~__AddFriendCtx()
		{
			delete_and_erase_collection_elements(&m_sessions);
		}

		void checkSessionTimeout()
		{
			std::vector<uint32_t> add_friend_timeout_uins;
			for (auto it = m_sessions.begin(); it != m_sessions.end(); ++it)
			{
				if (it->second->isSessionTimeout())
				{
					slog_i("add_friend_session timeout, uin=%0", it->first);
					add_friend_timeout_uins.push_back(it->first);
				}
			}
			delete_and_erase_map_elements_by_keys(&m_sessions, add_friend_timeout_uins);
		}

		__AddFriendSession* createOrRefreshSession(uint32_t uin, socket_id_t sid, session_id_t ssid)
		{
			__AddFriendSession* session = getSessionByUin(uin);
			if (session == nullptr)
				session = createSession(uin, sid, ssid);
			else
				session->refreshSession(sid, ssid);
			return session;
		}

		__AddFriendSession* createSession(uint32_t uin, socket_id_t sid, session_id_t ssid)
		{
			__AddFriendSession* af_session = new __AddFriendSession();
			af_session->m_sid = sid;
			af_session->m_ssid = ssid;
			af_session->m_session_create_time_ms = TimeUtil::getMsTime();
			af_session->m_session_live_start_time_ms = af_session->m_session_create_time_ms;
			af_session->m_uin = uin;
			m_sessions[af_session->m_uin] = af_session;
			slog_i("create add_friend_session, uin=%0", uin);
			return af_session;
		}

		__AddFriendSession* getSessionByUin(uint32_t uin)
		{
			auto it = m_sessions.find(uin);
			if (it == m_sessions.end())
				return nullptr;
			else
				return it->second;
		}

		std::map <uint32_t, __AddFriendSession*> m_sessions;
	};





	virtual void onMsg_startCmd_end(Message * msg) override
	{
		//FileUtil::deleteFile("server.db");
		m_db.open("server.db");
		m_db.getAllReInfos(&m_re_infos);
		m_statistic_db.open("statistic.db");
		__dumpAllReInfos();

		m_check_session_timer = getLooper()->createTimer(NULL);
		getLooper()->startTimer(m_check_session_timer, 1, 1 * 1000);
	}

	virtual void onMessageTimerTick(uint64_t timer_id, void* user_data) override
	{
		ServerNetworkMsgLooperHandler::onMessageTimerTick(timer_id, user_data);

		if (timer_id != m_check_session_timer)
			return;

		// giver
		{
			std::vector<uint32_t> timeout_giver_uins;
			for (auto it = m_giver_sessions.begin(); it != m_giver_sessions.end(); ++it)
			{
				it->second->m_scan_table.checkAndDeleteTimeOutRecord();
				if (it->second->isSessionTimeout())
				{
					slog_i("giver_session timeout, uin=%0", it->first);
					timeout_giver_uins.push_back(it->first);
				}
			}
			delete_and_erase_map_elements_by_keys(&m_giver_sessions, timeout_giver_uins);
		}

		// receiver
		{
			std::vector<uint32_t> timeout_receiver_uins;
			for (auto it = m_receiver_sessions.begin(); it != m_receiver_sessions.end(); ++it)
			{
				it->second->m_scan_table.checkAndDeleteTimeOutRecord();
				if (it->second->isSessionTimeout())
				{
					slog_i("receiver_session timeout, uin=%0", it->first);
					timeout_receiver_uins.push_back(it->first);
				}
			}
			delete_and_erase_map_elements_by_keys(&m_receiver_sessions, timeout_receiver_uins);
		}

		// add friend
		{
			m_add_friend_ctx.checkSessionTimeout();
		}
	}

	virtual void onServerCgiMgr_sessionCreated(socket_id_t sid, session_id_t ssid) override
	{
		ServerNetworkMsgLooperHandler::onServerCgiMgr_sessionCreated(sid, ssid);
		slog_d("create session, sid=%0, ssid=%1", sid, ssid.toString().c_str());
	}

	virtual void onServerCgiMgr_sessionClosed(socket_id_t sid, session_id_t ssid) override
	{
		ServerNetworkMsgLooperHandler::onServerCgiMgr_sessionClosed(sid, ssid);
		slog_d("close session, sid=%0, ssid=%1", sid, ssid.toString().c_str());
	}

	virtual void onServerCgiMgr_recvC2sReqPack(std::unique_ptr<ServerCgi::RecvPack>* recv_pack) override
	{
		ServerNetworkMsgLooperHandler::onServerCgiMgr_recvC2sReqPack(recv_pack);

		ServerCgi::RecvPack* p = recv_pack->get();
		__ServerCgiCtx cgi_ctx;
		cgi_ctx.m_callback = this;
		cgi_ctx.m_network = getNetwork();
		cgi_ctx.m_packer = m_packer;
		cgi_ctx.m_uin = 0;
		cgi_ctx.m_sid = p->m_sid;
		cgi_ctx.m_ssid = p->m_ssid;

		// re --
		// giver
		if (p->m_recv_cmd_type == __ECgiCmdType_c2sReq_RedEnvelope_GiverCreateSession)
		{
			__onServerCgiMgr_recvC2sReqPack_RedEnvelope_giverCreateSession(recv_pack, cgi_ctx);
		}
		else if (p->m_recv_cmd_type == __ECgiCmdType_c2sReq_RedEnvelope_GiverDeleteSession)
		{
			__onServerCgiMgr_recvC2sReqPack_RedEnvelope_giverDeleteSession(recv_pack, cgi_ctx);
		}
		else if (p->m_recv_cmd_type == __ECgiCmdType_c2sReq_RedEnvelope_GiverUpdateSession)
		{
			__onServerCgiMgr_recvC2sReqPack_RedEnvelope_giverUpdateSession(recv_pack, cgi_ctx);
		}
		else if (p->m_recv_cmd_type == __ECgiCmdType_c2sReq_RedEnvelope_GiverReportScanResult)
		{
			__onServerCgiMgr_recvC2sReqPack_RedEnvelope_giverReportScanResult(recv_pack, cgi_ctx);
		}
		// receiver
		else if (p->m_recv_cmd_type == __ECgiCmdType_c2sReq_RedEnvelope_ReceiverCreateSession)
		{
			__onServerCgiMgr_recvC2sReqPack_RedEnvelope_receiverCreateSession(recv_pack, cgi_ctx);
		}
		else if (p->m_recv_cmd_type == __ECgiCmdType_c2sReq_RedEnvelope_ReceiverDeleteSession)
		{
			__onServerCgiMgr_recvC2sReqPack_RedEnvelope_receiverDeleteSession(recv_pack, cgi_ctx);
		}
		else if (p->m_recv_cmd_type == __ECgiCmdType_c2sReq_RedEnvelope_ReceiverUpdateSession)
		{
			__onServerCgiMgr_recvC2sReqPack_RedEnvelope_receiverUpdateSession(recv_pack, cgi_ctx);
		}
		else if (p->m_recv_cmd_type == __ECgiCmdType_c2sReq_RedEnvelope_ReceiverReportScanResult)
		{
			__onServerCgiMgr_recvC2sReqPack_RedEnvelope_receiverReportScanResult(recv_pack, cgi_ctx);
		}
		// statistic
		else if (p->m_recv_cmd_type == __ECgiCmdType_c2sReq_RedEnvelope_ReportStatisticZishi)
		{
			__onServerCgiMgr_recvC2sReqPack_RedEnvelope_reportStatisticZishi(recv_pack, cgi_ctx);
		}

		// add friend --
		else if (p->m_recv_cmd_type == __ECgiCmdType_c2sReq_AddFriend_ReportUserInfo)
		{
			__onServerCgiMgr_recvC2sReqPack_AddFriend_reportUserInfo(recv_pack, cgi_ctx);
		}
		else if (p->m_recv_cmd_type == __ECgiCmdType_c2sReq_AddFriend_QueryUserInfo)
		{
			__onServerCgiMgr_recvC2sReqPack_AddFriend_queryUserInfo(recv_pack, cgi_ctx);
		}
		else
		{
			slog_e("recv unkonw c2s_req pack\n");
		}
	}
	
	virtual void onServerCgi_cgiDone(ServerCgi* cgi) override
	{
		ServerNetworkMsgLooperHandler::onServerCgi_cgiDone(cgi);

		if (cgi->getSendPack() != NULL && __getCgiInfoIndexBySendCmdType(cgi->getSendPack()->m_send_cmd_type) >= 0)
		{
			std::string cgi_name = __sendCmdTypeToString(cgi->getSendPack()->m_send_cmd_type) + "_cgi";
			if (cgi->getIsCgiSuccess())
				slog_d("%0 ok", cgi_name);
			else
				slog_e("%0 fail", cgi_name);
		}
		else
		{
			slog_e("unkonw cgi done");
		}
		delete cgi;
	}

	void __onServerCgiMgr_recvC2sReqPack_RedEnvelope_giverCreateSession(std::unique_ptr<ServerCgi::RecvPack>* recv_pack, const __ServerCgiCtx& cgi_ctx)
	{
		__ServerCgi_RedEnvelope_giverCreateSession* cgi = new __ServerCgi_RedEnvelope_giverCreateSession();
		cgi->setRecvPack(recv_pack->release());
		slog_i("req = seq:%0, %1", cgi->getRecvPack()->m_recv_seq, cgi->m_c2sReq_body.c_str());

		uint32_t err_code = 0;
		{
			__dumpAll();

			// 创建红包
			NfServerDb::Record_ReInfo* re_info = __getReInfoByGiverUin(cgi->m_c2sReq_giver_uin);
			if (re_info != nullptr)
			{
				if (re_info->m_wx_re_id != cgi->m_c2sReq_wx_re_id) // 红包id都不一样了，相当于删掉旧红包并新建
				{
					//slog_d("re id changed");
					__deleteReInfoAndSessionAndPushByGiverUin(cgi->m_c2sReq_giver_uin);
					//slog_d("delete re and session ok");
					re_info = nullptr;
				}
			}

			if (re_info == nullptr)
			{
				// 红包不存在，新建
				m_re_infos.push_back(NfServerDb::Record_ReInfo());
				re_info = &m_re_infos[m_re_infos.size() - 1];
				re_info->m_giver_uin = cgi->m_c2sReq_giver_uin;
				re_info->m_re_count = cgi->m_c2sReq_re_count;
				re_info->m_re_qr_code = cgi->m_c2sReq_re_qr_code;
				re_info->m_re_qr_code_create_time = TimeUtil::getMsTime();
				re_info->m_session_create_time_ms = TimeUtil::getMsTime();
				re_info->m_wx_re_id = cgi->m_c2sReq_wx_re_id;
				slog_d("create re=%0", re_info->toString());

				if (!m_db.addReInfo(*re_info))
				{
					slog_e("fail to m_db.addReInfo");
					err_code = 1234;
				}
			}
			else
			{
				// 红包无变化，忽略
			}

			// 创建或更新giver_session
			__RedEnvelopeGiverSession* giver_session = __createOrRefreshGiverSession(cgi->m_c2sReq_giver_uin, cgi->getRecvPack()->m_sid, cgi->getRecvPack()->m_ssid);
			giver_session->m_scan_table.clear();
			slog_d("create or update giver_sesion ok, giver_session=%0", giver_session->toString());

			__dumpAll();
		}

		cgi->initSendPack(cgi_ctx, err_code);
		slog_i("resp= seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_s2cResp_body.c_str());
		if (!getCgiMgr()->startCgi(cgi))
		{
			slog_e("fail to start cgi");
		}
	}

	void __onServerCgiMgr_recvC2sReqPack_RedEnvelope_giverDeleteSession(std::unique_ptr<ServerCgi::RecvPack>* recv_pack, const __ServerCgiCtx& cgi_ctx)
	{
		__ServerCgi_RedEnvelope_giverCreateSession* cgi = new __ServerCgi_RedEnvelope_giverCreateSession();
		cgi->setRecvPack(recv_pack->release());
		slog_i("req = seq:%0, %1", cgi->getRecvPack()->m_recv_seq, cgi->m_c2sReq_body.c_str());

		uint32_t err_code = 0;
		{
			uint32_t giver_uin = cgi->m_c2sReq_giver_uin;
			__deleteReInfoAndSessionAndPushByGiverUin(giver_uin);
			__dumpAll();
		}

		cgi->initSendPack(cgi_ctx, err_code);
		slog_i("resp= seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_s2cResp_body.c_str());
		if (!getCgiMgr()->startCgi(cgi))
		{
			slog_e("fail to start cgi");
		}
	}

	void __onServerCgiMgr_recvC2sReqPack_RedEnvelope_giverUpdateSession(std::unique_ptr<ServerCgi::RecvPack>* recv_pack, const __ServerCgiCtx& cgi_ctx)
	{
		__ServerCgi_RedEnvelope_giverUpdateSession* cgi = new __ServerCgi_RedEnvelope_giverUpdateSession();
		cgi->setRecvPack(recv_pack->release());
		slog_i("req = seq:%0, %1", cgi->getRecvPack()->m_recv_seq, cgi->m_c2sReq_body.c_str());


		uint32_t giver_uin = cgi->m_c2sReq_giver_uin;
		uint32_t err_code = 0;
		{
			// 更新红包
			NfServerDb::Record_ReInfo* info = __getReInfoByGiverUin(giver_uin);
			if (info == nullptr)
			{
				err_code = 1;
			}
			else
			{
				info->m_re_qr_code = cgi->m_c2sReq_re_qr_code;
				info->m_re_qr_code_create_time = TimeUtil::getMsTime();
				m_db.updateReInfo(*info);

				// 新建或更新session
				__createOrRefreshGiverSession(cgi->m_c2sReq_giver_uin, cgi_ctx.m_sid, cgi_ctx.m_ssid);
			}

			__dumpAll();
		}


		cgi->initSendPack(cgi_ctx, err_code);
		slog_i("resp= seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_s2cResp_body.c_str());
		if (!getCgiMgr()->startCgi(cgi))
		{
			slog_e("fail to start cgi");
		}


		if (err_code == 0)
		{
			__pushMatchReResultToAllMatchedReceivers(giver_uin);
		}
	}

	void __onServerCgiMgr_recvC2sReqPack_RedEnvelope_giverReportScanResult(std::unique_ptr<ServerCgi::RecvPack>* recv_pack, const __ServerCgiCtx& cgi_ctx)
	{
		__ServerCgi_RedEnvelope_giverReportScanResult* cgi = new __ServerCgi_RedEnvelope_giverReportScanResult();
		cgi->setRecvPack(recv_pack->release());
		slog_i("req = seq:%0, %1", cgi->getRecvPack()->m_recv_seq, cgi->m_c2sReq_body.c_str());


		uint32_t err_code = 0;
		std::set<uint32_t> new_scaned_receiver_uins;
		{
			// 更新扫描列表
			__RedEnvelopeGiverSession* giver_session = __getGiverSessionByUin(cgi->m_c2sReq_giver_uin);
			if (giver_session == nullptr)
			{
				err_code = 92883;
			}
			else
			{
				giver_session->refreshSession(cgi_ctx.m_sid, cgi_ctx.m_ssid);
				uint64_t cur_ms = TimeUtil::getMsTime();

				for (size_t i = 0; i < cgi->m_c2sReq_scan_uins.size(); ++i)
				{
					uint32_t receiver_uin = cgi->m_c2sReq_scan_uins[i];
					if (!giver_session->m_scan_table.hasScanUin(receiver_uin))
					{
						new_scaned_receiver_uins.insert(receiver_uin);
						giver_session->m_scan_table.addScanRecord(__ScanRecord(receiver_uin, cur_ms));
					}
				}


				// dump
				{
					slog_d("giver_session=%0", giver_session->toString());
					NfServerDb::Record_ReInfo* re_info = __getReInfoByGiverUin(giver_session->m_giver_uin);
					if (re_info != nullptr)
					{
						slog_d("giver_re_info=%0", re_info->toString());
					}

					for (size_t i = 0; i < giver_session->m_scan_table.m_scan_records.size(); ++i)
					{
						uint32_t receiver_uin = giver_session->m_scan_table.m_scan_records[i].m_uin;
						__RedEnvelopeReceiverSession* rs = __getReceiverSessionByUin(receiver_uin);
						if (rs == nullptr)
							continue;
						slog_d("scaned_receiver_session=%0", rs->toString());
					}
				}
			}
		}


		cgi->initSendPack(cgi_ctx, err_code);
		slog_i("resp= seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_s2cResp_body.c_str());
		if (!getCgiMgr()->startCgi(cgi))
		{
			slog_e("fail to start cgi");
		}


		if (err_code == 0 && new_scaned_receiver_uins.size() > 0)
		{
			__matchReAndPush();
		}
	}

	void __onServerCgiMgr_recvC2sReqPack_RedEnvelope_receiverCreateSession(std::unique_ptr<ServerCgi::RecvPack>* recv_pack, const __ServerCgiCtx& cgi_ctx)
	{
		__ServerCgi_RedEnvelope_receiverCreateSession* cgi = new __ServerCgi_RedEnvelope_receiverCreateSession();
		cgi->setRecvPack(recv_pack->release());
		slog_i("req = seq:%0, %1", cgi->getRecvPack()->m_recv_seq, cgi->m_c2sReq_body.c_str());

		uint32_t err_code = 0;
		{
			__dumpAll();

			// 新建或更新session
			uint32_t receiver_uin = cgi->m_c2sReq_receiver_uin;
			__RedEnvelopeReceiverSession* receiver_session =  __createOrRefreshReceiverSession(receiver_uin, cgi_ctx.m_sid, cgi_ctx.m_ssid);
			receiver_session->m_scan_table.clear();
			slog_d("create or update receiver session ok, %0", receiver_session->toString());

			for (size_t i = 0; i < m_re_infos.size(); ++i)
			{
				uint32_t giver_uin = m_re_infos[i].m_giver_uin;
				if (__isReceiverMatchedWithGiver(receiver_uin, giver_uin))
				{
					__pushMatchReResultToReceiver(giver_uin, receiver_uin);
				}
			}

			__dumpAll();
		}

		cgi->initSendPack(cgi_ctx, err_code);
		slog_i("resp= seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_s2cResp_body.c_str());
		if (!getCgiMgr()->startCgi(cgi))
		{
			slog_e("fail to start cgi");
		}
	}

	void __onServerCgiMgr_recvC2sReqPack_RedEnvelope_receiverDeleteSession(std::unique_ptr<ServerCgi::RecvPack>* recv_pack, const __ServerCgiCtx& cgi_ctx)
	{
		__ServerCgi_RedEnvelope_receiverDeleteSession* cgi = new __ServerCgi_RedEnvelope_receiverDeleteSession();
		cgi->setRecvPack(recv_pack->release());
		slog_i("req = seq:%0, %1", cgi->getRecvPack()->m_recv_seq, cgi->m_c2sReq_body.c_str());

		uint32_t err_code = 0;
		{
			// 删除session
			delete_and_erase_map_element_by_key(&m_receiver_sessions, cgi->m_c2sReq_receiver_uin);
			__dumpAll();
		}

		cgi->initSendPack(cgi_ctx, err_code);
		slog_i("resp= seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_s2cResp_body.c_str());
		if (!getCgiMgr()->startCgi(cgi))
		{
			slog_e("fail to start cgi");
		}
	}

	void __onServerCgiMgr_recvC2sReqPack_RedEnvelope_receiverUpdateSession(std::unique_ptr<ServerCgi::RecvPack>* recv_pack, const __ServerCgiCtx& cgi_ctx)
	{
		__ServerCgi_RedEnvelope_receiverUpdateSession* cgi = new __ServerCgi_RedEnvelope_receiverUpdateSession();
		cgi->setRecvPack(recv_pack->release());
		slog_i("req = seq:%0, %1", cgi->getRecvPack()->m_recv_seq, cgi->m_c2sReq_body.c_str());


		uint32_t err_code = 0;
		{
			uint32_t receiver_uin = cgi->m_c2sReq_receiver_uin;
			uint32_t giver_uin = cgi->m_c2sReq_giver_uin;
			NfServerDb::Record_ReInfo* re = __getReInfoByGiverUin(giver_uin);
			if (re == nullptr || re->m_wx_re_id != cgi->m_c2sReq_wx_re_id || !__isReceiverMatchedWithGiver(receiver_uin, giver_uin))
			{
				slog_w("fail to get info || re->m_wx_re_id != cgi->m_c2sReq_wx_re_id || !__isReceiverMatchedWithGiver, ignore");
			}
			else
			{
				for (size_t i = 0; i < re->m_match_receivers.size(); ++i)
				{
					if (re->m_match_receivers[i].m_receiver_uin == receiver_uin)
					{
						re->m_match_receivers[i].m_has_opened_re = cgi->m_c2sReq_is_envelope_opend;
						break;
					}
				}
				m_db.updateReInfo(*re);

				__dumpAll();
			}
		}


		cgi->initSendPack(cgi_ctx, err_code);
		slog_i("resp= seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_s2cResp_body.c_str());
		if (!getCgiMgr()->startCgi(cgi))
		{
			slog_e("fail to start cgi");
		}
	}

	void __onServerCgiMgr_recvC2sReqPack_RedEnvelope_receiverReportScanResult(std::unique_ptr<ServerCgi::RecvPack>* recv_pack, const __ServerCgiCtx& cgi_ctx)
	{
		__ServerCgi_RedEnvelope_receiverReportScanResult* cgi = new __ServerCgi_RedEnvelope_receiverReportScanResult();
		cgi->setRecvPack(recv_pack->release());
		slog_i("req = seq:%0, %1", cgi->getRecvPack()->m_recv_seq, cgi->m_c2sReq_body.c_str());


		uint32_t err_code = 0;
		std::set<uint32_t> new_scaned_giver_uins;
		{
			// 新建或更新session
			__RedEnvelopeReceiverSession* receiver_session = __createOrRefreshReceiverSession(cgi->m_c2sReq_receiver_uin, cgi_ctx.m_sid, cgi_ctx.m_ssid);

			// 更新扫描列表
			uint64_t cur_ms = TimeUtil::getMsTime();
			for (size_t i = 0; i < cgi->m_c2sReq_scan_uins.size(); ++i)
			{
				uint32_t giver_uin = cgi->m_c2sReq_scan_uins[i];
				if (receiver_session->m_scan_table.hasScanUin(giver_uin))
				{
					receiver_session->m_scan_table.refreshScanRecord(giver_uin);
				}
				else
				{
					new_scaned_giver_uins.insert(giver_uin);
					receiver_session->m_scan_table.addScanRecord(__ScanRecord(giver_uin, cur_ms));
				}
			}


			// dump
			{
				slog_d("receiver_session=%0", receiver_session->toString());
				for (size_t i = 0; i < receiver_session->m_scan_table.m_scan_records.size(); ++i)
				{
					uint32_t giver_uin = receiver_session->m_scan_table.m_scan_records[i].m_uin;
					__RedEnvelopeGiverSession* gs = __getGiverSessionByUin(giver_uin);
					if (gs == nullptr)
						continue;
					slog_d("scaned_giver_session=%0", gs->toString());
					NfServerDb::Record_ReInfo* re_info = __getReInfoByGiverUin(giver_uin);
					if (re_info == nullptr)
						continue;
					slog_d("scaned_giver_re_info=%0", re_info->toString());
				}
			}
		}


		cgi->initSendPack(cgi_ctx, err_code);
		slog_i("resp= seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_s2cResp_body.c_str());
		if (!getCgiMgr()->startCgi(cgi))
		{
			slog_e("fail to start cgi");
		}


		if (err_code == 0 && new_scaned_giver_uins.size() > 0)
		{
			__matchReAndPush();
		}
	}

	void __onServerCgiMgr_recvC2sReqPack_RedEnvelope_reportStatisticZishi(std::unique_ptr<ServerCgi::RecvPack>* recv_pack, const __ServerCgiCtx& cgi_ctx)
	{
		__ServerCgi_RedEnvelope_reportStatisticZishi* cgi = new __ServerCgi_RedEnvelope_reportStatisticZishi();
		cgi->setRecvPack(recv_pack->release());
		slog_i("req = seq:%0, %1", cgi->getRecvPack()->m_recv_seq, cgi->m_c2sReq_body.c_str());

		uint32_t err_code = 0;
		{
			NfStatisticDb::Record_zishi record;
			record.m_create_time = TimeUtil::getMsTime() / 1000;
			record.m_uin = cgi->m_c2sReq_uin;
			record.m_is_sender = cgi->m_c2sReq_is_sender;
			record.m_angle = cgi->m_c2sReq_angle;
			record.m_ax = cgi->m_c2sReq_ax;
			record.m_ay = cgi->m_c2sReq_ay;
			record.m_az = cgi->m_c2sReq_az;
			if (!m_statistic_db.addZishi(record))
			{
				slog_e("fail to m_statistic_db.addZishi");
			}
		}

		cgi->initSendPack(cgi_ctx, err_code);
		slog_i("resp= seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_s2cResp_body.c_str());
		if (!getCgiMgr()->startCgi(cgi))
		{
			slog_e("fail to start cgi");
		}
	}

	void __onServerCgiMgr_recvC2sReqPack_AddFriend_reportUserInfo(std::unique_ptr<ServerCgi::RecvPack>* recv_pack, const __ServerCgiCtx& cgi_ctx) // report my user info
	{
		__ServerCgi_AddFriend_reportUserInfo* cgi = new __ServerCgi_AddFriend_reportUserInfo();
		cgi->setRecvPack(recv_pack->release());
		slog_i("req = seq:%0, %1", cgi->getRecvPack()->m_recv_seq, cgi->m_c2sReq_body.c_str());

		uint32_t err_code = 0;
		{
			__AddFriendSession* session = m_add_friend_ctx.createOrRefreshSession(cgi->m_c2sReq_uin, cgi->getRecvPack()->m_sid, cgi->getRecvPack()->m_ssid);
			session->m_user_name = cgi->m_c2sReq_user_name;
		}

		cgi->initSendPack(cgi_ctx, err_code);
		slog_i("resp= seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_s2cResp_body.c_str());
		if (!getCgiMgr()->startCgi(cgi))
		{
			slog_e("fail to start cgi");
		}
	}

	void __onServerCgiMgr_recvC2sReqPack_AddFriend_queryUserInfo(std::unique_ptr<ServerCgi::RecvPack>* recv_pack, const __ServerCgiCtx& cgi_ctx) // report scaned other user
	{
		__ServerCgi_AddFriend_queryUserInfo* cgi = new __ServerCgi_AddFriend_queryUserInfo();
		cgi->setRecvPack(recv_pack->release());
		slog_i("req = seq:%0, %1", cgi->getRecvPack()->m_recv_seq, cgi->m_c2sReq_body.c_str());

		uint32_t err_code = 0;
		uint32_t scaned_uin = 0;
		std::string scaned_user_name;
		{
			__AddFriendSession* my_session = m_add_friend_ctx.getSessionByUin(cgi->m_c2sReq_uin);
			if(my_session != nullptr)
			{
				my_session->refreshSession(cgi->getRecvPack()->m_sid, cgi->getRecvPack()->m_ssid);
				my_session->m_scaned_uin = cgi->m_c2sReq_scaned_uin;
			}

			__AddFriendSession* scaned_session = m_add_friend_ctx.getSessionByUin(cgi->m_c2sReq_scaned_uin);
			if (scaned_session != nullptr)
			{
				scaned_uin = scaned_session->m_uin;
				scaned_user_name = scaned_session->m_user_name;
			}
		}

		cgi->initSendPack(cgi_ctx, err_code, scaned_uin, scaned_user_name);
		slog_i("resp= seq:%0, %1", cgi->getSendPack()->m_send_seq, cgi->m_s2cResp_body.c_str());
		if (!getCgiMgr()->startCgi(cgi))
		{
			slog_e("fail to start cgi");
		}
		
		__matchAndPushAddFriend(cgi->m_c2sReq_uin, cgi->m_c2sReq_scaned_uin);
	}





	void __matchAndPushAddFriend(uint32_t uin, uint32_t scaned_uin)
	{
		__AddFriendSession* s1 = m_add_friend_ctx.getSessionByUin(uin);
		if (s1 == nullptr)
			return;

		__AddFriendSession* s2 = m_add_friend_ctx.getSessionByUin(scaned_uin);
		if (s2 == nullptr)
			return;

		bool has_scaned = false;
		if (s1->m_scaned_uin == s2->m_uin || s2->m_scaned_uin == s1->m_uin)
			has_scaned = true;
		if (!has_scaned)
			return;

		__pushAddFriend(s1, s2);
		__pushAddFriend(s2, s1);
	}

	void __pushAddFriend(__AddFriendSession* my_session, __AddFriendSession* scaned_session)
	{
		__ServerCgiCtx cgi_ctx = __genServerCgiCtx(my_session->m_sid, my_session->m_ssid);
		__ServerCgi_AddFriend_matchResult* cgi = new __ServerCgi_AddFriend_matchResult();
		cgi->initSendPack(cgi_ctx, my_session->m_uin, scaned_session->m_uin, scaned_session->m_user_name);

		slog_i("push= %0", cgi->m_s2c_push_body.c_str());

		if (!getCgiMgr()->startCgi(cgi))
		{
			slog_e("fail to start cgi");
		}
	}

	void __matchReAndPush()
	{
		for (auto it = m_giver_sessions.begin(); it != m_giver_sessions.end(); ++it)
		{
			__matchReAndPush(it->second);
		}
	}

	void __matchReAndPush(__RedEnvelopeGiverSession* giver_session)
	{
		uint32_t giver_uin = giver_session->m_giver_uin;

		NfServerDb::Record_ReInfo* re_info = __getReInfoByGiverUin(giver_uin);
		if (re_info->m_match_receivers.size() >= re_info->m_re_count) // 红包已经领完了
			return;

		std::set<uint32_t> new_matched_receiver_uins;
		for (size_t i = 0; i < giver_session->m_scan_table.m_scan_records.size(); ++i)
		{
			if (new_matched_receiver_uins.size() + re_info->m_match_receivers.size() >= re_info->m_re_count) // 红包已经领完了
				break;

			uint32_t receiver_uin = giver_session->m_scan_table.m_scan_records[i].m_uin;
			if (!__isReceiverScanedGiver(receiver_uin, giver_uin))
				continue;
			if (__isReceiverMatchedWithGiver(receiver_uin, giver_uin))
				continue;
			new_matched_receiver_uins.insert(receiver_uin);
			slog_d("new matched: giver_uin=%0, receiver_uin=%1, wx_re_id=%2", giver_uin, receiver_uin, re_info->m_wx_re_id);
		}

		if (new_matched_receiver_uins.size() > 0)
		{
			{	// update re_info to db
				for (auto it = new_matched_receiver_uins.begin(); it != new_matched_receiver_uins.end(); ++it)
				{
					NfServerDb::MatchReceiver mr;
					mr.m_receiver_uin = *it;
					re_info->m_match_receivers.push_back(mr);
				}
				if (!m_db.updateReInfo(*re_info))
				{
					slog_e("fail to m_db.updateReInfo");
				}
			}

			{	// push match result
				__pushMatchReResultToGiver(giver_uin);
				__pushMatchReResultToAllMatchedReceivers(giver_uin);
			}
		}
	}

	void __pushMatchReResultToGiver(uint32_t giver_uin)
	{
		__RedEnvelopeGiverSession* giver_session = __getGiverSessionByUin(giver_uin);
		if (giver_session == NULL)
			return;
		__ServerCgi_redEnvelopeMatchResult* cgi = __genPushMatchResultCgi(giver_session->m_giver_uin, giver_session->m_sid, giver_session->m_ssid);
		__pushMatchReResultCgi(cgi);
	}

	void __pushMatchReResultToReceiver(uint32_t giver_uin, uint32_t receiver_uin)
	{
		NfServerDb::Record_ReInfo* re = __getReInfoByGiverUin(giver_uin);
		if (re == nullptr)
			return;
		int index = re->getMatchedReceiverIndexByReceiverUin(receiver_uin); // 有没有匹配成功
		if (index < 0)
			return;
		if (re->m_match_receivers[index].m_has_opened_re) // 红包已经打开，不需要再push
			return;
		if (__isReQrCodeTimeOut(*re)) // qrcode超时，不需要再push
			return;

		__RedEnvelopeReceiverSession* recevier_session = __getReceiverSessionByUin(receiver_uin);
		if (recevier_session == nullptr)
			return;

		__ServerCgi_redEnvelopeMatchResult* cgi = __genPushMatchResultCgi(re->m_giver_uin, recevier_session->m_sid, recevier_session->m_ssid);
		__pushMatchReResultCgi(cgi);
	}

	void __pushMatchReResultCgi(__ServerCgi_redEnvelopeMatchResult* cgi)
	{
		slog_i("push= %0", cgi->m_s2c_push_body.c_str());

		if (!getCgiMgr()->startCgi(cgi))
		{
			slog_e("fail to start cgi");
		}
	}

	void __pushMatchReResultToAllMatchedReceivers(uint32_t giver_uin)
	{
		NfServerDb::Record_ReInfo* info = __getReInfoByGiverUin(giver_uin);
		for (size_t i = 0; i < info->m_match_receivers.size(); ++i)
		{
			if (info->m_match_receivers[i].m_has_opened_re)
				continue;
			__pushMatchReResultToReceiver(giver_uin, info->m_match_receivers[i].m_receiver_uin);
		}
	}

	void __deleteReInfoAndSessionAndPushByGiverUin(uint32_t giver_uin)
	{
		// TODO: push delete msg to all matched receiver
		//slog_d("delete re info and session, giver_uin=%0", giver_uin);

		// delete giver session
		delete_and_erase_map_element_by_key(&m_giver_sessions, giver_uin);

		// delete matched receiver session ?

		// delete re_info
		if (!m_db.deleteReInfoByGiverUin(giver_uin))
		{
			slog_d("fail to m_db.deleteReInfoByGiverUin, giver_uin=%0", giver_uin);
		}

		for (size_t i = 0; i < m_re_infos.size(); ++i)
		{
			if (m_re_infos[i].m_giver_uin == giver_uin)
			{
				m_re_infos.erase(m_re_infos.begin() + i);
				break;
			}
		}
	}

	bool __isReceiverMatchedWithGiver(uint32_t receiver_uin, uint32_t giver_uin)
	{
		NfServerDb::Record_ReInfo* info = __getReInfoByGiverUin(giver_uin);
		if (info == nullptr)
			return false;

		return info->getHasMatchedReceiverByReceiverUin(receiver_uin);
	}

	bool __isReceiverScanedGiver(uint32_t receiver_uin, uint32_t giver_uin)
	{
		__RedEnvelopeReceiverSession* s = __getReceiverSessionByUin(receiver_uin);
		if (s == nullptr)
			return false;
		return s->m_scan_table.hasScanUin(giver_uin);
	}




	std::vector<uint32_t> __genMatchedReceiverUinsFromReInfo(NfServerDb::Record_ReInfo* info)
	{
		std::vector<uint32_t> match_recevier_uins;
		for (size_t i = 0; i < info->m_match_receivers.size(); ++i)
		{
			match_recevier_uins.push_back(info->m_match_receivers[i].m_receiver_uin);
		}
		return match_recevier_uins;
	}

	__ServerCgiCtx __genServerCgiCtx(socket_id_t sid, session_id_t ssid)
	{
		__ServerCgiCtx cgi_ctx;
		cgi_ctx.m_callback = this;
		cgi_ctx.m_network = getNetwork();
		cgi_ctx.m_packer = m_packer;
		cgi_ctx.m_sid = sid;
		cgi_ctx.m_ssid = ssid;
		return cgi_ctx;
	}

	__ServerCgi_redEnvelopeMatchResult* __genPushMatchResultCgi(uint32_t giver_uin, socket_id_t sid, session_id_t ssid)
	{
		NfServerDb::Record_ReInfo* info = __getReInfoByGiverUin(giver_uin);
		if (info == nullptr)
			return nullptr;
		std::vector<uint32_t> match_recevier_uins = __genMatchedReceiverUinsFromReInfo(info);
		__ServerCgiCtx cgi_ctx = __genServerCgiCtx(sid, ssid);

		__ServerCgi_redEnvelopeMatchResult* cgi = new __ServerCgi_redEnvelopeMatchResult();
		if (info->m_re_qr_code.size() == 0)
		{
			slog_e("info->m_re_qr_code.size() == 0");
		}
		cgi->initSendPack(cgi_ctx, info->m_wx_re_id, info->m_giver_uin, match_recevier_uins, info->m_re_qr_code);
		return cgi;
	}

	__RedEnvelopeGiverSession* __getGiverSessionByUin(uint32_t uin)
	{
		auto it = m_giver_sessions.find(uin);
		if (it == m_giver_sessions.end())
			return nullptr;
		return it->second;
	}
	
	__RedEnvelopeGiverSession* __createGiverSession(uint32_t uin, socket_id_t sid, session_id_t ssid)
	{
		__RedEnvelopeGiverSession* giver_session = new __RedEnvelopeGiverSession();
		giver_session->m_sid = sid;
		giver_session->m_ssid = ssid;
		giver_session->m_session_create_time_ms = TimeUtil::getMsTime();
		giver_session->m_session_live_start_time_ms = giver_session->m_session_create_time_ms;
		giver_session->m_giver_uin = uin;
		m_giver_sessions[giver_session->m_giver_uin] = giver_session;
		slog_i("create giver_session, giver_uin=%0", uin);
		return giver_session;
	}

	__RedEnvelopeGiverSession* __createOrRefreshGiverSession(uint32_t uin, socket_id_t sid, session_id_t ssid)
	{
		__RedEnvelopeGiverSession* giver_session = __getGiverSessionByUin(uin);
		if (giver_session == nullptr)
			giver_session = __createGiverSession(uin, sid, ssid);
		else
			giver_session->refreshSession(sid, ssid);
		return giver_session;
	}

	__RedEnvelopeReceiverSession* __getReceiverSessionByUin(uint32_t uin)
	{
		auto it = m_receiver_sessions.find(uin);
		if (it == m_receiver_sessions.end())
			return nullptr;
		return it->second;
	}

	__RedEnvelopeReceiverSession* __createReceiverSession(uint32_t uin, socket_id_t sid, session_id_t ssid)
	{
		__RedEnvelopeReceiverSession* receiver_session = new __RedEnvelopeReceiverSession();
		receiver_session->m_sid = sid;
		receiver_session->m_ssid = ssid;
		receiver_session->m_session_create_time_ms = TimeUtil::getMsTime();
		receiver_session->m_session_live_start_time_ms = receiver_session->m_session_create_time_ms;
		receiver_session->m_receiver_uin = uin;
		m_receiver_sessions[receiver_session->m_receiver_uin] = receiver_session;
		slog_i("create receiver_session, receiver_uin=%0", receiver_session->m_receiver_uin);
		return receiver_session;
	}

	__RedEnvelopeReceiverSession* __createOrRefreshReceiverSession(uint32_t uin, socket_id_t sid, session_id_t ssid)
	{
		__RedEnvelopeReceiverSession* receiver_session = __getReceiverSessionByUin(uin);
		if (receiver_session == nullptr)
			receiver_session = __createReceiverSession(uin, sid, ssid);
		else
			receiver_session->refreshSession(sid, ssid);
		return receiver_session;
	}

	bool __isReQrCodeTimeOut(const NfServerDb::Record_ReInfo& info)
	{
		uint64_t cur_time = TimeUtil::getMsTime();
		return cur_time - info.m_re_qr_code_create_time > 60 * 1000;
	}

	NfServerDb::Record_ReInfo* __getReInfoByGiverUin(uint32_t giver_uin)
	{
		for (size_t i = 0; i < m_re_infos.size(); ++i)
		{
			if (m_re_infos[i].m_giver_uin == giver_uin)
				return &m_re_infos[i];
		}
		return nullptr;
	}

	std::string __sendCmdTypeToString(uint32_t send_cmd_type)
	{
		switch (send_cmd_type)
		{
		case __ECgiCmdType_s2cResp_RedEnvelope_GiverCreateSession: return "re_giver_create_sessin";
		case __ECgiCmdType_s2cResp_RedEnvelope_GiverDeleteSession: return "re_giver_delete_sessin";
		case __ECgiCmdType_s2cResp_RedEnvelope_GiverUpdateSession: return "re_giver_update_sessin";
		case __ECgiCmdType_s2cResp_RedEnvelope_GiverReportScanResult: return "re_giver_report_scan";
		case __ECgiCmdType_s2cResp_RedEnvelope_ReceiverCreateSession: return "re_receiver_create_sessin";
		case __ECgiCmdType_s2cResp_RedEnvelope_ReceiverDeleteSession: return "re_receiver_delete_sessin";
		case __ECgiCmdType_s2cResp_RedEnvelope_ReceiverUpdateSession: return "re_receiver_update_sessin";
		case __ECgiCmdType_s2cResp_RedEnvelope_ReceiverReportScanResult: return "re_receiver_report_scan";
		case __ECgiCmdType_s2cPush_RedEnvelope_MatchResult: return "re_match_result";
		case __ECgiCmdType_s2cResp_AddFriend_QueryUserInfo: return "af_query_user_info";
		case __ECgiCmdType_s2cResp_AddFriend_ReportUserInfo: return "af_report_user_info";
		case __ECgiCmdType_s2cPush_AddFriend_MatchResult: return "af_match_result";
		default:
			break;
		}
		return "unkonw";
	}

	int __getCgiInfoIndexBySendCmdType(uint32_t send_cmd_type)
	{
		for (size_t i = 0; i < m_cgi_infos.size(); ++i)
		{
			if (m_cgi_infos[i].m_send_cmd_type == send_cmd_type)
				return (int)i;
		}
		return -1;
	}

	void __dumpAll()
	{
		__dumpAllReInfos();
		__dumpAllGiverSessions();
		__dumpAllReceiverSessions();
	}

	void __dumpAllReInfos()
	{
		slog_d("dump all re_info");
		for (size_t i = 0; i < m_re_infos.size(); ++i)
		{
			slog_d("re_info=%0", m_re_infos[i].toString());
		}
	}

	void __dumpAllGiverSessions()
	{
		slog_d("dump all giver_sessions");
		for (auto it = m_giver_sessions.begin(); it != m_giver_sessions.end(); ++it)
		{
			slog_d("giver_sssion=%0", it->second->toString());
		}
	}

	void __dumpAllReceiverSessions()
	{
		slog_d("dump all receiver_sessions");
		for (auto it = m_receiver_sessions.begin(); it != m_receiver_sessions.end(); ++it)
		{
			slog_d("receiver_sssion=%0", it->second->toString());
		}
	}



	std::vector<ServerCgiInfo> m_cgi_infos;
	StPacker* m_packer;
	uint64_t m_check_session_timer;
	NfServerDb m_db;
	NfStatisticDb m_statistic_db;
	std::vector<NfServerDb::Record_ReInfo> m_re_infos;
	std::map<uint32_t, __RedEnvelopeGiverSession*> m_giver_sessions;
	std::map<uint32_t, __RedEnvelopeReceiverSession*> m_receiver_sessions;
	__AddFriendCtx m_add_friend_ctx;
};






void __initLog()
{
	Log::releaseInstance();


#ifdef S_OS_WIN
	{
		ConsoleAppender* ca = new ConsoleAppender();
		FormatInfo formatInfo;
		formatInfo.m_isShowTime = true;
		formatInfo.m_isShowFunction = false;
		formatInfo.m_isShowFileName = false;
		formatInfo.m_isShowLineNum = false;
		ca->getFormator()->setFormatInfo(formatInfo);
		Log::instance().addAppender(ca);
	}
#endif // S_OS_WIN

	{
		FileAppender* fa = new FileAppender();
		fa->init(".", "nfserver");
		FormatInfo formatInfo;
		formatInfo.m_isShowFunction = false;
		formatInfo.m_isShowFileName = false;
		formatInfo.m_isShowLineNum = false;
		fa->getFormator()->setFormatInfo(formatInfo);
		Log::instance().addAppender(fa);
	}
	Log::instance().setEnableLogLevel(ELogLevel_debug);
}


#if defined(S_OS_LINUX) | defined(S_OS_MAC)
#include <stdio.h>
#include <stdlib.h>   // exit()
#include <unistd.h>   // fork() setsid() chdir() close()
//#include <sys/types.h>    // fork() umask()
#include <sys/stat.h> // umask()

// 与终端无关，在后台默默运行的守护进程
void init_daemon(const char* work_dir)
{
	// 调用setsid() 的不能是进程组组长，当前程序有可能是进程组组长
	pid_t pid = fork();

	// 非子进程则退出
	if (pid != 0)
		exit(-1);

	// 父进程退出，留下子进程

	// 创建一个新的会话期，从而脱离原有的会话期
	// 进程同时与控制终端脱离
	setsid();

	// 此时子进程成为新会话期的领导和新的进程组长
	// 但这样的身份可能会通过fcntl去获到终端
	pid = fork();

	// 非子进程则退出
	if (pid != 0)
		exit(-1);

	// 此时留下来的是孙子进程,再也不能获取终端

	// 通常来讲, 守护进程应该工作在一个系统永远不会删除的目录下
	chdir(work_dir);

	// 关闭输入输出和错误流 (通过日志来查看状态)
	close(0);
	close(1);
	close(2);

	//去掩码位
	umask((mode_t)0);//sys/stat.h
}
#endif


int main(int argc, char** argv)
{
#if defined(S_OS_LINUX) | defined(S_OS_MAC)
	init_daemon(".");
#endif

	__initLog();
	//__testNfStatisticDb();
	//__testNfServerDb();

	slog_i("NfServer ---------------------------------------------------------");
	__NfServerNetworkLogic* logic = new __NfServerNetworkLogic();
	MessageLoopThread* t = new MessageLoopThread(logic);
	logic->init(t->getLooper());

	t->start();
	logic->sendMsg_startCmd();

	t->join();
	delete t;
    return 0;
}

