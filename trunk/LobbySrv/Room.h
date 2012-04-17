#pragma once

#include "SServerObj.h"
#include "SPacket.h"

//���� ����
#define MIN_PLAYER_COUNT 6
#define MAX_PLAYER_COUNT 8

#define ROOMCOUNT		4

class LobbySession;

class Room : public SServerObj
{
private:
	//<sessionId, MgrIndex(IOCP�� ��ϵ� �ڵ� ��)>
	std::map<int, int>		m_mapPlayerlist;

	int						m_nowPleyrCount;		//���� ���� �ִ� ������� ��
	int						m_readyCount;			//�غ������ ĳ���� ��
	int						m_leader;				//������ ���� ��ȣ

	BOOL					m_visible;				//���� ������� �ִ� ������?
	BOOL					m_isPlay;				//���� �������� ������

	TCHAR					m_tstrRoomTitle[50];	//�� ����

	int						m_AttectTeam;
	int						m_DefenceTeam;


public:
	Room(void);
	~Room(void);
	
	//���ʱ�ȭ
	void Init();

	//--------------------------------------
	// �� ����
	//--------------------------------------
	//���� ���� �ִ����� Ȯ��
	inline BOOL IsOpen() { return m_visible; }
	//title ����
	inline void SetTitle( TCHAR* title )
	{
		_tcsncpy_s( m_tstrRoomTitle, 50, title, _tcslen( title ) );
		m_visible = TRUE;
	}
	inline TCHAR* GetTitle() { return m_tstrRoomTitle; }
	//������ ���� �� �� �ִ� ���� �ΰ��� Ȯ���� �ش�.
	BOOL PossiblePlay();
	//���� play���·� ����� �ش�.
	BOOL SetPlay();

	//--------------------------------------
	// player����
	//--------------------------------------
	//���� �ִ� �ο� Ȯ��
	inline int GetPlayerCount() { return m_nowPleyrCount; }
	//ready������ �ο� Ȯ��
	inline int GetReadyCount() { return m_readyCount; }
	//���� ����
	void SetLeader( int sessionId );
	//�濡 ����ִ� player�� �ڵ鰪�� ������ �ش�
	void SetPlayerIndex( int sessionId, int Index );
	//player�߰�
	void AddPlayerInRoom( int sessionId, int index );
	//player����
	// return ���� FALSE�̸� �濡 ����� ��� �����ٴ� ���̴�.
	BOOL DelPlayerInRoom( int sessionId );
	//player�� �Ѹ� ready���°� �ǰų� ���°� Ǯ��
	//TRUE : ready ���� +
	//FALSE: ready ���� -
	void ChangeReadyState( BOOL isReady = TRUE );
	//���� �ο� ��
	// team 0 : ����
	//		1 : ����
	// isCount �� TRUE : +
	//			  FALSE: -
	void TeamCount( int team, BOOL isCountUp = TRUE );
	//���� �Ҵ� ����
	//0 : ����
	//1 : ����
	int GetTeam();


	//--------------------------------------
	// ����
	//--------------------------------------
	void PackageRoomInfo( SPacket &packet );
	//�濡 �ִ� ��� �̿��� packet�� ������.
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

