#include "SDump.h"

class CObject
{
public:
	BOOL mIsOpend;
};

void main()
{
	SDump tmpDump;

	tmpDump.Begin();

	CObject* tmp = NULL;
	tmp->mIsOpend = FALSE;

	tmpDump.End();
}