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
	//방을 게임중으로 만들고

	//방의 모든 플레이어의 세션에 표시한다.

	//방의 모든 플레이어에게 게임서버로 이동하라는 페킷을 보낸다.

	return TRUE;
}

void Room::SendPacketAllInRoom( SPacket &packet )
{

}

BOOL Room::PossiblePlay()
{
	//모두 ready상태가 아니면 안됨
	if( m_nowPleyrCount == m_readyCount )
		return FALSE;

	//최소인원보다 적으면 안됨
	if( m_nowPleyrCount < MIN_PLAYER_COUNT )
		return FALSE;

	//팀의 비율이 다르면 안됨
	if( m_AttectTeam == m_DefenceTeam )
		return FALSE;

	return TRUE;
}
