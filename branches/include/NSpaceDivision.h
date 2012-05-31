#pragma once

#include "NL_Base.h"
#include "NL_BoundingBox.h"

//=========================================================================
//	Name   : NSpaceDivision
//	Content: ���� ���� ����ü
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