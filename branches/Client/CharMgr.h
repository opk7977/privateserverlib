#pragma once

#include "SServerStd.h"

class CLobby;

class Character
{
public:
	//���� ���̵�
	int			m_sessionId;

	//�г���
	TCHAR		m_csId[50];

	//���� ���� ��(0:�κ�)
	int			m_roomNo;

	//���� �� ����
	int			m_Team;


public:
	Character() {};
	~Character() {};

	inline void SetSessionID( int indexId ) { m_sessionId = indexId; }
	inline int GetSessionID() { return m_sessionId; }

	inline void SetID( TCHAR* id ) { _tcsncpy_s( m_csId, 50, id, _tcslen(id) ); }
	inline TCHAR* GetID() { return m_csId; }

	inline void SetRoomNo( int roomNo ) { m_roomNo = roomNo; }
	inline int GetRoomNo() { return m_roomNo; }

	inline void SetTeam( int team ) { m_Team = team; }
	inline int GetTeam() { return m_Team; }
};





class CharMgr : public SSingleton <CharMgr>
{
private:
	friend class SSingleton<CharMgr>;

private:
	//��ü ĳ���͸��
	std::list<Character*>		m_charList;

	//��
	Character*					m_itMe;

	//��ü ĳ������ ��
	int							m_playerCount;


public:
	//view�� �޾� ���
	CLobby*						m_pLobby;

public:
	CharMgr(void);
	~CharMgr(void);

	//ĳ���� �߰�
	void AddChar( int sessionId, TCHAR* id );

	//ĳ���� ����
	void DelChar( int sessionId );

	//ĳ���� �˻�
	Character* FindChar( int sessionId );

	inline Character* GetMe() { return m_itMe; }

	//�� ���� ��� ĳ���� ������ ������.
	void ClearCharacter();

	//��� ������ �����list clear����
	void Release();
};

#define GetCharMgr CharMgr::GetInstance()

