#pragma once

#include "SServerObj.h"

class SPacket;
class LobbySession;

class LobbyMgr : public SSingleton <LobbyMgr>
{
private:
	friend class SSingleton<LobbyMgr>;

private:
	//로비에 있는 player list
	std::list<LobbySession*>			m_listPlayerInLobby;

	//로비에 있는 사람 수
	int									m_iPlayerCountInLobby;

	//임계영역을 위한 obj
	static SServerObj*					m_critical;

private:
	LobbyMgr(void);
	~LobbyMgr(void);

public:
	void Init();
	void Release();

	//로비에 있는 사람들에게만 패킷 보내기
	void SendPacketAllInLobby( SPacket& packet, LobbySession* mySession = NULL );

	//모두의 정보를 담기
	void PackageDataAllInLobby( SPacket& packet );

	//사람 추가하기
	void AddUser( LobbySession* _session );
	//사람 뺴기
	void MinusUser( LobbySession* _session );

	//사람 수 받기
	inline int GetPlayerCount() { return m_iPlayerCountInLobby; }
};

#define GetLobbyMgr LobbyMgr::GetInstance()
