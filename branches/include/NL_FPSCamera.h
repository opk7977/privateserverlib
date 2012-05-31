#pragma once

#include "NL_Camera.h"

//=========================================================================
//	Name   : NFPSCamera
//	Content: 1ÀÎÄª Ä«¸Þ¶ó
//=========================================================================
class NDXInput;
class NFPSCamera : public NCamera
{
	//==================================
	//	Private Data
	//==================================
	float		m_fovYLimitTop;
	float		m_fovYLimitBottom;
	float		m_baseHeight;
	bool		m_bMove;
	NDXInput*	m_pDXInput;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NFPSCamera();
	~NFPSCamera(){}

	//==================================
	//	External Function
	//==================================
	void Init();
	void Frame(){}
	void FrameEx();
	void UpdateView();
	void RotateXY(float xDelta, float yDelta);
	void MoveForword2D(float delta);
	void MoveSide2D(float delta);
	void SetYpos(float yPos)			{	m_matCameraWorld._42 = yPos + m_baseHeight;	}
	
	float GetXpos() const	{	return m_matCameraWorld._41;	}
	float GetZpos() const	{	return m_matCameraWorld._43;	}
	bool  IsMoved()	const	{	return m_bMove;					}

	void SetBaseHeight(float height)	{	m_baseHeight = height;				}
	void SetFovYLimitTop(int angle)		{	m_fovYLimitTop		= (float)angle;	}
	void SetFovYLimitBottom(int angle)	{	m_fovYLimitBottom	= (float)angle;	}

	float GetBaseHeight()		const	{	return m_baseHeight;				}
	int  GetFovYLimitTop()		const	{	return (int)m_fovYLimitTop;			}
	int  GetFovYLimitBottom()	const	{	return (int)m_fovYLimitBottom;		}
};