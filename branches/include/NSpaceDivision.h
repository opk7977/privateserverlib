#pragma once

#include "NL_Base.h"
#include "NL_BoundingBox.h"

//=========================================================================
//	Name   : NSpaceDivision
//	Content: 공간 분할 구조체
//=========================================================================
struct NSpaceDivision
{
	//==================================
	//	Data
	//==================================
	NSphere			m_sphere;
	NBoundingBox	m_bbox;

	//==================================
	//	Ctor & Dtor
	//==================================
	NSpaceDivision();
	virtual ~NSpaceDivision();

	//==================================
	//	External Function
	//==================================
	void SetSpace(const NVECTOR3& vMin, const NVECTOR3& vMax);
};