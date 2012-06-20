#include "PlayerMgr.h"
#include "SSynchronize.h"
#include "SPacket.h"
#include "DBMgr.h"

RankDataMgr*	PlayerObj::m_rankData	= &GetRankMgr;
DBMgr*			PlayerObj::m_dbMgr		= &GetDBMgr;

PlayerObj::PlayerObj(void)
{
}

PlayerObj::~PlayerObj(void)
{
}

void PlayerObj::Init( int index )
{
	m_index = index;

	Init();
}

void PlayerObj::Init()
{
	SSynchronize sync( this );

	m_sessionID = 0;
	ZeroMemory( m_UserID, 10*sizeof(TCHAR) );
	m_Rank_ID = 0;
	m_Rank_Point = 0;
	m_AccumulatedKill = 0;
	m_AccumulatedDeath = 0;
}

void PlayerObj::PackageMyAllInfo( SPacket& packet )
{
	SSynchronize sync( this );

	packet << m_sessionID;
	int size = _tcslen( m_UserID ) * sizeof( TCHAR );
	packet << size;
	packet.PutData( m_UserID, size );
	packet << m_Rank_ID;
	packet << m_Rank_Point;
	packet << m_AccumulatedKill;
	packet << m_AccumulatedDeath;
}

void PlayerObj::PackageMyPointInfo( SPacket& packet )
{
	SSynchronize sync( this );

	packet << m_sessionID;
	packet << m_Rank_ID;
	packet << m_Rank_Point;
	packet << m_AccumulatedKill;
	packet << m_AccumulatedDeath;
}

void PlayerObj::SetPlayer( int sessionID, TCHAR* _id, int rank_id, int rank_point, int AccumulKill, int AccumulDeath )
{
	m_sessionID = sessionID;
	int size = _tcslen(_id)*sizeof(TCHAR);
	_tcsncpy_s( m_UserID, _id, size );
	m_Rank_ID = rank_id;
	m_Rank_Point = rank_point;
	m_AccumulatedKill = AccumulKill;
	m_AccumulatedDeath = AccumulDeath;
}

void PlayerObj::AddRankPoint( int point )
{
	SSynchronize sync( this );

	//포인트 정산
	m_Rank_Point += point;

	//ID를 point에 맞게 새로 셋팅
	m_Rank_ID = m_rankData->GetRank( m_Rank_ID, m_Rank_Point );
}

BOOL PlayerObj::DBDataUpdate()
{
	return m_dbMgr->UpdateUserData( m_sessionID, m_Rank_ID, m_Rank_Point, m_AccumulatedKill, m_AccumulatedDeath );
}

//==============================================================

RankDataMgr::RankDataMgr()
{
}

RankDataMgr::~RankDataMgr()
{
}

BOOL RankDataMgr::Init()
{
	//
	return TRUE;
}

void RankDataMgr::Release()
{
	m_mapRank.RemoveAll();
}

void RankDataMgr::AddData( int rankID, int point )
{
	m_mapRank[rankID] = point;
}

int RankDataMgr::GetRank( int nowRankID, int point )
{
	// 받아온 랭크를 먼저 체크
	if( nowRankID < 1 || nowRankID > 20 )
		return -1;

	//20에서는 변화 없음
	if( nowRankID == 20 )
		return 20;

	//원래의 rank부터 확인을 하기 시작한다.
	for( int i=nowRankID; i<=20; ++i )
	{
		if( m_mapRank[i] >= point )
			return --i;
	}

	//찾지 못하고 for문이 끝났다면 20레벨
	return 20;
}

int RankDataMgr::GetRank( int point )
{
	//0렝크부터 찾기 시작한다.
	for( int i=1; i<=20; ++i )
	{
		if( m_mapRank[i] >= point )
			return --i;
	}

	//찾지 못하고 for문이 끝났다면 20레벨
	return 20;
}

//==============================================================

PlayerMgr::PlayerMgr(void)
{
}

PlayerMgr::~PlayerMgr(void)
{
	Release();
}

BOOL PlayerMgr::Init()
{
	m_indexQ.Create( Character_Space, 0 );

	m_vecPlayerSpace.reserve( Character_Space );

	//player공간 할당
	for( int i=0; i<Character_Space; ++i )
	{
		PlayerObj* tmpPlayer = new PlayerObj;
		tmpPlayer->Init( i );

		m_vecPlayerSpace.push_back( tmpPlayer );
	}

	return TRUE;
}

void PlayerMgr::Release()
{
	//공간 해제
	if( m_vecPlayerSpace.empty() )
		return;

	for( int i=0; i<Character_Space; ++i )
	{
		delete m_vecPlayerSpace[i];
	}
	m_vecPlayerSpace.clear();
	m_mapPlayer.RemoveAll();
}

PlayerObj* PlayerMgr::GetPlayerSpace()
{
	int index = m_indexQ.GetIndex();
	//더이상 공간이 없으면 NULL을 반환
	if( index < 0 )
		return NULL;

	return m_vecPlayerSpace[index];
}

BOOL PlayerMgr::AddPlayer( int sessionid, PlayerObj* player )
{
	m_mapPlayer[sessionid] = player;

	return TRUE;
}

void PlayerMgr::ReturnPlayerSpace( PlayerObj* space )
{
	//받아온 공간에 문제가 있으면 그냥 돌아가!
	if( space == NULL )
		return;

	int index = space->GetIndex();

	//map에서 지워주고
	m_mapPlayer.RemoveKey( space->GetSessionID() );

	//공간은 초기화 하고
	space->Init();

	//index는 반납
	m_indexQ.PutIndex( index );
}

PlayerObj* PlayerMgr::FindPlayerBySessionId( int sessionId )
{
	SSynchronize sync( this );

	PlayerObj* tmpPlayer;
	if( !m_mapPlayer.Lookup( sessionId, tmpPlayer ) )
		return NULL;

	return tmpPlayer;
}
