#include "DemoDBMgr.h"
#include "SLogger.h"

DemoDBMgr::DemoDBMgr(void)
{
}

DemoDBMgr::~DemoDBMgr(void)
{
}

int DemoDBMgr::AskLogin( TCHAR* charID, TCHAR* charPW )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select ID, U_ID,U_PW from tblUser where U_ID='%s'"), charID );

	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[DemoDBMgr] SQLExecDirect Failed(AskLogin())\n") );
		return -10;
	}

	int			id = 0;
	TCHAR		name[50]={0};
	TCHAR		pass[50]={0};
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		id = m_query.GetInt( _T("ID") );
		m_query.GetStr( _T("U_ID"), name );
		m_query.GetStr( _T("U_PW"), pass );
	}
	m_query.Clear();

	if( id <= 0 )
	{	
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[DemoDBMgr] �α��� ����_ ID�� �������� �ʽ��ϴ�.\n") );
		return -1;
	}

	//id�� ���� Ȯ�� ��
	if( _tcscmp( charPW, pass ) != 0 )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[DemoDBMgr] �α��� ����_ ����� Ʋ���ϴ�.\n") );
		return 0;		//����� Ʋ��
	}
	else
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[DemoDBMgr] �α��� ����.\n") );
		return id;
	}
}

BOOL DemoDBMgr::Init( TCHAR* filename )
{
	return m_query.ConnectMdb( filename );
}

void DemoDBMgr::Release()
{
	m_query.DisConnect();
}
