#include "NL_Time.h"

//===============================================================
//  NAME	  : NTime
//  COMMENTS  : 타임 클래스 구현
//===============================================================

//============================
//	Constructor & Destructor
//============================
NTime::NTime( void ) : m_fpsCount(0),
					   m_framePerSec(0),
					   m_secPerFrame(0.0f)
{}
NTime::~NTime( void )
{}

//============================
//	Function
//============================
bool	NTime::Init( void )
{
	::QueryPerformanceFrequency( &m_freq );
	::QueryPerformanceCounter( &m_start );

	m_end			= m_start;
	m_framePerSec	= 0;
	m_fpsCount		= 0;
	m_invFreqLow	= float(1.0 / m_freq.LowPart);

	return true;
}
bool	NTime::Frame( void )
{
	::QueryPerformanceCounter( &m_end );
	m_secPerFrame = float( m_end.LowPart - m_start.LowPart ) * m_invFreqLow;

	static float sumTime = 0.0f;
	
	sumTime += m_secPerFrame;
	if( sumTime >= 1.0f )
	{
		m_framePerSec	= m_fpsCount;
		m_fpsCount		= 0;
		sumTime			= 0.0f;
	}
	++m_fpsCount;

	m_start = m_end;
	return true;
}