#pragma once

#include "SServerStd.h"

class Character;

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

public:
	RoomObj();
	~RoomObj();

	inline void SetRoomNum( int num ) { m_roomNum = num; }
	inline int GetRoomNum() { return m_roomNum; }

	inline void SetRoomTitle( TCHAR* title ) { _tcsncpy_s( m_Title, 50, title, _tcslen(title) ); }
	inline TCHAR* GetRoomTitle() { return m_Title; }

	//
	void AddPlayer( Character* character );
	void DelPlayer( Character* character );
	BOOL PlayerIs( Character* character );

	inline int GetPlayerCount() { return m_playerCount; }

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
	RoomMgr(void);
	~RoomMgr(void);

	//방생성
	void CreateRoom();

	void Init( int roomNo );
	//void 
};

#define GetRoomMgr RoomMgr::GetInstance()


