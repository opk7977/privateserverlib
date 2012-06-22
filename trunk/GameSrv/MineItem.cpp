#include "MineItem.h"

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

BOOL MineItem::IsCollision( float posX, float posY, float posZ )
{
	//���� ��(ĳ����)
// 	float tmpX = ( m_pos.m_X - posX )*( m_pos.m_X - posX );
// 	float tmpZ = ( m_pos.m_Z - posZ )*( m_pos.m_Z - posZ );
// 
// 	float dist = tmpX + tmpZ;
// 
// 	if( dist <= MINE_COLLISION_ROUND*MINE_COLLISION_ROUND )
// 		return TRUE;
// 	else
// 		return FALSE;

	//���� ��(ĳ����)
 	float tmpX = ( m_pos.m_X - posX )*( m_pos.m_X - posX );
	float tmpZ = ( m_pos.m_Z - posZ )*( m_pos.m_Z - posZ );

	float dist = tmpX + tmpZ;

	if( dist <= (MINE_COLLISION_ROUND+MINE_COLLISION_CHAR_ROUND)*(MINE_COLLISION_ROUND+MINE_COLLISION_CHAR_ROUND) )
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

int MineItem::IsBoomCollision( float posX, float posY, float posZ )
{
	//���� ��(ĳ����)
	float tmpX = ( m_pos.m_X - posX )*( m_pos.m_X - posX );
	float tmpZ = ( m_pos.m_Z - posZ )*( m_pos.m_Z - posZ );

	float dist = tmpX + tmpZ;

	if( dist <= (MINE_BOOM_COL_ROUND+MINE_COLLISION_CHAR_ROUND)*(MINE_BOOM_COL_ROUND+MINE_COLLISION_CHAR_ROUND) )
		return MINE_BOOM_DAMEGE;
	else
		return 0;
}

int MineItem::GetDamege( float posX, float posY, float posZ )
{
	//���� ���� �Ÿ��� ���Ѵ�
	
	return 0;
}
