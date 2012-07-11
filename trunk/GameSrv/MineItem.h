#pragma once

#include "SServerObj.h"

#define BOOM_TIME 0.5

//사람 충돌 반경
#define MINE_COLLISION_CHAR_ROUND	35
//지뢰 충돌 반경
#define MINE_COLLISION_ROUND		20
//지뢰 폭발 반경
#define MINE_BOOM_COL_ROUND			50
//캐릭터 구 반경
// #define MINE_CHAR_ROUND				

//데미지
#define MINE_BOOM_DAMEGE			100

class MineItem : public SServerObj
{
private:
	//이미 사용한애면 FALSE
	BOOL		m_isUseable;

	//설치 했나?
	BOOL		m_isInstall;

 	//터질 준비중?
 	BOOL		m_isRun;

	int			m_masterSessionID;
	int			m_masterTeam;
	POINT3		m_pos;
	POINT3		m_dir;

	//실행(?)되고 터지기까지의 시간, 기본 0.5
	float		m_boomTime;

	//벡터 공간 관리용
	int			m_vecIndex;

public:
	MineItem(void);
	~MineItem(void);

	void Init( int index );
	void Init();

	void Reset();

	inline BOOL CanUse() { return m_isUseable; }

	inline int GetSessionID() { return m_masterSessionID; }
	inline int GetTeam() { return m_masterTeam; }

	int GetVecIndex() { return m_vecIndex; }
	void SetMineSpace( int sessionId, int team );
	//지뢰를 설치, 설치에 실패하면 FALSE를 return
	BOOL SetMine( float posX, float posY, float posZ, float dirX, float dirY, float dirZ );
	inline float GetX() { return m_pos.m_X; }
	inline float GetY() { return m_pos.m_Y; }
	inline float GetZ() { return m_pos.m_Z; }

	//터질 준비 중?
	inline BOOL IsRun() { return m_isRun; }
	inline void SetRun() { m_isRun = TRUE; }

	//지뢰 설치됨?
	inline void SetInstall() { m_isInstall = TRUE; }
	inline BOOL IsInstall() { return m_isInstall; }

	//지뢰 터짐
	inline void SetExplosion()
	{
		m_isInstall = FALSE;
		m_isRun		= FALSE;
	}
	
	//지금 터져도 되는 조건인지?
	//터져도 되면 TRUE를 return
	BOOL IsBoom();

	//캐릭터와의 지뢰 충돌체크
	BOOL IsCollision( float posX, float posY, float posZ );

	//터질예정에 있는 지뢰들의 시간을 줄임
	void DownTimer( float ElapsedTime );
	//강제로 폭파
	void Boom();

	//캐릭터와 폭발범위의 충돌체크
	//return 되는 값은 damege )
	int IsBoomCollision( float posX, float posY, float posZ );
};
