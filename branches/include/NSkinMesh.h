#pragma once

#include "NL_Base.h"
#include "NFrameInfo.h"
#include "NMaterial.h"
#include "NL_Resource.h"
#include "NL_TemplatePool.h"

//////////////////////////////////////////////////////////////////////////

typedef std::vector< P3NT_VERTEX >	P3NT_VEC;
typedef std::vector< NVECTOR3 >		PosVec;
typedef std::vector< NVECTOR3* >	PosPVec;
typedef std::vector< NVECTOR2 >		TexVec;
typedef std::vector< NVECTOR2* >	TexPVec;
typedef std::vector< DWORD >		ColorVec;
typedef std::vector< NVECTOR3 >		NormalVec;
typedef std::vector< NVECTOR3* >	NormalPVec;
typedef std::vector< NVECTOR3 >		TangentVec;
typedef std::vector< NVECTOR3* >	TangentPVec;
typedef std::vector< P3C_VERTEX >	BoundLineVec;
typedef	std::map< int, NMATRIX* >	MatrixPMap;
typedef	MatrixPMap::iterator		MatrixPItor;
typedef std::map< int, int >		KeepIndexMap;
typedef KeepIndexMap::iterator		KeepIndexItor;
typedef std::vector< NMATRIX* >		MatrixPVec;

//==================================
//	IW4_VERTEX
//==================================
struct IW4_VERTEX
{
	IW4_VERTEX()
	{
		memset( this, 0, sizeof(float) * 8 );
	}
	float index[4];
	float weight[4];
};
typedef std::vector< IW4_VERTEX >	IW4_VEC;
typedef std::vector< IW4_VERTEX* >	IW4_PVEC;

//==================================
//	IW4_VERTEX
//==================================
struct IW8_VERTEX
{
	IW8_VERTEX()
	{
		memset( this, 0, sizeof(float) * 16 );
	}
	float index[8];
	float weight[8];
};
typedef std::vector< IW8_VERTEX >	IW8_VEC;
typedef std::vector< IW8_VERTEX* >	IW8_PVEC;

//==================================
//	NFace
//==================================
struct NFace
{
	INDEX3	optIndex;
	INDEX3	posIndex;
	INDEX3	texIndex;
	INDEX3	colIndex;
	INDEX3	normIndex;
	INDEX3	tanIndex;
	
	NVECTOR3 faceNormal;
};
typedef std::vector<NFace>	NFaceVec;

//==================================
//	NFaceGroup
//==================================
struct NFaceGroup
{
	int			mtlRef;
	int			startIndex;
	int			faceCount;
};
typedef std::vector<NFaceGroup>		NFaceGroupVec;

#ifdef _DEBUG

struct LINE_RENDER
{
	NVECTOR3 pos;
	DWORD	col;
	NVECTOR3 norm;
	IW4_VERTEX iw;
	float	u, v;
};
typedef std::vector<LINE_RENDER>	LineVec;

struct LINE_RENDER2
{
	NVECTOR3 pos;
	DWORD col;
	NVECTOR3 norm;
	IW8_VERTEX iw;
	float u, v;
};
typedef std::vector<LINE_RENDER2>	Line2Vec;

#endif

//=========================================================================
//	Name   : NSkinMesh
//	Content: 스킨 메쉬 구조체
//=========================================================================
enum SKIN_TYPE
{
	ST_STATIC_OBJECT,
	ST_DYNAMIC_OBJECT,
	ST_RIGID_OBJECT,

	ST_MAX
};
class ND3d;
class NMeshRenderer;
struct NAniMeshObj;
struct NSkinMesh
{
	//==================================
	//	NSkinMesh Data
	//==================================
	CString			m_name;
	int				m_format;
	int				m_type;
	int				m_skinType;
	int				m_solidIndex;
	PosVec			m_posVec;
	TexVec			m_texVec;
	ColorVec		m_colVec;
	NormalVec		m_normVec;
	TangentVec		m_tanVec;
	int				m_maxWeightCount;
	IW4_VEC			m_weightVec;
	IW4_VEC			m_srcWeightVec;
	IW8_VEC			m_weight8Vec;
	IW8_VEC			m_srcWeight8Vec;
	bool			m_bUseNormalmap;
	NBoundingBox	m_bbox;
	NSphere			m_sphere;

	PosPVec			m_optPosVec;
	TexPVec			m_optTexVec;
	ColorVec		m_optColVec;
	NormalPVec		m_optNormVec;
	TangentPVec		m_optTanVec;
	IW4_PVEC		m_optWeightVec;
	IW8_PVEC		m_optWeight8Vec;
	std::vector<int>	m_optMatrixRefVec;
	int					m_optMatrixCount;
	NVECTOR4*		m_optMatTransRow0;
	NVECTOR4*		m_optMatTransRow1;
	NVECTOR4*		m_optMatTransRow2;

#ifdef _DEBUG
	LineVec			m_normLineVec;
	LineVec			m_tanLineVec;
	Line2Vec		m_normLine2Vec;
	Line2Vec		m_tanLine2Vec;
	void	SetNormalLineBuffer();
	void	SetTangentLineBuffer();
#endif

