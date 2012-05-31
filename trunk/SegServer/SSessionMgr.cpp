#include "SSessionMgr.h"
#include "SegIOCP.h"
#include "SObject.h"

SSessionMgr::SSessionMgr(void)
{
}

SSessionMgr::~SSessionMgr(void)
{
	Release();
}

void SSessionMgr::Create( SRTClass* pRTC, int _size )
{
	//�켱 ��������� �� ���� ����
	m_spaceSize = _size;

	//indexQueue�� ����� ����
	// ������ ���� ���ڸ� �Ѱ� �ش�.
	m_indexQueue.Create( _size, 1 );

	//Session�� ���� ������ �̸� ����� ���´�!
	//map�� 1���� �����ؾ� �ϴϱ� 1~size+1���� ����� �־� �־� �Ѵ�
// 	for( int i=1; i<=_size; ++i )
// 	{
// 		SObject* pObj = pRTC->CreateObject();
// 		m_sessionMap.PushIndex( i, (SServerObj*)pObj );
// 	}

	for( int i=1; i<=m_spaceSize; ++i )
	{
		SObject* pObj = pRTC->CreateObject();
		m_mapSession[i] = (SServerObj*)pObj;
	}
}

void SSessionMgr::CreateNewSession( SOCKET socket, SOCKADDR addr )
{
	SSession* newSession = 0;
	int index;

	//���� ������ ��� ���� �������� ����Ī�� �Ͼ�� �ڷᰡ ��Ű�� �ʵ��� ���� �ش�
	{
		SSynchronize sync( &m_indexQueue );
		index = m_indexQueue.GetIndex();
	}

	//������ ���� index���� -1�� �޾� �� ���/////////////////////////////////
	if( index < 0 )
	{
		m_tmpSession.SetSocket( socket );

		m_tmpSession.SendPacket(0);

		return;
	}
	//////////////////////////////////////////////////////////////////////////

// 	{
// 		SSynchronize sync( &m_sessionMap );
// 		SServerObj* tmp = m_sessionMap.PeekObj( index );
// 		newSession = (SSession*)tmp;
// 		if( newSession == 0 )
// 			return;
// 	}

	{
		SSynchronize sync( this );
		newSession = (SSession*)m_mapSession[index];
		if( newSession == 0 )
			return;
	}

	//���� �Է�
	newSession->SetSocket( socket );
	newSession->SetSockAddr( addr );

	//IOCP�� �ڵ� �߰�!!
	GetIOCP.AddHandleToIOCP( (HANDLE)socket, (DWORD)index );

	////�ڵ��� ��������� ��Ŷ�� �ޱ� ���� WSARecv�� ȣ��!!(���ǿ� ����� �ش�)
	{
		SSynchronize sync( newSession );
		newSession->OnCreate();
	}
	
	//index���� ����������list�� �߰��� ����
// 	{
// 		SSynchronize sync( &m_playerList );
// 		m_playerList.PushTail( index );
// 	}
	{
		SSynchronize Sync( &m_listPlayer );
		m_listPlayer.AddItem( index );
	}
}

void SSessionMgr::RemoveSession( int index )
{
	SSession* newSession = 0;

	//�ش� index�� session�� �޾� �´�
	{
// 		SSynchronize sync( &m_sessionMap );
// 		newSession = (SSession*)(m_sessionMap.PeekObj( index ));
// 		if( newSession == 0 )
// 			return;
		SSynchronize sync( this );
		newSession = (SSession*)m_mapSession[index];
		if( newSession == 0 )
			return;

		//����� socket�� �ݾ� �ְ� �����������
		newSession->OnDestroy();
	}

	//���ӵ� player list���� �ش� index�� �����ش�
// 	{
// 		SSynchronize sync( &m_playerList );
// 		m_playerList.RemoveItem( index );
// 	}
	{
		SSynchronize Sync( &m_listPlayer );
		m_listPlayer.DelItem( index );
	}

	//�ݳ��� index�� �ٽ� Queue�� �־� �ش�
	{
		SSynchronize sync( &m_indexQueue );
		m_indexQueue.PutIndex( index );
	}

}

SServerObj* SSessionMgr::GetSession( int index )
{
	SServerObj* newSession = 0;

// 	{
// 		SSynchronize sync( &m_sessionMap );
// 		newSession = m_sessionMap.PeekObj( index );
// 
// 		if( newSession == 0 )
// 			return NULL;
// 	}
	SSynchronize sync( this );

	newSession = m_mapSession[index];
	if( newSession == 0 )
		return NULL;

	return newSession;
}

void SSessionMgr::SendAllSession( SPacket &packet, SSession* itme/* = NULL*/ )
{
	//�켱 ������ ��� �ִٸ� �׳� ���� ����
// 	if( m_playerList.Size() <= 0 )
// 		return;
// 	{
// 		SSynchronize sync( &m_sessionMap );
// 
// 		SSession* tmpSession;
// 		std::list<int>::iterator iter = m_playerList.HeadPosiont();
// 		for( ; !m_playerList.IsEnd(iter); ++iter )
// 		{
// 			//���� �Ѱ�ٸ� ������ ������ �ʴ´�.
// // 			if( (*iter) == mySession->GetKey() )
// // 				continue;
// 
// 			//��� ���ǿ� ��Ŷ�� ������.
// 			tmpSession = (SSession*)m_sessionMap.PeekObj((*iter));
// 			tmpSession->SendPacket( packet );
// 		}
// 	}
	SSynchronize Sync( this );

	if( m_listPlayer.IsEmpty() )
		return;

	SSession* tmpSession;

	std::list<int>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		tmpSession = (SSession*)m_mapSession[(*iter)];

		//���� �Ѱ����� ������ ������ �ʴ´�.
		if( tmpSession == itme )
			continue;

		tmpSession->SendPacket( packet );
	}
}

SServerObj* SSessionMgr::FindSession( int SessionId )
{
	//if( m_playerList.Size() <= 0 )
	SSynchronize Sync( this );

	if( m_listPlayer.IsEmpty() )
		return NULL;
// 	{
// 		SSynchronize sync( &m_sessionMap );
// 
// 		SSession* tmpSession;
// 		//std::list<int>::iterator iter = m_playerList.HeadPosiont();
// 		std::list<int>::iterator	iter = m_listPlayer.GetHeader();
// 		for( ; !m_listPlayer.IsEnd(iter); ++iter )
// 		{
// 			//������ �ִ� ���ǿ��� ã�´�.
// 			tmpSession = (SSession*)m_sessionMap.PeekObj((*iter));
// 			if( SessionId == tmpSession->GetSessionID() )
// 				return (SServerObj*)(*iter);
// 		}
// 
// 		return NULL;
// 	}

	SSession* tmpSession;
	std::list<int>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		tmpSession = (SSession*)m_mapSession[(*iter)];
		if( SessionId == tmpSession->GetSessionID() )
			return (SServerObj*)tmpSession;
	}

	return NULL;
}

void SSessionMgr::Release()
{
	//m_sessionMap.Release();
	SSynchronize sync( this );

	SServerObj* tmpSession;
	POSITION pos = m_mapSession.GetStartPosition();
	while( pos )
	{
		tmpSession = m_mapSession.GetNextValue( pos );
		delete tmpSession;
		tmpSession = 0;
	}
	m_mapSession.RemoveAll();

	m_indexQueue.Release();
}
