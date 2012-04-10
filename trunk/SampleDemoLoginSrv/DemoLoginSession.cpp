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
	//���� ������ �˸�
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
// �޾� ó���ϴ� �Լ�
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
												  _T("\t�α��� ��û ��Ŷ�� ����\n")
												  _T("\tID : %s\n")
												  _T("\tPW : %s\n")
												  _T("[Network] -------------------------------------RecvLogin\n")
												  , myId
												  , myPw );
	/////////////////////////////////////////////////////////////////////////////////////////

	GetLogger.PutLog( 0, _T("[Network] �α��� ����\n") );
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
		//���� ����
		break;
	default:
		SendLogin( loginResult );
		break;
	}
}


//======================================
// ������ �Լ�
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

	//�켱 �α��� ����� �־� �ְ�
	sendPacket << loginResult;

	//�α��ο� ������ ��� ���Ӽ����� ip�� port��ȣ�� ������ �Ѵ�
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
