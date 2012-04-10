#include "DemoGameSession.h"
#include "DemoGameProtocol.h"
#include "time.h"
#include "SLogger.h"

#include "SSessionMgr.h"
#include "CharacterMgr.h"

SIMPLEMENT_DYNAMIC(DemoGameSession)
SIMPLEMENT_DYNCREATE(DemoGameSession)

DemoGameSession::DemoGameSession(void)
{
}

DemoGameSession::~DemoGameSession(void)
{
}

void DemoGameSession::OnCreate()
{
	SSession::OnCreate();

	//우선 로그인쪽에서 받아온 list에서 확인해서
	//정상적으로 접속했다면
	//connectOK를 보낸다.
	SendPacket( SC_GAME_CONNECT_OK );
}

void DemoGameSession::OnDestroy()
{
	SSession::OnDestroy();

	SendDeleteCharacter();
}

//////////////////////////////////////////////////////////////////////////

void DemoGameSession::PacketParsing( SPacket& packet )
{
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[DemoGamesession::PacketParsing()] Complete Recv Packet / size : %d\n"), packet.GetPacketSize() );

	switch( packet.GetID() )
	{
	case CS_GAME_SEND_INDECX:
		RecvCreateChar( &packet );
		break;
	case CS_GAME_CHANGE_STATE:
		RecvChangeChar( &packet );
		break;
	}
}


//////////////////////////////////////////////////////////////////////////

void DemoGameSession::RecvCreateChar( SPacket* packet )
{
	int indexID, size;
	TCHAR charID[30];


	//우선 id부터 받아 오고
	(*packet) >> indexID;
	//우선 저장해 놓는다.
	m_indexID = indexID;

	(*packet) >> size;
	packet->GetData( charID, size );

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[DemoGameSession] Recv Packet------------------------------------\n")
												  _T("\tPACKET ID : CS_GAME_SEND_INDECX\n")
												  _T("\t접속한 캐릭터의 세션 id를 받아 새로운 캐릭터를 생성한다.\n")
												  _T("\tSessionID : %d\n")
												  _T("\tchar ID	 : %s\n")
												  _T("[DemoGameSession] -------------------------------------RecvCreateChar\n")
												  , indexID
												  , charID );
	/////////////////////////////////////////////////////////////////////////////////////////

	srand( (unsigned int)time(NULL) );
	//캐릭터를 생성!->메니저를 통해 모두에게 해당 캐릭터 정보를 보낸다
	int x = rand()%800;
	int y = rand()%600;
	GetCharMgr.CreateCharacter( indexID, x, y, charID );

	//모두에게 만들라는 명령을 내려 준다.
	SendCreateCharacter( indexID, x, y, charID );

	//나에게 나 빼고 모든 캐릭터의 정보를 보낸다
	SendCreateCharacter( indexID );
}

void DemoGameSession::RecvChangeChar( SPacket* packet )
{
	int indexId, state, x, y;
	
	(*packet) >> indexId;
	(*packet) >> state;
	(*packet) >> x;
	(*packet) >> y;

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[DemoGameSession] Recv Packet------------------------------------\n")
												  _T("\tPACKET ID : CS_GAME_CHANGE_STATE\n")
												  _T("\t캐릭터의 상태가 변화한다.\n")
												  _T("\tSessionID     : %d\n")
												  _T("\tchar state    : %d\n")
												  _T("\tchar Pos(x/y) : %d/ %d\n")
												  _T("[DemoGameSession] -------------------------------------RecvChangeChar\n")
												  , indexId
												  , state
												  , x
												  , y );
	/////////////////////////////////////////////////////////////////////////////////////////


	GetCharMgr.StartCritical();

	CCharacter* tmp;
	tmp = GetCharMgr.FindChar( indexId );

	tmp->SetPos( x, y );
	tmp->SetState( state );

	SendChangeCharacter( indexId, state, x, y );

	GetCharMgr.EndCritical();

}

//////////////////////////////////////////////////////////////////////////

void DemoGameSession::SendCreateCharacter( int indexId, int x, int y, TCHAR* id )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CREATE_CHARACTER );

	//우선 만들어야 하는 캐릭터의 수는 1명
	sendPacket << 1;

	//index넣고
	sendPacket << indexId;

	//id넣고
	int size = _tcslen( id ) * sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( id, size );

	//state넣고
	sendPacket << (int)0;

	//xy넣고
	sendPacket << x;
	sendPacket << y;

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[DemoGameSession] SendCreateCharacter-------------------------------------\n")
												  _T("\tPACKET ID : SC_GAME_CREATE_CHARACTER\n")
												  _T("\tPACKET SIZE / DATA SIZE : %d / %d\n")
												  _T("\tsessionID     : %d\n")
												  _T("\tchar ID       : %s\n")
												  _T("\tchar state    : 0\n")
												  _T("\tchar Pos(x/y) : %d, %d\n")
												  _T("[DemoGameSession] -------------------------------------SendCreateCharacter\n")
												  , sendPacket.GetPacketSize()
												  , sendPacket.GetDateFiledSize()
												  , indexId
												  , id 
												  , x
												  , y );
	/////////////////////////////////////////////////////////////////////////////////////////


	//이제 모두에게 보낸다.
	GetSessionMgr.SendAllSession( sendPacket );
}

