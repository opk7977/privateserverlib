#pragma once

#include "SServerObj.h"

class SPacket;
class LobbySession;

class LobbyMgr : public SSingleton <LobbyMgr>
{
private:
	friend class SSingleton<LobbyMgr>;

private:
	//�κ� �ִ� player list
	std::list<LobbySession*>			m_listPlayerInLobby;

	//�κ� �ִ� ��� ��
	int									m_iPlayerCountInLobby;

	//�Ӱ迵���� ���� obj
	static SServerObj*					m_critical;

private:
	LobbyMgr(void);
	~LobbyMgr(void);

public:
	void Init();
	void Release();

	//�κ� �ִ� ����鿡�Ը� ��Ŷ ������
	void SendPacketAllInLobby( SPacket& packet, LobbySession* mySession = NULL );

	//����� ������ ���
	void PackageDataAllInLobby( SPacket& packet );

	//��� �߰��ϱ�
	void AddUser( LobbySession* _session );
	//��� ����
	void MinusUser( LobbySession* _session );

	//��� �� �ޱ�
	inline int GetPlayerCount() { return m_iPlayerCountInLobby; }
};

#define GetLobbyMgr LobbyMgr::GetInstance()
