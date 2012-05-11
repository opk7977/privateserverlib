//#include "WinMgr.h"
#include "LoginMain.h"

#include "DataLeader.h"
#include "SLogger.h"

void main()
{
	//로그초기화
	GetLogger.Create( "LoginSrv" );

	//데이터 부터 셋팅 하자
 	if( !GetDocument.DataSetting() )
 		return;

	LoginMain lMain;
	lMain.Init();

	lMain.Run();

	lMain.Relase();
}