	int				m_vertCount;
	int				m_faceCount;
	NFaceVec		m_faceVec;
	int				m_faceGroupCount;
	NFaceGroupVec	m_faceGroup;
	
	LPDIRECT3DVERTEXBUFFER9	m_PNCT_VB;
	LPDIRECT3DVERTEXBUFFER9	m_Tan_VB;
	LPDIRECT3DVERTEXBUFFER9	m_Weight_VB;
	LPDIRECT3DINDEXBUFFER9	m_optIB;

	NMeshRenderer*	m_pRenderer;
	ND3d*			m_pDevice;

	typedef void (NSkinMesh::*PFN_PRERENDER)(NAniMeshObj* pAniObj);
	PFN_PRERENDER			m_preRender[ST_MAX];

	//==================================
	//	Ctor & Dtor
	//==================================
	NSkinMesh();
	~NSkinMesh();

	//==================================
	//	External Function
	//==================================
	void Init( NMeshRenderer*	pRenderer );
	void PreRenderStatic(NAniMeshObj* pAniObj);
	void PreRenderDynamic(NAniMeshObj* pAniObj);
	void PreRenderRigid(NAniMeshObj* pAniObj);
	void Render(NAniMeshObj* pAniObj);
	void Release();
	void DeleteBuffer();

	bool SetRenderBuffer();
	void Optimize();
	void OptimizeMatrix();
	int  CheckCurrentRenderType();
	void ComputeTangent();
	bool SetPNCTBuffer();
	bool SetWeightBuffer();
	bool SetTangentBuffer();
	bool SetIndexBuffer();
	
private:
	//==================================
	//	Internal Function
	//==================================
	void ResetOptVec();
	void OptimizeP3NCT(bool bWeight, bool bW8 = false);
	void OptimizeP3NCTT(bool bWeight, bool bW8 = false);
};
typedef std::vector<NSkinMesh*>	NSkinMeshPVec;

//=========================================================================
//	Name   : NSkinMeshObj
//	Content: 스킨 메쉬 오브젝트 클래스
//=========================================================================
struct NSkinMeshObj : public NResource
{
	//==================================
	//	Private Data
	//==================================
	CString				m_name;
	NFrameInfo			m_frame;
	NFrameInfo			m_frameTop;
	NFrameInfo			m_frameBottom;
	NSkinMeshPVec		m_skinMeshVec;
	NAniMeshObj*		m_pAniMeshObj;
	MatrixIndexVec		m_optMatrixIndexVec;
	MatrixIndexVec		m_topIndexVec;
	MatrixIndexVec		m_bottomIndexVec;
	int					m_highestMatIndex;
	int					m_faceCount;
	NMeshRenderer*		m_pRenderer;
	NMtlVec				m_mtlVec;
	NSphere				m_sphere;
	NBoundingBox		m_bbox;

	//==================================
	//	Ctor & Dtor
	//==================================
	NSkinMeshObj();
	~NSkinMeshObj();

	//==================================
	//	External Function
	//==================================
	void Init();
	bool Load( const TCHAR* fileName );
	void Animation(float elapsed);
	void Render(float elapsed);
	void RenderStatic();
	void RenderSphere(NMATRIX& matWorld);
	void RenderBox(NMATRIX& matWorld);
	void Remove();
	bool Reset();
	void Release();
	bool IsNull() const
	{
		return m_skinMeshVec.empty();
	}
	bool IsLoaded() const
	{
		return !m_skinMeshVec.empty();
	}

	bool ValidateAniMatrix(NAniMeshObj* aniObj);
	bool SetAniMatrix(NAniMeshObj* aniObj);
	void DetachAniMatrix();
	void SetFrame(int startFrame, int lastFrame, bool bLoop = true);
	void SetStartFrame(int startFrame);
	void SetEndFrame(int endFrame);
	void SetCurFrame(int curFrame);
	void DeltaFrame(int delta);
	void SetFrameReset();
	void SetSpeed(float speed);
	void SetLoop(bool bLoop);
	void SetTopFrame(int startFrame, int lastFrame, bool bLoop = true);
	void SetBottomFrame(int startFrame, int lastFrame, bool bLoop = true);

private:
	void OptimizeAllMatrix();
	void SetNameFromPath(const TCHAR* filePath);
	void CountingFaceTotal();
};

#define		Mgr_NSkinObj	NTemplatePool< NSkinMeshObj >::GetInstance()