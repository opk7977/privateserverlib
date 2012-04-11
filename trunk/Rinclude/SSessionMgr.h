#ifndef __SESSION_MANAGER__
#define __SESSION_MANAGER__

//////////////////////////////////////////////////////////////////////////
#include "SIndexQueue.h"
#include "SMap.h"
#include "SIndexList.h"
//////////////////////////////////////////////////////////////////////////

#include "SSession.h"

//���� player �ο�
#define DEFUALT_INDEX_COUNT		100


class SSessionMgr : public SSingleton <SSessionMgr>
{
private:
	friend class SSingleton<SSessionMgr>;

private:
	//��� player ������ ���� ����
	SMap					m_sessionMap;
	//����ִ� ������ �ε����� ����
	SIndexQueue				m_indexQueue;
	//������ player���� ������ ����
	SIndexList				m_playerList;


	//���� �ο��� ��� ���־ ������ ���ϴ� ���
	//���ӺҰ��� �����ϱ� ���� �ӽ� Session����
	SSession				m_tmpSession;

private:
	SSessionMgr(void);
	~SSessionMgr(void);

public:
	//�Ѱ� ���� class( ����class )�� count��ŭ �����Ѵ�
	void Create( SRTClass* pRTC = NULL, int _size = DEFUALT_INDEX_COUNT );
	//player�� ������ ������ Session�� ä�� �ִ°�
	//���� �����Ҵ��� �޴°��� �ƴ�!
	void CreateNewSession( SOCKET socket, SOCKADDR addr );
	//player�� ������ ������ Session�� ���� �ִ°�
	//���� �����Ҵ��� �����ϴ� ���� �ƴ�!
	void RemoveSession( int index );

	//�ش� index�� �´� Session�� return�� �ش�.
	//����ڰ� ����� �Լ�
	SServerObj*	GetSession( int index );


	//��� ���ǿ��� ������ ��Ŷ ����
	void SendAllSession( SPacket packet );

	//��� �����Ҵ� ����
	void Release();
};

#define GetSessionMgr SSessionMgr::GetInstance()

#endif
