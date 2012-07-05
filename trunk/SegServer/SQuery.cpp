#include "SQuery.h"

#ifdef _DEBUG
	#include "SLogger.h"
#endif

SQuery::SQuery(void)
{
}

SQuery::~SQuery(void)
{
	//DisConnect();
}

BOOL SQuery::ConnectMdb( TCHAR* conStr )
{
	SQLRETURN	retcode;

	//ȯ���ڵ� �Ҵ� �ޱ�//////////////////////////////////////////////////////
	retcode = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] ȯ���ڵ� �Ҵ� ����\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] ȯ���ڵ� �Ҵ� ����\n") );
#endif
	//////////////////////////////////////////////////////////////////////////

	//ȯ���ڵ� ������ 3.0���� ����////////////////////////////////////////////
	retcode = SQLSetEnvAttr( m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] ȯ���ڵ� ���� ���� ����\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] ȯ���ڵ� ���� ���� ����\n") );
#endif
	//////////////////////////////////////////////////////////////////////////

	//�����ڵ� �Ҵ��ϰ� ȯ���ڵ鿡 �����ϱ�///////////////////////////////////
	retcode = SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hDbc );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] �����ڵ� �Ҵ� ����\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] �����ڵ� �Ҵ� ����\n") );
#endif
	//////////////////////////////////////////////////////////////////////////

	//mdb�� ����//////////////////////////////////////////////////////////////
	//��θ� ���ڿ��� �ϼ��ϱ� ���� ����
	TCHAR		Dir[255];

	//���� ������ ��θ� ��� �´�.
	GetCurrentDirectory( 255, Dir );
	//��ο� mdb������ �ּҸ� ���� �ϼ��� ��θ� �����.
	wsprintf( (TCHAR*)ConnectDirMdb, _T("DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s\\%s;"), Dir, conStr );
	//�α׷� ��� ���
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] %s\n"), (TCHAR*)ConnectDirMdb );
#endif

	retcode = SQLDriverConnect( m_hDbc, NULL, ConnectDirMdb, sizeof(ConnectDirMdb), NULL, 0, NULL, SQL_DRIVER_NOPROMPT );
	//���ῡ ������ �ִٸ� return�ϰ� log�� �����.
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] mdb���� ����\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] mdb���� ����\n") );
#endif

	//��� �ڵ��� �Ҵ�����
	retcode = SQLAllocHandle( SQL_HANDLE_STMT, m_hDbc, &m_hStmt );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] ����ڵ� �Ҵ� ����\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] ����ڵ� �Ҵ� ����\n") );
#endif

	//��
	return TRUE; 
}

BOOL SQuery::ConnectSrv( TCHAR* conStr, TCHAR* srvID, TCHAR* srvPW )
{
	SQLRETURN	retcode;

	//ȯ���ڵ� �Ҵ� �ޱ�//////////////////////////////////////////////////////
	retcode = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] ȯ���ڵ� �Ҵ� ����\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] ȯ���ڵ� �Ҵ� ����\n") );
#endif
	//////////////////////////////////////////////////////////////////////////

	//ȯ���ڵ� ������ 3.0���� ����////////////////////////////////////////////
	retcode = SQLSetEnvAttr( m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] ȯ���ڵ� ���� ���� ����\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] ȯ���ڵ� ���� ���� ����\n") );
#endif
	//////////////////////////////////////////////////////////////////////////

	//�����ڵ� �Ҵ��ϰ� ȯ���ڵ鿡 �����ϱ�///////////////////////////////////
	retcode = SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hDbc );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] �����ڵ� �Ҵ� ����\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] �����ڵ� �Ҵ� ����\n") );
#endif
	//////////////////////////////////////////////////////////////////////////

	//Srv�� ����//////////////////////////////////////////////////////////////
	retcode = SQLConnect( m_hDbc, conStr, SQL_NTS, srvID, SQL_NTS, srvPW, SQL_NTS );
	//���ῡ ������ �ִٸ� return�ϰ� log�� �����.
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] mdb���� ����\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] mdb���� ����\n") );
#endif

	//��� �ڵ��� �Ҵ�����
	retcode = SQLAllocHandle( SQL_HANDLE_STMT, m_hDbc, &m_hStmt );
	if( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] ����ڵ� �Ҵ� ����\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] ����ڵ� �Ҵ� ����\n") );
#endif

	//��
	return TRUE; 
}

void SQuery::DisConnect()
{
	//������ �Ҵ� �ߴ� �ݴ� ������ �� ��� �Ѵ�.
	//����ڵ�
	if( m_hStmt )
	{
		SQLFreeHandle( SQL_HANDLE_STMT, m_hStmt );
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] ��� �ڵ� �Ҵ� ����\n") );
#endif
	}

	//�����ڵ�
	if( m_hDbc )
	{
		//db���� ������� ���� �ش�
		SQLDisconnect( m_hDbc );
		SQLFreeHandle( SQL_HANDLE_DBC, m_hDbc );
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] ���� �ڵ� �Ҵ� ����\n") );
#endif
	}

	//ȯ���ڵ�
	if( m_hEnv )
	{
		SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] ȯ�� �ڵ� �Ҵ� ����\n") );
#endif
	}
}

