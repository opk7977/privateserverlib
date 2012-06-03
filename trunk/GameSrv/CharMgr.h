#pragma once

#include "SIndexQueue.h"
#include "SPacket.h"

class GameSession;

const int Character_Space = 100;

enum SRV_CHAR_STATE
{
	SRV_CHAR_STATE_STAND = 0,
	SRV_CHAR_STATE_MOVE,

	SRV_CHAR_STATE_ATTECT,
	SRV_CHAR_STATE_DAMAGE,
	SRV_CHAR_STATE_DIE,
};

enum SRV_CHAR_TEAM
{
	SRV_CHAR_TEAM_ATT = 0,
	SRV_CHAR_TEAM_DEF,
};



class CharObj : public SServerObj
{
private:
	GameSession*		m_session;

	int					m_sessionID;
	TCHAR				m_tstrID[50];
	int					m_iTeam;
	int					m_startPoint;

	int					m_killCount;
	int					m_deathCount;

	//에너지
	int					m_HP;

	//은신 수치 등등
	//
	//

	//vec의 Index_공간 관리용 index
	int					m_vecIndex;

public:
	CharObj();
	~CharObj();

	void Init();
	void Init( int index );

	void SetSessionID( int session ) { m_sessionID = session; }
	int GetSessionID() { return m_sessionID; }

	inline void SetSession( GameSession* session ) { m_session = session; }
	inline GameSession* GetSession() { return m_session; }

	inline void SetID( TCHAR* id ) { _tcsncpy_s( m_tstrID, 50, id, _tcslen(id) ); }
	inline TCHAR* GetID() { return m_tstrID; }

	inline void SetTeam( int team ) { m_iTeam = team; }
	inline int GetTeam() { return m_iTeam; }

	inline void SetStartPoint( int p ) { m_startPoint = p; }
	inline int GetStartPoint() { return m_startPoint; }

	//공간 관리용 index
	inline int GetVecIndex() { return m_vecIndex; }

	//공격을 당해 데이지를 입음
	void DownHP( int damage );
	//내 hp return
	int	GetHP() const { return m_HP; }
	//피가 다 달아서 죽었는지?
	BOOL IsDie();
	//살리기
	void SetAlive() { m_HP = 100; }

	//죽은 횟수
	void DeathCountUp();
	int GetDeathCount();

	//죽인 횟수
	void KillCountUp();
	int GetKillCount();
	
};


class CharMgr : public SSingleton <CharMgr>
{
private:
	friend class SSingleton<CharMgr>;

private:
	//캐릭터 공간
	std::vector<CharObj*>		m_vecCharSpace;
	//공간 관리용 index
	SIndexQueue					m_IndexQ;

private:
	CharMgr(void);
	~CharMgr(void);

public:
	//생성과 해제
	void Init();
	void Release();

	//캐릭터 공간을 할당
	CharObj* GetCharSpace();

	//캐릭터 공간을 반납
	void ReturnCharSpace( CharObj* charspace );

	//해당 index에 해당하는 character를 받아 간다.
	CharObj* FindCharAsIndex( int index );
	CharObj* FindCharAsSessionId( int sessionID );

};

#define GetCharMgr CharMgr::GetInstance()

