#pragma once

#include "NL_Vector.h"

//===================================================================
//  NAME      : INDEX3
//  COMMENTS  : 삼각형을 이루는 3개의 정점 인덱스 구조체
//===================================================================
struct INDEX3
{
	union 
	{
		struct 
		{
			DWORD _0, _1, _2;
		};
		DWORD v[3];
	};

	INDEX3(void): _0(0), _1(0), _2(0){}
	INDEX3(DWORD i0, DWORD i1, DWORD i2): 
		_0(i0), _1(i1), _2(i2){}
	NL_INLINE 
	void Set(DWORD i0, DWORD i1, DWORD i2)	
	{	_0 = i0; _1 = i1; _2 = i2;	}	
};

//===================================================================
//  NAME      : P3_VERTEX
//  COMMENTS  : 위치 정점 구조체
//===================================================================
struct P3_VERTEX
{
	// P3_VERTEX Data
	NVECTOR3	vp;
	enum {	FVF = D3DFVF_XYZ  };

	// Function
	P3_VERTEX(void){}
	P3_VERTEX(const NVECTOR3& pos) : vp(pos){}
	P3_VERTEX(float x, float y, float z) : vp(x, y, z){}

	void Set(const NVECTOR3& pos)
	{
		vp = pos;
	}
	void Set(float x, float y, float z)
	{
		vp.Set(x, y, z);
	}
};

//*******************************************************************
//
//  NAME      : P3N_VERTEX
//  COMMENTS  : 위치, 방향 정점 구조체
//
//*******************************************************************
struct P3N_VERTEX
{
	// P3N_VERTEX Data
	NVECTOR3	vp;
	NVECTOR3	vn;
	enum {	FVF = D3DFVF_XYZ | D3DFVF_NORMAL };

	// Function
	P3N_VERTEX(void){}
	P3N_VERTEX(const NVECTOR3& pos, const NVECTOR3& norm) :
		vp(pos), vn(norm){}
	P3N_VERTEX(float x, float y, float z, float nx, float ny, float nz) :
		vp(x, y, z), vn(nx, ny, nz){}

	void Set(const NVECTOR3& pos, const NVECTOR3& norm)
	{
		vp = pos;
		vn = norm;
	}
	void Set(float x, float y, float z, float nx, float ny, float nz)
	{
		vp.Set(x, y, z);
		vn.Set(nx, ny, nz);
	}
};

//*******************************************************************
//
//  NAME      : P3C_VERTEX
//  COMMENTS  : 위치, 컬러 정점 구조체
//
//*******************************************************************
struct P3C_VERTEX
{
	// P3C_VERTEX Data
	NVECTOR3	vp;
	D3DCOLOR	vc;
	enum {	FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

	// Function
	P3C_VERTEX(void){}
	P3C_VERTEX(const NVECTOR3& pos, D3DCOLOR color) :
		vp(pos), vc(color){}
	P3C_VERTEX(float x, float y, float z, D3DCOLOR color) :
		vp(x, y, z), vc(color){}

	void Set(const NVECTOR3& pos, D3DCOLOR color)
	{
		vp = pos;
		vc = color;
	}
	void Set(float x, float y, float z, D3DCOLOR color)
	{
		vp.Set(x, y, z);
		vc = color;
	}
};

//*******************************************************************
//
//  NAME      : P3T_VERTEX
//  COMMENTS  : 위치, 텍스처 정점 구조체
//
//*******************************************************************
struct P3T_VERTEX
{
	// P3T_VERTEX Data
	NVECTOR3	vp;
	NVECTOR2	vt;
	enum {	FVF = D3DFVF_XYZ | D3DFVF_TEX1 };

	// Function
	P3T_VERTEX(void){}
	P3T_VERTEX(const NVECTOR3& pos, const NVECTOR2& tex) :
		vp(pos), vt(tex){}
	P3T_VERTEX(float x, float y, float z, float u, float v) :
		vp(x, y, z), vt(u, v){}

	void Set(const NVECTOR3& pos, const NVECTOR2& tex)
	{
		vp = pos;
		vt = tex;
	}
	void Set(float x, float y, float z, float u, float v)
	{
		vp.Set(x, y, z);
		vt.Set(u, v);
	}
};

//*******************************************************************
//
//  NAME      : P3NC_VERTEX
//  COMMENTS  : 위치, 방향, 컬러 정점 구조체
//
//*******************************************************************
struct P3NC_VERTEX
{
	// P3NC_VERTEX Data
	NVECTOR3	vp;
	NVECTOR3	vn;
	D3DCOLOR	vc;
	enum {	FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE	};

