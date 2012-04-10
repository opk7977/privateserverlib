#ifndef __SAMPLE_QUERY__
#define __SAMPLE_QUERY__


#include "SQuery.h"

#define STRING_NUM 255


class SampleQuery
{
private:
	SQuery m_query;

public:
	SampleQuery(void);
	~SampleQuery(void);

	BOOL Init( TCHAR *str );
	void Release();

	BOOL AllPrint();
	BOOL AddMember();
	BOOL ChangeMember();
	BOOL DelMember();

	void ScanStr( TCHAR* str, TCHAR* tmp );
	void GetChForMoment();
};



#endif