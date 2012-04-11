#pragma once

#include "SServerObj.h"

//���� ����
#define MIN_PLAYER_COUNT 6
#define MAX_PLAYER_COUNT 8


class Room : public SServerObj
{
private:
	//<sessionId, MgrIndex>
	std::map<int, int>		m_listPlayer;

	int						m_nowPleyrCount;		//���� ���� �ִ� ������� ��
	int						m_readyCount;			//�غ������ ĳ���� ��
	int						m_leader;				//������ ���� ��ȣ

	BOOL					m_visible;				//���� ������� �ִ� ������?
	BOOL					m_isPlay;				//���� �������� ������

	TCHAR					*m_tstrRoomTitle;		//�� ����

	int						m_AttectTeam;
	int						m_DefenceTeam;


public:
	Room(void);
	~Room(void);

	void Init();

	inline BOOL IsVisible() { return m_visible; }
	inline void SetTitle( TCHAR* title ) { m_tstrRoomTitle = title; }
	inline int GetPlayerCount() { return m_nowPleyrCount; }
	inline int GetReadyCount() { return m_readyCount; }
	
	//������ ���� �� �� �ִ� ���� �ΰ��� Ȯ���� �ش�.
	BOOL PossiblePlay();

	void SetIndex( int sessionId, int Index );

	BOOL Play();

	//�濡 �ִ� ��� �̿��� packet�� ������.
	void SendPacketAllInRoom( SPacket &packet );
};
