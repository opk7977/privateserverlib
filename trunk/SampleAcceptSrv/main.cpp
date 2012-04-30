#include "Sample.h"
#include "SLogger.h"

void main()
{
	Sample mySample;

	//로그초기화
	GetLogger.Create( "SampleAcceptSrv" );

	if( !mySample.Init() )
		return;

	if( !mySample.Setting() )
		return;

	mySample.Run();

	mySample.Release();
}