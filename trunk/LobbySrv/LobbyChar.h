#pragma once

#include "SIndexQueue.h"
#include "SServerObj.h"
#include "SList.h"

class LobbySession;
class SPacket;
class Room;
// 
// const int Character_Space = 100;

enum ROOM_TEAM
{
	ROOM_TEAM_ATT		=	0,
	ROOM_TEAM_DEF,
};

enum ROOM_READY
{
	ROOM_READY_NON		=	0,
	ROOM_READY_OK,
};

enum CHAR_WEAPON
{
	SWEAPON_BLASTER		= 0,
	SWEAPON_MACHINEGUN,
	SWEAPON_DARKMATTER,
	SWEAPON_GRENADE,
	SWEAPON_NAILGUN,
	SWEAPON_RAILGUN,
	SWEAPON_SHOTGUN,
};

class LobbyChar : public SServerObj
{
private:
	//�ش� ĳ������session����
	LobbySession*	m_session;

	//�ڽ��� �� ����
	Room*			m_myRoom;

	//ĳ������ ���°� ���� ���� �������� Ȯ��
	BOOL			m_isPlay;

	//ĳ���� ����
	int				m_sessionId;
	TCHAR			m_tstrId[30];
	int				m_myTeam;
	BOOL			m_ready;

	int				m_firstWeapon;
	int				m_secondWeapon;

	int				m_rankID;
	int				m_rankPoint;
	int				m_AccumulatedKill;
	int				m_AccumulatedDeath;

	//������ index
	int				m_vecIndex;

public:
	LobbyChar(void);
	~LobbyChar(void);

	void Init();
	void Init( int i );

	void SetSession( LobbySession* session );
	LobbySession* GetSession() const;

	void SetRoom( Room* room );
	Room* GetRoom() const;

	//==============================================================

	void SetIsPlay( BOOL isPlay = TRUE );
	BOOL GetIsPlay() const;

	void SetSessionID( int session );
	int GetSessionID() const;

	void SetID( TCHAR* _id );
	TCHAR* GetID();

	void SetTeam( int Team );
	int GetTeam() const;

	void SetReady( BOOL ready );
	BOOL GetReady() const;

	void SetCharData( int rankID, int rankPoint, int AccumulKillCount, int AccumulDeathCount );
	int GetRankID() const;
	int GetRankPoint() const;
	int GetAccumulatedKillCount() const;
	int GetAccumulatedDeathCount() const;

	void SetWeapon( int fWeapon, int sWeapon );
	inline int GetFirstWeapon() { return m_firstWeapon; }
	inline int GetSecondWeapon() { return m_secondWeapon; }

	//==============================================================
	//�κ� ���� �ʿ��� ������ ���
	void PackageMyInfoForLobby( SPacket& packet );
	//�濡 �ʿ��� ����(team, ready)�����ؼ� ���
	void PackageMyInfoForRoom( SPacket& packet );
	//���� ������ �ʿ��� ����(team)�� ��´�
	void PackageMyInfoForGame( SPacket& packet );

	//==============================================================

	inline int GetVecIndex() const { return m_vecIndex; };
};

class DataLeader;

class CharMgr : public SSingleton <CharMgr>, public SServerObj
{
private:
	friend class SSingleton<CharMgr>;

private:
	DataLeader*					m_document;

private:
	//ĳ���� ����
	std::vector<LobbyChar*>			m_vecCharSpace;
	//���� ������ index
	SIndexQueue						m_IndexQ;
	//�������ִ� ��ü ĳ���� list
	SList<int>						m_listPlayer;

	//������ ��ٸ��� ĳ���͵�
	ATL::CAtlMap<int, LobbyChar*>	m_waitChar;

// 	std::list<int>				m_connectPlayer;
// 	//�������ִ� ��ü ĳ������ ��
// 	int							m_connectPlayerCount;

private:
	CharMgr();
	~CharMgr();

public:
	//���� ������ ����
	void Init();
	void Release();

	inline int GetConnectPlayerCount() { return m_listPlayer.GetItemCount(); }

	//ĳ���� ������ �Ҵ����ִ� �Լ�
	LobbyChar* GetCharSPace();

	//ĳ���� ������ �ݳ�
	void ReturnCharSpace( LobbyChar* charspace );
	void ReturnCharSpace( int sessionID );

	//�α����� �κ񼭹��� ���ӵǱ� ����ϰ� �ִ� ĳ���� ������
	//�߰�
	void AddWaitChar( int sessionId, LobbyChar* charspace );
	//���ӵǾ� ĳ���� ������ list�� �Ѱ����� �����
	void DelWaitChar( int sessionId );
	//��ٸ��� ���߿� �˻�
	LobbyChar* GetWaitCharInfo( int sessionID );

	//session�� �ش��ϴ� character�� ����
	LobbyChar* FindCharAsSessionId( int session );

	//��� ĳ���͸� ����
	void PackageAllCharacter( SPacket& packet );
};

#define GetCharMgr CharMgr::GetInstance()
