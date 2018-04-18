#ifndef _NF_SERVER_DB_H_
#define _NF_SERVER_DB_H_
#include "nfComm.h"
USING_NAMESPACE_S


class NfServerDb
{
public:
	class MatchReceiver
	{
	public:
		MatchReceiver() { m_receiver_uin = 0; m_has_opened_re = false; }

		uint32_t m_receiver_uin;
		bool m_has_opened_re;
	};

	class Record_ReInfo
	{
	public:
		Record_ReInfo() { m_giver_uin = 0; m_session_create_time_ms = 0; m_re_qr_code_create_time = 0; m_re_count = 0; }

		int getMatchedReceiverIndexByReceiverUin(uint32_t uin)
		{
			for (size_t i = 0; i < m_match_receivers.size(); ++i)
			{
				if (m_match_receivers[i].m_receiver_uin == uin)
					return (int)i;
			}
			return -1;
		}

		bool getHasMatchedReceiverByReceiverUin(uint32_t uin)
		{
			return getMatchedReceiverIndexByReceiverUin(uin) >= 0;
		}

		std::string toString()
		{
			std::string str;
			str += "(";

			str = str + "giver_uin=" + StringUtil::toString(m_giver_uin)
				+ ", wx_re_id=" + m_wx_re_id
				+ ", re_qr_code=" + m_re_qr_code
				+ ", re_count=" + StringUtil::toString(m_re_count)
				+ ", match_receivers=(" + __matchReceiversToString(m_match_receivers) + ")";

			str += ")";
			return str;
		}

		uint32_t	m_giver_uin;
		std::string m_wx_re_id;
		uint64_t	m_session_create_time_ms;
		std::string m_re_qr_code;
		uint64_t	m_re_qr_code_create_time;
		uint32_t	m_re_count;
		std::set<uint32_t> m_scan_receiver_uins;
		std::vector<MatchReceiver> m_match_receivers;
	};

	bool open(const std::string& db_path)
	{
		m_db.open(db_path.c_str());

		const char* create_table_sql = "create table if not exists re_giver_session("
			" giver_uin integer primary key,"
			" wx_re_id text,"
			" session_create_time integer,"
			" re_qr_code text,"
			" re_qr_code_create_time integer,"
			" re_count integer,"	
			" scan_receiver_uins text,"
			" matched_receivers text,"
			" ext1 blob, ext2 blob, ext3 blob);";

		if (!m_db.exec(create_table_sql))
		{
			slog_e("fail to db.exec");
			return false;
		}
		
		create_table_sql = "create table if not exists re_receiver_session("
			" receiver_uin integer primary key,"
			" session_create_time integer,"
			" scan_giver_uins text,"
			" ext1 blob, ext2 blob, ext3 blob);";

		if (!m_db.exec(create_table_sql))
		{
			slog_e("fail to db.exec");
			return false;
		}

		return true;
	}

	void close()
	{
		m_db.close();
	}

	bool addReInfo(const Record_ReInfo& record)
	{
		SqliteStmt stmt;
		if (!m_db.prepare("insert into re_giver_session"
				"(giver_uin, wx_re_id, session_create_time, re_qr_code, re_qr_code_create_time, re_count, scan_receiver_uins, matched_receivers)"
				"values(?,?,?,?,?,?,?,?);", &stmt))
			return false;

		std::string scan_receiver_uins_text = __uinsToString(record.m_scan_receiver_uins);
		std::string matched_receivers_text = __matchReceiversToString(record.m_match_receivers);
		stmt.bindColumInt32(0, record.m_giver_uin);
		stmt.bindColumText(1, record.m_wx_re_id);
		stmt.bindColumInt64(2, record.m_session_create_time_ms);
		stmt.bindColumText(3, record.m_re_qr_code);
		stmt.bindColumInt64(4, record.m_re_qr_code_create_time);
		stmt.bindColumInt32(5, record.m_re_count);
		stmt.bindColumText(6, scan_receiver_uins_text);
		stmt.bindColumText(7, matched_receivers_text);

		if (!stmt.step())
			return false;
		return true;
	}

	bool getReInfoByGiverUin(uint32_t uin, Record_ReInfo* record)
	{
		SqliteStmt stmt;
		if (!m_db.prepare("select * from re_giver_session "
			"where giver_uin=?", &stmt))
			return false;

		stmt.bindColumInt32(0, uin);
		if (!stmt.step())
			return false;
		if (!stmt.getHasStepResultRow())
			return false;

		__stmtToReInfo(stmt, record);
		return true;
	}

