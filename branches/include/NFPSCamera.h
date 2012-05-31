#pragma once

#include "NL_Camera.h"
#include "NObjDefine.h"

//=========================================================================
//	Name   : NFPSCamera
//	Content: 1ÀÎÄª Ä«¸Þ¶ó
//=========================================================================
class NDXInput;
class NTerrain;
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
	NTerrain*	m_pTerrain;

	typedef void (NFPSCamera::*FUNC_MOVE)(float);
	static FUNC_MOVE	m_moveFunc[ MD_MAX ];

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NFPSCamera();
	~NFPSCamera();

	//==================================
	//	External Function
	//==================================
	void Init();
	void Frame(){}
	void UpdateView();
	void RotateXY();

	void MoveFrame(int dir, float delta);
	void MoveForward2D(float delta);
	void MoveBackward2D(float delta);
	void MoveLeft(float delta);
	void MoveRight(float delta);
	void MoveForwardLeft2D(float delta);
	void MoveForwardRight2D(float delta);
	void MoveBackwardLeft2D(float delta);
	void MoveBackwardRight2D(float delta);

	void SetTerrain(NTerrain* pTerrain)	{	m_pTerrain = pTerrain;	}
	void SetPosition(const NVECTOR3& pos);
	void SetPosition(float x, float y, float z);
	const NVECTOR3 GetPosition() const;
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

private:
	//==================================
	//	Internal Function
	//==================================
};