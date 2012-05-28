#include "Network.h"
#include "SThreadMgr.h"

void main()
{
	if( !GetNetwork.Init( 30 ) )
		return;

	if( !GetNetwork.SrvSetting( 12345 ) )
		return;

	GetThreadMgr.IsEndAllThread();
}