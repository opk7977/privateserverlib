#include "Room.h"

Room::Room(void)
{
	Init();
}

Room::~Room(void)
{
}

void Room::Init()
{
	m_listPlayer.clear();

	m_nowPleyrCount = 0;
	m_leader = 0;

	m_visible = FALSE;

	m_tstrRoomTitle = NULL;

	m_AttectTeam = m_DefenceTeam = 0;
}

void Room::SetIndex( int sessionId, int Index )
{
	m_listPlayer[sessionId] = Index;
}

BOOL Room::Play()
{
	//���� ���������� �����

	//���� ��� �÷��̾��� ���ǿ� ǥ���Ѵ�.

	//���� ��� �÷��̾�� ���Ӽ����� �̵��϶�� ��Ŷ�� ������.

	return TRUE;
}

void Room::SendPacketAllInRoom( SPacket &packet )
{

}

BOOL Room::PossiblePlay()
{
	//��� ready���°� �ƴϸ� �ȵ�
	if( m_nowPleyrCount == m_readyCount )
		return FALSE;

	//�ּ��ο����� ������ �ȵ�
	if( m_nowPleyrCount < MIN_PLAYER_COUNT )
		return FALSE;

	//���� ������ �ٸ��� �ȵ�
	if( m_AttectTeam == m_DefenceTeam )
		return FALSE;

	return TRUE;
}
