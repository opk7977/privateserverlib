#include "DBMgr.h"
#include "PlayerMgr.h"

#include "SQuery.h"
#include "DBProtocol.h"

#include "SLogger.h"


DBMgr::DBMgr(void)
{
	m_rankMgr = &GetRankMgr;
	m_logger = &GetLogger;

	m_query = new SQuery;
}

DBMgr::~DBMgr(void)
{
}

BOOL DBMgr::Init( TCHAR* dbname, TCHAR* _id, TCHAR* _pw )
{
	if( !m_query->ConnectSrv( dbname, _id, _pw ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("DBMgr::Init() DB연결 실패\n\n") );
		return FALSE;
	}

	return TRUE;
}

void DBMgr::Release()
{
	m_query->DisConnect();

	delete m_query;
	m_query = NULL;
}

BOOL DBMgr::RankDataSetting()
{
	if( !m_query->Exec( _T("{call AllRank};") ) )
		return FALSE;

	int rankId;
	int needPoint;
	while( m_query->Fetch() != SQL_NO_DATA )
	{
		rankId = m_query->GetInt( _T("RANK_ID") );
		needPoint = m_query->GetInt( _T("RANK_NEED_POINT") );

		m_rankMgr->AddData( rankId, needPoint );
	}

	m_query->Clear();

	return TRUE;
}

BOOL DBMgr::CheckID( TCHAR* _id )
{
	// return 받을 결과값
	int result = 0;
	SQLINTEGER iJnk = SQL_NTS;
	SQLRETURN sqlret;

	sqlret = m_query->BindParamaterInt( 1, result, iJnk, FALSE );
	sqlret = m_query->BindParamaterStr( 2, _id, iJnk, TRUE );

	if( !m_query->Exec( _T("{?=call CheckID(?)};") ) )
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

	sqlret = m_query->BindParamaterStr( 1, _id, iJnk, TRUE );
	sqlret = m_query->BindParamaterStr( 2, _pw, iJnk, TRUE );
	sqlret = m_query->BindParamaterStr( 3, email, iJnk, TRUE );

	if( !m_query->Exec( _T("{call InsertAccountItem(?,?,?)};") ) )
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

	sqlret = m_query->BindParamaterInt( 1, result, iJnk, FALSE );
	sqlret = m_query->BindParamaterStr( 2, _id, iJnk, TRUE );
	sqlret = m_query->BindParamaterStr( 3, _pw, iJnk, TRUE );

	if( !m_query->Exec( _T("{?=call TryLogin(?,?)};") ) )
	{
		return SERVER_ERROR;
	}

	return result;
}

BOOL DBMgr::SettingUserData( int sessionID, PlayerObj* player )
{
	SQLINTEGER iJnk = SQL_NTS;
	SQLRETURN sqlret;

	sqlret = m_query->BindParamaterInt( 1, sessionID, iJnk, TRUE );

	if( !m_query->Exec( _T("{call GetUserData(?)};") ) )
		return FALSE;

	TCHAR tmpUid[10]={0,};

	while( m_query->Fetch() != SQL_NO_DATA )
	{
		m_query->GetStr( _T("USER_ID"), tmpUid );
		player->SetPlayer( m_query->GetInt(_T("ID"))
						 , tmpUid
						 , m_query->GetInt(_T("RANK_ID"))
						 , m_query->GetInt(_T("RANK_POINT"))
						 , m_query->GetInt(_T("ACCUMULATED_KILL"))
						 , m_query->GetInt(_T("ACCUMULATED_DEATH")) );
	}

	m_query->Clear();

	return TRUE;
}

BOOL DBMgr::UpdateUserData( int sessionID, int rankId, int rankPoint, int accumulKill, int accumulDeath )
{
	SQLINTEGER iJnk = SQL_NTS;
	SQLRETURN sqlret;

	sqlret = m_query->BindParamaterInt( 1, sessionID, iJnk, TRUE );
	sqlret = m_query->BindParamaterInt( 2, rankId, iJnk, TRUE );
	sqlret = m_query->BindParamaterInt( 3, rankPoint, iJnk, TRUE );
	sqlret = m_query->BindParamaterInt( 4, accumulKill, iJnk, TRUE );
	sqlret = m_query->BindParamaterInt( 5, accumulDeath, iJnk, TRUE );

	if( !m_query->Exec( _T("{call UpdateUserData(?,?,?,?,?)};") ) )
		return FALSE;

	return TRUE;
}
