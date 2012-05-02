#pragma once

// #include "SServerObj.h"
/*#include "SPacket.h"*/
/*#include "SIndexQueue.h"*/


class SPacket;
class LobbyChar;
class LobbySession;


//���� ����
#define MIN_PLAYER_COUNT 6
#define MAX_PLAYER_COUNT 8

#define ROOMCOUNT		4

// enum ROOM_TEAM
// {
// 	ROOM_TEAM_ATT	=	0,
// 	ROOM_TEAM_DEF,
// };
// enum ROOM_READY
// {
// 	ROOM_READY_NON	=	0,
// 	ROOM_READY_OK,
// };

class Room : public SServerObj
{
private:
	std::list<LobbyChar*>		m_listPlayer;

	int							m_nowPleyrCount;		//���� ���� �ִ� ������� ��
	int							m_readyCount;			//�غ������ ĳ���� ��

	LobbyChar*					m_leader;				//������ ���� ��ȣ

	BOOL						m_visible;				//���� ������� �ִ� ������?
	BOOL						m_isPlay;				//���� �������� ������

	TCHAR						m_tstrRoomTitle[50];	//�� ����

	int							m_AttectTeam;
	int							m_DefenceTeam;


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
	void SetLeader( LobbyChar* sessionId );
	LobbyChar* GetLeader() { return m_leader; }
	LobbyChar* ChangeLeader();

	//�濡 ����ִ� player�� �ڵ鰪�� ������ �ش�
	BOOL SetPlayerSession( LobbySession* session, LobbyChar* charSpace );

	//player�߰�
	void AddPlayerInRoom( LobbyChar* charspace );
	//player����
	// return ���� FALSE�̸� �濡 ����� ��� �����ٴ� ���̴�.
	BOOL DelPlayerInRoom( LobbyChar* charspace );

	//player�� �Ѹ� ready���°� �ǰų� ���°� Ǯ��
	//TRUE : ready ���� +
	//FALSE: ready ���� -
	//int ChangeReadyState( int sessionId );
	//�Ű������� �����ؾ� �ϴ� ���°��� ��Ÿ����
	//ex) ������ ready���°� �Ǹ� TRUE�� �Ѱ� ready���� ����
	void ChangReadyState( BOOL isReady );
	//�ٲ� ���� return�Ѵ�
	//int TeamChange( int sessionID );
	
	//���� ����
	//�����ϴ� ���� �Ű������� �޴´�
	//ex) TRUE�̸� �������� �ø��� �������� ������
	void ChangeTeam( BOOL isATT );

	//���� �Ҵ� ����
	//0 : ����
	//1 : ����
	int GetTeam();

	//--------------------------------------
	// ����
	//--------------------------------------
	//�� �ڽ��� ������ �ִ´�.
	void PackageRoomInfo( SPacket &packet );
	//�濡 �ִ� ��� �̿��� packet�� ������.
	void SendPacketAllInRoom( SPacket &packet, LobbyChar* itMe = NULL );
};

//////////////////////////////////////////////////////////////////////////

class RoomMgr : public SSingleton <RoomMgr>
{
private:
	friend class SSingleton<RoomMgr>;

private:
	std::map<int, Room*>			m_mapRoomlist;

	//����ȭ ��
	SServerObj*						m_critical;

private:
	RoomMgr();
	~RoomMgr();

public:
	void CreateRoomSpace();
	void Release();

	Room* OpenRoom( int roomNum,/* int SessionID, int iocpHandle,*/ TCHAR* title );
	void CloseRoom( int roomNum );

	Room* FindRoom( int roomNum );

	void PackageRoomInfoAll( SPacket &packet );
};

#define GetRoomMgr RoomMgr::GetInstance()

