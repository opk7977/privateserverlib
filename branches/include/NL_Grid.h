#pragma once

#include "NL_Base.h"
#include "NL_Singleton.h"

//=========================================================================
//	Name   : NGrid
//	Content: 그리드 클래스
//=========================================================================
class NDebug;
class NGrid : public NSingleton< NGrid >
{
	DECL_FRIEND( NSingleton< NGrid > )

	//==================================
	//	Private Data
	//==================================
	NDebug*		m_pDebug;
	P3C_VERTEX*	m_pLineVert;
	float		m_interval;
	int			m_cellCount;
	int			m_lineCount;
	DWORD		m_color;
	
public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NGrid();
	~NGrid();

	//==================================
	//	External Function
	//==================================
	void Init();
	void Frame(){}
	void Render();
	void Remove(){}
	bool Reset(){	return true;	}
	void Release();

	void SetGrid(int cellCount = 128, DWORD color = 0xffaaaaaa, float interval = 10.0f);
};

#define I_NGrid		NGrid::GetInstance()