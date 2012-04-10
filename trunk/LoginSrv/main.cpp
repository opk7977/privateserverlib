#include "LoginMain.h"

void main()
{
	LoginMain lMain;

	//√ ±‚»≠
	if( !lMain.Init() )
		return;

	lMain.Run();

}