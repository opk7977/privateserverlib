#include "Network.h"
#include "SThreadMgr.h"
#include "PlayerMgr.h"
#include "SLogger.h"


void main()
{
	//�α� �ʱ�ȭ
	GetLogger.Create( "testSrvforUDP" );

	//ĳ���� ���� ����� ����
	if( !GetPlayerMgr.Init( 30 ) )
		return;

	if( !GetNetwork.Init( 30 ) )
		return;

	if( !GetNetwork.SrvSetting( 1234 ) )
		return;

	GetThreadMgr.IsEndAllThread();

	GetLogger.Release();
}