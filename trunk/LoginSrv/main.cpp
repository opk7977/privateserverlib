#include "LoginMain.h"

void main()
{
	LoginMain lMain;

	//�ʱ�ȭ
	if( !lMain.Init() )
		return;

	lMain.Run();

}