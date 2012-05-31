#pragma once

#include "NL_Base.h"

//====================================
//	NTexmap Type
//====================================
enum TEXMAP_TYPE
{
	TM_AMBIENT,
	TM_DIFFUSE,
	TM_SPECULAR,
	TM_SHININESS,
	TM_SHININESS_STR,
	TM_SELF_ILLUMMINATION,
	TM_OPACITY,
	TM_FILTER_COLOR,
	TM_BUMP,
	TM_REFLECTION,
	TM_REFRACTION,
	TM_DISPLACEMENT,
	TM_GENERIC,
	TM_ENVMAP,

	TM_MAX
};

//====================================
//	Supported Texmap Type
//====================================
enum SUPPORT_TEXMAP_TYPE
{
	STM_AMBIENT,
	STM_SPECULAR,

	STM_MAX
};

//=====================================
//  NTexmap
//=====================================
class NTexture;
struct NTexmap
{
	int			type;
	NTexture*	texture;
};
typedef std::vector<NTexmap>	NTexmapVec;

//=====================================
//  Material Type
//=====================================
enum MATRIAL_TYPE
{
	MT_STANDARD,
	MT_MULTI_SUB_OBJECT
};

//==================================
//	NMaterial
//==================================
struct NMaterial
{
	CString		mtlName;
	int			mtlType;
	int			texmapCount;
	NTexmapVec	texmap;
	int			specPower;

	NMaterial():
		mtlType(1),
		texmapCount(0),
		specPower(0)
	{

	}
};
typedef std::vector<NMaterial>	NMtlVec;