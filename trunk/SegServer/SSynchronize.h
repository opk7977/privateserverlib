#ifndef __SYNCHRONIZE__
#define __SYNCHRONIZE__

class SServerObj;

class SSynchronize
{
private:
	SServerObj* m_pServerObj;

public:
	SSynchronize( SServerObj* obj );
	virtual ~SSynchronize(void);
};


#endif