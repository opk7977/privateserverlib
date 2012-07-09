#pragma once

//======================================
// DB�����κ����� ��(?)��
// ��ٸ��� session����
// �ӽ� index�� �����ϴ� class
// ����°� �˻��� ���
//======================================

#include "SIndexQueue.h"

class LoginSession;

class TmpSessionSpace : public SSingleton <TmpSessionSpace>, public SServerObj
{
private:
	friend class SSingleton<TmpSessionSpace>;

private:
	//������ ��ٸ��� �������� Session
	std::vector<LoginSession*>		m_waitSession;

	//index
	SIndexQueue						m_indexQ;

	//���� ũ��
	int								m_spaceSize;

public:
	TmpSessionSpace(void);
	~TmpSessionSpace(void);

	void Init( int SessionCount );
	void Release();

	//��ȣ�� �ϳ� �޾� ����.
	int GetIndex();

	//��� ������ ����
	void WaitForDBSignal( int index, LoginSession* session );
	//��ȣ�� �޾� ó������ ���ǰ����� ����ش�/ index�� ��ȯ�� �ش�.
	void GetSignal( int index );

	//index�� �ش� session�� ã�´�
	LoginSession* FindSessionByIndex( int index );
};

#define GetTmpSpace TmpSessionSpace::GetInstance()
