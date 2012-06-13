#pragma once

#include "SIndexQueue.h"

class RankDataMgr;
class SPacket;

//--------------------------------------
// player
//--------------------------------------
class PlayerObj : public SServerObj
{
private:
	//======================================
	// singleTon 객체들
	//======================================
	static RankDataMgr*			m_rankData;
	//======================================
	int		m_sessionID;
	TCHAR	m_UserID[10];
	int		m_Rank_ID;
	int		m_Rank_Point;
	int		m_AccumulatedKill;
	int		m_AccumulatedDeath;

	//map index
	int		m_index;

public:
	PlayerObj(void);
	~PlayerObj(void);

	void Init( int index );
	void Init();

	inline int GetIndex() { return m_index; }

	void PackageMyAllInfo( SPacket& packet );
	void PackageMyPointInfo( SPacket& packet );

	void SetPlayer( int sessionID, TCHAR* _id, int rank_id, int rank_point, int AccumulKill, int AccumulDeath );

	inline int GetSessionID() const { return m_sessionID; }
	inline TCHAR* GetUserID() { return &m_UserID[0]; }
	inline int GetRankID() const { return m_Rank_ID; }
	inline int GetRankPoint() const { return m_Rank_Point; }
	inline int GetAccumulKillCount() const { return m_AccumulatedKill; }
	inline int GetAccumulDeathCount() const { return m_AccumulatedDeath; }

	inline void AddKillCount( int killCount ) { m_AccumulatedKill += killCount; }
	inline void AddDeathCount( int deathCount ) { m_AccumulatedDeath += deathCount; }

	void AddRankPoint( int point );
};

//======================================
//--------------------------------------
// rankDataMgr
//--------------------------------------
class RankDataMgr : public SSingleton <RankDataMgr>
{
private:
	friend class SSingleton<RankDataMgr>;

private:
	ATL::CAtlMap<int, int>			m_mapRank;

private:
	RankDataMgr();
	~RankDataMgr();

public:
	BOOL Init();
	void Release();

	void AddData( int rankID, int point );

	int GetRank( int nowRankID, int point );
	int GetRank( int point );
};

#define GetRankMgr RankDataMgr::GetInstance()

//======================================
//--------------------------------------
// playerMgr
//--------------------------------------
const int Character_Space = 150;


class PlayerMgr : public SSingleton <PlayerMgr>, public SServerObj
{
private:
	friend class SSingleton<PlayerMgr>;

private:
	//캐릭터 공간
	std::vector<PlayerObj*>		m_vecPlayerSpace;
	//공간관리용 index
	SIndexQueue					m_indexQ;
	//접속(?)해 있는 캐릭터 list
	//SList<PlayerObj*>			m_listPlayer;
	CAtlMap<int, PlayerObj*>	m_mapPlayer;

private:
	PlayerMgr(void);
	~PlayerMgr(void);

public:
	BOOL Init();
	void Release();

	//캐릭터 공간을 할당
	PlayerObj* GetPlayerSpace();
	//map에 player를 추가
	BOOL AddPlayer( int sessionid, PlayerObj* player );

	//캐릭터 공간을 반납
	void ReturnPlayerSpace( PlayerObj* space );

	//sessionId로 character를 받아 간다.
	PlayerObj* FindPlayerBySessionId( int sessionId );
};

#define GetPlayerMgr PlayerMgr::GetInstance()
