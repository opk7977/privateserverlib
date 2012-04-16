#pragma once

#include "SServerStd.h"

#define MIN_PLAYER_COUNT 6
#define MAX_PLAYER_COUNT 8


class Character;
class CLobby;

class RoomObj
{
private:
	//방 번호
	int						m_roomNum;

	//room Title
	TCHAR					m_Title[50];

	//player list
	std::list<Character*>	m_playerListInRoom;

	//현재 인원
	int						m_playerCount;

	//방의 상태 0: 일반/ 1: 게임중
	int						m_roomState;


public:
	RoomObj();
	~RoomObj();

	void Init();

	inline void SetRoomNum( int num ) { m_roomNum = num; }
	inline int GetRoomNum() { return m_roomNum; }

	void SetRoomTitle( TCHAR* title );
	inline TCHAR* GetRoomTitle() { return m_Title; }

	//player관련
	void AddPlayer( Character* character );
	void DelPlayer( Character* character );

	inline void SetRoomState( int state ) { m_roomState = state; }
	inline int GetRoomState() { return m_roomState; }

	void SetPlayerCount( int count );
	inline int GetPlayerCount() { return m_playerCount; }

	Character* FindChar( int sessionId );

};



#define ROOMCOUNT  4



class RoomMgr : public SSingleton <RoomMgr>
{
private:
	friend class SSingleton<RoomMgr>;

private:
	//방list
	std::map<int, RoomObj*>	m_mapRoom;

public:
	//view를 받아 논다
	CLobby*					m_pLobby;

public:
	RoomMgr(void);
	~RoomMgr(void);

	//방생성
	void CreateRoom();
	void Release();	
	
	RoomObj* FindRoom( int roomNo );
};

#define GetRoomMgr RoomMgr::GetInstance()


