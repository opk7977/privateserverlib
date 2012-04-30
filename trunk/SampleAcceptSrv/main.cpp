#include "Sample.h"
#include "SLogger.h"

void main()
{
	Sample mySample;

	//�α��ʱ�ȭ
	GetLogger.Create( "SampleAcceptSrv" );

	if( !mySample.Init() )
		return;

	if( !mySample.Setting() )
		return;

	mySample.Run();

	mySample.Release();
}