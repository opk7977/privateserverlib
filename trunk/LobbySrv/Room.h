#pragma once

#include "SServerObj.h"
#include "SPacket.h"

//방의 정원
#define MIN_PLAYER_COUNT 6
#define MAX_PLAYER_COUNT 8

#define ROOMCOUNT		4

class LobbySession;

class Room : public SServerObj
{
private:
	//<sessionId, MgrIndex(IOCP에 등록된 핸들 값)>
	std::map<int, int>		m_mapPlayerlist;

	int						m_nowPleyrCount;		//현재 들어와 있는 사람들의 수
	int						m_readyCount;			//준비상태의 캐릭터 수
	int						m_leader;				//방장의 세션 번호

	BOOL					m_visible;				//방이 만들어져 있는 방인지?
	BOOL					m_isPlay;				//현재 게임중인 방인지

	TCHAR					m_tstrRoomTitle[50];	//방 문구

	int						m_AttectTeam;
	int						m_DefenceTeam;


public:
	Room(void);
	~Room(void);
	
	//방초기화
	void Init();

	//--------------------------------------
	// 방 관련
	//--------------------------------------
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

	//--------------------------------------
	// player관련
	//--------------------------------------
	//들어와 있는 인원 확인
	inline int GetPlayerCount() { return m_nowPleyrCount; }
	//ready상태의 인원 확인
	inline int GetReadyCount() { return m_readyCount; }
	//리더 설정
	void SetLeader( int sessionId );
	//방에 들어있는 player의 핸들값을 변경해 준다
	void SetPlayerIndex( int sessionId, int Index );
	//player추가
	void AddPlayerInRoom( int sessionId, int index );
	//player제거
	// return 값이 FALSE이면 방에 사람이 모두 나갔다는 뜻이다.
	BOOL DelPlayerInRoom( int sessionId );
	//player가 한명 ready상태가 되거나 상태가 풀림
	//TRUE : ready 상태 +
	//FALSE: ready 상태 -
	void ChangeReadyState( BOOL isReady = TRUE );
	//팀의 인원 수
	// team 0 : 공격
	//		1 : 수비
	// isCount 가 TRUE : +
	//			  FALSE: -
	void TeamCount( int team, BOOL isCountUp = TRUE );
	//팀을 할당 받음
	//0 : 공격
	//1 : 수비
	int GetTeam();


	//--------------------------------------
	// 전송
	//--------------------------------------
	void PackageRoomInfo( SPacket &packet );
	//방에 있는 모든 이에게 packet을 보낸다.
	void SendPacketAllInRoom( SPacket &packet, LobbySession* mySession );
};

//////////////////////////////////////////////////////////////////////////

class RoomMgr : public SSingleton <RoomMgr>
{
private:
	friend class SSingleton<RoomMgr>;

private:
	std::map<int, Room*>			m_mapRoomlist;

private:
	RoomMgr();
	~RoomMgr();

public:
	void CreateRoomSpace();
	void Release();

	BOOL OpenRoom( int roomNum, int SessionID, int iocpHandle, TCHAR* title );
	void CloseRoom( int roomNum );

	Room* FindRoom( int roomNum );

	void PackageRoomInfoAll( SPacket &packet );
};

#define GetRoomMgr RoomMgr::GetInstance()

