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

	//�о�� ������ ���!
	printf("//////////����Ǿ� �ִ� ���////////////\n\n");
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		//�޾� ����
		m_query.GetStr( L"U_ID", name );
		m_query.GetStr( L"U_PW", pass );
		_tcprintf( L" %3d�� �̸�: %s, ���: %s\n", m_query.GetInt( L"ID" ), name, pass );
	}
	printf("\n////////////////////////////////////////\n");

	m_query.Clear();
	GetChForMoment();
	return TRUE;
}

BOOL SampleQuery::AddMember()
{
	TCHAR newName[STRING_NUM]={0,};
	ScanStr( _T("���� �Է��� �̸��� ���� �ּ���.\n"), newName );

	////ODBC����/////////////////////////////////////////////////////////////////////////
	//�̸��� �޾����� DB�� �ߺ��Ǵ� �̸��� �ִ��� Ȯ�� �ؾ��Ѵ�.
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select U_ID from tblUser where U_ID='%s'"), newName );

	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[SampleQuery] AddMember() select error\n") );
		return FALSE;
	}

	//ID�ߺ� Ȯ��
	if( m_query.Fetch() != SQL_NO_DATA )
	{
		_tcprintf( _T("�Է��Ͻ� ID %s�� ���� �����ϴ� ID�Դϴ�.\n"), newName );
		m_query.Clear();
		GetChForMoment();
		return TRUE;
	}

	m_query.Clear();

	/////////////////////////////////////////////////////////////////////////////////////
	TCHAR	pass[STRING_NUM]={0};
	ScanStr( _T("�н����带 �Է��ϼ���.\n"), pass );

	////ODBC����////////////////////////////////////////////////////////////////////////
	//������ �ѹ� �ʱ�ȭ �� �ش�����
	ZeroMemory( strQuery, 255 );
	//�Է� ���� ���� �߰����ش�.
	wsprintf( (TCHAR*)strQuery, _T("insert into tblUser (U_ID,U_PW) VALUES ('%s','%s')"), newName, pass );

	if( !m_query.Exec(strQuery) )
	{
		GetLogger.PutLog( 0, _T("[SampleQuery] ���ڵ� �߰� ����\n") );
		return FALSE;
	}

	m_query.Clear();
	printf( "���������� �߰� �Ǿ����ϴ�.\n" );
	GetChForMoment();
	////////////////////////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL SampleQuery::ChangeMember()
{
	TCHAR newName[STRING_NUM]={0};
	ScanStr( _T("������ �̸��� ���� �ּ���.\n"), newName );

	////ODBC����/////////////////////////////////////////////////////////////////////////
	//�̸��� �޾����� DB�� �̸��� �ִ��� Ȯ�� �ؾ��Ѵ�.
	TCHAR		name[STRING_NUM]={0};
	TCHAR		pass[STRING_NUM]={0};

	//SQL���� �����Ѵ�.
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
		_tcprintf( _T("�Է��Ͻ� ID %s�� �������� �ʴ� ID�Դϴ�.\n"), newName );
		m_query.Clear();
		GetChForMoment();
		return TRUE;
	}
	else if( count > 1 )
	{
		_tcprintf( _T("�˻��� ID�� data�� �����Դϴ�.\n�����ϼ���..\n") );
		GetChForMoment();
		return TRUE;
	}

	m_query.Clear();

	//////////////////////////////////////////////////////////////////////////
	TCHAR	myPass[STRING_NUM];
	ScanStr( _T("ID �� Ȯ�εǾ����ϴ�\n�����Ͻ� �н����带 �Է��ϼ���\n"), myPass );
	/////////////////////////////////////////////////////////////////////////////////////

	////ODBC����////////////////////////////////////////////////////////////////////////
	//�Է� ���� ���� �߰����ش�.
	//������ �ѹ� �ʱ�ȭ �� �ش�����
	ZeroMemory( strQuery, 255 );

	wsprintf( (TCHAR*)strQuery, _T("update tblUser set U_PW='%s' where U_ID='%s'"), myPass, newName );
	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( 0, _T("[SampleQuery] ���ڵ� ���� ����\n") );
		return FALSE;
	}
	m_query.Clear();
	printf( "���������� ���� �Ǿ����ϴ�.\n" );
	GetChForMoment();
	////////////////////////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL SampleQuery::DelMember()
{
	TCHAR newName[STRING_NUM]={0};
	ScanStr( _T("������ �̸��� ���� �ּ���.\n"), newName );

	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select U_ID,U_PW from tblUser where U_ID='%s'"), newName );

	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[SampleQuery] del error\n") );
		return FALSE;
	}

	//�̸��� ����� �޾� ���� ����.
	TCHAR		name[STRING_NUM]={0};
	TCHAR		pass[STRING_NUM]={0};
	//ID�� �ִ����� Ȯ���ϱ� ���� �ӽ� ����
	BOOL		isData = false;

	while( m_query.Fetch() != SQL_NO_DATA )
	{
		//data�� �ִٰ� ǥ�� �� �ְ�
		if( !isData )
			isData = TRUE;

		m_query.GetStr( _T("U_ID"), name );
		m_query.GetStr( _T("U_PW"), pass );
	}

	m_query.Clear();

	if( !isData )
	{
		_tcprintf( _T("�Է��Ͻ� ID %s�� �������� �ʴ� ID�Դϴ�.\n"), newName );
		m_query.Clear();
		GetChForMoment();
		return TRUE;
	}

	//���Ȯ���� ���� �ӽ� ����
	TCHAR	myPass[STRING_NUM]={0};
	ScanStr( _T("ID �� Ȯ�εǾ����ϴ�\n������ �����Ͻ÷��� �н����带 �Է��ϼ���.\n"), myPass );

	if( wcscmp( pass, myPass ) != 0 )
	{
		printf("password�� �ٽ� Ȯ���� �ּ���\n");
		m_query.Clear();
		GetChForMoment();
		return TRUE;
	}
	//�̷��� �������� pass�� �´ٴ°� �ȴ�
	//������ ����!!
	ZeroMemory( strQuery, 255 );

	wsprintf( (TCHAR*)strQuery, _T("delete from tblUser where U_ID='%s'"), name );
	if ( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[SampleQuery] ���ڵ� ���� ����\n") );
		return FALSE;
	}

	m_query.Clear();

	printf( "���������� ���� �Ǿ����ϴ�.\n" );

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