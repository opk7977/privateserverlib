#ifndef __SAMPLE_SESSION__
#define __SAMPLE_SESSION__

#include "SSessionObj.h"

class SampleSession : public SSessionObj
{
public:
	SampleSession(void);
	virtual ~SampleSession(void);

public:
	virtual SRTClass* GetRTClass() const { return &classSampleSession; }
	SDECLARE_DYNAMIC(SampleSession)		//static Runtime ����ü
	SDECLARE_DYNCREATE(SampleSession)		//static CreateObject�Լ�

	void OnCreate();
};


#endif

