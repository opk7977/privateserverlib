#pragma once

#include "NL_Base.h"
#include "NL_Math.h"

//=========================================================================
//	Name   : NBoundingBox
//	Content: 바운딩 박스 구조체
//=========================================================================
class NDebug;
struct NBoundingBox
{
	//==================================
	//	Private Data
	//==================================
	static NDebug*	pDebug;
	NVECTOR3		vMin, vMax;

	//==================================
	//	Ctor & Dtor
	//==================================
	NBoundingBox();
	~NBoundingBox();

	//==================================
	//	External Function
	//==================================
	void Init();
	void Init(const NVECTOR3& vMin, const NVECTOR3& vMax);
	void Render(NMATRIX& matWorld);
	void ResetYpos(float yMin, float yMax);

	bool IsIntersect(const NBoundingBox& bbox) const;
};