#include "SWindow.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	if( strcmp( IpzCmdParam, "" ) == 0 )
		MessageBoxA( NULL, "Non", "¾øÀ½", MB_OK );
	else
		MessageBoxA( NULL, IpzCmdParam, IpzCmdParam, MB_OK );

	return 0;
}