void DemoGameSession::SendCreateCharacter( int myindex )
{
	//접속한게 나 혼자면 그냥 안해!
	if( GetCharMgr.GetCount() <= 1 )
		return;

 	SPacket sendPacket;
 
 	sendPacket.SetID( SC_GAME_CREATE_CHARACTER );

	//나 빼고 다머지를 만들어야 하니까 1을 빼준다.
	sendPacket << (GetCharMgr.GetCount()-1);

	//////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[DemoGameSession] SendCreateCharacter-------------------------------------\n")
												  _T("\tPACKET ID : SC_GAME_CREATE_CHARACTER\n")
												  _T("\t Char Count : %d\n")
												  , (GetCharMgr.GetCount()-1) );
	//////////////////////////////////////////////////////////////////////////

	int size = 0;
	//우선 크리티컬 섹션으로 들어 가 주고
	GetCharMgr.StartCritical();

 	itrCHARACTER tmpIter = GetCharMgr.GetHeader();
 	while( !GetCharMgr.IsEnd( tmpIter ) )
 	{
		//만약 내 정보면 넘어 간다.
		if( tmpIter->second->GetIndexID() == myindex )
		{
			++tmpIter;
			continue;
		}

		//indexId넣고
		sendPacket << tmpIter->second->GetIndexID();

		//id넣고
		size = _tcslen(tmpIter->second->GetStrID()) * sizeof(TCHAR);
		sendPacket << size;
		sendPacket.PutData( tmpIter->second->GetStrID(), size );

		//state넣고
		sendPacket << tmpIter->second->GetState();

		//x,y넣고
		sendPacket << tmpIter->second->GetX();
		sendPacket << tmpIter->second->GetY();

		//////////////////////////////////////////////////////////////////////////
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("\tsessionID     : %d\n")
													  _T("\tchar ID       : %s\n")
													  _T("\tchar state    : %d\n")
													  _T("\tchar Pos(x/y) : %d, %d\n")
													  _T("--------------------------------------------------------------------\n")
													  , tmpIter->second->GetIndexID()
													  , tmpIter->second->GetStrID()
													  , tmpIter->second->GetState()
													  , tmpIter->second->GetX()
													  , tmpIter->second->GetY() );
		//////////////////////////////////////////////////////////////////////////

		++tmpIter;
	}
	
	GetCharMgr.EndCritical();

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("\tPACKET SIZE / DATA SIZE : %d / %d\n")
												  _T("[DemoGameSession] -------------------------------------SendCreateCharacter\n")
			 									  , sendPacket.GetPacketSize()
												  , sendPacket.GetDateFiledSize() );
	/////////////////////////////////////////////////////////////////////////////////////////


	SendPacket( sendPacket );
}

void DemoGameSession::SendChangeCharacter( int indexId, int state, int x, int y )
{
	//접속한게 나 혼자면 안해도 된다.
	if( GetCharMgr.GetCount() < 1 )
		return;

	SPacket sendPacket;

	sendPacket.SetID( SC_GAME_CHANGE_STATE );

	sendPacket << indexId;
	sendPacket << state;
	sendPacket << x;
	sendPacket << y;

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[DemoGameSession] SendChangeCharacter-------------------------------------\n")
												  _T("\tPACKET ID : SC_GAME_CHANGE_STATE\n")
												  _T("\tPACKET SIZE / DATA SIZE : %d / %d\n")
												  _T("\tsessionID : %d\n")
												  _T("\tstate : %d\n")
												  _T("\tx, y : %d, %d\n")
												  _T("[DemoGameSession] -------------------------------------SendChangeCharacter\n")
												  , sendPacket.GetPacketSize()
												  , sendPacket.GetDateFiledSize()
												  , indexId
												  , state
												  , x
												  , y );
	/////////////////////////////////////////////////////////////////////////////////////////


	GetSessionMgr.SendAllSession( sendPacket );
	
}

void DemoGameSession::SendDeleteCharacter()
{
	//캐릭터는 지워주고
	GetCharMgr.DeleteCharacter( m_indexID );

	//접속한게 나 혼자면 그냥 안해!
	if( GetCharMgr.GetCount() < 1 )
		return;

	//모든 접속자에게 접속종료 패킷을 보내줘야 한다.
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_DELETE_CHARACTER );
	sendPacket << m_indexID;

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[DemoGameSession] SendDeleteCharacter-------------------------------------\n")
												  _T("\tPACKET ID : SC_GAME_DELETE_CHARACTER\n")
												  _T("\tPACKET SIZE / DATA SIZE : %d / %d\n")
												  _T("\tsessionID : %d\n")
												  _T("[DemoGameSession] -------------------------------------SendDeleteCharacter\n")
												  , sendPacket.GetPacketSize()
												  , sendPacket.GetDateFiledSize()
												  , m_indexID );
	/////////////////////////////////////////////////////////////////////////////////////////

	GetSessionMgr.SendAllSession( sendPacket );
}
