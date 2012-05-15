#pragma once

#include "SIndexQueue.h"

class LobbySession;
class SPacket;

const int Character_Space = 100;

enum ROOM_TEAM
{
	ROOM_TEAM_ATT	=	0,
	ROOM_TEAM_DEF,
};
enum ROOM_READY
{
	ROOM_READY_NON	=	0,
	ROOM_READY_OK,
};


class LobbyChar : public SServerObj
{
private:
	//해당 캐릭터의session공간
	LobbySession*	m_session;

	//캐릭터의 상태가 지금 게임 중인지를 확인
	BOOL			m_isPlay;

	//캐릭터 정보
	int				m_sessionId;
	TCHAR			m_tstrId[30];
	int				m_myTeam;
	BOOL			m_ready;

	//백터의 index
	int				m_vecIndex;

public:
	LobbyChar(void);
	~LobbyChar(void);

	void Init();
	void Init( int i );

	void SetSession( LobbySession* session );
	LobbySession* GetSession() const;

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

	//==============================================================

	void PackageMyInfo( SPacket& packet );
	//방에 필요한 정보(team, ready)포함해서 담기
	void PackageMyInfoForRoom( SPacket& packet );
	//게임 서버에 필요한 정보(team)를 담는다
	void PackageMyInfoForGame( SPacket& packet );

	//==============================================================

	inline int GetVecIndex() const { return m_vecIndex; };
};


class CharMgr : public SSingleton <CharMgr>
{
private:
	friend class SSingleton<CharMgr>;

private:
	//캐릭터 공간
	std::vector<LobbyChar*>		m_vecCharSpace;
	//공간 관리용 index
	SIndexQueue					m_IndexQ;

private:
	CharMgr();
	~CharMgr();

public:
	//공간 생성과 해제
	void Init();
	void Release();

	//캐릭터 공간을 할당해주는 함수
	LobbyChar* GetCharSPace();

	//캐릭터 공간을 반납
	void ReturnCharSpace( LobbyChar* charspace );
	void ReturnCharSpace( int sessionID );

	//해당 session에 해당하는 character를 받음
	LobbyChar* FindCharAsSessionId( int session );
};

#define GetCharMgr CharMgr::GetInstance()
