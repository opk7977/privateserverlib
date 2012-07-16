#include "DBFileMgr.h"
#include "PlayerMgr.h"

#include "SQuery.h"
#include "DBProtocol.h"

#include "SLogger.h"

DBFileMgr::DBFileMgr(void)
{
	m_rankMgr	= &GetRankMgr;
	m_logger	= &GetLogger;

	m_query = new SQuery;
}

DBFileMgr::~DBFileMgr(void)
{
}

BOOL DBFileMgr::Init( TCHAR* dbname, TCHAR* _id /*= NULL*/, TCHAR* _pw /*= NULL */ )
{
	TCHAR fileName[32] = {0,};
	wsprintf( fileName, _T("%s.mdb"), dbname );

	if( !m_query->ConnectMdb( fileName ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("DBFileMgr::Init() DB연결 실패\n\n") );
		return FALSE;
	}
	return TRUE;
}

void DBFileMgr::Release()
{
	m_query->DisConnect();

	delete m_query;
	m_query = NULL;
}

BOOL DBFileMgr::RankDataSetting()
{
	if( !m_query->Exec( _T("select RANK_ID, RANK_NEED_POINT from RankTbl;")) )
		return FALSE;

	int rankId;
	int needPoint;
	while( m_query->Fetch() != SQL_NO_DATA )
	{
		rankId		= m_query->GetInt( _T("RANK_ID") );
		needPoint	= m_query->GetInt( _T("RANK_NEED_POINT") );

		m_rankMgr->AddData( rankId, needPoint );
	}

	m_query->Clear();

	return TRUE;
}

BOOL DBFileMgr::CheckID( TCHAR* _id )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select ID from AccountTbl where U_ID='%s'"), _id );

	if( !m_query->Exec( strQuery ) )
	{
		return FALSE;
	}

	int	id = -1;
	while( m_query->Fetch() != SQL_NO_DATA )
	{
		id = m_query->GetInt( _T("ID") );
	}
	m_query->Clear();

	if( id > 0 )
	{	
		return FALSE;
	}

	return TRUE;
}

BOOL DBFileMgr::CreateAccount( TCHAR* _id, TCHAR* _pw, TCHAR* email )
{
	SQLWCHAR	strQuery[512];
	wsprintf( (TCHAR*)strQuery, _T("insert into AccountTbl (USER_ID,USER_PW,E_MAIL) VALUES ('%s','%s','%s')"), _id, _pw, email );

	if( !m_query->Exec(strQuery) )
	{
		return FALSE;
	}

	return TRUE;
}

int DBFileMgr::TryLogin( TCHAR* _id, TCHAR* _pw )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("select ID,USER_ID,USER_PW from AccountTbl where USER_ID='%s'"), _id );

	if( !m_query->Exec( strQuery ) )
	{
		//db오류
		return SERVER_ERROR;
	}

	int			id = 0;
	TCHAR		name[50]={0};
	TCHAR		pass[50]={0};
	while( m_query->Fetch() != SQL_NO_DATA )
	{
		id = m_query->GetInt( _T("ID") );
		m_query->GetStr( _T("USER_ID"), name );
		m_query->GetStr( _T("USER_PW"), pass );
	}
	m_query->Clear();

	if( id <= 0 )
	{
		//id없음
		return NONEXISTENT_ID;
	}

	//id는 이제 확인 됨
	if( _tcscmp( _pw, pass ) != 0 )
	{
		return WRONG_PW;		//비번이 틀림
	}
	else
	{
		return id;
	}
}

BOOL DBFileMgr::SettingUserData( int sessionID, PlayerObj* player )
{
	SQLWCHAR	strQuery[255];
	//wsprintf( (TCHAR*)strQuery, _T("select ID,USER_ID,RANK_ID,RANK_POINT,ACCUMULATED_KILL,ACCUMULATED_DEATH from AccountTbl where ID=%d;"), sessionID );
	wsprintf( (TCHAR*)strQuery, _T("select * from AccountTbl where ID=%d;"), sessionID );

	if( !m_query->Exec( strQuery ) )
	{
		return FALSE;
	}

	TCHAR tmpUid[16]={0,};

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

BOOL DBFileMgr::UpdateUserData( int sessionID, int rankId, int rankPoint, int accumulKill, int accumulDeath )
{
	SQLWCHAR	strQuery[255];
	wsprintf( (TCHAR*)strQuery, _T("update AccountTbl set RANK_ID=%d, RANK_POINT=%d, ACCUMULATED_KILL=%d, ACCUMULATED_DEATH=%d where ID=%d;")
								, rankId, rankPoint, accumulKill, accumulDeath
								, sessionID );

	if( !m_query->Exec( strQuery ) )
	{
		return FALSE;
	}

	return TRUE;
}
