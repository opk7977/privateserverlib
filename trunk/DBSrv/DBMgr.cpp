#include "DBMgr.h"
#include "PlayerMgr.h"

DBMgr::DBMgr(void)
{
	m_rankMgr = &GetRankMgr;
}

DBMgr::~DBMgr(void)
{
}

BOOL DBMgr::Init( TCHAR* dbname, TCHAR* _id, TCHAR* _pw )
{
	if( !m_query.ConnectSrv( dbname, _id, _pw ) )
		return FALSE;

	return TRUE;
}

void DBMgr::Release()
{
	m_query.DisConnect();
}

BOOL DBMgr::RankDataSetting()
{
	if( !m_query.Exec( _T("{call AllRank};") ) )
		return FALSE;

	int rankId;
	int needPoint;
	while( m_query.Fetch() != SQL_NO_DATA )
	{
		rankId = m_query.GetInt( _T("RANK_ID") );
		needPoint = m_query.GetInt( _T("RANK_NEED_POINT") );

		m_rankMgr->AddData( rankId, needPoint );
	}

	m_query.Clear();

	return TRUE;
}

BOOL DBMgr::CheckID( TCHAR* _id )
{
	// return 받을 결과값
	int result = 0;
	SQLINTEGER iJnk = SQL_NTS;
	SQLRETURN sqlret;

	sqlret = m_query.BindParamaterInt( 1, result, iJnk, FALSE );
	sqlret = m_query.BindParamaterStr( 2, _id, iJnk, TRUE );

	if( !m_query.Exec( _T("{?=call CheckID(?)};") ) )
	{
		return FALSE;
	}

	// id존재 사용불가
	if( result > 0 )
		return FALSE;
	
	return TRUE;
}

BOOL DBMgr::CreateAccount( TCHAR* _id, TCHAR* _pw, TCHAR* email )
{
	SQLINTEGER iJnk = SQL_NTS;
	SQLRETURN sqlret;

	sqlret = m_query.BindParamaterStr( 1, _id, iJnk, TRUE );
	sqlret = m_query.BindParamaterStr( 2, _pw, iJnk, TRUE );
	sqlret = m_query.BindParamaterStr( 3, email, iJnk, TRUE );

	if( !m_query.Exec( _T("{call InsertAccountItem(?,?,?)};") ) )
	{
		return FALSE;
	}

	return TRUE;
}

int DBMgr::TryLogin( TCHAR* _id, TCHAR* _pw )
{
	// return 받을 결과값
	int result = 0;
	SQLINTEGER iJnk = SQL_NTS;
	SQLRETURN sqlret;

	sqlret = m_query.BindParamaterInt( 1, result, iJnk, FALSE );
	sqlret = m_query.BindParamaterStr( 2, _id, iJnk, TRUE );
	sqlret = m_query.BindParamaterStr( 3, _pw, iJnk, TRUE );

	if( !m_query.Exec( _T("{?=call TryLogin(?,?)};") ) )
	{
		return -10;
	}

	return result;
}

BOOL DBMgr::SettingUserData( int sessionID, PlayerObj* player )
{
	SQLINTEGER iJnk = SQL_NTS;
	SQLRETURN sqlret;

	sqlret = m_query.BindParamaterInt( 1, sessionID, iJnk, TRUE );

	if( !m_query.Exec( _T("{call GetUserData(?)};") ) )
		return FALSE;

	TCHAR tmpUid[10]={0,};

	while( m_query.Fetch() != SQL_NO_DATA )
	{
		m_query.GetStr( _T("USER_ID"), tmpUid );
		player->SetPlayer( m_query.GetInt(_T("ID"))
						 , tmpUid
						 , m_query.GetInt(_T("RANK_ID"))
						 , m_query.GetInt(_T("RANK_POINT"))
						 , m_query.GetInt(_T("ACCUMULATED_KILL"))
						 , m_query.GetInt(_T("ACCUMULATED_DEATH")) );
	}

	m_query.Clear();

	return TRUE;
}
