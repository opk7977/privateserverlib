#include "DemoLoginSession.h"

#include "SPacket.h"
#include "DemoLoginProtocol.h"
#include "DemoDBMgr.h"

#include "SLogger.h"

SIMPLEMENT_DYNAMIC(DemoLoginSession)
SIMPLEMENT_DYNCREATE(DemoLoginSession)

DemoLoginSession::DemoLoginSession(void)
{
}

DemoLoginSession::~DemoLoginSession(void)
{
}

void DemoLoginSession::OnCreate()
{
	SSession::OnCreate();
	//
	//연결 성공을 알림
	SendPacket( SC_LOGIN_CONNECT_OK );

}

void DemoLoginSession::PacketParsing( SPacket& packet )
{
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[DemoLoginSession::PacketParsing()] Complete Recv Packet / size : %d\n"), packet.GetPacketSize() );

	switch( packet.GetID() )
	{
	case CS_LOGIN_LOGIN:
		RecvLogin( &packet );
		break;
	}
}

//======================================
// 받아 처리하는 함수
//======================================
void DemoLoginSession::RecvLogin( SPacket* packet )
{
	int len=0;
	TCHAR myId[50]={0,};
	TCHAR myPw[50]={0,};
	
	(*packet) >> len;
	packet->GetData( myId, len );
	(*packet) >> len;
	packet->GetData( myPw, len );

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] RecvLogin--------------------------------------\n")
												  _T("\tPACKET ID : CS_LOGIN_LOGIN\n")
												  _T("\t로그인 요청 패킷을 받음\n")
												  _T("\tID : %s\n")
												  _T("\tPW : %s\n")
												  _T("[Network] -------------------------------------RecvLogin\n")
												  , myId
												  , myPw );
	/////////////////////////////////////////////////////////////////////////////////////////

	GetLogger.PutLog( 0, _T("[Network] 로그인 쿼리\n") );
	int loginResult = GetDBMgr.AskLogin( myId, myPw );

	switch( loginResult )
	{
	case -1:
		SendLoginNonId();
		break;
	case 0:
		SendLoginNotPw();
		break;
	case -10:
		//쿼리 오류
		break;
	default:
		SendLogin( loginResult );
		break;
	}
}


//======================================
// 보내는 함수
//======================================
void DemoLoginSession::SendLoginNonId()
{
	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] SendLoginNonId-------------------------------------\n")
												  _T("\tPACKET ID : SC_LOGIN_NONID\n")
												  _T("\tPACKET SIZE / DATA SIZE : 4 / 0\n")
												  _T("[Network] -------------------------------------SendLoginNonId\n") );
	/////////////////////////////////////////////////////////////////////////////////////////

	SendPacket( SC_LOGIN_NONID );
}

void DemoLoginSession::SendLoginNotPw()
{
	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] SendLoginNonId-------------------------------------\n")
												  _T("\tPACKET ID : SC_LOGIN_NOTPW\n")
												  _T("\tPACKET SIZE / DATA SIZE : 4 / 0\n")
												  _T("[Network] -------------------------------------SendLoginNotPw\n") );
	/////////////////////////////////////////////////////////////////////////////////////////

	SendPacket( SC_LOGIN_NOTPW );
}

void DemoLoginSession::SendLogin( int loginResult )
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOGIN_SUCCESS );

	//우선 로그인 결과를 넣어 주고
	sendPacket << loginResult;

	//로그인에 성공한 경우 게임서버의 ip와 port번호를 보내야 한다
	char ipAddr[50] = "192.168.0.70";

	sendPacket << (int)(strlen( ipAddr ));
	sendPacket.PutData( ipAddr, strlen( ipAddr ) );
	sendPacket << 9000;

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] SendLogin-------------------------------------\n")
												  _T("\tPACKET ID : SC_LOGIN_SUCCESS\n")
												  _T("\tPACKET SIZE / DATA SIZE : %d / %d\n")
												  _T("\tSessionId	: %d\n")
												  _T("\tGameSrv Ip	: 192.168.0.70\n")
												  _T("\tGameSrv port : 9000\n")
												  _T("[Network] -------------------------------------SendLogin\n")
												  , sendPacket.GetPacketSize()
												  , sendPacket.GetDateFiledSize()
												  , loginResult );
	/////////////////////////////////////////////////////////////////////////////////////////

	SendPacket( sendPacket );
}
