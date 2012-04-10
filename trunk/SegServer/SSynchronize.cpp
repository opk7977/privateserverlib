#include "SSynchronize.h"
#include "SServerObj.h"

SSynchronize::SSynchronize( SServerObj* obj ) : m_pServerObj(obj)
{
	//��� �ڵ�
	if( m_pServerObj == 0 )
		return;

	EnterCriticalSection( &m_pServerObj->m_CS );
}

SSynchronize::~SSynchronize(void)
{
	//��� �ڵ�
	if( m_pServerObj == 0 )
		return;

	LeaveCriticalSection( &m_pServerObj->m_CS );
}
