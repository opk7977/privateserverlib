#pragma once

#include "SIndexQueue.h"
#include "GameObj.h"
#include "SPacket.h"

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



class CharObj : public GameObj
{
private:
	TCHAR		m_tstrID[50];
	int			m_iTeam;
	int			m_State;
	POINT3		m_Direction;
	int			m_DirInt;

	//에너지
	int			m_HP;

	//은신 수치 등등
	//
	//

	//vec의 Index
	int			m_vecIndex;

public:
	CharObj();
	~CharObj();

	void Init();
	void Init( int index );

	inline void SetID( TCHAR* id ) { _tcsncpy_s( m_tstrID, 50, id, _tcslen(id) ); }
	inline TCHAR* GetID() { return m_tstrID; }

	inline void SetTeam( int team ) { m_iTeam = team; }
	inline int GetTeam() { return m_iTeam; }

	inline void SetState( int state ) { m_State = state; }
	inline int GetState() { return m_State; }

	inline void SetDirection( float x, float y, float z ) { m_Direction.SetElement( x, y, z ); }
	inline void SetDirection( POINT3 dir ) { m_Direction = dir; }
	inline void SetDirInt( int dir ) { m_DirInt = dir; }
	inline POINT3 GetDirection() const { return m_Direction; }
	inline float GetDirX() { return m_Direction.m_X; }
	inline float GetDirY() { return m_Direction.m_Y; }
	inline float GetDirZ() { return m_Direction.m_Z; }
	inline int GetDirInt() { return m_DirInt; }

	inline int GetVecIndex() { return m_vecIndex; }

	//자신의 정보를 패킷에 넣는다
	void PackageMyInfo( SPacket& packet );

	//공격을 당해 데이지를 입음
	void DownHP( int damage );
	//피가 다 달아서 죽었는지?
	BOOL IsDie();
	
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

