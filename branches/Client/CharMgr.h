#pragma once

#include "SServerStd.h"

class CLobby;

class Character
{
public:
	//세션 아이디
	int			m_sessionId;

	//닉네임
	TCHAR		m_csId[50];

	//내가 속한 방(0:로비)
	int			m_roomNo;

	//나의 팀 정보
	int			m_Team;


public:
	Character() {};
	~Character() {};

	inline void SetSessionID( int indexId ) { m_sessionId = indexId; }
	inline int GetSessionID() { return m_sessionId; }

	inline void SetID( TCHAR* id ) { _tcsncpy_s( m_csId, 50, id, _tcslen(id) ); }
	inline TCHAR* GetID() { return m_csId; }

	inline void SetRoomNo( int roomNo ) { m_roomNo = roomNo; }
	inline int GetRoomNo() { return m_roomNo; }

	inline void SetTeam( int team ) { m_Team = team; }
	inline int GetTeam() { return m_Team; }
};





class CharMgr : public SSingleton <CharMgr>
{
private:
	friend class SSingleton<CharMgr>;

private:
	//전체 캐릭터목록
	std::list<Character*>		m_charList;

	//나
	Character*					m_itMe;

	//전체 캐릭터의 수
	int							m_playerCount;


public:
	//view를 받아 논다
	CLobby*						m_pLobby;

public:
	CharMgr(void);
	~CharMgr(void);

	//캐릭터 추가
	void AddChar( int sessionId, TCHAR* id );

	//캐릭터 삭제
	void DelChar( int sessionId );

	//캐릭터 검색
	Character* FindChar( int sessionId );

	inline Character* GetMe() { return m_itMe; }

	//나 빼고 모든 캐릭터 정보를 날린다.
	void ClearCharacter();

	//모든 정보를 지우고list clear까지
	void Release();
};

#define GetCharMgr CharMgr::GetInstance()

