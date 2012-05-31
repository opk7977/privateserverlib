#pragma once

#include "NL_Base.h"
#include "NSkinMesh.h"

struct NAniMeshObj;
typedef std::vector< NSkinMeshObj >	NSkinObjVec;
typedef NSkinObjVec::iterator		NSkinObjItor;

class NTime;
class NMeshRenderer;

//=========================================================================
//	Name   : NObj
//	Content: 오브젝트 클래스
//=========================================================================
class NObj
{
	//==================================
	//	Private Data
	//==================================
	NTime*				m_pTime;
	NMeshRenderer*		m_pRenderer;
	CString				m_objName;
	NSkinObjVec			m_skinObjVec;
	int					m_skinCount;
	NMATRIX				m_controlWorld;
	int					m_state;
	
public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NObj();
	~NObj();

	//==================================
	//	External Function
	//==================================
	void Init(	int				skinCount = 0 );
	int  Load(	const TCHAR*	skinFile,
				NAniMeshObj*	pAniObj = NULL,
				int				skinIndex = -1 );
	bool LoadSet( const TCHAR* charSet );
	void PreRender();
	void Render();
	void RenderSelect(int index);
	void Remove();
	void Reset();
	void Release();

	//==================================
	//	External Function
	//==================================
	void DeleteSkin(int index);
	void DeleteSkinByName(const TCHAR* name);
	void DeleteSkinAll();
	bool SetAniMatrix( int index, NAniMeshObj* pAniObj );
	bool SetAniMatrixAll( NAniMeshObj* pAniObj );
	void DetachAniMatrix( int index );
	void DetachAniMatrixAll();
	void SetFrame( int skinIndex, int startFrame, int endFrame, bool bLoop = true );
	void SetFrameAll( int startFrame, int endFrame, bool bLoop = true );
	void SetAniSpeed( int skinIndex, float speed );
	void SetAniSpeedAll( float speed );
	void RotateY( float radian );
	void MoveForward( float delta );
	void SetScale( float scl );
	void SetPosition( float x, float y, float z );
	void SetDirection( float x, float y, float z );
	void SetState( int state ) {	m_state = state;	}
	
	const TCHAR*	GetName() const			{	return m_objName.GetString();		}
	NSkinMeshObj&	GetSkin(int index)		{	return m_skinObjVec[index];			}
	int				GetSkinCount()	const	{	return m_skinCount;					}
	const NMATRIX&	GetMatrix() const		{	return m_controlWorld;				}
	const NVECTOR3& GetRight() const		{	return m_controlWorld.GetRow3(0);	}
	const NVECTOR3& GetUp() const			{	return m_controlWorld.GetRow3(1);	}
	const NVECTOR3& GetDirection() const	{	return m_controlWorld.GetRow3(2);	}
	const NVECTOR3& GetPosition() const		{	return m_controlWorld.GetRow3(3);	}
	
	int  GetState() {	return m_state;		}
		
private:
	//==================================
	//	Internal Function
	//==================================
	int i;
	bool CheckAlreadyLoaded(NSkinMeshObj& obj);
};