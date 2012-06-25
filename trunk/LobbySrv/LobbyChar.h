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
	//해당 캐릭터의session공간
	LobbySession*	m_session;

	//자신의 방 공간
	Room*			m_myRoom;

	//캐릭터의 상태가 지금 게임 중인지를 확인
	BOOL			m_isPlay;

	//캐릭터 정보
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

	//백터의 index
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
	//로비를 위해 필요한 정보를 담기
	void PackageMyInfoForLobby( SPacket& packet );
	//방에 필요한 정보(team, ready)포함해서 담기
	void PackageMyInfoForRoom( SPacket& packet );
	//게임 서버에 필요한 정보(team)를 담는다
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
	//캐릭터 공간
	std::vector<LobbyChar*>			m_vecCharSpace;
	//공간 관리용 index
	SIndexQueue						m_IndexQ;
	//접속해있는 전체 캐릭터 list
	SList<int>						m_listPlayer;

	//접속을 기다리는 캐릭터들
	ATL::CAtlMap<int, LobbyChar*>	m_waitChar;

// 	std::list<int>				m_connectPlayer;
// 	//접속해있는 전체 캐릭터의 수
// 	int							m_connectPlayerCount;

private:
	CharMgr();
	~CharMgr();

public:
	//공간 생성과 해제
	void Init();
	void Release();

	inline int GetConnectPlayerCount() { return m_listPlayer.GetItemCount(); }

	//캐릭터 공간을 할당해주는 함수
	LobbyChar* GetCharSPace();

	//캐릭터 공간을 반납
	void ReturnCharSpace( LobbyChar* charspace );
	void ReturnCharSpace( int sessionID );

	//로그인후 로비서버로 접속되길 대기하고 있는 캐릭터 정보들
	//추가
	void AddWaitChar( int sessionId, LobbyChar* charspace );
	//접속되어 캐릭터 정보를 list로 넘겨지고 지운다
	void DelWaitChar( int sessionId );
	//기다리를 애중에 검색
	LobbyChar* GetWaitCharInfo( int sessionID );

	//session에 해당하는 character를 받음
	LobbyChar* FindCharAsSessionId( int session );

	//모든 캐릭터를 담자
	void PackageAllCharacter( SPacket& packet );
};

#define GetCharMgr CharMgr::GetInstance()
