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

	//���� ��ġ
	m_pos.SetElement( posX, posY, posZ );
	m_boomTime			= BOOM_TIME;

	//�ѹ� ��������� �ٽ� ����� �� ����.
	m_isUseable			= FALSE;

	return TRUE;
}

BOOL MineItem::IsBoom()
{
	//���� �ð��� 0���� �۰ų� ������ ������
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
	//���� ���� �Ÿ��� ���Ѵ�
	
	return 0;
}
