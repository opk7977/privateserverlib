#include "sample.h"
#include "crtdbg.h"

[!output PROJECT_NAME]::[!output PROJECT_NAME](void)
{
}

[!output PROJECT_NAME]::~[!output PROJECT_NAME](void)
{
}

bool [!output PROJECT_NAME]::Init(void)
{
	// 
	return true;
}

bool [!output PROJECT_NAME]::Frame(void)
{
	// 
	return true;
}

bool [!output PROJECT_NAME]::Render(void)
{
	// 
	return true;
}

bool [!output PROJECT_NAME]::Release(void)
{
	// 
	return true;
}

int APIENTRY _tWinMain( HINSTANCE hInst, HINSTANCE, LPTSTR, int )
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(  );

	int ret = 0;
	{
		[!output PROJECT_NAME] obj;
		if( !obj.CreateWin( hInst, _T("[!output PROJECT_NAME]"), 800, 600 ) )
			return -1;

		if( !obj.EngineInit() )
			return -1;

		ret = obj.Run();
	}

	return ret;
}