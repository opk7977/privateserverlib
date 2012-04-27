#include "Scheduler.h"

#include "SPacketQueue.h"
#include "NetProtocol.h"
#include "SNetwork.h"

unsigned int __stdcall _Schdul( void* pArg )
{
	return ((CScheduler*)pArg)->Run();
}

CScheduler::CScheduler(void)
{
}

CScheduler::~CScheduler(void)
{
}

void CScheduler::Init()
{

	m_startWork = ::CreateEvent( NULL, TRUE, FALSE, NULL); 

	unsigned int iThread;
	m_schThread = (HANDLE)_beginthreadex( NULL, 0, &::_Schdul, (void*)this, 0, &iThread );
	if( !m_schThread )
		return;

	m_timer.Init();
	m_frame = 0.f;

	SetEvent( m_startWork );
}

unsigned int CScheduler::Run()
{
	while(1)
	{
		Sleep(10);

		WaitForSingleObject( m_startWork, INFINITE );

		m_timer.ProcessTime();

		m_frame += m_timer.GetElapsedTime();

		if( m_frame >= 1.f )
		{
			m_frame -= 1.f;

			PacketProcess();
		}
	}
}

void CScheduler::PacketProcess()
{
	int count = GetPacketQ.GetDataCount();

	if( count <= 0 )
		return;

	//30���� ũ�� �׳� 30����
	if( count > 30 )
		count = 30;

	for( int i=0; i<count; ++i )
	{
		GetPacketQ.GetPacket( m_packet );

		//��Ŷ ó��
		PacketParsing();

		//ó���� ������ ��ġ�� �̵��Ѵ�.
		GetPacketQ.MoveReadPos();
	}
}

void CScheduler::PacketParsing()
{
	switch( m_packet.GetID() )
	{
	//==============================================================> LoginSrv
	
	//==============================================================> LobbySrv
	case SC_GAME_CONNECT_OK:
		RecvGameConnectOK();
		break;
	case SC_GAME_CHARINFO_INGAME:
		RecvGameCharInfoInGame();
		break;
	case SC_GAME_MOVE_CHAR:
		RecvGameMoveChar();
		break;

	//==============================================================> GameSrv

	//==============================================================> Error
	default:
		MessageBox( NULL, _T("���� ��Ŷ�̾� �̰�?"), _T("?????"), MB_OK );
	}
}

//��Ŷ�� ó���ϴ� �Լ�

//======================================
// LoginSrv
//======================================

//======================================
// LobbySrv
//======================================

//======================================
// GAME
//======================================
void CScheduler::RecvGameConnectOK()
{
	SPacket sendPacket;
	sendPacket.SetID( CS_GAME_INGAME );

	int retval = MessageBox( NULL, _T("id ����...����"), _T(""), MB_YESNOCANCEL );
	
	int session;

	if( retval == IDYES )
		session = 1;
	else if( retval == IDNO )
		session = 2;
	else
		session = 3;

	sendPacket << session;
	sendPacket << 1;

	retval = GetNetwork.SendPacket( &sendPacket );
	if( retval != sendPacket.GetPacketSize() )	
	{
		MessageBox( NULL, _T("��Ŷ ũ��� ��Ŷ�� ���۷��� �ٸ��ϴ�."), _T("����"), MB_OK );
		return;
	}
}

void CScheduler::RecvGameCharInfoInGame()
{
	MessageBox( NULL, _T("ĳ���� ������ �޾ҽ��ϴ�."), _T("����"), MB_OK );
}

void CScheduler::RecvGameMoveChar()
{
	MessageBox( NULL, _T("ĳ���� �̵������� �޾ҽ��ϴ�"), _T("����"), MB_OK );
}



