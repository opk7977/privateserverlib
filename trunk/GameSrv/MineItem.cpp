#include "MineItem.h"
#include "myMath.h"

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
	m_isUseable			= TRUE;
	m_isRun				= FALSE;
	m_pos.Clear();
	m_boomTime			= BOOM_TIME;
}

void MineItem::SetMineSpace( int sessionId, int team )
{
	m_masterSessionID	= sessionId;
	m_masterTeam		= team;
}

BOOL MineItem::SetMine( float posX, float posY, float posZ )
{
	if( !m_isUseable )
		return FALSE;

	//지뢰 설치
	m_pos.SetElement( posX, posY, posZ );
	m_boomTime			= BOOM_TIME;

	//한번 사용했으면 다시 사용할 수 없다.
	m_isUseable			= FALSE;

	return TRUE;
}

BOOL MineItem::IsBoom()
{
	//남은 시간이 0보다 작거나 같으면 터진다
	if( m_boomTime <= 0 )
		return TRUE;

	return FALSE;
}

BOOL MineItem::IsCollision( float posX, float posZ )
{
	float tmpX = myAbs( m_pos.m_X - posX );
	float tmpZ = myAbs( m_pos.m_Z - posZ );

	int dist = tmpX*tmpX + tmpZ*tmpZ;

	if( dist <= MINE_COLLISION_ROUND*MINE_COLLISION_ROUND )
		return TRUE;
	else
		return FALSE;
}

void MineItem::DownTimer( float ElapsedTime )
{
	m_boomTime -= ElapsedTime;
}

void MineItem::Boom()
{
	m_boomTime = 0;
}

int MineItem::IsBoomCollision( float posX, float posZ )
{
	float tmpX = myAbs( m_pos.m_X - posX );
	float tmpZ = myAbs( m_pos.m_Z - posZ );

	int dist = tmpX*tmpX + tmpZ*tmpZ;

	if( dist <= MINE_BOOM_COL_ROUNT*MINE_BOOM_COL_ROUNT )
		return MINE_BOOM_DAMEGE;
	else
		return 0;
}

int MineItem::GetDamege( float posX, float posZ )
{
	//점과 점의 거리를 구한다
	
	return 0;
}
