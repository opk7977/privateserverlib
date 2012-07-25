#include "FieldSrvMgr.h"
#include "SSynchronize.h"

FieldSrvMgr::FieldSrvMgr(void)
{
}

FieldSrvMgr::~FieldSrvMgr(void)
{
}

BOOL FieldSrvMgr::Init( int _count )
{
	SSynchronize sync( this );

	m_fieldCount = _count;
	//우선 공간을 만들어 두자
	for( int i=1; i<=m_fieldCount; ++i )
	{
		m_mapFieldSrv[i] = NULL;
	}
}

void FieldSrvMgr::Release()
{
	SSynchronize sync( this );

	//다 지운다.
	m_fieldCount = 0;
	m_mapFieldSrv.RemoveAll();
}

BOOL FieldSrvMgr::SetSession( int _id, WorldSession* session )
{
	SSynchronize sync( this );

	WorldSession* tmpSession = NULL;
	//id확인 (1~m_fieldCount 의 값이어야 한다 )
	if( !m_mapFieldSrv.Lookup( _id, tmpSession ) )
		return FALSE;

	//해당 id의 서버가 이미 설정되어 있는지를 확인
	if( tmpSession != NULL )
		return FALSE;

	//정상적이라면 설정해 주자
	tmpSession = session;

	return TRUE;
}

BOOL FieldSrvMgr::SendToPacket( int _id, SPacket &packet )
{
	SSynchronize sync( this );

	WorldSession* tmpSession = NULL;
	//id확인 (1~m_fieldCount 의 값이어야 한다 )
	if( !m_mapFieldSrv.Lookup( _id, tmpSession ) )
		return FALSE;

	//해당 id의 서버가 설정되어 있는지 않다면 FALSE
	if( tmpSession == NULL )
		return FALSE;

	//전송
	tmpSession->SendPacket( packet );

	return TRUE;
}
