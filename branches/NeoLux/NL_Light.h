#pragma once

#include "NL_Math.h"

enum NLIGHT_TYPE
{
	LT_DIRECTIONAL,
	LT_POINT
};

// light
struct NLight
{
	int			type;
	
	// For Directional Light
	NVECTOR4	vDir;

	// For Point Light
	NVECTOR4	vPos;
	float		range;

	NVECTOR4	ambient;
	NVECTOR4	diffuse;

	NLight():	type(0), 
				vDir(0.0f, 1.0f, 0.0f, 1.0f), 
				range(0.0f),
				ambient(0.1f, 0.1f, 0.1f, 1.0f), 
				diffuse(1.0f, 1.0f ,1.0f, 1.0f)
	{
		
	}
};