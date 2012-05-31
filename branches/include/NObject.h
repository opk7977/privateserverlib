#pragma once

#include "NL_Base.h"
#include "NSkinMesh.h"
#include "NActionFrame.h"
#include "NL_OBB.h"

struct NAniMeshObj;
typedef std::vector< NSkinMeshObj >	NSkinObjVec;
typedef NSkinObjVec::iterator		NSkinObjItor;

class NTime;
class NMeshRenderer;

//=========================================================================
//	Name   : NObject
//	Content: 오브젝트 클래스
//=========================================================================
class NObject
{
	DECL_FRIEND( NMeshRenderer )

protected:
	//==================================
	//	Protected Data
	//==================================
	CString				m_name;
	NTime*				m_pTime;
	NMeshRenderer*		m_pRenderer;
	NSkinObjVec			m_skinObjVec;
	int					m_skinCount;
	NMATRIX				m_controlWorld;
	ActionFrameVec		m_actionFrame;
	NSphere				m_sphere;
	NSphere				m_sphereWorld;
	NBoundingBox		m_aabb;
	NBoundingBox		m_aabbWorld;
	NOBB				m_obb;
	NCapsule			m_capsule;
	NCapsule			m_capsuleWorld;
	NVECTOR3			m_ambientCol;
	NVECTOR3			m_diffuseCol;
	
public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NObject();
	~NObject();

	bool operator == ( const NObject& rhs );
	bool operator < ( const NObject& rhs );
	bool operator < ( const NObject& rhs ) const;

	//==================================
	//	External Function
	//==================================
	void Init();
	int  Load(	const TCHAR*	skinFile,
				NAniMeshObj*	pAniObj = NULL );
	bool LoadSet( const TCHAR* charSet, NAniMeshObj** pOutAniObj = NULL );
	bool SaveSet( const TCHAR* charSet );
	void PreRender();
	void Render();
	void RenderSphere();
	void RenderBox();
	void RenderSphereSelect(int index);
	void RenderBoxSelect(int index);
	void RenderObjectSphere();
	void RenderObjectBox();
	void RenderObjectOBB();
	void RenderObjectCapsule();
	void RenderSelect(int index);
	void RenderStatic();
	void RenderStaticSelect(int index);
	void Remove();
	void Reset();
	void Release();
	bool IsInit() const	{	return m_skinCount > 0;		}

	//==================================
	//	External Function
	//==================================
	void SetName( const TCHAR* name );
	void DeleteSkin(int index);
	void DeleteSkinByName(const TCHAR* name);
	void DeleteSkinAll();
	bool SetAniMatrix( int index, NAniMeshObj* pAniObj );
	bool SetAniMatrixAll( NAniMeshObj* pAniObj );
	void DetachAniMatrix( int index );
	void DetachAniMatrixAll();
	void SetFrame( int skinIndex, int startFrame, int endFrame, bool bLoop = true );
	void SetFrameAll( int startFrame, int endFrame, bool bLoop = true );
	void SetStartFrameAll( int startFrame );
	void SetEndFrameAll( int endFrame );
	void SetCurFrameAll( int curFrame );
	void DeltaFrameAll( int deltaFrame );
	void SetFrameResetAll();
	void SetLoopAll(bool bLoop);
	void SetAniSpeed( int skinIndex, float speed );
	void SetAniSpeedAll( float speed );
	void AddAction(const TCHAR* actionName, int start, int end);
	void DeleteAction(int index);
	void DeleteActionAll();
	void SetAction(int skinIndex, int actionIndex);
	void SetActionAll(int actionIndex);
	void ModifyAction(int index, const TCHAR* actionName, int start, int end);
	void RotateY( float radian );
	void MoveForward( float delta );
	void MoveSide( float delta );
	void SetScale( float scl );
	void SetPosition( float x, float y, float z );
	void SetDirection( float x, float y, float z );
	void BuildWorldAABB();
	void BuildSphere();
	void BuildOBB();
	void BuildCapsule();
	void SetMatrix(const NMATRIX& mat);
	void SetCapsule(NCapsule& capsule);
	void SetAmbientCol(int r, int g, int b);
	void SetDiffuseCol(int r, int g, int b);
	void ResetColor();

	const TCHAR*	GetName() const			{	return m_name.GetString();			}
	NSkinMeshObj&	GetSkin(int index)		{	return m_skinObjVec[index];			}
	NActionFrame&	GetAction(int index)	{	return m_actionFrame[index];		}
	int				GetSkinCount()	const	{	return m_skinCount;					}
	int				GetActionCount() const	{	return (int)m_actionFrame.size();	}
	const NMATRIX&	GetMatrix() const		{	return m_controlWorld;				}
	const NVECTOR3& GetRight() const		{	return m_controlWorld.GetRow3(0);	}
	const NVECTOR3& GetUp() const			{	return m_controlWorld.GetRow3(1);	}
	const NVECTOR3& GetDirection() const	{	return m_controlWorld.GetRow3(2);	}
	const NVECTOR3& GetPosition() const		{	return m_controlWorld.GetRow3(3);	}
	NSphere&		GetSphere()				{	return m_sphere;					}
	NSphere&		GetWorldSphere()		{	return m_sphereWorld;				}
	NBoundingBox&	GetAABB()				{	return m_aabb;						}
	NBoundingBox&	GetWorldAABB()			{	return m_aabbWorld;					}
	NOBB&			GetOBB()				{	return m_obb;						}
	NCapsule&		GetCapsule()			{	return m_capsule;					}
	NCapsule&		GetWorldCapsule()		{	return m_capsuleWorld;				}
	NMATRIX&		GetMatrix()				{	return m_controlWorld;				}
		
private:
	//==================================
	//	Internal Function
	//==================================
	int i;
	bool CheckAlreadyLoaded(NSkinMeshObj& obj);
};