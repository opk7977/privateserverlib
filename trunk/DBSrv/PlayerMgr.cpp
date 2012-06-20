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

	//����Ʈ ����
	m_Rank_Point += point;

	//ID�� point�� �°� ���� ����
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
	// �޾ƿ� ��ũ�� ���� üũ
	if( nowRankID < 1 || nowRankID > 20 )
		return -1;

	//20������ ��ȭ ����
	if( nowRankID == 20 )
		return 20;

	//������ rank���� Ȯ���� �ϱ� �����Ѵ�.
	for( int i=nowRankID; i<=20; ++i )
	{
		if( m_mapRank[i] >= point )
			return --i;
	}

	//ã�� ���ϰ� for���� �����ٸ� 20����
	return 20;
}

int RankDataMgr::GetRank( int point )
{
	//0��ũ���� ã�� �����Ѵ�.
	for( int i=1; i<=20; ++i )
	{
		if( m_mapRank[i] >= point )
			return --i;
	}

	//ã�� ���ϰ� for���� �����ٸ� 20����
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

	//player���� �Ҵ�
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
	//���� ����
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
	//���̻� ������ ������ NULL�� ��ȯ
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
	//�޾ƿ� ������ ������ ������ �׳� ���ư�!
	if( space == NULL )
		return;

	int index = space->GetIndex();

	//map���� �����ְ�
	m_mapPlayer.RemoveKey( space->GetSessionID() );

	//������ �ʱ�ȭ �ϰ�
	space->Init();

	//index�� �ݳ�
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
