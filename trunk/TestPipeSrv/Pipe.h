#pragma once

#include "SThread.h"
#include "SPacket.h"

class Pipe : public SThread
{
private:
	HANDLE m_hPipe;

public:
	Pipe(void);
	~Pipe(void);

	BOOL Init();
	void Release();

	BOOL Run();
};