	// Function
	P3NC_VERTEX(void){}
	P3NC_VERTEX(const NVECTOR3& pos, const NVECTOR3& norm, D3DCOLOR color) :
		vp(pos), vn(norm), vc(color){}
	P3NC_VERTEX(float x, float y, float z, float nx, float ny, float nz, D3DCOLOR color) :
		vp(x, y, z), vn(nx, ny, nz), vc(color){}

	void Set(const NVECTOR3& pos, const NVECTOR3& norm, D3DCOLOR color)
	{
		vp = pos;
		vn = norm;
		vc = color;
	}
	void Set(float x, float y, float z, float nx, float ny, float nz, D3DCOLOR color)
	{
		vp.Set(x, y, z);
		vn.Set(nx, ny, nz);
		vc = color;
	}
};

//*******************************************************************
//
//  NAME      : P3NT_VERTEX
//  COMMENTS  : 위치, 방향, 텍스쳐 정점 구조체
//
//*******************************************************************
struct P3NT_VERTEX
{
	// P3NT_VERTEX Data
	NVECTOR3	vp;
	NVECTOR3	vn;
	NVECTOR2	vt;
	enum {	FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1	};

	// Function
	P3NT_VERTEX(void){}
	P3NT_VERTEX(const NVECTOR3& pos, const NVECTOR3& norm, const NVECTOR2 tex) :
		vp(pos), vn(norm), vt(tex){}
	P3NT_VERTEX(float x, float y, float z, float nx, float ny, float nz, float u, float v) :
		vp(x, y, z), vn(nx, ny, nz), vt(u, v){}

	void Set(const NVECTOR3& pos, const NVECTOR3& norm, const NVECTOR2& tex)
	{
		vp = pos;
		vn = norm;
		vt = tex;
	}
	void Set(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		vp.Set(x, y, z);
		vn.Set(nx, ny, nz);
		vt.Set(u, v);
	}
};

//*******************************************************************
//
//  NAME      : P3CT_VERTEX
//  COMMENTS  : 위치, 컬러, 텍스쳐 정점 구조체
//
//*******************************************************************
struct P3CT_VERTEX
{
	// P3CT_VERTEX Data
	NVECTOR3	vp;
	D3DCOLOR	vc;
	NVECTOR2	vt;
	enum {	FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1	};

	// Function
	P3CT_VERTEX(void){}
	P3CT_VERTEX(const NVECTOR3& pos, D3DCOLOR color, const NVECTOR2& tex) :
		vp(pos), vc(color), vt(tex){}
	P3CT_VERTEX(float x, float y, float z, D3DCOLOR color, float u, float v) :
		vp(x, y, z), vc(color), vt(u, v){}

	void Set(const NVECTOR3& pos, D3DCOLOR color, const NVECTOR2& tex)
	{
		vp = pos;
		vc = color;
		vt = tex;
	}
	void Set(float x, float y, float z, D3DCOLOR color, float u, float v)
	{
		vp.Set(x, y, z);
		vc = color;
		vt.Set(u, v);
	}
};

//*******************************************************************
//
//  NAME      : P3NCT_VERTEX
//  COMMENTS  : 위치, 방향, 컬러, 텍스쳐 정점 구조체
//
//*******************************************************************
struct P3NCT_VERTEX
{
	// P3NCT_VERTEX Data
	NVECTOR3	vp;
	NVECTOR3	vn;
	D3DCOLOR	vc;
	NVECTOR2	vt;
	enum {	FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1	};

	// Function
	P3NCT_VERTEX(void){}
	P3NCT_VERTEX(const NVECTOR3& pos, const NVECTOR3& norm, D3DCOLOR color, const NVECTOR2& tex) :
		vp(pos), vn(norm), vc(color), vt(tex){}
	P3NCT_VERTEX(float x, float y, float z, float nx, float ny, float nz, D3DCOLOR color, float u, float v) :
		vp(x, y, z), vn(nx, ny, nz), vc(color), vt(u, v){}

	void Set(const NVECTOR3& pos, const NVECTOR3& norm, D3DCOLOR color, const NVECTOR2& tex)
	{
		vp = pos;
		vn = norm;
		vc = color;
		vt = tex;
	}
	void Set(float x, float y, float z, float nx, float ny, float nz, D3DCOLOR color, float u, float v)
	{
		vp.Set(x, y, z);
		vn.Set(nx, ny, nz);
		vc = color;
		vt.Set(u, v);
	}
};