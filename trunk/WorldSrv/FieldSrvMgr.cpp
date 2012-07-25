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
	//�켱 ������ ����� ����
	for( int i=1; i<=m_fieldCount; ++i )
	{
		m_mapFieldSrv[i] = NULL;
	}
}

void FieldSrvMgr::Release()
{
	SSynchronize sync( this );

	//�� �����.
	m_fieldCount = 0;
	m_mapFieldSrv.RemoveAll();
}

BOOL FieldSrvMgr::SetSession( int _id, WorldSession* session )
{
	SSynchronize sync( this );

	WorldSession* tmpSession = NULL;
	//idȮ�� (1~m_fieldCount �� ���̾�� �Ѵ� )
	if( !m_mapFieldSrv.Lookup( _id, tmpSession ) )
		return FALSE;

	//�ش� id�� ������ �̹� �����Ǿ� �ִ����� Ȯ��
	if( tmpSession != NULL )
		return FALSE;

	//�������̶�� ������ ����
	tmpSession = session;

	return TRUE;
}

BOOL FieldSrvMgr::SendToPacket( int _id, SPacket &packet )
{
	SSynchronize sync( this );

	WorldSession* tmpSession = NULL;
	//idȮ�� (1~m_fieldCount �� ���̾�� �Ѵ� )
	if( !m_mapFieldSrv.Lookup( _id, tmpSession ) )
		return FALSE;

	//�ش� id�� ������ �����Ǿ� �ִ��� �ʴٸ� FALSE
	if( tmpSession == NULL )
		return FALSE;

	//����
	tmpSession->SendPacket( packet );

	return TRUE;
}
