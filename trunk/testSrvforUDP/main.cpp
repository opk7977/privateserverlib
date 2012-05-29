#include "Network.h"
#include "SThreadMgr.h"
#include "PlayerMgr.h"


void main()
{
	//캐릭터 부터 만들어 놓자
	if( !GetPlayerMgr.Init( 30 ) )
		return;

	if( !GetNetwork.Init( 30 ) )
		return;

	if( !GetNetwork.SrvSetting( 1234 ) )
		return;

	GetThreadMgr.IsEndAllThread();
}