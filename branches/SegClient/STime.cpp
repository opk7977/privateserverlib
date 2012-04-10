#include "STime.h"

STime::STime(void) : m_bUseQPF(FALSE)
				   , m_fElapsedTime(0.f)
				   , m_llQPFTicksPerSec(0)
				   , m_llLastElapsedTime(0)
{
}

STime::~STime(void)
{
}

void STime::Init()
{
	LARGE_INTEGER qwTicksPerSec, qwTime;
	m_bUseQPF = (BOOL)(QueryPerformanceFrequency(&qwTicksPerSec) != 0);

	if( !m_bUseQPF )
		return;

	m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;

	QueryPerformanceCounter( &qwTime );
	m_llLastElapsedTime = qwTime.QuadPart;
}

void STime::ProcessTime()
{
	if( !m_bUseQPF )
	{
		//시스템이 QP를 지원하지 않는 경우는
		//GetTickCount를 사용하자. 혹은 다른 방법을..
		return;
	}

	LARGE_INTEGER qwTime;
	QueryPerformanceCounter( &qwTime );

	m_fElapsedTime = (float)((double)(qwTime.QuadPart - m_llLastElapsedTime)
						   / (double)m_llQPFTicksPerSec );

	m_llLastElapsedTime = qwTime.QuadPart;
}