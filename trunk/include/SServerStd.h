#ifndef __SERVER_STANDARD__
#define __SERVER_STANDARD__

#pragma comment( lib, "ws2_32.lib" )

#include <WinSock2.h>
#include <Windows.h>
#include <tchar.h>
#include <string>

#include <vector>
#include <list>
#include <map>
#include <atlcoll.h>

#define WORKER_THREAD_COUNT			4
#define MAX_SESSION_COUNT			200

#define ASYNCFLAG_SEND				0x01
#define ASYNCFLAG_RECEIVE			0x02


// 싱글턴 템플릿
//----------------------------------------------------
//////사용법//////
//
// class classname : public SSingleton <classname>
// {
// private:
//		friend class SSingleton<classname>
//		.
//		.
//		.
// };
//----------------------------------------------------
template<class T> class SSingleton
{		
public:
	static T& GetInstance() 
	{		
		static T theSingleInstance;
		return theSingleInstance; 
	}
};

typedef struct CUSTOMOVERLAPPED : OVERLAPPED
{
	enum
	{
		MODE_RECV,			//packet을 받음
		MODE_SEND,			//packet을 보냄(일반)
		//packet을 보냄(브로드캐스트)...등등으로 늘어날 수 있음
	};
	DWORD		flag;		//완료 포트의 이벤트 종류flag
	int			myIndex;	//정보가 들어 있는 자신의 index위치를 가지고 있다.

	//pakcet pool에서 사용될 변수는
	//더 추가해 간다

	CUSTOMOVERLAPPED()
	{
	}
	~CUSTOMOVERLAPPED()
	{
	}
}CUSTOMOVERLAPPED;

#endif