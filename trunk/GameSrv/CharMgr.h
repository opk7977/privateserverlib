#pragma once

#include "SIndexQueue.h"
#include "SPacket.h"

class GameSession;
class DataLeader;
class SLogger;

//======================================
// 무적시간
//======================================
#define CHARACTER_INVINCIBLE_TIME			5

//======================================	
// 은신
//======================================	
#define CHARACTER_HIDE_POINT				15
#define CHARACTER_HIDEON_TIME				1.0f
#define CHARACTER_HIDEOFF_TIME				3.0f

//======================================	
// 스캔
//======================================	
#define CHARACTER_SCAN_POINT				15
#define CHARACTER_SCANON_TIME				0.75f
#define CHARACTER_SCANOFF_TIME				3.0f

//======================================	
// HP
//======================================	
#define CHARACTER_HP_INCREASE_POINT			1
#define CHARACTER_HP_INCREASE_TIME			1
#define CHARACTER_HP_MAX_INCREASE_POINT		100

//======================================	
// 게임 결과 정산Point
//======================================	
enum INCREASE_POINT
{
	DEATHMATCH_WINNER_KILL_POINT			= 10,
	DEATHMATCH_LOSER_KILL_POINT				= 5,

	DESTROY_WINNER_KILL_POINT				= 10,
	DESTROY_LOSER_KILL_POINT				= 5,
	DESTROY_WINNER_INCREASE_POINT			= 50,
	DESTROY_LOSER_INCREASE_POINT			= 20,
};

//======================================
// 팀 정보
//======================================
enum SRV_CHAR_TEAM
{
	SRV_CHAR_TEAM_ATT						= 0,
	SRV_CHAR_TEAM_DEF,
};

//======================================	
// 무기_지금은 사용하지 않음...
//======================================	
enum SRV_CHAR_WEAPON
{
	SWEAPON_BLASTER							= 0,
	SWEAPON_MACHINEGUN,
	SWEAPON_DARKMATTER,
	SWEAPON_GRENADE,
	SWEAPON_NAILGUN,
	SWEAPON_RAILGUN,
	SWEAPON_SHOTGUN,
};

//======================================	
// 캐릭터 스킬 사용상태
//======================================	
enum SRV_CHAR_SKILL_STATE
{
	SKILL_NONE								= 100,
	SKILL_HIDE,
	SKILL_SCAN,
};

class CharObj : public SServerObj
{
private:
	GameSession*			m_session;

	int						m_sessionID;
	TCHAR					m_tstrID[50];
	int						m_iTeam;

	//======================================
	//현재의
	int						m_rankID;
	//======================================
	//이번 게임
	int						m_rankPoint;

	int						m_killCount;
	int						m_deathCount;
	//======================================

	//위치
	POINT3					m_pos;

	//======================================
	//에너지
	int						m_HP;
	float					m_increaseHPTime;
	//======================================

	//======================================
	// 무적
	BOOL					m_isInvincible;
	float					m_invincibleTime;
	//======================================

	//======================================
	//은신 수치
	int						m_hidePoint;
	float					m_hideOnTime;
	float					m_hideOffTime;
	//======================================
	//스캔 수치
	int						m_scanPoint;
	float					m_scanOnTime;
	float					m_scanOffTime;
	//======================================
	//스킬 상태
	SRV_CHAR_SKILL_STATE	m_skillState;
	//======================================
	//vec의 Index_공간 관리용 index
	int						m_vecIndex;

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

	inline void SetRankID( int rankID ) { m_rankID = rankID; }
	inline int GetRankID() { return m_rankID; }

	inline void SetRankPoint( int rankPoint ) { m_rankPoint = rankPoint; }
	inline int GetRankPoint() { return m_rankPoint; }

	//공간 관리용 index
	inline int GetVecIndex() { return m_vecIndex; }

	//무적
	inline void SetInvincible() { m_isInvincible = TRUE; }
	inline BOOL IsInvincible() { return m_isInvincible; }
	void CountDownInvincibleTime( float elaps );

	//==============================================================
	//공격을 당해 데이지를 입음
	void DownHP( int damage );
	//내 hp return
	int	GetHP() const { return m_HP; }
	//피가 다 달아서 죽었는지?
	BOOL IsDie();
	//살리기
 	void SetAlive();
	//==============================================================
	//피1 올리기_ return 1 POINT가 올랐다면 TRUE를 return
	BOOL HPUpOnePoint( float elaps );
	//==============================================================

	//==============================================================
	// skill 포인트 증감 변화가 있으면 TRUE를 return한다.
	// 그때 클라에게 전송하면 됨
	BOOL CountSkillPoint( float elaps );
	
	//--------------------------------------------------------------
	inline int GetSkillState() { return m_skillState; }
	void SetSkillNone();
	void SetSkillHide();
	void SetSkillScan();

	inline int GetHidePoint() { return m_hidePoint; }
	inline int GetScanPoint() { return m_scanPoint; }
	//==============================================================

	//죽은 횟수
	int GetDeathCount();

	//죽인 횟수
	void KillCountUp();
	int GetKillCount();

	//======================================
	// pos
	//======================================
	inline void SetPos( float posX, float posY, float posZ ) { m_pos.SetElement( posX, posY, posZ ); }
	inline POINT3 GetPos() { return m_pos; }
	inline float GetPosX() { return m_pos.m_X; }
	inline float GetPosY() { return m_pos.m_Y; }
	inline float GetPosZ() { return m_pos.m_Z; }

	//==================================================
	// 게임한판이 끝나고 지금까지의 결과를 넣는다.
	//==================================================
	void PackageMyInfoForDB( SPacket& packet );
};


class CharMgr : public SSingleton <CharMgr>, public SServerObj
{
private:
	friend class SSingleton<CharMgr>;

private:
	//======================================
	// singleTon객체들
	//======================================
	DataLeader*			m_document;
	SLogger*			m_logger;

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

