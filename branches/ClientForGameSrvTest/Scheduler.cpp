#include "Scheduler.h"

#include "PacketQueue.h"
#include "NetProtocol.h"
#include "Network.h"

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

	char* tmpBuf;

	for( int i=0; i<count; ++i )
	{
		tmpBuf = GetPacketQ.GetPacket();
		if( tmpBuf == NULL )
			return;

		m_packet.CopyToPacket( tmpBuf, PACKETDATA_SIZE );

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
	int a = 10;
}




