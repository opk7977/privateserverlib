#pragma once

#include <windows.h>

class NMonitor;
class NSyncObj
{
	friend class NMonitor;
	CRITICAL_SECTION	sync;

public:
	NSyncObj()
	{
		InitializeCriticalSection(&sync);
	}
	~NSyncObj()
	{
		DeleteCriticalSection(&sync);
	}
};

class NMonitor
{
	NSyncObj* pObj;

public:
	explicit NMonitor(NSyncObj& obj)
	{
		pObj = &obj;
		EnterCriticalSection( &pObj->sync );
	}
	~NMonitor()
	{
		LeaveCriticalSection( &pObj->sync );
	}
};