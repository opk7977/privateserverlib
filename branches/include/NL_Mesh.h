#pragma once

#include "NL_Math.h"
#include "NL_D3DType.h"

//===============================================================
//  NAME	  : NMeshRenderInfo
//  COMMENTS  : 그리기 정보 구조체
//===============================================================
struct NMeshRenderInfo
{
	int	startVertexIndex;
	int	faceCount;
};
typedef std::map< int, NMeshRenderInfo > NMeshRenderInfoMap;

//===============================================================
//  NAME	  : NMesh
//  COMMENTS  : 메쉬 구조체 선언
//===============================================================
struct NMesh
{
	//-------------------------------
	//	NMesh Data
	//-------------------------------
	CString					m_nodeName;
	CString					m_parentName;
	int						m_type;

	NVECTOR3				m_basePos;
	NMATRIX					m_baseRot;
	NMATRIX					m_baseScale;
	NMATRIX					m_matWorld;
	NMATRIX					m_matInverse;
	NMATRIX					m_matCalculate;
	NAniPosVec				m_aniPos;
	NAniRotVec				m_aniRot;
	NAniScaleVec			m_aniScale;

	P3NCT_VERTEX*			m_vertice;
	int						m_verticeCount;
	NFaceVec				m_faces;
	int						m_faceCount;
	NMesh*					m_pParent;
	std::list<NMesh*>		m_pChildList;
	NMeshRenderInfoMap		m_renderInfo;

	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	//-------------------------------
	//	Constructor
	//-------------------------------
	NMesh(void);
	~NMesh(void);
	
	//-------------------------------
	//	Function
	//-------------------------------
	void	Release(void);
};

typedef std::vector< NMesh* >	NMeshVec;