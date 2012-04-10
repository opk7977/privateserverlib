#ifndef __MY_STRING__
#define __MY_STRING__

#include "SServerobj.h"

class SStringFunc : public SSingleton<SStringFunc>
{
private:
	friend class SSingleton<SStringFunc>;

public:
	SStringFunc(void);
	~SStringFunc(void);

	//Multi->Uni
	BOOL ConvertMultiToUni( WCHAR* wstrTmp, const CHAR* strSource, int cchDestCount );
	//Uni->Multi
	BOOL ConvertUniToMulti( CHAR* strTmp, const WCHAR* wstrSource, int cchDestCount );
};
#define GetStringFunc SStringFunc::GetInstance()

#endif

