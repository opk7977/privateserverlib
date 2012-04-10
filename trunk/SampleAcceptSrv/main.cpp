#include "Sample.h"

void main()
{
	Sample mySample;

	if( !mySample.Init() )
		return;

	if( !mySample.Setting() )
		return;

	mySample.Run();

	mySample.Release();
}