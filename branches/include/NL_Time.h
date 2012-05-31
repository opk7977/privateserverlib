#pragma once

#include "NL_Base.h"
#include "NL_Singleton.h"

//===============================================================
//  NAME	  : NTime
//  COMMENTS  : 타임 클래스 선언
//===============================================================
class NTime : public NSingleton< NTime >
{
	//============================
	//	NTime Data
	//============================
	LARGE_INTEGER	m_start;
	LARGE_INTEGER	m_end;
	LARGE_INTEGER	m_freq;
	float			m_invFreqLow;
	int				m_fpsCount;
	int				m_framePerSec;
	float			m_secPerFrame;
	
public:
	//============================
	//	Constructor & Destructor
	//============================
	NTime( void );
	~NTime( void );

	//============================
	//	Function
	//============================
	bool			Init( void );
	bool			Frame( void );
	const int&		GetFPS( void )			const	{	return m_framePerSec;	}
	const float&	GetElapsedTime( void )	const	{	return m_secPerFrame;	}
	void			Release(){}
};

#define I_NTime NTime::GetInstance()