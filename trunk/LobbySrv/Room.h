#pragma once

#include "SServerObj.h"
#include "SIndexQueue.h"
#include "SList.h"

class SPacket;
class SLogger;
class CharMgr;

class LobbyChar;
class LobbySession;


//���� ����
#define MIN_PLAYER_COUNT	2
#define MAX_PLAYER_COUNT	8
#define MAX_TIME_COUNT		4

enum ROOM_STATE
{
	ROOM_STATE_READY	= -1,
	ROOM_STATE_NORMAL,
	ROOM_STATE_PLAYING,
};

class Room : public SServerObj
{
private:
	static SLogger*				m_logger;
	static CharMgr*				m_charMgr;

private:
	//======================================
	// ���� ����
	//======================================
	int							m_roomNum;
	TCHAR						m_tstrRoomTitle[50];	//�� ����

	BOOL						m_visible;				//���� �ִ� ������?
	int							m_roomState;			//���� ����
	//======================================
	
	//======================================
	// �� ���� player����
	//======================================
	//�濡 �����ִ� playerlist�� �ο�
	SList<LobbyChar*>			m_listPlayer;

	int							m_readyCount;			//�غ������ ĳ���� ��
	int							m_stageMap;				//���� ��

	LobbyChar*					m_leader;				//����

	int							m_AttectTeam;			//������ count
	int							m_DefenceTeam;			//������ count
	//======================================

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
	inline TCHAR* GetTitle()
	{
		return m_tstrRoomTitle;
	}
	inline int	GetStageMap() { return m_stageMap; }
	inline void SetStageMap( int stageMap ) { m_stageMap = stageMap; }
	//������ ���� �� �� �ִ� ���� �ΰ��� Ȯ���� �ش�.
	BOOL PossiblePlay();
	//���� play���·� ����� �ش�.
	void SetPlay();
	inline void ListReset() 
	{
		m_listPlayer.Clear();
	}
	//���� ready���·� ����� �ش�
	void SetReady();
	//���� �ٽ� �Ϲ� ���·�
	void SetNormal() { m_roomState = ROOM_STATE_NORMAL; }
	//�� ���¸� return�� �ش�
	int GetRoomState() { return m_roomState; }
	//���� ������ ���°��� ��������
	BOOL CanInsert();

	//--------------------------------------
	// player����
	//--------------------------------------
	//���� �ִ� �ο� Ȯ��
	inline int GetPlayerCount() { return m_listPlayer.GetItemCount(); }
	//ready������ �ο� Ȯ��
	inline int GetReadyCount() { return m_readyCount; }

	//���� ����
	void SetLeader( LobbyChar* sessionId );
	LobbyChar* GetLeader() { return m_leader; }
	LobbyChar* ChangeLeader();

	//�濡 ����ִ� player�� ���� �ּҸ� ������ �ش�
	//BOOL SetPlayerSession( LobbySession* session, LobbyChar* charSpace );

	//player�߰�
	void AddPlayerInRoom( LobbyChar* charspace );
	//player����
	// return ���� FALSE�̸� �濡 ����� ��� �����ٴ� ���̴�.
	BOOL DelPlayerInRoom( LobbyChar* charspace );
	BOOL DelPlayerInRoom( int sessionId );

	//�Ű������� �����ؾ� �ϴ� ���°��� ��Ÿ����
	//ex) ������ ready���°� �Ǹ� TRUE�� �Ѱ� ready���� ����
	void ChangReadyCount( BOOL isReady );
	
	//���� ����
	//�����ϴ� ���� �Ű������� �޴´�
	//ex) TRUE�̸� �������� �ø��� �������� ������
	//return ���� FALSE�̸� ���� ������� ������!
	BOOL ChangeTeam( BOOL isATT );

	//���� �Ҵ� ����
	//0 : ����
	//1 : ����
	int GetTeam();

	//--------------------------------------
	// ����
	//--------------------------------------
	//�� �ڽ��� ������ �ִ´�.
	void PackageRoomInfo( SPacket &packet );
	//�濡 �ִ� ��� player�� ������ ��´�( team�� ready�� ��� ��´� )
	void PackagePlayerInRoom( SPacket &packet, LobbyChar* itMe = NULL );
	//�濡 �ִ� ��� player�� ������ ��´�( team������ ��´� )
	void PackagePlayerInRoomForGame( SPacket &packet, LobbyChar* itMe = NULL );
	//�濡 �ִ� ��� �̿��� packet�� ������.
	void SendPacketAllInRoom( SPacket &packet, LobbyChar* itMe = NULL );
};

//////////////////////////////////////////////////////////////////////////

class DataLeader;


class RoomMgr : public SSingleton <RoomMgr>, public SServerObj
{
private:
	friend class SSingleton<RoomMgr>;

private:
	//======================================
	// singleTon��ü
	//======================================
	DataLeader*						m_document;
	//======================================
	//���� �Ѱ���
	int								m_roomCount;
	ATL::CAtlMap<int, Room*>		m_mapRoom;
	//�� ������ index(���� �ִ� �� ��ȣ ť)
	SIndexQueue						m_roomIndexQ;

	//���� �ִ� ���� list
	SList<Room*>					m_listOpenRoom;

private:
	RoomMgr();
	~RoomMgr();

public:
	void	CreateRoomSpace();
	void	Release();

	Room*	OpenRoom( TCHAR* title );
	int		OpenRoomCount() { return m_listOpenRoom.GetItemCount(); }
	void	CloseRoom( int roomNum );

	Room*	FindRoom( int roomNum );

	void	PackageOpenRoomInfo( SPacket &packet );
};

#define GetRoomMgr RoomMgr::GetInstance()

