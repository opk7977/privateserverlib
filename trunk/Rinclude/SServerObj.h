#ifndef __SERVER_OBJECT__
#define __SERVER_OBJECT__

#include "SObject.h"
#include "SSynchronize.h"

class SServerObj : public SObject
{
private:
	friend SSynchronize;
	CRITICAL_SECTION	m_CS;

public:
	SServerObj(void);
	virtual ~SServerObj(void);

	BOOL Equals( SServerObj* obj );
};


#endif