#pragma once

#include "SServerObj.h"

class LobbyChar;
class SPacket;

class LobbyMgr : public SSingleton <LobbyMgr>
{
private:
	friend class SSingleton<LobbyMgr>;

private:
	//�κ� �ִ� player list
	//std::list<LobbySession*>			m_listPlayerInLobby;
	std::list<LobbyChar*>				m_listPlayerInLobby;

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
	void SendPacketAllInLobby( SPacket& packet, LobbyChar* mySession = NULL );

	//����� ������ ���
	void PackageDataAllInLobby( SPacket& packet );

	//��� �߰��ϱ�
	void AddUser( LobbyChar* _session );
	//��� ����
	void MinusUser( LobbyChar* _session );

	//��� �� �ޱ�
	inline int GetPlayerCount() { return m_iPlayerCountInLobby; }
};

#define GetLobbyMgr LobbyMgr::GetInstance()
