#include "SAcceptor.h"
#include "SSessionMgr.h"

#ifdef _DEBUG
	#include "SLogger.h"
#endif

BOOL SAcceptor::m_bThreadLoop = TRUE;

SAcceptor::SAcceptor(void)
{
}

SAcceptor::~SAcceptor(void)
{
	m_bThreadLoop = FALSE;
	WaitForSingleObject( GetThreadHandle(), 1000 );
	Release();
}

BOOL SAcceptor::Init()
{
	if( !m_srvSock.Init() )
		return FALSE;
	
	return TRUE;
}

BOOL SAcceptor::SetAcceptor( int port, int backlogCount )
{
	//소켓 생성
	if( !m_srvSock.CreateSocket() )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Acceptor::SetAcceptor()] 소겟을 생성 실패\n") );
#endif
		return FALSE;
	}

	//log를 남긴다
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Acceptor::SetAcceptor()] 소겟을 생성\n") );
#endif

	//ip 재사용 설정( 서버 필수! )
	if( !m_srvSock.SetScokReuseAddr() )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Acceptor::SetAcceptor()] ip재사용을 위해 ResuseAddr옵션을 설정 실패\n") );
#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Acceptor::SetAcceptor()] ip재사용을 위해 ResuseAddr옵션을 설정\n") );
#endif

	//소켓 넌블럭 설정
	if( !m_srvSock.SetNonBlkSock() )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Acceptor::SetAcceptor()] NonBlock 옵션 설정 실패\n") );
#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Acceptor::SetAcceptor()] NonBlock 옵션 설정\n") );
#endif

	//소켓을 서버로 bind한다.
	if( !m_srvSock.BindSocket( port ) )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Acceptor::SetAcceptor()] 소켓 Bind 실패\n") );
#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Acceptor::SetAcceptor()] 소켓 Bind\n") );
#endif

	//소켓 listen 활성화
	if( !m_srvSock.SocketListen( backlogCount ) )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Acceptor::SetAcceptor()] 소켓 Listen 실패\n") );
#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Acceptor::SetAcceptor()] 소켓 Listen\n") );
#endif

	//쓰레드 동작
	BeginThread();

	return TRUE;
}

BOOL SAcceptor::Run()
{
	TIMEVAL timeout;
	SOCKADDR addr;

	timeout.tv_sec	= 0;
	timeout.tv_usec	= 5;

	while( m_bThreadLoop )
	{
		SOCKET clientSock = INVALID_SOCKET;

		fd_set fds;
		int len = sizeof( SOCKADDR );
		FD_ZERO( &fds );
		FD_SET( m_srvSock.GetSocket(), &fds );

		if( select( 0, &fds, 0, 0, &timeout ) == SOCKET_ERROR )
		{
			INT32 tmp = WSAGetLastError();
#ifdef _DEBUG
			GetLogger.ErrorLog( tmp, _T("[Acceptor::Run()] " ) );
#endif

			if( tmp == WSAENOTSOCK )
				return TRUE;
		}

		//소켓에 accept가 set 되어 있다면 accept해 주면 된다
		if( FD_ISSET( m_srvSock.GetSocket(), &fds ) )
			clientSock = accept( m_srvSock.GetSocket(), &addr, &len );

		//accept되지 않았으면 다시 돌아가서 돌자
		if( clientSock == INVALID_SOCKET )
			continue;

#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Acceptor::Run()] client Listen\n") );
#endif

		//player를 추가 해 주는 작업을 한다
		GetSessionMgr.CreateNewSession( clientSock, addr );
	}

	return TRUE;
}

void SAcceptor::Release()
{
	m_bThreadLoop = FALSE;
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Acepptor::Release()] srv소켓을 닫습니다.\n") );
#endif
	m_srvSock.Release();
}