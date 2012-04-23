#pragma once

// #include "SServerObj.h"
#include "SPacket.h"

// #include "SIndexQueue.h"

//���� ����
#define MIN_PLAYER_COUNT 6
#define MAX_PLAYER_COUNT 8

#define ROOMCOUNT		4

class LobbySession;

class RoomChar /*: public SServerObj*/
{
private:
	int		m_iocpKey;
	TCHAR	m_charID[50];
	int		m_charTeam;
	int		m_ready;

public:
	RoomChar();
	RoomChar( const RoomChar& p );
	~RoomChar();

	void Init();

	void SetIocp( int key );
	int GetIocp();

	void SetID( TCHAR *uId );
	TCHAR* GetID();

	void SetTeam( int team );
	int GetTeam();

	void SetReady( int ready );
	int GetReady();
};



class Room : public SServerObj
{
private:
	std::map<int, RoomChar>	m_mapPlayer;
	//<sessionId, MgrIndex(IOCP�� ��ϵ� �ڵ� ��)>
	//std::map<int, int>			m_mapPlayerlist;

	int							m_nowPleyrCount;		//���� ���� �ִ� ������� ��
	int							m_readyCount;			//�غ������ ĳ���� ��
	int							m_leader;				//������ ���� ��ȣ

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
	void SetLeader( int sessionId );
	int GetLeader() { return m_leader; }
	int ChangeLeader();
	//�濡 ����ִ� player�� �ڵ鰪�� ������ �ش�
	void SetPlayerIocp( int sessionId, int iocpKey );
	//player�߰�( �����ϸ� ������ ����ȣ�� return�Ѵ� )
	//int AddPlayerInRoom( int sessionId, int iocpKey );
	int AddPlayerInRoom( int sessionId, int iocpKey, TCHAR* charID );
	//player����
	// return ���� FALSE�̸� �濡 ����� ��� �����ٴ� ���̴�.
	BOOL DelPlayerInRoom( int sessionId );
	//player�� �Ѹ� ready���°� �ǰų� ���°� Ǯ��
	//TRUE : ready ���� +
	//FALSE: ready ���� -
	int ChangeReadyState( int sessionId );
	//�ٲ� ���� return�Ѵ�
	int TeamChange( int sessionID );
	//���� �Ҵ� ����
	//0 : ����
	//1 : ����
	int GetTeam();


	//--------------------------------------
	// ����
	//--------------------------------------
	//�� �ڽ��� ������ �ִ´�.
	void PackageRoomInfo( SPacket &packet );
	//�濡 �ִ� ��� ���� ������ packet�� �ִ´�
	void PackageAllPlayerInRoom( SPacket &packet );
	//�濡 �ִ� ��� �̿��� packet�� ������.
	void SendPacketAllInRoom( SPacket &packet, LobbySession* mySession = NULL );
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

