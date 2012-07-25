#include "MineItem.h"
#include "SSynchronize.h"

MineItem::MineItem(void)
{
}

MineItem::~MineItem(void)
{
}

void MineItem::Init( int index )
{
	m_vecIndex = index;

	Init();
}

void MineItem::Init()
{
	m_masterSessionID	= 0;
	m_masterTeam		= -1;

	Reset();
}

void MineItem::Reset()
{
	m_mineCount			= MINE_COUNT;

	m_isInstall			= FALSE;
	m_isRun				= FALSE;
	m_pos.Clear();
	m_dir.Clear();
	m_boomTime			= BOOM_TIME;
}

BOOL MineItem::CanUse()
{
	SSynchronize sync( this );

	//지미 지뢰가 설치 되어 있으면 사용할 수 없음
	if( m_isInstall )
		return FALSE;

	//지뢰사용횟수를 넘었으면 사용할 수 없음
	if( m_mineCount <= 0 )
		return FALSE;

	//그렇지 않으면 설치 할 수 있음
	return TRUE;
}

void MineItem::SetMineSpace( int sessionId, int team )
{
	SSynchronize sync( this );

	m_masterSessionID	= sessionId;
	m_masterTeam		= team;
}

BOOL MineItem::SetMine( float posX, float posY, float posZ, float dirX, float dirY, float dirZ )
{
	SSynchronize sync( this );

	//지뢰 설치
	m_pos.SetElement( posX, posY, posZ );
	m_dir.SetElement( dirX, dirY, dirZ );
	m_boomTime			= BOOM_TIME;

	//지뢰 사용횟수를 줄인다
	--m_mineCount;

	return TRUE;
}

BOOL MineItem::IsBoom()
{
	SSynchronize sync( this );

	//남은 시간이 0보다 작거나 같으면 터진다
	if( m_boomTime <= 0 )
		return TRUE;

	return FALSE;
}

BOOL MineItem::IsCollision( float posX, float posY, float posZ )
{
	//원과 점(캐릭터)
// 	float tmpX = ( m_pos.m_X - posX )*( m_pos.m_X - posX );
// 	float tmpZ = ( m_pos.m_Z - posZ )*( m_pos.m_Z - posZ );
// 
// 	float dist = tmpX + tmpZ;
// 
// 	if( dist <= MINE_COLLISION_ROUND*MINE_COLLISION_ROUND )
// 		return TRUE;
// 	else
// 		return FALSE;

	//원과 원(캐릭터)
//  	float tmpX = ( m_pos.m_X - posX )*( m_pos.m_X - posX );
// 	float tmpZ = ( m_pos.m_Z - posZ )*( m_pos.m_Z - posZ );
// 
// 	float dist = tmpX + tmpZ;
// 
// 	if( dist <= (MINE_COLLISION_ROUND+MINE_COLLISION_CHAR_ROUND)*(MINE_COLLISION_ROUND+MINE_COLLISION_CHAR_ROUND) )
// 		return TRUE;
// 	else
// 		return FALSE;
	SSynchronize sync( this );

	//반구와 구(캐릭터)
	//캐릭터 키 반만큼 위로 올려 준다
	float charY = posY + MINE_COLLISION_CHAR_ROUND;
	float tmpX = ( m_pos.m_X - posX )*( m_pos.m_X - posX );
	float tmpY = ( m_pos.m_Y - charY )*( m_pos.m_Y - charY );
	float tmpZ = ( m_pos.m_Z - posZ )*( m_pos.m_Z - posZ );

	float dist = tmpX + tmpY + tmpZ;

	//우선 구와 구의 충돌을 체크해서 충돌이 아니면 -1
	if( dist > (MINE_COLLISION_ROUND+MINE_COLLISION_CHAR_ROUND)*(MINE_COLLISION_ROUND+MINE_COLLISION_CHAR_ROUND) )
		return FALSE;

	//충돌이면 내적계산해서 다시 충돌체크
	if( m_dir.DotVector( posX-m_pos.m_X, charY-m_pos.m_Y, posZ-m_pos.m_Z ) < 0 )
		return FALSE;

	//진짜 충돌이면 
	return TRUE;	
}

void MineItem::DownTimer( float ElapsedTime )
{
	m_boomTime -= ElapsedTime;
}

void MineItem::Boom()
{
	SSynchronize sync( this );

	m_boomTime = 0;
	SetRun();
}

int MineItem::IsBoomCollision( float posX, float posY, float posZ )
{
	SSynchronize sync( this );

	//반구와 구(캐릭터)
	//캐릭터 키 반만큼 위로 올려 준다
	float charY = posY + MINE_COLLISION_CHAR_ROUND;
	float tmpX = ( m_pos.m_X - posX )*( m_pos.m_X - posX );
	float tmpY = ( m_pos.m_Y - charY )*( m_pos.m_Y - charY );
	float tmpZ = ( m_pos.m_Z - posZ )*( m_pos.m_Z - posZ );

	float dist = tmpX + tmpY + tmpZ;

	//우선 구와 구의 충돌을 체크해서 충돌이 아니면 -1
	if( dist > (MINE_BOOM_COL_ROUND+MINE_COLLISION_CHAR_ROUND)*(MINE_BOOM_COL_ROUND+MINE_COLLISION_CHAR_ROUND) )
		return 0;

	//충돌이면 내적계산해서 방향 충돌체크
	if( m_dir.DotVector( posX-m_pos.m_X, charY-m_pos.m_Y, posZ-m_pos.m_Z ) < 0 )
		return 0;

	//진짜 충돌이면 데미지를 return
	//return (100-(dir*100/(MINE_COLLISION_CHAR_ROUND+MINE_BOOM_COL_ROUND)));
	return MINE_BOOM_DAMEGE;
	
}
