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

	//�켱 �α����ʿ��� �޾ƿ� list���� Ȯ���ؼ�
	//���������� �����ߴٸ�
	//connectOK�� ������.
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


	//�켱 id���� �޾� ����
	(*packet) >> indexID;
	//�켱 ������ ���´�.
	m_indexID = indexID;

	(*packet) >> size;
	packet->GetData( charID, size );

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[DemoGameSession] Recv Packet------------------------------------\n")
												  _T("\tPACKET ID : CS_GAME_SEND_INDECX\n")
												  _T("\t������ ĳ������ ���� id�� �޾� ���ο� ĳ���͸� �����Ѵ�.\n")
												  _T("\tSessionID : %d\n")
												  _T("\tchar ID	 : %s\n")
												  _T("[DemoGameSession] -------------------------------------RecvCreateChar\n")
												  , indexID
												  , charID );
	/////////////////////////////////////////////////////////////////////////////////////////

	srand( (unsigned int)time(NULL) );
	//ĳ���͸� ����!->�޴����� ���� ��ο��� �ش� ĳ���� ������ ������
	int x = rand()%800;
	int y = rand()%600;
	GetCharMgr.CreateCharacter( indexID, x, y, charID );

	//��ο��� ������ ����� ���� �ش�.
	SendCreateCharacter( indexID, x, y, charID );

	//������ �� ���� ��� ĳ������ ������ ������
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
												  _T("\tĳ������ ���°� ��ȭ�Ѵ�.\n")
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

	//�켱 ������ �ϴ� ĳ������ ���� 1��
	sendPacket << 1;

	//index�ְ�
	sendPacket << indexId;

	//id�ְ�
	int size = _tcslen( id ) * sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( id, size );

	//state�ְ�
	sendPacket << (int)0;

	//xy�ְ�
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


	//���� ��ο��� ������.
	GetSessionMgr.SendAllSession( sendPacket );
}

void DemoGameSession::SendCreateCharacter( int myindex )
{
	//�����Ѱ� �� ȥ�ڸ� �׳� ����!
	if( GetCharMgr.GetCount() <= 1 )
		return;

 	SPacket sendPacket;
 
 	sendPacket.SetID( SC_GAME_CREATE_CHARACTER );

	//�� ���� �ٸ����� ������ �ϴϱ� 1�� ���ش�.
	sendPacket << (GetCharMgr.GetCount()-1);

	//////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[DemoGameSession] SendCreateCharacter-------------------------------------\n")
												  _T("\tPACKET ID : SC_GAME_CREATE_CHARACTER\n")
												  _T("\t Char Count : %d\n")
												  , (GetCharMgr.GetCount()-1) );
	//////////////////////////////////////////////////////////////////////////

	int size = 0;
	//�켱 ũ��Ƽ�� �������� ��� �� �ְ�
	GetCharMgr.StartCritical();

 	itrCHARACTER tmpIter = GetCharMgr.GetHeader();
 	while( !GetCharMgr.IsEnd( tmpIter ) )
 	{
		//���� �� ������ �Ѿ� ����.
		if( tmpIter->second->GetIndexID() == myindex )
		{
			++tmpIter;
			continue;
		}

		//indexId�ְ�
		sendPacket << tmpIter->second->GetIndexID();

		//id�ְ�
		size = _tcslen(tmpIter->second->GetStrID()) * sizeof(TCHAR);
		sendPacket << size;
		sendPacket.PutData( tmpIter->second->GetStrID(), size );

		//state�ְ�
		sendPacket << tmpIter->second->GetState();

		//x,y�ְ�
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
	//�����Ѱ� �� ȥ�ڸ� ���ص� �ȴ�.
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
	//ĳ���ʹ� �����ְ�
	GetCharMgr.DeleteCharacter( m_indexID );

	//�����Ѱ� �� ȥ�ڸ� �׳� ����!
	if( GetCharMgr.GetCount() < 1 )
		return;

	//��� �����ڿ��� �������� ��Ŷ�� ������� �Ѵ�.
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
