#ifndef __SAMPLE__
#define __SAMPLE__

#include "SAcceptor.h"

class Sample
{
	SAcceptor			m_acceptor;

public:
	Sample(void);
	virtual ~Sample(void);

	BOOL Init();
	BOOL Setting();
	void Run();
	void Release();
};

#endif

