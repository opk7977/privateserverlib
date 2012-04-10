#include "SampleQuery.h"
#include "SLogger.h"

SampleQuery::SampleQuery(void)
{
}

SampleQuery::~SampleQuery(void)
{
}

BOOL SampleQuery::Init( TCHAR *str )
{
	return m_query.ConnectMdb( str );
}

void SampleQuery::Release()
{
	m_query.DisConnect();
}

BOOL SampleQuery::AllPrint()
{
	TCHAR		name[STRING_NUM]={0,}, pass[STRING_NUM]={0,};

	if( !m_query.Exec( _T("select * from tblUser") ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[SampleQuery] query error\tselect * from tblUser\n") );
		return FALSE;
	}

	system("cls");

	//읽어온 데이터 출력!
	printf("//////////저장되어 있는 사람////////////\n\n");
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		//받아 오기
		m_query.GetStr( L"U_ID", name );
		m_query.GetStr( L"U_PW", pass );
		_tcprintf( L" %3d번 이름: %s, 비번: %s\n", m_query.GetInt( L"ID" ), name, pass );
	}
	printf("\n////////////////////////////////////////\n");

	m_query.Clear();
	GetChForMoment();
	return TRUE;
}

BOOL SampleQuery::AddMember()
{
	TCHAR newName[STRING_NUM]={0,};
	ScanStr( _T("새로 입력할 이름을 적어 주세요.\n"), newName );

	////ODBC연결/////////////////////////////////////////////////////////////////////////
	//이름을 받았으면 DB에 중복되는 이름이 있는지 확인 해야한다.
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select U_ID from tblUser where U_ID='%s'"), newName );

	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[SampleQuery] AddMember() select error\n") );
		return FALSE;
	}

	//ID중복 확인
	if( m_query.Fetch() != SQL_NO_DATA )
	{
		_tcprintf( _T("입력하신 ID %s는 현재 존재하는 ID입니다.\n"), newName );
		m_query.Clear();
		GetChForMoment();
		return TRUE;
	}

	m_query.Clear();

	/////////////////////////////////////////////////////////////////////////////////////
	TCHAR	pass[STRING_NUM]={0};
	ScanStr( _T("패스워드를 입력하세요.\n"), pass );

	////ODBC연결////////////////////////////////////////////////////////////////////////
	//공간을 한번 초기화 해 준다음에
	ZeroMemory( strQuery, 255 );
	//입력 받은 값을 추가해준다.
	wsprintf( (TCHAR*)strQuery, _T("insert into tblUser (U_ID,U_PW) VALUES ('%s','%s')"), newName, pass );

	if( !m_query.Exec(strQuery) )
	{
		GetLogger.PutLog( 0, _T("[SampleQuery] 레코드 추가 에러\n") );
		return FALSE;
	}

	m_query.Clear();
	printf( "정상적으로 추가 되었습니다.\n" );
	GetChForMoment();
	////////////////////////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL SampleQuery::ChangeMember()
{
	TCHAR newName[STRING_NUM]={0};
	ScanStr( _T("수정할 이름을 적어 주세요.\n"), newName );

	////ODBC연결/////////////////////////////////////////////////////////////////////////
	//이름을 받았으면 DB에 이름이 있는지 확인 해야한다.
	TCHAR		name[STRING_NUM]={0};
	TCHAR		pass[STRING_NUM]={0};

	//SQL문을 실행한다.
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select U_ID,U_PW from tblUser where U_ID='%s'"), newName );

	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( 0, _T("[SampleQuery] SQLExecDirect(changeMember())\n") );
		return FALSE;
	}
	int count = 0;
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		++count;

		m_query.GetStr( _T("U_ID"), name );
		m_query.GetStr( _T("U_PW"), pass );
	}
	m_query.Clear();

	if( count <= 0 )
	{	
		_tcprintf( _T("입력하신 ID %s는 존재하지 않는 ID입니다.\n"), newName );
		m_query.Clear();
		GetChForMoment();
		return TRUE;
	}
	else if( count > 1 )
	{
		_tcprintf( _T("검색된 ID의 data가 복수입니다.\n수정하세요..\n") );
		GetChForMoment();
		return TRUE;
	}

	m_query.Clear();

	//////////////////////////////////////////////////////////////////////////
	TCHAR	myPass[STRING_NUM];
	ScanStr( _T("ID 가 확인되었습니다\n수정하실 패스워드를 입력하세요\n"), myPass );
	/////////////////////////////////////////////////////////////////////////////////////

	////ODBC연결////////////////////////////////////////////////////////////////////////
	//입력 받은 값을 추가해준다.
	//공간을 한번 초기화 해 준다음에
	ZeroMemory( strQuery, 255 );

	wsprintf( (TCHAR*)strQuery, _T("update tblUser set U_PW='%s' where U_ID='%s'"), myPass, newName );
	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( 0, _T("[SampleQuery] 레코드 수정 에러\n") );
		return FALSE;
	}
	m_query.Clear();
	printf( "정상적으로 수정 되었습니다.\n" );
	GetChForMoment();
	////////////////////////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL SampleQuery::DelMember()
{
	TCHAR newName[STRING_NUM]={0};
	ScanStr( _T("삭제할 이름을 적어 주세요.\n"), newName );

	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select U_ID,U_PW from tblUser where U_ID='%s'"), newName );

	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[SampleQuery] del error\n") );
		return FALSE;
	}

	//이름과 비번을 받아 놓을 변수.
	TCHAR		name[STRING_NUM]={0};
	TCHAR		pass[STRING_NUM]={0};
	//ID가 있는지를 확인하기 위한 임시 변수
	BOOL		isData = false;

	while( m_query.Fetch() != SQL_NO_DATA )
	{
		//data가 있다고 표시 해 주고
		if( !isData )
			isData = TRUE;

		m_query.GetStr( _T("U_ID"), name );
		m_query.GetStr( _T("U_PW"), pass );
	}

	m_query.Clear();

	if( !isData )
	{
		_tcprintf( _T("입력하신 ID %s는 존재하지 않는 ID입니다.\n"), newName );
		m_query.Clear();
		GetChForMoment();
		return TRUE;
	}

	//비번확인을 위한 임시 변수
	TCHAR	myPass[STRING_NUM]={0};
	ScanStr( _T("ID 가 확인되었습니다\n삭제를 진행하시려면 패스워드를 입력하세요.\n"), myPass );

	if( wcscmp( pass, myPass ) != 0 )
	{
		printf("password를 다시 확인해 주세요\n");
		m_query.Clear();
		GetChForMoment();
		return TRUE;
	}
	//이렇게 내려오면 pass도 맞다는게 된다
	//삭제해 주자!!
	ZeroMemory( strQuery, 255 );

	wsprintf( (TCHAR*)strQuery, _T("delete from tblUser where U_ID='%s'"), name );
	if ( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[SampleQuery] 레코드 삭제 에러\n") );
		return FALSE;
	}

	m_query.Clear();

	printf( "정상적으로 삭제 되었습니다.\n" );

	GetChForMoment();
	////////////////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void SampleQuery::ScanStr( TCHAR* str, TCHAR* tmp )
{
	_tcprintf( _T("%s"), str );
	printf("=> ");
	_tscanf_s( _T("%s"), tmp, STRING_NUM );
}

void SampleQuery::GetChForMoment()
{
	fflush( stdin );
	getchar();
}