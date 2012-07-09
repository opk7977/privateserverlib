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

	//index�����
	m_indexQ.Create( m_spaceSize, 0 );

	//vector������ ����� �д�
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
	//�ش� index��° �ش��ϴ� ���� ������ session�� �־� �ش�
	SSynchronize sync( this );

	m_waitSession[index] = session;
}

void TmpSessionSpace::GetSignal( int index )
{
	SSynchronize sync( this );

	//���� ������ �ٽ� ��ȣ�� �ݳ��Ͽ� �ش�.
	m_waitSession[index] = NULL;
	m_indexQ.PutIndex( index );
}

LoginSession* TmpSessionSpace::FindSessionByIndex( int index )
{
	SSynchronize sync( this );

	//��ȣ Ȯ���ϰ�
	if( index >= m_spaceSize || index < 0 )
		return NULL;

	return m_waitSession[index];
}