	bool getAllReInfos(std::vector<Record_ReInfo>* records)
	{
		SqliteStmt stmt;
		if (!m_db.prepare("select * from re_giver_session", &stmt))
			return false;
		while (stmt.step())
		{
			if (stmt.getIsStepDone())
				break;
			if (stmt.getHasStepResultRow())
			{
				Record_ReInfo r;
				__stmtToReInfo(stmt, &r);
				records->push_back(r);
			}
		}
		return true;
	}

	bool updateReInfo(const Record_ReInfo& r)
	{
		SqliteStmt stmt;
		if (!m_db.prepare("update re_giver_session set "
			" wx_re_id=?"
			" ,session_create_time=?"
			" ,re_qr_code=?"
			" ,re_qr_code_create_time=?"
			" ,re_count=?"
			" ,scan_receiver_uins=?"
			" ,matched_receivers=?"
			" where giver_uin=?", &stmt))
			return false;

		std::string scan_receiver_uins_text = __uinsToString(r.m_scan_receiver_uins);
		std::string matched_receivers_text = __matchReceiversToString(r.m_match_receivers);
		stmt.bindColumText(0, r.m_wx_re_id);
		stmt.bindColumInt64(1, r.m_session_create_time_ms);
		stmt.bindColumText(2, r.m_re_qr_code);
		stmt.bindColumInt64(3, r.m_re_qr_code_create_time);
		stmt.bindColumInt32(4, r.m_re_count);
		stmt.bindColumText(5, scan_receiver_uins_text);
		stmt.bindColumText(6, matched_receivers_text);
		stmt.bindColumInt32(7, r.m_giver_uin);

		if (!stmt.step())
			return false;
		return true;
	}

	bool deleteReInfoByGiverUin(uint32_t uin)
	{
		SqliteStmt stmt;
		if (!m_db.prepare("delete from re_giver_session"
			" where giver_uin=?", &stmt))
			return false;

		stmt.bindColumInt32(0, uin);
		if (!stmt.step())
			return false;
		return true;
	}




private:
	static void __stmtToReInfo(SqliteStmt& stmt, Record_ReInfo* r)
	{
		std::string scan_receiver_uins_text;
		std::string matched_receivers_text;
		stmt.getColumUint32(0, &(r->m_giver_uin));
		stmt.getColumText(1, &(r->m_wx_re_id));
		stmt.getColumUint64(2, &(r->m_session_create_time_ms));
		stmt.getColumText(3, &(r->m_re_qr_code));
		stmt.getColumUint64(4, &(r->m_re_qr_code_create_time));
		stmt.getColumUint32(5, &(r->m_re_count));
		stmt.getColumText(6, &scan_receiver_uins_text);
		stmt.getColumText(7, &matched_receivers_text);
		r->m_scan_receiver_uins = __stringToUins(scan_receiver_uins_text);
		r->m_match_receivers = __stringToMatchReceivers(matched_receivers_text);
	}

	static std::string __uinsToString(const std::set<uint32_t>& uins)
	{
		std::string str;
		str = str + "uin_count=" + StringUtil::toString(uins.size()) + ",";

		uint32_t i = 0;
		for (auto it = uins.begin(); it != uins.end(); ++it)
		{
			str = str + "uin" + StringUtil::toString(i) + "=" + StringUtil::toString(*it) + ",";
			++i;
		}
		return str;
	}

	static std::set<uint32_t> __stringToUins(const std::string& str)
	{
		std::set<uint32_t> uins;
		uint32_t uin_count = StringUtil::parseUint(StringUtil::fetchMiddle(str, "uin_count=", ","));
		for (size_t i = 0; i < uin_count; ++i)
		{
			std::string tag = std::string() + "uin" + StringUtil::toString(i) + "=";
			uint32_t uin = StringUtil::parseUint(StringUtil::fetchMiddle(str, tag, ","));
			uins.insert(uin);
		}
		return uins;
	}

	static std::string __matchReceiversToString(const std::vector<MatchReceiver>& receivers)
	{
		std::string str;
		str = str + "receiver_count=" + StringUtil::toString(receivers.size()) + ",";

		for (size_t i = 0; i < receivers.size(); ++i)
		{
			str = str + "receiver" + StringUtil::toString(i) + "=" + __matchRecevierToString(receivers[i]) + ",";
		}
		return str;
	}

