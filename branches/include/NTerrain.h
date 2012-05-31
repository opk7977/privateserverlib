#pragma once

#include "NL_Base.h"
#include "NPatch.h"
#include "NQuadTree.h"
#include "NPicking.h"

//=========================================================================
//	Name   : NTerrain
//	Content: 지형 클래스
//=========================================================================
class NTerrainRenderer;
class NCameraPool;
class NTime;
class NTerrain
{
	DECL_FRIEND( NTerrainRenderer )

	//==================================
	//	Private Data
	//==================================
	ND3d*				m_pDevice;
	NCameraPool*		m_pCamera;
	NTime*				m_pTime;

	CString				m_name;
	NTerrainRenderer*	m_pRenderer;
	int					m_cellCount;
	int					m_cellWidth;
	float				m_cellSpace;

	NPatch*		m_patchArray;
	int			m_patchCount;
	int			m_patchWidth;
	float		m_patchSpace;
	int			m_patchVertWidth;
	int			m_patchVertCount;
	int			m_patchFaceCount;
	int			m_patchDepth;
	int			m_alphaTexPixelCount;
		
	int*		m_renderPatchArray;
	int			m_renderPatchCount;
	
	int			m_vertWidth;
	int			m_vertCount;
	int			m_faceCount;
	
	float		m_terrainSize;
	float		m_terrainHalfSize;


	NTexture*					m_baseTex;
	NVECTOR2					m_alphaPixelPosTable[ ALPHA_TEX_SIZE * ALPHA_TEX_SIZE ];
	P3NCT_VERTEX*				m_vertexArray;
	INDEX3*						m_patchIndexArray;
	INDEX3***					m_patchLODIndexArray;
	int**						m_patchLODFaceCount;
	int							m_patchLODSetCount;
	float						m_patchLODRatio;
	float						m_patchLODInvLengthSq;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DINDEXBUFFER9**	m_ppLODIB;

	NQuadTree				m_quadTree;
	NPicking				m_pick;
	FILE*					m_fp;

	struct PatchVertex
	{
		int		vertIndex;
		int		patchVertIndex;
		float	distSq;
	};
	typedef CAtlList< PatchVertex >				PatchVertexList;
	typedef CAtlMap< NPatch*, PatchVertexList >	CollisionPatchMap;
	typedef CollisionPatchMap::CPair			CollisionPatchPair;

	CollisionPatchMap	m_collPatchMap;
	CollisionPatchPair*	m_collPatchPair;
	bool				m_bUseMinHeight;
	bool				m_bUseMaxHeight;
	float				m_minHeight;
	float				m_maxHeight;

	typedef CAtlMap< int, PatchVertex* >		CircleVertexMap;
	typedef CircleVertexMap::CPair				CircleVertexPair;
	
	CircleVertexMap		m_circleVertMap;
	CircleVertexPair*	m_circleVertPair;

	float				m_tmpRadius;
	float				m_tmpRadiusSq;
	float				m_tmpRadiusSmoothSq;
	float				m_tmpRadiusDiffInvSq;
	NVECTOR3*			m_tmpIntersect;
	NVECTOR2			m_tmpIntersect2d;
	float				m_tmpRectLeft;
	float				m_tmpRectTop;
	float				m_tmpRectRight;
	float				m_tmpRectBottom;

	typedef CAtlMap< NPatch*, NPatch* >	SplattingPatchMap;
	typedef SplattingPatchMap::CPair	SplattingPatchPair;
	SplattingPatchMap	m_splattingPatchMap;
	SplattingPatchPair*	m_splattingPatchPair;

	typedef std::set<QUAD_PICKED_OBJECT, QUAD_PICKED_OBJECT_ASCEND> QUAD_PICKED_OBJECT_SET;
	bool					m_bObjectPicked;
	QUAD_PICKED_OBJECT_SET	m_pickedObject;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NTerrain();
	~NTerrain();

	//==================================
	//	External Function
	//==================================
	void Init();
	void Frame();
	void Render();
	void Remove();
	void Reset();
	void Release();

	bool Save(const TCHAR* fileName);
	bool Load(const TCHAR* fileName);
	void SetName(const TCHAR* name)	{	m_name = name;				}
	const TCHAR* GetName() const	{	return m_name.GetString();	}

