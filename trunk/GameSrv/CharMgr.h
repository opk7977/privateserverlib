#pragma once

#include "SIndexQueue.h"
#include "SPacket.h"

class GameSession;
class DataLeader;
class SLogger;

//const int Character_Space = 100;
enum INCREASE_POINT
{
	DEATHMATCH_WINNER_KILL_POINT	= 10,
	DEATHMATCH_LOSER_KILL_POINT		= 5,
	MISSION_WINNER_KILL_POINT		= 2,
	MISSION_LOSER_KILL_POINT		= 1,
	MISSION_WINNER_INCREASE_POINT	= 50,
};

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

enum SRV_CHAR_WEAPON
{
	SWEAPON_BLASTER		= 0,
	SWEAPON_MACHINEGUN,
	SWEAPON_DARKMATTER,
	SWEAPON_GRENADE,
	SWEAPON_NAILGUN,
	SWEAPON_RAILGUN,
	SWEAPON_SHOTGUN,
};


class CharObj : public SServerObj
{
private:
	GameSession*		m_session;

	int					m_sessionID;
	TCHAR				m_tstrID[50];
	int					m_iTeam;

	//������
	int					m_rankID;
// 	int					m_untilRankPoint;
// 	int					m_accumulKillCount;
// 	int					m_accumulDeathCount;

	//����
	int					m_weapon[2];
	int					m_onWeapon;
	
	//======================================
	//�̹� ����
	int					m_rankPoint;

	int					m_killCount;
	int					m_deathCount;

	//======================================
	//��ġ
	POINT3				m_pos;

	//������
	int					m_HP;

	//���� ��ġ ���
	//
	//

	//vec�� Index_���� ������ index
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

	inline void SetRankID( int rankID ) { m_rankID = rankID; }
	inline int GetRankID() { return m_rankID; }

	inline void SetRankPoint( int rankPoint ) { m_rankPoint = rankPoint; }
	inline int GetRankPoint() { return m_rankPoint; }
// 	inline int GetUntileRankPoint() { return m_untilRankPoint; }

// 	inline void SetAccumulKillCount( int killCount ) { m_accumulKillCount = killCount; }
// 	inline int GetAccumulKillCount() { return m_accumulKillCount; }
// 
// 	inline void SetAccumulDeathCount( int deathCount ) { m_accumulDeathCount = deathCount; }
// 	inline int GetAccumulDeathCount() { return m_accumulDeathCount; }

	void SetWeapon( int first, int second );
	inline void SetOnWeapon( int weapon ) { m_onWeapon = weapon; }
	inline int GetWeapon() { return m_onWeapon; }

	//���� ������ index
	inline int GetVecIndex() { return m_vecIndex; }

	//������ ���� �������� ����
	void DownHP( int damage );
	//�� hp return
	int	GetHP() const { return m_HP; }
	//�ǰ� �� �޾Ƽ� �׾�����?
	BOOL IsDie();
	//�츮��
// 	void SetAlive();
	void SetAlive() { m_HP = 100; }
	//��1 �ø���_ return FALSE�� ���� �ǰ� 100�̶� �ø��� ���� ���
	BOOL HPUpOnePoint();

	//���� Ƚ��
	//void DeathCountUp();
	int GetDeathCount();

	//���� Ƚ��
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
	// ���������� ������ ���ݱ����� ����� �ִ´�.
	//==================================================
	void PackageMyInfoForDB( SPacket& packet );
//	void PackageCharInfoForResult( SPacket& packet );
};


class CharMgr : public SSingleton <CharMgr>, public SServerObj
{
private:
	friend class SSingleton<CharMgr>;

private:
	//======================================
	// singleTon��ü��
	//======================================
	DataLeader*			m_document;
	SLogger*			m_logger;

private:
	//ĳ���� ����
	std::vector<CharObj*>		m_vecCharSpace;
	//���� ������ index
	SIndexQueue					m_IndexQ;

private:
	CharMgr(void);
	~CharMgr(void);

public:
	//������ ����
	void Init();
	void Release();

	//ĳ���� ������ �Ҵ�
	CharObj* GetCharSpace();

	//ĳ���� ������ �ݳ�
	void ReturnCharSpace( CharObj* charspace );

	//�ش� index�� �ش��ϴ� character�� �޾� ����.
	CharObj* FindCharAsIndex( int index );
	CharObj* FindCharAsSessionId( int sessionID );

};

#define GetCharMgr CharMgr::GetInstance()