	static std::vector<MatchReceiver> __stringToMatchReceivers(const std::string& str)
	{
		std::vector<MatchReceiver> receivers;
		uint32_t count = StringUtil::parseUint(StringUtil::fetchMiddle(str, "receiver_count=", ","));
		for (size_t i = 0; i < count; ++i)
		{
			std::string tag = std::string() + "receiver" + StringUtil::toString(i) + "=";
			std::string m_str = StringUtil::fetchMiddle(str, tag, ",");
			MatchReceiver r = __stringToMatchReceiver(m_str);
			receivers.push_back(r);
		}
		return receivers;
	}

	static std::string __matchRecevierToString(const MatchReceiver& r)
	{
		return StringUtil::toString(r.m_receiver_uin) + "|" + StringUtil::toString(r.m_has_opened_re);
	}

	static MatchReceiver __stringToMatchReceiver(const std::string& str)
	{
		MatchReceiver r;
		std::vector<std::string> words;
		StringUtil::split(str, '|', &words);
		r.m_receiver_uin = StringUtil::parseUint(words[0]);
		r.m_has_opened_re = StringUtil::parseUint(words[1]);
		return r;
	}

	SqliteDb m_db;
};



bool operator ==(const NfServerDb::MatchReceiver& r1, const NfServerDb::MatchReceiver& r2)
{
	if (r1.m_has_opened_re != r2.m_has_opened_re || r1.m_receiver_uin != r2.m_receiver_uin)
		return false;
	return true;
}

bool __isEqual(const NfServerDb::Record_ReInfo& r1, const NfServerDb::Record_ReInfo& r2)
{
	if (r1.m_giver_uin != r2.m_giver_uin
		|| r1.m_re_count != r2.m_re_count
		|| r1.m_re_qr_code != r2.m_re_qr_code
		|| r1.m_session_create_time_ms != r2.m_session_create_time_ms
		|| r1.m_wx_re_id != r2.m_wx_re_id)
	{
		return false;
	}

	if (r1.m_scan_receiver_uins.size() != r2.m_scan_receiver_uins.size())
		return false;
	if (r1.m_match_receivers.size() != r2.m_match_receivers.size())
		return false;
	if (r1.m_scan_receiver_uins != r2.m_scan_receiver_uins)
		return false;
	if (r1.m_match_receivers != r2.m_match_receivers)
		return false;

	return true;
}





class NfStatisticDb
{
public:
	class Record_zishi
	{
	public:
		uint32_t m_uin;
		bool m_is_sender;
		int16_t m_angle;
		int16_t m_ax;
		int16_t m_ay;
		int16_t m_az;
		uint64_t m_create_time;
	};

	bool open(const std::string& db_path)
	{
		m_db.open(db_path.c_str());

		const char* create_table_sql = "create table if not exists zhishi("
			" uin integer,"
			" is_sender integer,"
			" angle integer,"
			" ax integer,"
			" ay integer,"
			" az integer,"
			" create_time integer,"
			" ext1 blob, ext2 blob, ext3 blob);";

		if (!m_db.exec(create_table_sql))
		{
			slog_e("fail to db.exec");
			return false;
		}

		return true;
	}

	void close()
	{
		m_db.close();
	}

	bool addZishi(const Record_zishi& record)
	{
		SqliteStmt stmt;
		if (!m_db.prepare("insert into zhishi"
			"(uin, is_sender, angle, ax, ay, az, create_time)"
			"values(?,?,?,?,?,?,?);", &stmt))
			return false;

		stmt.bindColumUint32(0, record.m_uin);
		stmt.bindColumInt32(1, record.m_is_sender);
		stmt.bindColumInt32(2, record.m_angle);
		stmt.bindColumInt32(3, record.m_ax);
		stmt.bindColumInt32(4, record.m_ay);
		stmt.bindColumInt32(5, record.m_az);
		stmt.bindColumUint64(6, record.m_create_time);

		if (!stmt.step())
			return false;
		return true;
	}

private:
	SqliteDb m_db;
};



