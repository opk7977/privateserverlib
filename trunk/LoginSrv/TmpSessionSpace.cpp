#include "TmpSessionSpace.h"
#include "LoginSession.h"
#include "SSynchronize.h"

TmpSessionSpace::TmpSessionSpace(void)
{
}

TmpSessionSpace::~TmpSessionSpace(void)
{
	Release();
}

void TmpSessionSpace::Init( int SessionCount )
{
	SSynchronize sync( this );

	m_spaceSize = SessionCount;

	//index만들고
	m_indexQ.Create( m_spaceSize, 0 );

	//vector공간을 만들어 둔다
	m_waitSession.reserve( m_spaceSize );

	for( int i=0; i<SessionCount; ++i )
	{
		LoginSession* tmpSession = NULL;
		m_waitSession.push_back( tmpSession );
	}
}

void TmpSessionSpace::Release()
{
	SSynchronize sync( this );

	m_indexQ.Release();
	std::vector<LoginSession*>().swap( m_waitSession );
}

int TmpSessionSpace::GetIndex()
{
	return m_indexQ.GetIndex();
}

void TmpSessionSpace::WaitForDBSignal( int index, LoginSession* session )
{
	//해당 index번째 해당하는 벡터 공간에 session을 넣어 준다
	SSynchronize sync( this );

	m_waitSession[index] = session;
}

void TmpSessionSpace::GetSignal( int index )
{
	SSynchronize sync( this );

	//사용된 공간은 다시 번호를 반납하여 준다.
	m_waitSession[index] = NULL;
	m_indexQ.PutIndex( index );
}

LoginSession* TmpSessionSpace::FindSessionByIndex( int index )
{
	SSynchronize sync( this );

	//번호 확인하고
	if( index >= m_spaceSize || index < 0 )
		return NULL;

	return m_waitSession[index];
}
