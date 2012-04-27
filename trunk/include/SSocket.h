#ifndef __SOCKET__
#define __SOCKET__


#include "SServerobj.h"


class SSocket : public SServerObj
{
private:
	SOCKET		m_socket;

public:
	SSocket(void);
	virtual ~SSocket(void);
	
	//초기화
	BOOL Init();

	//run함수
	BOOL Run();

	//release
	void Release();

	//소켓을 생성
	BOOL CreateSocket();

	//ip재사용 설정
	BOOL SetScokReuseAddr();

	//소켓 넌블럭형으로 설정
	BOOL SetNonBlkSock();

	//클라 소켓을 위한 connect함수
	BOOL ConnectSock( char* ipAddr, int port, SOCKADDR *sAddr = NULL );

	//서버 소켓을 위한 bind함수
	BOOL BindSocket( int port );
	//서버 소켓을 위한 listen함수
	BOOL SocketListen( int backlogCount = SOMAXCONN );

	//소켓 해제 하기
	BOOL CloseSocket();

	//소켓을 반환
	inline SOCKET GetSocket() { return m_socket; }
};



#endif