SQLRETURN SQuery::BindParamaterStr( int index, TCHAR* strParam, SQLINTEGER &jnk, BOOL isIn /*= TRUE */ )
{
	SQLSMALLINT inOut;
	SQLINTEGER iJnk = SQL_NTS;

	if( isIn )
		inOut = SQL_PARAM_INPUT;
	else
		inOut = SQL_PARAM_OUTPUT;

	return SQLBindParameter( m_hStmt, index, inOut, SQL_C_WCHAR, SQL_WCHAR, _tcslen(strParam), 0, strParam, 0, &jnk );
}

SQLRETURN SQuery::BindParamaterInt( int index, int &iParam, SQLINTEGER &jnk, BOOL isIn /*= TRUE */ )
{
	SQLSMALLINT inOut;

	if( isIn )
		inOut = SQL_PARAM_INPUT;
	else
		inOut = SQL_PARAM_OUTPUT;

	return SQLBindParameter( m_hStmt, index, inOut, SQL_C_LONG, SQL_INTEGER, 0, 0, &iParam, 0, &jnk );
}

BOOL SQuery::Exec( TCHAR* szSQL )
{
	//����ڵ��� ����Ǿ� ���� �ʴٸ� return ( ��� �ڵ� )
	if( !m_hStmt )
		return FALSE;

	SQLRETURN	ret;
	ret = SQLExecDirect( m_hStmt, (SQLWCHAR*)szSQL, SQL_NTS );
	//������ ���� false�� return�� �ְ�log�� �����
	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Query] ���� ����\n%s\n\n"), szSQL );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] ���� ����\n")
												 _T("\t%s\n"), szSQL );
#endif

	//���ڵ��� ������ ���Ѵ�.
	SQLRowCount( m_hStmt, &m_iRowCount );
	//�÷��� ������ ���Ѵ�.
	SQLNumResultCols( m_hStmt, &m_iColCount );
	//�÷��� ������ �ִ�ġ�� �Ѿ� ���� �ȵȴ�
	if( m_iColCount > MAX_COL )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] �÷��� ������ �ִ�ġ�� �Ѿ� ���ϴ�.\n") );
#endif
		return FALSE;
	}

	//�÷��� ������ 0�ΰ�� - bind�� �ʿ䰡 ���� ����� ����( delete or insert )
	if( m_iColCount == 0 )
	{
		//�������� ������ ���� �ְ�
		Clear();
		return TRUE;
	}

	//bind�� ��� �Ѵ�.
	//col�迭�� 0���� �÷� ��ȣ�� 1����!!
	for( int i=0; i<m_iColCount; ++i )
	{
		SQLBindCol( m_hStmt, i+1, SQL_C_TCHAR, m_tstrBindCol[i], 255, &m_iCol[i] );
		//�÷��� ��Ű������ �޾� �´�( �Ӽ��� �̸��� )
		SQLDescribeCol( m_hStmt, i+1, m_tchColName[i], 40, NULL, NULL, NULL, NULL, NULL );
	}

	return TRUE;
}

SQLRETURN SQuery::Fetch()
{
	return SQLFetch( m_hStmt );
}

void SQuery::Clear()
{
	SQLCloseCursor( m_hStmt );
	SQLFreeStmt( m_hStmt, SQL_UNBIND );

	m_iRowCount = 0;
}

int SQuery::GetInt( int nCol )
{
	if( nCol > m_iColCount )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] �Ѱ� ���� int�� �÷� count���� Ů�ϴ�.\n") );
#endif
		return -1;
	}
	if( m_iCol[nCol-1] == SQL_NULL_DATA )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] �����Ͱ� �����ϴ�\n") );
#endif
		return -1;
	}
	else
	{
		int aaa = _ttoi( m_tstrBindCol[nCol-1] );
		return aaa;
	}
}

int SQuery::GetInt( TCHAR* sCol )
{
	//�޾ƿ� �÷��� �̸����� index�� �˻��Ѵ�
	int n = FindCollasName( sCol );
	if( n < 0 )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] �Է��� Col Name�� �ش��ϴ� �÷��� �����ϴ�\n") );
#endif
		return -1;
	}
	else
		return GetInt( n );
}

void SQuery::GetStr( int nCol, TCHAR* buf )
{
	//ZeroMemory( buf, 255 );

	if( nCol > m_iColCount )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] �Ѱ� ���� int�� �÷� count���� Ů�ϴ�.\n") );
#endif
		return;
	}
	if( m_iCol[nCol-1] == SQL_NULL_DATA )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] �����Ͱ� �����ϴ�\n") );
#endif
		lstrcpy( buf, _T("NULL") );
	}
	else
	{
		CopyMemory( buf, m_tstrBindCol[nCol-1], wcslen(m_tstrBindCol[nCol-1])*sizeof(TCHAR) );
	}
}

void SQuery::GetStr( TCHAR* sCol, TCHAR* buf )
{
	int n = FindCollasName( sCol );
	if( n < 0 )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Query] �Է��� Col Name�� �ش��ϴ� �÷��� �����ϴ�\n") );
#endif
		//ZeroMemory( buf, 255 );
	}
	else
		GetStr( n, buf );
}

int SQuery::FindCollasName( TCHAR* colName )
{
	for( int i=0; i<m_iColCount; ++i )
	{
		//����� 0�̶�� ���� ���ڿ��̴ϱ�
		if( wcscmp( colName, (LPCTSTR)m_tchColName[i] ) == 0 )
			return i+1;
	}
	//��ġ�ϴ� �÷��� ������ -1�� return�Ѵ�.
	return -1;
}
