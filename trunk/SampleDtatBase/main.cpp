#include "SampleQuery.h"
#include "SLogger.h"
//한글 입력을 위해 사용해야 하는 헤더
//#include <locale.h>

int SelectMenu()
{
	system( "cls" );
	printf( "============================\n" );
	printf( " 1. 새로 만들기\n" );
	printf( " 2. 회원 모두보기\n" );
	printf( " 3. 회원 정보수정하기\n" );
	printf( " 4. 회원 삭제하기\n" );
	printf( " 0. 종료\n" );
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
	//한글을 입력되게 선언해 줘야 한다!!!!!!!
	//_tsetlocale(LC_ALL, _T("Korean")); 

	SampleQuery query;

	//연결 초기화/////////////////////////////////////////////////////////////
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
		case 1:		//맴버 추가
			query.AddMember();
			break;
		case 2:		//맴버 모두보기
			query.AllPrint();
			break;
		case 3:		//맴버 수정
			query.ChangeMember();
			break;
		case 4:		//회원 삭제
			query.DelMember();
			break;
		case 0:		//종료
			//query.Release();
			return;
		}
	}
}