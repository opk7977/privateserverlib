#include "SSynchronize.h"
#include "SServerObj.h"

SSynchronize::SSynchronize( SServerObj* obj ) : m_pServerObj(obj)
{
	//방어 코드
	if( m_pServerObj == 0 )
		return;

	EnterCriticalSection( &m_pServerObj->m_CS );
}

SSynchronize::~SSynchronize(void)
{
	//방어 코드
	if( m_pServerObj == 0 )
		return;

	LeaveCriticalSection( &m_pServerObj->m_CS );
}
