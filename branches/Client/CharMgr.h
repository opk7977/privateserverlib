#pragma once

#include "SServerStd.h"

class Character
{
private:
	//���� ���̵�
	int			m_sessionId;

	//�г���
	CString		m_csId;

	//���� ���� ��(0:�κ�)
	int			m_roomNo;

public:
	Character() {};
	~Character() {};

	inline void SetSessionID( int indexId ) { m_sessionId = indexId; }
	inline int GetSessionID() { return m_sessionId; }

	inline void SetID( CString id ) { m_csId = id; }
	inline CString GetID() { return m_csId; }

	inline void SetRoomNo( int roomNo ) { m_roomNo = roomNo; }
	inline int GetRoomNo() { return m_roomNo; }
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
	CharMgr(void);
	~CharMgr(void);

	//ĳ���� �߰�
	void AddChar( int sessionId, CString id );

	//ĳ���� ����
	void DelChar( int sessionId );

	//ĳ���� �˻�
	Character* FindChar( int sessionId );
};

#define GetCharMgr CharMgr::GetInstance()

