//#include "WinMgr.h"
#include "LoginMain.h"

#include "DataLeader.h"
#include "SLogger.h"

void main()
{
	//�α��ʱ�ȭ
	GetLogger.Create( "LoginSrv" );

	//������ ���� ���� ����
 	if( !GetDocument.DataSetting() )
 		return;

	LoginMain lMain;
	lMain.Init();

	lMain.Run();

	lMain.Relase();
}