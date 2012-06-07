#include "SNetwork.h"
#include "SUDPNet.h"

#include "CharMgr.h"
#include "PacketParser.h"

#include "sample.h"
#include "crtdbg.h"

Sample_UDP_Client::Sample_UDP_Client(void)
{
	m_charMgr	= &GetCharMgr;
	m_parser	= &GetParser;
	m_net		= &GetNetwork;
}

Sample_UDP_Client::~Sample_UDP_Client(void)
{
}

bool Sample_UDP_Client::Init(void)
{
	m_charMgr->Init();

	//���� ����
	if( !m_net->Init() )
		return false;

	if( !m_net->ConnectToSrv( "192.168.0.56", 1234 ) )
		return false;

	//udp����
	if( !GetUNet.Init( 4321 ) )
		return false;

	return true;
}

bool Sample_UDP_Client::Frame(void)
{
	//���⼭ ��Ŷ parsing
	m_parser->PacketProcess();

 	m_charMgr->Frame();

	//���⼭ packet send
	m_net->SendPacket();

	return true;
}

bool Sample_UDP_Client::Render(void)
{
	m_charMgr->Render();

	return true;
}

bool Sample_UDP_Client::Release(void)
{
	m_charMgr->Release();

	return true;
}

int APIENTRY _tWinMain( HINSTANCE hInst, HINSTANCE, LPTSTR, int )
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(  );

	int ret = 0;
	{
		Sample_UDP_Client obj;
		if( !obj.CreateWin( hInst, _T("Sample_UDP_Client"), 800, 600 ) )
			return -1;

		if( !obj.EngineInit() )
			return -1;

		ret = obj.Run();
	}

	return ret;
}