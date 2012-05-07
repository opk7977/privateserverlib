#pragma once

#include "SServerObj.h"
#include "SIndexQueue.h"

class SPacket;
class LobbyChar;
class LobbySession;


//방의 정원
#define MIN_PLAYER_COUNT 6
#define MAX_PLAYER_COUNT 8

#define ROOMCOUNT		4

enum ROOM_STATE
{
	ROOM_STATE_READY = -1,
	ROOM_STATE_NORMAL,
	ROOM_STATE_PLAYING,
};

class Room : public SServerObj
{
private:
	std::list<LobbyChar*>		m_listPlayer;

	int							m_roomNum;

	int							m_nowPleyrCount;		//현재 들어와 있는 사람들의 수
	int							m_readyCount;			//준비상태의 캐릭터 수

	LobbyChar*					m_leader;				//방장의 세션 번호

	BOOL						m_visible;				//방이 만들어져 있는 방인지?
//	BOOL						m_isPlay;				//현재 게임중인 방인지
	int							m_roomState;			//방의 현재 상태

	TCHAR						m_tstrRoomTitle[50];	//방 문구

	int							m_AttectTeam;
	int							m_DefenceTeam;


public:
	Room(void);
	~Room(void);
	
	//방초기화
	void Init();
	void Init( int i );

	//--------------------------------------
	// 방 관련
	//--------------------------------------
	//방번호를 return
	inline int GetRoomNum() const { return m_roomNum; }
	//방이 열려 있는지를 확인
	inline BOOL IsOpen() { return m_visible; }
	//title 관련
	inline void SetTitle( TCHAR* title )
	{
		_tcsncpy_s( m_tstrRoomTitle, 50, title, _tcslen( title ) );
		m_visible = TRUE;
	}
	inline TCHAR* GetTitle() { return m_tstrRoomTitle; }
	//게임을 진행 할 수 있는 상태 인가를 확인해 준다.
	BOOL PossiblePlay();
	//방을 play상태로 만들어 준다.
	BOOL SetPlay();
	inline void ListReset() { m_listPlayer.clear(); }
	//방을 ready상태로 만들어 준다
	BOOL SetReady();
	//방을 다시 일반 상태로
	void SetNormal() { m_roomState = ROOM_STATE_NORMAL; }
	//현재 방으로 들어가는것이 가능한지
	BOOL CanInsert();

	//--------------------------------------
	// player관련
	//--------------------------------------
	//들어와 있는 인원 확인
	inline int GetPlayerCount() { return m_nowPleyrCount; }
	//ready상태의 인원 확인
	inline int GetReadyCount() { return m_readyCount; }

	//리더 설정
	void SetLeader( LobbyChar* sessionId );
	LobbyChar* GetLeader() { return m_leader; }
	LobbyChar* ChangeLeader();

	//방에 들어있는 player의 세션 주소를 변경해 준다
	BOOL SetPlayerSession( LobbySession* session, LobbyChar* charSpace );

	//player추가
	void AddPlayerInRoom( LobbyChar* charspace );
	//player제거
	// return 값이 FALSE이면 방에 사람이 모두 나갔다는 뜻이다.
	BOOL DelPlayerInRoom( LobbyChar* charspace );
	BOOL DelPlayerInRoom( int sessionId );

	//매개변수는 증가해야 하는 상태값을 나타낸다
	//ex) 누군가 ready상태가 되면 TRUE를 넘겨 ready값을 증가
	void ChangReadyState( BOOL isReady );
	
	//팀을 변경
	//증가하는 팀을 매개변수로 받는다
	//ex) TRUE이면 공격팀을 올리고 수비팀을 내린다
	void ChangeTeam( BOOL isATT );

	//팀을 할당 받음
	//0 : 공격
	//1 : 수비
	int GetTeam();

	//--------------------------------------
	// 전송
	//--------------------------------------
	//방 자신의 정보를 넣는다.
	void PackageRoomInfo( SPacket &packet );
	//방에 있는 모든 player의 정보를 담는다( team과 ready를 모두 담는다 )
	void PackagePlayerInRoom( SPacket &packet, LobbyChar* itMe = NULL );
	//방에 있는 모든 player의 정보를 담는다( team까지만 담는다 )
	void PackagePlayerInRoomForGame( SPacket &packet, LobbyChar* itMe = NULL );
	//방에 있는 모든 이에게 packet을 보낸다.
	void SendPacketAllInRoom( SPacket &packet, LobbyChar* itMe = NULL );
};

//////////////////////////////////////////////////////////////////////////

class RoomMgr : public SSingleton <RoomMgr>
{
private:
	friend class SSingleton<RoomMgr>;

private:
	//std::map<int, Room*>			m_mapRoomlist;
	std::vector<Room*>				m_vecRoom;
	std::list<Room*>				m_listOpenRoom;
	SIndexQueue						m_roomIndexQ;

	//열려 있는 방의 개수
	int								m_iOpenRoomCount;

	//동기화 용
	SServerObj*						m_critical;

private:
	RoomMgr();
	~RoomMgr();

public:
	void CreateRoomSpace();
	void Release();

	//Room* OpenRoom( int roomNum, TCHAR* title );
	Room* OpenRoom( TCHAR* title );
	void CloseRoom( int roomNum );

	Room* FindRoom( int roomNum );

	//void PackageAllRoomInfo( SPacket &packet );
	void PackageOpenRoomInfo( SPacket &packet );
};

#define GetRoomMgr RoomMgr::GetInstance()

