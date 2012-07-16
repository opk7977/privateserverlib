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
	m_isInstall			= FALSE;
	m_isRun				= FALSE;
	m_pos.Clear();
	m_dir.Clear();
	m_boomTime			= BOOM_TIME;
}

void MineItem::SetMineSpace( int sessionId, int team )
{
	m_masterSessionID	= sessionId;
	m_masterTeam		= team;
}

BOOL MineItem::SetMine( float posX, float posY, float posZ, float dirX, float dirY, float dirZ )
{
	if( !m_isUseable )
		return FALSE;

	//���� ��ġ
	m_pos.SetElement( posX, posY, posZ );
	m_dir.SetElement( dirX, dirY, dirZ );
	m_boomTime			= BOOM_TIME;
	//m_isInstall			= TRUE;

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
//  	float tmpX = ( m_pos.m_X - posX )*( m_pos.m_X - posX );
// 	float tmpZ = ( m_pos.m_Z - posZ )*( m_pos.m_Z - posZ );
// 
// 	float dist = tmpX + tmpZ;
// 
// 	if( dist <= (MINE_COLLISION_ROUND+MINE_COLLISION_CHAR_ROUND)*(MINE_COLLISION_ROUND+MINE_COLLISION_CHAR_ROUND) )
// 		return TRUE;
// 	else
// 		return FALSE;

	//�ݱ��� ��(ĳ����)
	//ĳ���� Ű �ݸ�ŭ ���� �÷� �ش�
	float charY = posY + MINE_COLLISION_CHAR_ROUND;
	float tmpX = ( m_pos.m_X - posX )*( m_pos.m_X - posX );
	float tmpY = ( m_pos.m_Y - charY )*( m_pos.m_Y - charY );
	float tmpZ = ( m_pos.m_Z - posZ )*( m_pos.m_Z - posZ );

	float dist = tmpX + tmpY + tmpZ;

	//�켱 ���� ���� �浹�� üũ�ؼ� �浹�� �ƴϸ� -1
	if( dist > (MINE_COLLISION_ROUND+MINE_COLLISION_CHAR_ROUND)*(MINE_COLLISION_ROUND+MINE_COLLISION_CHAR_ROUND) )
		return FALSE;

	//�浹�̸� ��������ؼ� �ٽ� �浹üũ
	if( m_dir.DotVector( posX-m_pos.m_X, charY-m_pos.m_Y, posZ-m_pos.m_Z ) < 0 )
		return FALSE;

	//��¥ �浹�̸� 
	return TRUE;	
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
	//�ݱ��� ��(ĳ����)
	//ĳ���� Ű �ݸ�ŭ ���� �÷� �ش�
	float charY = posY + MINE_COLLISION_CHAR_ROUND;
	float tmpX = ( m_pos.m_X - posX )*( m_pos.m_X - posX );
	float tmpY = ( m_pos.m_Y - charY )*( m_pos.m_Y - charY );
	float tmpZ = ( m_pos.m_Z - posZ )*( m_pos.m_Z - posZ );

	float dist = tmpX + tmpY + tmpZ;

	//�켱 ���� ���� �浹�� üũ�ؼ� �浹�� �ƴϸ� -1
	if( dist > (MINE_BOOM_COL_ROUND+MINE_COLLISION_CHAR_ROUND)*(MINE_BOOM_COL_ROUND+MINE_COLLISION_CHAR_ROUND) )
		return 0;

	//�浹�̸� ��������ؼ� ���� �浹üũ
	if( m_dir.DotVector( posX-m_pos.m_X, charY-m_pos.m_Y, posZ-m_pos.m_Z ) < 0 )
		return 0;

	//��¥ �浹�̸� �������� return
	//return (100-(dir*100/(MINE_COLLISION_CHAR_ROUND+MINE_BOOM_COL_ROUND)));
	return MINE_BOOM_DAMEGE;
	
}
