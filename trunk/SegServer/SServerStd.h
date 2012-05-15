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

#define WORKER_THREAD_COUNT			3
#define MAX_SESSION_COUNT			200

#define ASYNCFLAG_SEND				0x01
#define ASYNCFLAG_RECEIVE			0x02

#define BASIC_SESSIONSPACE			100

// �̱��� ���ø�
//----------------------------------------------------
//////����//////
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
		MODE_RECV,			//packet�� ����
		MODE_SEND,			//packet�� ����(�Ϲ�)
		//packet�� ����(��ε�ĳ��Ʈ)...������� �þ �� ����
	};
	DWORD		flag;		//�Ϸ� ��Ʈ�� �̺�Ʈ ����flag
	int			myIndex;	//������ ��� �ִ� �ڽ��� index��ġ�� ������ �ִ�.

	//pakcet pool���� ���� ������
	//�� �߰��� ����

	CUSTOMOVERLAPPED()
	{
	}
	~CUSTOMOVERLAPPED()
	{
	}
}CUSTOMOVERLAPPED;

#endif