#pragma once

#include "SServerStd.h"

class Character
{
private:
	//세션 아이디
	int			m_sessionId;

	//닉네임
	CString		m_csId;

	//내가 속한 방(0:로비)
	int			m_roomNo;

public:
	Character() {};
	~Character() {};

	inline void SetSessionID( int indexId ) { m_sessionId = indexId; }
	inline int GetSessionID() { return m_sessionId; }

	inline void SetID( CString id ) { m_csId = id; }
	inline CString GetID() { return m_csId; }

	inline void SetRoomNo( int roomNo ) { m_roomNo = roomNo; }
	inline int GetRoomNo() { return m_roomNo; }
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
	CharMgr(void);
	~CharMgr(void);

	//캐릭터 추가
	void AddChar( int sessionId, CString id );

	//캐릭터 삭제
	void DelChar( int sessionId );

	//캐릭터 검색
	Character* FindChar( int sessionId );
};

#define GetCharMgr CharMgr::GetInstance()

