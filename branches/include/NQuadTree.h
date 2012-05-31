#pragma once

#include "NSpaceDivision.h"
#include "NL_Callback.h"
#include "NObject.h"

enum CORNER_POSITION
{
	CP_TL,
	CP_TR,
	CP_BL,
	CP_BR
};

enum NEIGHBOR_POSITION
{
	NP_L,
	NP_R,
	NP_T,
	NP_B
};

enum FRUSTUMCULL_STATE
{
	NOT_IN,
	PARTIALLY_IN,
	COMPLETELY_IN
};

struct QUAD_OBJECT
{
	int		objRef;
	float	objRotX;
	float	objRotY;
	float	objRotZ;
	float	objScale;
	NObject object;
};

typedef std::vector<QUAD_OBJECT>	QuadObjVec;

//==================================
//	쿼드 노드 구조체
//==================================
class NQuadTree;
struct NQuadNode : public NSpaceDivision
{
	//==================================
	//	Data
	//==================================
	bool		m_bCulled;
	int			m_depth;
	int			m_center;
	NQuadTree*	m_pManager;
	DWORD		m_corner[4];
	NQuadNode*	m_pChild[4];
	NQuadNode*	m_pNeighbor[4];
	NQuadNode*  m_pParent;
	NCallback	m_callAtNotCull;
	QuadObjVec	m_objVec;
	
	//==================================
	//	Ctor
	//==================================
	NQuadNode(NQuadTree* pMgr);
	NQuadNode(NQuadTree* pMgr, NQuadNode* pParent);
	~NQuadNode();

	//==================================
	//	Function
	//==================================
	void		BuildQuadTree();
	void		BuildNeighborNode();
	void		SetCorner(int topLeft, int topRight, int bottomLeft, int bottomRight);
	void		CreateBox();
	bool		SubDivide(void);
	bool		GetNodeIndex(int dir, int& tLeft, int& tRight, int& bLeft, int& bRight);
	NQuadNode*	CreateNode(int topLeft, int topRight, int bottomLeft, int bottomRight);
	void		FrustumCull();
	void		AllInFrustum();
	void		AssignCallAtNotCull(NCallback& call);
	void		AddObject(QUAD_OBJECT& obj);
	void		DeleteObject(int index);
	bool		InAABBInQuad2D(NBoundingBox& aabbWorld);
	void		AddRenderObject();
	void		AddRenderObjectAll();
	void		RenderObject();
	void		RenderObjectSphere();
	void		RenderObjectBox();
	void		UpdateQuadSpaceByObject(NBoundingBox& bbox);
	void		UpdateSpaceLowLimitDepth();
	void		UpdateSpaceLowLimitDepthImpl();

	void		Release();
};

//////////////////////////////////////////////////////////////////////////

struct QUAD_PICKED_OBJECT
{
	float			distance;
	NVECTOR3		intersect;
	NQuadNode*		pNode;
	int				objIndex;
	NObject*		object;
	QUAD_OBJECT*	quadObject;
	
	QUAD_PICKED_OBJECT(){}
	QUAD_PICKED_OBJECT(float dist, NVECTOR3& vec, NQuadNode* qNode, int objID, NObject* obj, QUAD_OBJECT* quadObj) :
		distance(dist), intersect(vec), pNode(qNode), objIndex(objID), object(obj), quadObject(quadObj)
	{
		
	}
};

struct QUAD_PICKED_OBJECT_ASCEND
{
	bool operator()(const QUAD_PICKED_OBJECT& lhs, const QUAD_PICKED_OBJECT& rhs)
	{
		return fabs(lhs.distance) < fabs(rhs.distance);
	}
};

struct QUAD_EDIT_INFO
{
	NQuadNode*	pNode;
	int			objIndex;
};

//=========================================================================
//	Name   : NQuadTree
//	Content: 쿼드트리 클래스
//=========================================================================
class NQuadTree
{
	//==================================
	//	Private Data
	//==================================
	friend struct NQuadNode;

	NQuadNode*		m_pRootNode;
	P3NCT_VERTEX*	m_pVertData;
	int				m_width;
	int				m_height;
	int				m_depthLimit;
	NFRUSTUM*		m_pFrustum;
	
	typedef CAtlMap< int, NQuadNode* >	QuadMap;
	typedef QuadMap::CPair				QuadMapPair;
	QuadMap			m_quadMap;

	typedef std::vector<NObject>		NObjectVec;
	NObjectVec	m_objVec;
	
	typedef std::list<NObject*>				NObjectPList;
	typedef NObjectPList::iterator			NObjectPListItor;
	typedef std::map< int, NObjectPList >	NObjectListMap;
	typedef NObjectListMap::iterator		NObjectListMapItor;
	NObjectListMap	m_objRenderList;

	typedef std::list<QUAD_EDIT_INFO>		NObjectEditList;
	typedef NObjectEditList::iterator		NObjectEditItor;
	typedef std::map<int, NObjectEditList>	NObjectMap;
	typedef NObjectMap::iterator			NObjectMapItor;
	NObjectMap		m_objMap;

	FILE*	m_fp;
	
public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NQuadTree();
	~NQuadTree();

	//==================================
	//	External Function
	//==================================
	void Build( P3NCT_VERTEX* pVertData, int width, int height );
	void Save(FILE* fp);
	void Load(FILE* fp, P3NCT_VERTEX* pVertData);
	int  IsInFrustum(NQuadNode* pNode);
	bool IsDepthLimit(int depth) const;
	void SetDepthLimit(int depth);
	void UpdateSpaceLowLimitDepth();
	NQuadNode* FindQuad(int centerPoint);
	void FrustumCull(NFRUSTUM* pFrustum);
	void Release();
	NQuadNode* GetRootNode() {	return m_pRootNode;	}
	void AddObject(QUAD_OBJECT& obj);
	void RenderObject();
	void RenderObjectSphere();
	void RenderObjectBox();
			
private:
	//==================================
	//	Internal Function
	//==================================
	void SaveRecursive(NQuadNode* pNode);
	void LoadRecursive(NQuadNode* pNode);
	int  CalculateSquare(int num);
	void RecalculateBox(NQuadNode* pNode);
	int  FindObjectRef(NObject* pObj);
	void RegisterObject(int objRef, NQuadNode* pNode, int objIndex);
	void UnregisterObject(int objRef, NQuadNode* pNode, int objIndex);
	void AddRenderObject(int objRef, NObject* pObj);
};