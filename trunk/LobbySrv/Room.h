#pragma once

#include "SServerObj.h"

//방의 정원
#define MIN_PLAYER_COUNT 6
#define MAX_PLAYER_COUNT 8


class Room : public SServerObj
{
private:
	//<sessionId, MgrIndex>
	std::map<int, int>		m_listPlayer;

	int						m_nowPleyrCount;		//현재 들어와 있는 사람들의 수
	int						m_readyCount;			//준비상태의 캐릭터 수
	int						m_leader;				//방장의 세션 번호

	BOOL					m_visible;				//방이 만들어져 있는 방인지?
	BOOL					m_isPlay;				//현재 게임중인 방인지

	TCHAR					*m_tstrRoomTitle;		//방 문구

	int						m_AttectTeam;
	int						m_DefenceTeam;


public:
	Room(void);
	~Room(void);

	void Init();

	inline BOOL IsVisible() { return m_visible; }
	inline void SetTitle( TCHAR* title ) { m_tstrRoomTitle = title; }
	inline int GetPlayerCount() { return m_nowPleyrCount; }
	inline int GetReadyCount() { return m_readyCount; }
	
	//게임을 진행 할 수 있는 상태 인가를 확인해 준다.
	BOOL PossiblePlay();

	void SetIndex( int sessionId, int Index );

	BOOL Play();

	//방에 있는 모든 이에게 packet을 보낸다.
	void SendPacketAllInRoom( SPacket &packet );
};
