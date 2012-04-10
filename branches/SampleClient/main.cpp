#include "SSocket.h"


void main()
{
	SSocket	mySock;

	mySock.Init();

	mySock.CreateSocket();

	mySock.SetNonBlkSock();
	mySock.ConnectSock( "192.168.0.70", 8900 );

	while(1)
	{
	}

	mySock.CloseSocket();
}