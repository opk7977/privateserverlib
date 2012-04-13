#pragma once

#include "SServerStd.h"

class Character;

class Room
{
private:
	//�� ��ȣ
	int						m_roomNum;

	//room Title
	CString					m_Title;

	//player list
	std::list<Character*>	m_playerListInRoom;

	//���� �ο�
	int						m_playerCount;

public:
	Room();
	~Room();

	inline void SetRoomNum( int num ) { m_roomNum = num; }
	inline int GetRoomNum() { return m_roomNum; }

	inline void SetRoomTitle( CString title ) { m_Title = title; }
	inline CString GetRoomTitle() { return m_Title; }

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
	//��list
	std::map<int, Room*>	m_mapRoom;

public:
	RoomMgr(void);
	~RoomMgr(void);

	//�����
	void CreateRoom();

	void Init( int roomNo );
	//void 
};

#define GetRoomMgr RoomMgr::GetInstance()


