#pragma once

#include "NL_Base.h"
#include "NL_Math.h"

//=========================================================================
//	Name   : NBoundingBox
//	Content: 바운딩 박스 구조체
//=========================================================================
class NBoundingBox
{
	//==================================
	//	Private Data
	//==================================
	NVECTOR3	vMin, vMax;
	P3_VERTEX	lineVertex[8];
	DWORD		lineIndex[24];
	NVECTOR4	lineColor;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NBoundingBox();
	~NBoundingBox();

	//==================================
	//	External Function
	//==================================
	void Init(const NVECTOR3& vMin, const NVECTOR3& vMax);
	void SetRenderBuffer();
	void SetColor(float r, float g, float b, float a);
	void SetColor(BYTE r, BYTE g, BYTE b, BYTE a);
	void Render();
};