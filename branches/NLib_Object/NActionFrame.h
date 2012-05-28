#pragma once


#include <vector>
#include <atlstr.h>

//=========================================================================
//	Name   : NActionFrame
//	Content: 액션 프레임 구조체
//=========================================================================
struct NActionFrame
{
	//==================================
	//	Frame Data
	//==================================
	CString	name;
	int		start;
	int		end;
	bool	loop;
	int		nextAction;	

	//==================================
	//	Ctor & Dtor
	//==================================
	NActionFrame():
		start(0),
		end(0),
		loop(false),
		nextAction(-1)
	{

	}

	//==================================
	//	External Function
	//==================================
	void Init(	const TCHAR* actionName, 
				int startFrame, 
				int endFrame, 
				bool bLoop = false, 
				int nextAction = -1	)
	{
		name		= actionName;
		start		= startFrame;
		end			= endFrame;
		loop		= bLoop;
		nextAction	= -1;
	}	
};

typedef std::vector< NActionFrame >	ActionFrameVec;