void __testNfServerDb()
{
	FileUtil::deleteFile("test_server.db");
	NfServerDb db;
	db.open("test_server.db");


	NfServerDb::Record_ReInfo record;

	// add
	{
		NfServerDb::MatchReceiver mr1;
		mr1.m_receiver_uin = 3;
		mr1.m_has_opened_re = true;

		NfServerDb::MatchReceiver mr2;
		mr2.m_receiver_uin = 4;

		record.m_giver_uin = 1;
		record.m_session_create_time_ms = TimeUtil::getMsTime();
		record.m_wx_re_id = "wx_re_id_kdkjskfjsd";
		record.m_re_qr_code = "qr_code_ksdfsjd";
		record.m_re_qr_code_create_time = record.m_session_create_time_ms;
		record.m_re_count = 10;
		record.m_scan_receiver_uins.insert(3);
		record.m_scan_receiver_uins.insert(4);
		record.m_scan_receiver_uins.insert(5);
		record.m_match_receivers.push_back(mr1);
		record.m_match_receivers.push_back(mr2);

		if (!db.addReInfo(record)) { slog_e("fail to addReInfo"); return; }
	}

	// get
	{
		NfServerDb::Record_ReInfo record2;
		if (!db.getReInfoByGiverUin(1, &record2)) { slog_e("fail to getReInfoByGiverUin"); return; }
		if (!__isEqual(record, record2)) { slog_e("record2 != record"); return; }

		std::vector<NfServerDb::Record_ReInfo> sessions;
		if (!db.getAllReInfos(&sessions)) { slog_e("fail to getAllReInfos"); return; }
		if (!__isEqual(record, sessions[0])) { slog_e("sessions[0] != record"); return; }
	}

	// update
	{
		NfServerDb::Record_ReInfo record2;
		record.m_re_qr_code_create_time = TimeUtil::getMsTime();
		record.m_re_qr_code = "qr_code_11111111";
		if (!db.updateReInfo(record)) { slog_e("fail to updateReInfo"); return; }
		if (!db.getReInfoByGiverUin(1, &record2)) { slog_e("fail to getReInfoByGiverUin"); return; }
		if (!__isEqual(record, record2)) { slog_e("record2 != record"); return; }
	}

	// delete
	{
		if (!db.deleteReInfoByGiverUin(record.m_giver_uin)) { slog_e("fail to deleteReInfoByGiverUin"); return; }
	}


	slog_i("test NfServerDb ok");
}


void __testNfStatisticDb()
{
	FileUtil::deleteFile("test_statistic.db");
	NfStatisticDb db;
	db.open("test_statistic.db");


	NfStatisticDb::Record_zishi record;

	// add
	{
		record.m_uin = 1;
		record.m_is_sender = true;
		record.m_angle = 180;
		record.m_ax = 100;
		record.m_ay = 200;
		record.m_az = -400;
		time_t t = time(nullptr);
		record.m_create_time = TimeUtil::getMsTime() / 1000;

		if (!db.addZishi(record)) { slog_e("fail to addZishi"); return; }
	}
}


#endif








