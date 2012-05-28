#pragma once

#include "NL_Base.h"
#include "NL_Math.h"
#include "NFrameInfo.h"
#include "NL_Resource.h"
#include "NL_TemplatePool.h"

//==================================
//	NAniData
//==================================
struct NAniData
{
	int			tick;
	NMATRIX		m;
	NQUATERNION q;
	NVECTOR3	t;
};
typedef std::vector<NAniData>		NAniDataVec;
typedef std::vector<P3NC_VERTEX>	P3NC_VEC;

//=========================================================================
//	Name   : NAnimateMesh
//	Content: 애니메이트 메쉬 구조체
//=========================================================================
class NMeshRenderer;
struct NAnimateMesh
{
	//==================================
	//	Private Data
	//==================================
	int					m_index;
	int					m_type;
	int					m_parent;
	CString				m_name;
	NAniDataVec			m_aniData;
	int					m_faceCount;
	int					m_vertCount;
	P3NC_VEC			m_vertData;
	NMeshRenderer*		m_pRenderer;
	NMATRIX				m_matCalculate;

	//==================================
	//	Ctor & Dtor
	//==================================
	NAnimateMesh();
	~NAnimateMesh();

	//==================================
	//	External Function
	//==================================
	void Init(	NMeshRenderer*	pRenderer );
	void Frame( NFrameInfo& frame );
	void Render();
	void Release();
	
private:
	//==================================
	//	Internal Function
	//==================================
};
typedef std::vector< NAnimateMesh* > NAniMeshPVec;

//=========================================================================
//	Name   : NAniMeshObj
//	Content: 애니메이트 메쉬 오브젝트 클래스
//=========================================================================
struct NAniMeshObj : public NResource
{
	//==================================
	//	Private Data
	//==================================
	CString				m_name;
	NScene				m_scene;
	int					m_meshCount;
	NAniMeshPVec		m_aniMesh;
	NMeshRenderer*		m_pRenderer;
	bool				m_bRender;
	NFrameInfo			m_frame;

	//==================================
	//	Ctor & Dtor
	//==================================
	NAniMeshObj();
	~NAniMeshObj(){}

	//==================================
	//	External Function
	//==================================
	void Init();
	bool Load( const TCHAR* fileName );
	void Frame( NFrameInfo& frame, MatrixIndexVec& optIndexVec );
	void FrameSelf( float elapsed );
	void Render(bool bRender);
	void RenderStatic();
	void RenderSelect( int index );
	void Remove();
	bool Reset();
	void Release();

	void SetFrame( int startFrame, int endFrame, bool bLoop = true );
	void SetStartFrame( int frame );
	void SetLastFrame( int frame );
	void SetCurFrame( int frame );
	void DeltaFrame( int frame );
	void SetFrameReset();
	void SetLoop( bool bLoop );
	void SetSpeed( float speed );
	
	float	GetFrameRate()	const	{	return m_scene.frameSpeed;		}
	int		GetStartFrame()	const	{	return m_frame.startFrame;		}
	int		GetEndFrame()	const	{	return m_frame.lastFrame;		}
	int		GetCurFrame()	const	{	return m_frame.curFrameI;		}
	int		GetMaxFrame()	const	{	return (int)m_scene.lastFrame;	}
	bool	IsNull()		const	{	return m_meshCount == 0;		}
	bool	IsLoaded()		const	{	return m_meshCount > 0;			}

private:
	//==================================
	//	Internal Function
	//==================================
	void CalculateFrame( NFrameInfo& frame );
	void SetNameFromPath(const TCHAR* filePath);
};

#define		Mgr_NAniObj		NTemplatePool< NAniMeshObj >::GetInstance()