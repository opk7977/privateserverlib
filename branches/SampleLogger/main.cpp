#include"SLogger.h"
#include <Windows.h>

void main()
{
	int count = 0;

	while(1)
	{
		Sleep( (int)(rand()%1000) );

		printf("%d\n", count);

		//GetLogger.PutLog( SLogger::LOG_LEVEL_CONINFO, _T("LOG_LEVEL_CONINFO\n") );
		//GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LOG_LEVEL_DBGINFO\n") );
		//GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LOG_LEVEL_SYSTEM\n") );
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LOG_LEVEL_WORRNIG\n") );

		Sleep( (int)(rand()%1000) );

		++count;

		if( count >= 20 )
			break;
	}
}