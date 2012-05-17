#pragma once

#include "SSessionObj.h"

enum LOG_SERVER
{
	//========================================================
	// �α׼��� -> �ٸ�����
	// �α� ������ ���������� ���ӵǾ���
	//--------------------------------------------------------
	// ������ ����
	//========================================================
	LOG_SERVER_CONNECT_OK = 10000,

	//========================================================
	// �ٸ����� -> �α׼���
	// �α� ������ �Ѿ� ���� ��� ��Ŷ�� ������ ID�� ���´�
	//--------------------------------------------------------
	// int			-			ServerID
	// int			-			LogLevel
	// int			-			LogDataLenth
	// TCHAR		-			LogString
	//========================================================
	RECV_SERVER_LOG,

	//========================================================
	// �α׼��� -> �ٸ�����
	// �α� ���� �� ����
	//--------------------------------------------------------
	// ������ ����
	//========================================================
	LOG_SERVER_SHOTDOWN,
};

class LogDataBase;
class SPacketQueue;

class LogSession : public SSessionObj
{
public:
	SDECLARE_DYNAMIC(LogSession)
	SDECLARE_DYNCREATE(LogSession)

private:
	//======================================
	// single ton ��ü
	//======================================
	LogDataBase		*m_database;
	SPacketQueue	*m_packetQ;

public:
	LogSession(void);
	~LogSession(void);

	void OnCreate();
	void OnDestroy();

	void PacketParsing( SPacket& packet );

	

};
