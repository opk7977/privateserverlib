#include "SrvNet.h"
#include "SPacket.h"
#include "SLogger.h"

#include "GameSession.h"
#include "SSessionMgr.h"

SrvNet::SrvNet(void) : m_LobbySrv(NULL)
{
}

SrvNet::~SrvNet(void)
{
}

BOOL SrvNet::ConnectToSrv( char* ipAddr, int port )
{
	if( !m_sock.Init() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::ConnectToSrv()\n소켓 초기화에 실패했습니다.\n\n") );
		return FALSE;
	}

	if( !m_sock.CreateSocket() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::ConnectToSrv()\n소켓 생성화에 실패했습니다.\n\n") );
		return FALSE;
	}

	//연결을 하고
	if( !m_sock.ConnectSock( ipAddr, port, &m_sockAddr ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::ConnectToSrv()\n로비서버와의 연결에 실패했습니다.\n\n") );
		return FALSE;
	}

	if( !m_sock.SetNonBlkSock() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::ConnectToSrv()\n소켓 논블럭설정에 실패했습니다.\n\n") );
		return FALSE;
	}

// 	Sleep(1);

	//직접 세션Mgr에 세션 생성을 요청
	GetSessionMgr.CreateNewSession( m_sock.GetSocket(), m_sockAddr );

	return TRUE;
}

void SrvNet::DisConnect()
{
	m_sock.Release();
}

BOOL SrvNet::SendToLobbyServer( SPacket &packet )
{
	if( m_LobbySrv == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::SendToLobbyServer()\n서버와의 연결이 되어 있지 않습니다.\n\n") );
		return FALSE;
	}

	if( m_LobbySrv->SendPacket( packet ) != packet.GetPacketSize() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::SendToLobbyServer()\n보낸패킷이 패킷의 크기가 다릅니다\n\n") );
		return FALSE;
	}

	return TRUE;
}