//
//bool addReReceiverSession(const Record_ReReceiverSession& r)
//{
//	SqliteStmt stmt;
//	if (!m_db.prepare("insert into re_receiver_session"
//		" (receiver_uin, session_create_time, scan_giver_uins)"
//		" values(?,?,?);", &stmt))
//		return false;
//
//	std::string scan_giver_uins_text = __uinsToString(r.m_scan_giver_uins);
//	stmt.bindColumInt32(0, r.m_receiver_uin);
//	stmt.bindColumInt64(1, r.m_session_create_time_ms);
//	stmt.bindColumText(2, scan_giver_uins_text);
//
//	if (!stmt.step())
//		return false;
//	return true;
//}
//
//bool getReReceiverSessionByReceiverUin(uint32_t uin, Record_ReReceiverSession* r)
//{
//	SqliteStmt stmt;
//	if (!m_db.prepare("select * from re_receiver_session"
//		" where receiver_uin=?", &stmt))
//		return false;
//
//	stmt.bindColumInt32(0, uin);
//	if (!stmt.step())
//		return false;
//	if (!stmt.getHasStepResultRow())
//		return false;
//
//	__stmtToReReceiverSession(stmt, r);
//	return true;
//}
//
//bool getAllReReceiverSessions(std::vector<Record_ReReceiverSession>* sessions)
//{
//	SqliteStmt stmt;
//	if (!m_db.prepare("select * from re_receiver_session", &stmt))
//		return false;
//	while (stmt.step())
//	{
//		if (stmt.getIsStepDone())
//			break;
//		if (stmt.getHasStepResultRow())
//		{
//			Record_ReReceiverSession r;
//			__stmtToReReceiverSession(stmt, &r);
//			sessions->push_back(r);
//		}
//	}
//	return true;
//}
//
//bool updateReReceiverSession(const Record_ReReceiverSession& r)
//{
//	SqliteStmt stmt;
//	if (!m_db.prepare("update re_receiver_session set "
//		" session_create_time=?"
//		" ,scan_giver_uins=?"
//		" where receiver_uin=?", &stmt))
//		return false;
//
//	std::string scan_giver_uins_text = __uinsToString(r.m_scan_giver_uins);
//	stmt.bindColumInt64(0, r.m_session_create_time_ms);
//	stmt.bindColumText(1, scan_giver_uins_text);
//	stmt.bindColumInt32(2, r.m_receiver_uin);
//
//	if (!stmt.step())
//		return false;
//	return true;
//}
//
//bool deleteReReceiverSessionByReceiverUin(uint32_t uin)
//{
//	SqliteStmt stmt;
//	if (!m_db.prepare("delete from re_receiver_session "
//		"where receiver_uin=?", &stmt))
//		return false;
//
//	stmt.bindColumInt32(0, uin);
//	if (!stmt.step())
//		return false;
//	return true;
//}

//
//
//
//void __stmtToReReceiverSession(SqliteStmt& stmt, Record_ReReceiverSession* r)
//{
//	std::string scan_giver_uins_text;
//	stmt.getColumUint32(0, &(r->m_receiver_uin));
//	stmt.getColumUint64(1, &(r->m_session_create_time_ms));
//	stmt.getColumText(2, &scan_giver_uins_text);
//	r->m_scan_giver_uins = __stringToUins(scan_giver_uins_text);
//}


// Record_ReReceiverSession
//{
//	NfServerDb::Record_ReReceiverSession record;

//	// add
//	{
//		record.m_receiver_uin = 3;
//		record.m_session_create_time_ms = TimeUtil::getMsTime();

//		if (!db.addReReceiverSession(record)) { slog_e("fail to addReReceiverSession"); return; }
//	}

//	// get
//	{
//		NfServerDb::Record_ReReceiverSession record2;
//		if (!db.getReReceiverSessionByReceiverUin(3, &record2)) { slog_e("fail to getReReceiverSessionByReceiverUin"); return; }
//		if (!__isEqual(record, record2)) { slog_e("record2 != record"); return; }

//		std::vector<NfServerDb::Record_ReReceiverSession> sessions;
//		if (!db.getAllReReceiverSessions(&sessions)) { slog_e("fail to getAllReReceiverSessions"); return; }
//		if (!__isEqual(record, sessions[0])) { slog_e("sessions[0] != record"); return; }
//	}

//	// update
//	{
//		NfServerDb::Record_ReReceiverSession record2;
//		record.m_session_create_time_ms = TimeUtil::getMsTime();
//		if (!db.updateReReceiverSession(record)) { slog_e("fail to updateReReceiverSession"); return; }
//		if (!db.getReReceiverSessionByReceiverUin(3, &record2)) { slog_e("fail to getReReceiverSessionByReceiverUin"); return; }
//		if (!__isEqual(record, record2)) { slog_e("record2 != record"); return; }
//	}

//	// delete
//	{
//		if (!db.deleteReReceiverSessionByReceiverUin(record.m_receiver_uin)) { slog_e("fail to deleteReReceiverSessionByReceiverUin"); return; }
//	}
//}

//class Record_ReReceiverSession
//{
//public:
//	uint32_t m_receiver_uin;
//	uint64_t m_session_create_time_ms;
//	std::set<uint32_t> m_scan_giver_uins;
//};

//
//bool __isEqual(const NfServerDb::Record_ReReceiverSession& r1, const NfServerDb::Record_ReReceiverSession& r2)
//{
//	if (r1.m_receiver_uin != r2.m_receiver_uin
//		|| r1.m_session_create_time_ms != r2.m_session_create_time_ms)
//	{
//		return false;
//	}
//	if (r1.m_scan_giver_uins != r2.m_scan_giver_uins)
//		return false;
//	return true;
//}
