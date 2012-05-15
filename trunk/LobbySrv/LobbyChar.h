#pragma once

#include "SIndexQueue.h"

class LobbySession;
class SPacket;

const int Character_Space = 100;

enum ROOM_TEAM
{
	ROOM_TEAM_ATT	=	0,
	ROOM_TEAM_DEF,
};
enum ROOM_READY
{
	ROOM_READY_NON	=	0,
	ROOM_READY_OK,
};


class LobbyChar : public SServerObj
{
private:
	//�ش� ĳ������session����
	LobbySession*	m_session;

	//ĳ������ ���°� ���� ���� �������� Ȯ��
	BOOL			m_isPlay;

	//ĳ���� ����
	int				m_sessionId;
	TCHAR			m_tstrId[30];
	int				m_myTeam;
	BOOL			m_ready;

	//������ index
	int				m_vecIndex;

public:
	LobbyChar(void);
	~LobbyChar(void);

	void Init();
	void Init( int i );

	void SetSession( LobbySession* session );
	LobbySession* GetSession() const;

	//==============================================================

	void SetIsPlay( BOOL isPlay = TRUE );
	BOOL GetIsPlay() const;

	void SetSessionID( int session );
	int GetSessionID() const;

	void SetID( TCHAR* _id );
	TCHAR* GetID();

	void SetTeam( int Team );
	int GetTeam() const;

	void SetReady( BOOL ready );
	BOOL GetReady() const;

	//==============================================================

	void PackageMyInfo( SPacket& packet );
	//�濡 �ʿ��� ����(team, ready)�����ؼ� ���
	void PackageMyInfoForRoom( SPacket& packet );
	//���� ������ �ʿ��� ����(team)�� ��´�
	void PackageMyInfoForGame( SPacket& packet );

	//==============================================================

	inline int GetVecIndex() const { return m_vecIndex; };
};


class CharMgr : public SSingleton <CharMgr>
{
private:
	friend class SSingleton<CharMgr>;

private:
	//ĳ���� ����
	std::vector<LobbyChar*>		m_vecCharSpace;
	//���� ������ index
	SIndexQueue					m_IndexQ;

private:
	CharMgr();
	~CharMgr();

public:
	//���� ������ ����
	void Init();
	void Release();

	//ĳ���� ������ �Ҵ����ִ� �Լ�
	LobbyChar* GetCharSPace();

	//ĳ���� ������ �ݳ�
	void ReturnCharSpace( LobbyChar* charspace );
	void ReturnCharSpace( int sessionID );

	//�ش� session�� �ش��ϴ� character�� ����
	LobbyChar* FindCharAsSessionId( int session );
};

#define GetCharMgr CharMgr::GetInstance()
