#include "Network.h"
#include "SThreadMgr.h"
#include "PlayerMgr.h"


void main()
{
	//ĳ���� ���� ����� ����
	if( !GetPlayerMgr.Init( 30 ) )
		return;

	if( !GetNetwork.Init( 30 ) )
		return;

	if( !GetNetwork.SrvSetting( 1234 ) )
		return;

	GetThreadMgr.IsEndAllThread();
}