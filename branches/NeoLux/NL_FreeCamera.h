#pragma once

#include "NL_Camera.h"

//=========================================================================
//	Name   : NFreeCamera
//	Content: 자유 시점 카메라
//=========================================================================
enum FREE_CAMERA_KEY
{
	FCK_RIGHT,
	FCK_LEFT,
	FCK_UP,
	FCK_DOWN,
	FCK_FORWARD,
	FCK_BACKWARD,

	FCK_MAX
};

class NFreeCamera : public NCamera
{
	//==================================
	//	Private Data
	//==================================
	ULONG m_cameraKey[ FCK_MAX ];
	
public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NFreeCamera();
	~NFreeCamera();

	//==================================
	//	External Function
	//==================================
	void Init();
	void Frame();
	void Render();
	void Reset();
	void Release();

	//==================================
	//	Camera Function
	//==================================
	void  SetCameraKey( FREE_CAMERA_KEY enumKey, ULONG key )	{	m_cameraKey[ enumKey ] = key;	}
	ULONG GetCameraKey( FREE_CAMERA_KEY enumKey ) const			{	return m_cameraKey[ enumKey ];	}

private:
	//==================================
	//	Internal Function
	//==================================
		
};