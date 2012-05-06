#pragma once

#include "SServerObj.h"
#include "SIndexQueue.h"

class SPacket;
class LobbyChar;
class LobbySession;


//���� ����
#define MIN_PLAYER_COUNT 6
#define MAX_PLAYER_COUNT 8

#define ROOMCOUNT		4

class Room : public SServerObj
{
private:
	std::list<LobbyChar*>		m_listPlayer;

	int							m_roomNum;

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
	void Init( int i );

	//--------------------------------------
	// �� ����
	//--------------------------------------
	//���ȣ�� return
	inline int GetRoomNum() const { return m_roomNum; }
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

	//�濡 ����ִ� player�� ���� �ּҸ� ������ �ش�
	BOOL SetPlayerSession( LobbySession* session, LobbyChar* charSpace );

	//player�߰�
	void AddPlayerInRoom( LobbyChar* charspace );
	//player����
	// return ���� FALSE�̸� �濡 ����� ��� �����ٴ� ���̴�.
	BOOL DelPlayerInRoom( LobbyChar* charspace );
	BOOL DelPlayerInRoom( int sessionId );

	//�Ű������� �����ؾ� �ϴ� ���°��� ��Ÿ����
	//ex) ������ ready���°� �Ǹ� TRUE�� �Ѱ� ready���� ����
	void ChangReadyState( BOOL isReady );
	
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
	//�濡 �ִ� ��� player�� ������ ��´�
	void PackagePlayerInRoom( SPacket &packet, LobbyChar* itMe = NULL );
	//�濡 �ִ� ��� �̿��� packet�� ������.
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

	//���� �ִ� ���� ����
	int								m_iOpenRoomCount;

	//����ȭ ��
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

