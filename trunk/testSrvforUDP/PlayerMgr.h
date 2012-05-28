#pragma once

#include "SServerObj.h"

#include "SIndexQueue.h"
#include "SList.h"

class UDPSession;
class SPacket;

class Player : public SServerObj
{
private:
	int				m_id;
	UDPSession*		m_session;

	//==============================================================
	char			m_ip[15];
	int				m_port;

public:
	Player(void);
	~Player(void);

	void Init( int _id );
	void Init();

	void SetID( int _id );
	int GetID() const;

	void SetSession( UDPSession* session );
	UDPSession* GetSession() const;

	void SetIP( char* ip );
	char* GetIP();

	void SetPort( int port );
	int GetPort() const;

	void PackageMyInfo( SPacket* packet );

};

//--------------------------------------------------------------

class PlayerMgr : public SSingleton <PlayerMgr>, public SServerObj
{
private:
	friend class SSingleton<PlayerMgr>;

private:
	std::map<int, Player*>	m_playerSpace;
	SIndexQueue				m_indexQ;
	SList<Player*>			m_listPlayer;

public:
	PlayerMgr(void);
	~PlayerMgr(void);

	BOOL Init( int playerCount );
	void Release();

	Player* AddPlayer();
	BOOL DelPlayer( Player* player );

	Player* FindPlayer( int _id );

	void SendAllPlayer( SPacket* packet, Player* itme = NULL );
	void PackageAllPlayer( SPacket* packet, Player* itme = NULL );
};

#define GetPlayerMgr PlayerMgr::GetInstance()
