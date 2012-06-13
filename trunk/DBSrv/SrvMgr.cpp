#include "SrvMgr.h"
#include "SSynchronize.h"
#include "SPacket.h"
#include "DBSession.h"

SrvMgr::SrvMgr(void)
: m_Srv(NULL)
{
}

SrvMgr::~SrvMgr(void)
{
}

void SrvMgr::Init()
{

}

void SrvMgr::Release()
{
	SSynchronize sync( this );

	if( m_Srv != NULL )
		m_Srv = NULL;
}

BOOL SrvMgr::SetSession( DBSession* session )
{
	SSynchronize sync( this );

	if( m_Srv != NULL )
		return FALSE;

	m_Srv = session;

	return TRUE;		
}

DBSession* SrvMgr::GetSession() const
{
	return m_Srv;
}

BOOL SrvMgr::SendPacketToSrv( SPacket& packet )
{
	SSynchronize sync( this );

	if( m_Srv == NULL )
		return FALSE;

	if( m_Srv->SendPacket( packet ) == packet.GetPacketSize() )
		return TRUE;
	
	return FALSE;
}
