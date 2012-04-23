#include "CheckDB.h"
#include "SLogger.h"

CheckDB::CheckDB(void)
{
}

CheckDB::~CheckDB(void)
{
}

BOOL CheckDB::Init( TCHAR* filename )
{
	return m_query.ConnectMdb( filename );
}

void CheckDB::Relase()
{
	m_query.DisConnect();
}

void CheckDB::GetData( int in_sessionId, TCHAR* out_ID )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select U_ID from tblUser where ID=%d"), in_sessionId );

	if( !m_query.Exec( strQuery ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("CheckDB::GetData()\nSQLExecDirect Failed\n") );
		return;
	}

	while( m_query.Fetch() != SQL_NO_DATA )
	{
		m_query.GetStr( _T("U_ID"), out_ID );
	}
	m_query.Clear();
}
