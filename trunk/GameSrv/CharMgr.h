#pragma once

#include "SIndexQueue.h"

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

struct POINT3
{
	float m_X;
	float m_Y;
	float m_Z;

	POINT3() {}
	POINT3( float x, float y, float z )
	{
		m_X = x;
		m_Y = y;
		m_Z = z;
	}
	~POINT3() {}

	void Clear()
	{
		m_X = m_Y = m_Z = 0.f;
	}

	void SetElement( float x, float y, float z )
	{
		m_X = x;
		m_Y = y;
		m_Z = z;
	}
};

class CharObj
{
private:
	int			m_iSessionId;
	TCHAR		m_ID[50];
	int			m_iTeam;
	int			m_State;
	POINT3		m_Position;
	POINT3		m_Direction;

public:
	CharObj();
	~CharObj();

	void Init();

	void SetSessionId( int session ) { m_iSessionId = session; }
	int GetSessionId() { return m_iSessionId; }

	void SetID( TCHAR* id ) { _tcsncpy_s( m_ID, 50, id, _tcslen(id) ); }
	TCHAR* GetID() { return m_ID; }

	void SetTeam( int team ) { m_iTeam = team; }
	int GetTeam() { return m_iTeam; }

	void SetState( int state ) { m_State = state; }
	int GetState() { return m_State; }

	void SetPosition( float x, float y, float z ) { m_Position.SetElement( x, y, z ); }
	POINT3 GetPosition() { return m_Position; }
	float GetPosX() { return m_Position.m_X; }
	float GetPosY() { return m_Position.m_Y; }
	float GetPosZ() { return m_Position.m_Z; }

	void SetDirection( float x, float y, float z ) { m_Direction.SetElement( x, y, z ); }
	POINT3 GetDirection() { return m_Direction; }
	float GetDirX() { return m_Direction.m_X; }
	float GetDirY() { return m_Direction.m_Y; }
	float GetDirZ() { return m_Direction.m_Z; }
};


class CharMgr : public SSingleton <CharMgr>
{
private:
	friend class SSingleton<CharMgr>;

private:
	//캐릭터 공간
	std::vector<CharObj*>		m_vecCharSpace;
	//공간 관리용 index
	SIndexQueue						m_IndexQ;

private:
	CharMgr(void);
	~CharMgr(void);

public:
	//생성과 해제
	void Init();
	void Release();

	//캐릭터 index를 할당 받는다
	int GetCharIndex();
	//캐릭터의 접속종료등으로 인해 index를 반납한다
	void ReturnIndex( int index );
	//해당 index에 해당하는 character를 받아 간다.
	CharObj* FindChar( int index );

};

#define GetCharMgr CharMgr::GetInstance()

