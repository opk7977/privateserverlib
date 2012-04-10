#ifndef __MY_TIME__
#define __MY_TIME__

#include "SServerobj.h"

class STime : public SServerObj
{
private:
	BOOL		m_bUseQPF;
	float		m_fElapsedTime;
	LONGLONG	m_llQPFTicksPerSec;
	LONGLONG	m_llLastElapsedTime;

public:
	STime(void);
	~STime(void);

	inline float GetElapsedTime() const
	{ return m_fElapsedTime; };

	//각종 초기화. 객체 생성이후 한번만 호출해주자.
	void Init();

	//시간 갱신을 일으키므로, 루프마다 한번씩 호출해주자.(경과시간을 계산하게 된다)
	void ProcessTime();
};

#endif

