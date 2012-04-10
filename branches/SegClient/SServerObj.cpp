#include "SServerObj.h"

SServerObj::SServerObj(void)
{
	InitializeCriticalSection( &m_CS );
}

SServerObj::~SServerObj(void)
{
	DeleteCriticalSection( &m_CS );
}

BOOL SServerObj::Equals( SServerObj* obj )
{
	return ( this == obj );
}