	QUAD_PICKED_OBJECT* GetPickedObject();
	bool CreateMap(int patchWidth, int cellWidth, float cellSpace, float lodRatio, NTexture* pBaseTex);
	void CreateMapData(int patchWidth, int cellWidth, float cellSpace, float lodRatio);
	void BuildQuadTree();
	void BuildPatch();
	bool SetRenderBuffer();
	void RenderBox();
	void AddRenderPatch(int index);
	bool IsInit() const {	return !!m_patchArray;	}
	void RenderPick();
	void RenderPickCross();
	bool CreateCircleOnPick(P3C_VERTEX* pData, int dataSize, float radius, DWORD color = 0xff00ff00);
	bool CreateCircleOnPickOptimize(P3C_VERTEX* pData, int dataSize, float radius, 
									float* cosTable, float* sinTable, DWORD color = 0xff00ff00);
	float GetHeightAt(float x, float z);
	void  CollectVertexInPickingCircle(float radius);
	void  CollectSplattingPatchInPickingCircle(float radius, float radiusSmooth);
	int	  GetPatchIndex(float x, float z);
	bool  IsVertexCollected() const;
	bool  IsSplattingPatchCollected() const;
	void  ResetCollectBuffer();
	void  ResetSplattingBuffer();
	void  ResetEditVertex();
	void  UpdateQuadTree();
	void  UpdateSplatting();
	void  ComputeVertexNormal(int index);
	void  ComputeVertexNormalRenderPatch();
	void  ComputeVertexNormalAll();
	void  RaiseFlat(float delta);
	void  RaiseCurve(float delta);
	void  PickHeight();
	void  ZeroHeight();
	void  DrawTexture(int layer, NTexture* pTex);
	void  RemoveTextureSelected(int layer);
	void  RemoveTextureAll();
	void  BuildLODIndexSet();
	void  CreateLODIndexBuffer(INDEX3**& ppIndex, int*& pCount, int cellCount);
	void  BuildIndexBuffer(	INDEX3* pIndex, 
							int zStart, int zEnd, 
							int xStart, int xEnd, 
							int& curIndex, 
							int rowSpace, int colSpace);
	enum CRACK_DIR
	{
		CD_L, CD_R, CD_T, CD_B, CD_TL, CD_TR, CD_BL, CD_BR
	};
	void  BuildIndexBufferPreventCrack(	INDEX3* pIndex, 
										int dir, int baseIndex, 
										int startIndex, int indexCount, 
										int& curIndex, 
										int rowSpace, int colSpace,
										int halfRowSpace, int halfColSpace );
	void  CheckPickInEdit();
	void  CheckPick();
	void  SetLODRatio(float ratio)
	{
		m_patchLODRatio = ratio;
		m_patchLODInvLengthSq = 1.0f / ( m_patchSpace * m_patchSpace * m_patchLODRatio );
	}
	void  SetUseMinHeight(bool bUse)	{	m_bUseMinHeight = bUse;		}
	void  SetUseMaxHeight(bool bUse)	{	m_bUseMaxHeight = bUse;		}
	void  SetMinHeight(float value)		{	m_minHeight = value;		}
	void  SetMaxHeight(float value)		{	m_maxHeight = value;		}

	float  GetLODRatio()	const		{	return m_patchLODRatio;		}
	bool   GetUseMinHeight() const		{	return m_bUseMinHeight;		}
	bool   GetUseMaxHeight() const		{	return m_bUseMaxHeight;		}
	float  GetMinHeight()	const		{	return m_minHeight;			}
	float  GetMaxHeight()	const		{	return m_maxHeight;			}

	const NVECTOR3* GetIntersection() const;
	void   RestructQuadBox();
	void   AddObject(NObject& obj, float rotX, float rotY, float rotZ, float scale );
	void   DeleteMapObject(QUAD_PICKED_OBJECT* obj);
	void   RenderObjectSphere();
	void   RenderObjectBox();
	
private:
	//==================================
	//	Internal Function
	//==================================
	void FrustumCull();
	void CalculateLODIndex();
	void FindIntersect(NQuadNode* pNode);
	void FindObjectIntersect(NQuadNode* pNode);
	void FindIntersectPatch(NPatch* pPatch);
	void CollectVertexInPatch(NPatch* pPatch);
	void CollectVertexImpl(NPatch* pPatch, NQuadNode* pNode);
	void CollectAllVertex(NPatch* pPatch, NQuadNode* pNode);
	void CollectSplattingPatch(NPatch* pPatch);
	bool IsInCircle(NVECTOR3& vert) const;
	bool CollisionWithPickingCircle(const NSphere& sphere) const;
	bool CollisionPatchBoxWithCircle(NPatch* pPatch) const;
	void AddEditVertex(NPatch* pPatch, int vert);
	int  GetVertexIndexInPatch(int patchIndex, int absIndex) const;
	void ConvertVertexIndexToPatchIndex();
	void ComputeVertexNormalPatch(NPatch* pPatch);
	void OptimizeSplattingInPatch(NPatch* pPatch);
};