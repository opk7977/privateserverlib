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
	//우선 만들어지는 총 수를 저장
	m_spaceSize = _size;

	//indexQueue를 만들어 놓자
	// 개수와 시작 숫자를 넘겨 준다.
	m_indexQueue.Create( _size, 1 );

	//Session을 담을 공간을 미리 만들어 놓는다!
	//map은 1부터 시작해야 하니까 1~size+1까지 만들어 넣어 둬야 한다
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

	//저장 공간을 얻어 오는 과정에서 스위칭이 일어나도 자료가 엉키지 않도록 묶어 준다
	{
		SSynchronize sync( &m_indexQueue );
		index = m_indexQueue.GetIndex();
	}

	//공간이 없어 index값이 -1을 받아 온 경우/////////////////////////////////
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

	//정보 입력
	newSession->SetSocket( socket );
	newSession->SetSockAddr( addr );

	//IOCP에 핸들 추가!!
	GetIOCP.AddHandleToIOCP( (HANDLE)socket, (DWORD)index );

	////핸들을 등록했으니 패킷을 받기 위해 WSARecv를 호출!!(세션에 등록해 준다)
	{
		SSynchronize sync( newSession );
		newSession->OnCreate();
	}
	
	//index값을 접속했으니list에 추가해 주자
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

	//해당 index의 session을 받아 온다
	{
// 		SSynchronize sync( &m_sessionMap );
// 		newSession = (SSession*)(m_sessionMap.PeekObj( index ));
// 		if( newSession == 0 )
// 			return;
		SSynchronize sync( this );
		newSession = (SSession*)m_mapSession[index];
		if( newSession == 0 )
			return;

		//연결된 socket을 닫아 주고 정보를지운다
		newSession->OnDestroy();
	}

	//접속된 player list에서 해당 index는 지워준다
// 	{
// 		SSynchronize sync( &m_playerList );
// 		m_playerList.RemoveItem( index );
// 	}
	{
		SSynchronize Sync( &m_listPlayer );
		m_listPlayer.DelItem( index );
	}

	//반납된 index는 다시 Queue로 넣어 준다
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
	//우선 세션이 비어 있다면 그냥 돌아 간다
// 	if( m_playerList.Size() <= 0 )
// 		return;
// 	{
// 		SSynchronize sync( &m_sessionMap );
// 
// 		SSession* tmpSession;
// 		std::list<int>::iterator iter = m_playerList.HeadPosiont();
// 		for( ; !m_playerList.IsEnd(iter); ++iter )
// 		{
// 			//나를 넘겼다면 나에겐 보내지 않는다.
// // 			if( (*iter) == mySession->GetKey() )
// // 				continue;
// 
// 			//모든 세션에 패킷을 보낸다.
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

		//나를 넘겼으면 나에겐 보내지 않는다.
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
// 			//접속해 있는 세션에서 찾는다.
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
