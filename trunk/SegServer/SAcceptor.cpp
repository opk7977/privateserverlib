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
	//���� ����
	if( !m_srvSock.CreateSocket() )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Acceptor::SetAcceptor()] �Ұ��� ���� ����\n") );
#endif
		return FALSE;
	}

	//log�� �����
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Acceptor::SetAcceptor()] �Ұ��� ����\n") );
#endif

	//ip ���� ����( ���� �ʼ�! )
	if( !m_srvSock.SetScokReuseAddr() )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Acceptor::SetAcceptor()] ip������ ���� ResuseAddr�ɼ��� ���� ����\n") );
#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Acceptor::SetAcceptor()] ip������ ���� ResuseAddr�ɼ��� ����\n") );
#endif

	//���� �ͺ� ����
	if( !m_srvSock.SetNonBlkSock() )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Acceptor::SetAcceptor()] NonBlock �ɼ� ���� ����\n") );
#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Acceptor::SetAcceptor()] NonBlock �ɼ� ����\n") );
#endif

	//������ ������ bind�Ѵ�.
	if( !m_srvSock.BindSocket( port ) )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Acceptor::SetAcceptor()] ���� Bind ����\n") );
#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Acceptor::SetAcceptor()] ���� Bind\n") );
#endif

	//���� listen Ȱ��ȭ
	if( !m_srvSock.SocketListen( backlogCount ) )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Acceptor::SetAcceptor()] ���� Listen ����\n") );
#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Acceptor::SetAcceptor()] ���� Listen\n") );
#endif

	//������ ����
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

		//���Ͽ� accept�� set �Ǿ� �ִٸ� accept�� �ָ� �ȴ�
		if( FD_ISSET( m_srvSock.GetSocket(), &fds ) )
			clientSock = accept( m_srvSock.GetSocket(), &addr, &len );

		//accept���� �ʾ����� �ٽ� ���ư��� ����
		if( clientSock == INVALID_SOCKET )
			continue;

#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Acceptor::Run()] client Listen\n") );
#endif

		//player�� �߰� �� �ִ� �۾��� �Ѵ�
		GetSessionMgr.CreateNewSession( clientSock, addr );
	}

	return TRUE;
}

void SAcceptor::Release()
{
	m_bThreadLoop = FALSE;
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Acepptor::Release()] srv������ �ݽ��ϴ�.\n") );
#endif
	m_srvSock.Release();
}