#include "SampleQuery.h"
#include "SLogger.h"
//�ѱ� �Է��� ���� ����ؾ� �ϴ� ���
//#include <locale.h>

int SelectMenu()
{
	system( "cls" );
	printf( "============================\n" );
	printf( " 1. ���� �����\n" );
	printf( " 2. ȸ�� ��κ���\n" );
	printf( " 3. ȸ�� ���������ϱ�\n" );
	printf( " 4. ȸ�� �����ϱ�\n" );
	printf( " 0. ����\n" );
	printf("==> ");
	
	int selectmenu;
	scanf_s( "%d", &selectmenu );
	if( selectmenu >= 1 && selectmenu <= 4 )
		return selectmenu;
	else
		return 0;
}

void main()
{
	//�ѱ��� �Էµǰ� ������ ��� �Ѵ�!!!!!!!
	//_tsetlocale(LC_ALL, _T("Korean")); 

	SampleQuery query;

	//���� �ʱ�ȭ/////////////////////////////////////////////////////////////
	if( !query.Init(_T("testDB.mdb")) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[main] DataBase Connect Error\n") );
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	int menu;

	while(1)
	{
		menu = SelectMenu();

		switch( menu )
		{
		case 1:		//�ɹ� �߰�
			query.AddMember();
			break;
		case 2:		//�ɹ� ��κ���
			query.AllPrint();
			break;
		case 3:		//�ɹ� ����
			query.ChangeMember();
			break;
		case 4:		//ȸ�� ����
			query.DelMember();
			break;
		case 0:		//����
			//query.Release();
			return;
		}
	}
}