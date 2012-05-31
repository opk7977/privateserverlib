#pragma once

//==================================
//	NFrameInfo
//==================================
struct NFrameInfo
{
	float	elapsed;
	int		startFrame;
	int		lastFrame;
	int		curFrameI;
	float	curFrameF;
	float	delta;
	bool	bLoop;
	bool	stopped;
	float	controlSpeed;

	NFrameInfo():
		elapsed(0.0f),
		startFrame(0),
		lastFrame(0),
		curFrameI(0),
		curFrameF(0.0f),
		delta(0.0f),
		bLoop(true),
		stopped(false),
		controlSpeed(1.0f)
	{

	}
};

typedef std::vector< int >	MatrixIndexVec;