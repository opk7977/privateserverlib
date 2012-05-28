#include "NObject.h"
#include "NL_Time.h"
#include "NL_Renderer.h"
#include "NAnimateMesh.h"
#include "NMeshRenderer.h"
#include <cassert>

NObject::NObject(void):
	m_skinCount(0)
{
	m_ambientCol.Set(1.0f, 1.0f, 1.0f);
	m_diffuseCol.Set(1.0f, 1.0f, 1.0f);
}

NObject::~NObject(void)
{
	
}

bool NObject::operator == ( const NObject& rhs )
{
	if( m_name == rhs.m_name )
		return true;

	return false;
}

bool NObject::operator < (const NObject& rhs)
{
	if( m_name < rhs.m_name )
		return true;

	return false;
}

bool NObject::operator < (const NObject& rhs) const
{
	if( m_name < rhs.m_name )
		return true;

	return false;
}

void NObject::Init()
{
	m_pTime = I_NTime;
	m_skinCount = 0;

	TCHAR* fileName = _T("MeshRender.fx");
	m_pRenderer = Mgr_NRenderer->GetByName_T< NMeshRenderer >( fileName );
}

int NObject::Load( const TCHAR* skinFile, NAniMeshObj* pAniObj )
{
	NSkinMeshObj* newSkin = Mgr_NSkinObj->GetByPath( skinFile );
	if(!newSkin)
		return -1;
	
	if( CheckAlreadyLoaded( *newSkin ) )
	{
		MessageBox( NULL, _T("This skin file is already loaded."), _T("NObject::Load()"), MB_OK );
		return -2;
	}
	newSkin->SetAniMatrix(pAniObj);

	if( m_skinObjVec.empty() )
	{
		m_sphere = newSkin->m_sphere;
		m_aabb   = newSkin->m_bbox;
	}

	m_skinObjVec.push_back( *newSkin );
	return m_skinCount++;
}

bool NObject::LoadSet(const TCHAR* charSet, NAniMeshObj** pOutAniObj)
{
	FILE* fp;
	_tfopen_s( &fp, charSet, _T("rt") );
	if( !fp )
	{
		MessageBox(NULL, _T("Invalid File Name."), _T("NObject - LoadSet()"), MB_OK);
		return false;
	}

	TCHAR buffer[MAX_PATH] = {0};

	// Validate
	_ftscanf_s( fp, _T("%s %s\n"), buffer, MAX_PATH, buffer, MAX_PATH );
	if( _tcsicmp( buffer, _T("CHARACTER") ) )
	{
		MessageBox(NULL, _T("Invalid NCH file."), _T("NObject - LoadSet()"), MB_OK);
		return false;
	}

	// Character Name
	_fgetts( buffer, MAX_PATH, fp );
	_ftscanf_s( fp, _T("%s %s\n"), buffer, MAX_PATH, buffer, MAX_PATH );
	m_name = buffer;

	// Skin Info
	CString baseDir = _T("../data/");
	CString curDir;
	_fgetts( buffer, MAX_PATH, fp );
	NSkinObjVec skinVec;
	int skinCount;
	_ftscanf_s( fp, _T("%s %d\n"), buffer, MAX_PATH, &skinCount );
	int index;
	skinVec.resize(skinCount);
	Mgr_NSkinObj->SetDir(_T("../data/"));
	for( int i = 0; i < skinCount; ++i )
	{
		_ftscanf_s( fp, _T("%s %d %s\n"), buffer, MAX_PATH, &index, buffer, MAX_PATH );
		curDir = baseDir + buffer;

		NSkinMeshObj* skin = Mgr_NSkinObj->GetByName( curDir.GetString() );
		if( !skin )
		{
			CString errStr = _T("Invalid NOE file - [ ");
			errStr += buffer;
			errStr += _T(" ]");
			MessageBox(NULL, errStr.GetString(), _T("NObject - LoadSet()"), MB_OK);
			//return false;
		}
		else
		{
			skinVec[i] = *skin;
		}
	}
	m_skinObjVec = skinVec;
	m_skinCount = skinCount;

	// Animation Info
	_fgetts( buffer, MAX_PATH, fp );
	NAniMeshObj* pAni;
	int aniCount;
	_ftscanf_s( fp, _T("%s %d\n"), buffer, MAX_PATH, &aniCount );
	if( aniCount == 0 )
		return true;

	_ftscanf_s( fp, _T("%s %s\n"), buffer, MAX_PATH, buffer, MAX_PATH );
	Mgr_NAniObj->SetDir(_T("../data/"));
	int chk = Mgr_NAniObj->AddByName( buffer );
	if( chk < 0 )
	{
		CString errStr = _T("Invalid NMT file - [ ");
		errStr += buffer;
		errStr += _T(" ]");
		MessageBox(NULL, errStr.GetString(), _T("NObject - LoadSet()"), MB_OK);
		//return false;
	}
	pAni = Mgr_NAniObj->GetByIndex( chk );
	SetAniMatrixAll( pAni );
	if( pOutAniObj )
		*pOutAniObj = pAni;

	// Action List
	_fgetts( buffer, MAX_PATH, fp );
	int actionCount;
	_ftscanf_s( fp, _T("%s %d\n"), buffer, MAX_PATH, &actionCount );
	int start, end;
	m_actionFrame.resize( actionCount );
	for( int i = 0; i < actionCount; ++i )
	{
		_ftscanf_s( fp, _T("%s %d %s %d %d\n"), 
			buffer, MAX_PATH, &index,
			buffer, MAX_PATH, 
			&start,
			&end );
		m_actionFrame[i].Init( buffer, start, end );
	}

	return true;
}

bool NObject::SaveSet(const TCHAR* charSet)
{
	FILE* fp;
	_tfopen_s( &fp, charSet, _T("wt") );
	if( !fp )
	{
		MessageBox(NULL, _T("Failed to Create File."), _T("NObject - SaveSet()"), MB_OK);
		return false;
	}

	// Header
	_ftprintf_s( fp, _T("*NOBJECT_EXPORTER CHARACTER\n") );

	// Character Info
	_ftprintf_s( fp, _T("*CHARACTER_INFO\n") );
	_ftprintf_s( fp, _T("\t*NAME %s\n"), m_name.GetString() );

	// Skin Mesh
	CString skinName;
	_ftprintf_s( fp, _T("*SKIN_INFO\n") );
	_ftprintf_s( fp, _T("\t*NUM_MESH %d\n"), m_skinCount );
	for( int i = 0; i < m_skinCount; ++i )
	{
		CString name_ext = m_skinObjVec[i].m_name;
		name_ext += _T(".NOE");
		_ftprintf_s( fp, _T("\t*MESH %d %s\n"), i+1, name_ext.GetString() );
	}

	// Animate Mesh
	_ftprintf_s( fp, _T("*ANIMATION_INFO\n") );
	_ftprintf_s( fp, _T("\t*NUM_MESH %d\n"), m_skinObjVec[0].m_pAniMeshObj ? 1 : 0 );
	if( !m_skinObjVec[0].m_pAniMeshObj )
		return true;

	CString aniName = m_skinObjVec[0].m_pAniMeshObj->m_name;
	aniName += _T(".NMT");
	_ftprintf_s( fp, _T("\t*MESH %s\n"), aniName.GetString() );

	// Action List
	int actionCount = (int)m_actionFrame.size();
	_ftprintf_s( fp, _T("*ACTION_INFO\n") );
	_ftprintf_s( fp, _T("\tNUM_ACTION %d\n"), actionCount );
	for( int i = 0; i < actionCount; ++i )
	{
		_ftprintf_s( fp, _T("\tACTION %d %s %d %d\n"), i,
			m_actionFrame[i].name.GetString(),
			m_actionFrame[i].start,
			m_actionFrame[i].end );
	}

	return true;
}

void NObject::PreRender()
{
	//m_pRenderer->SetWorld( m_controlWorld );
	m_pRenderer->PreRender(this);
}

void NObject::Render()
{
	PreRender();

	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].Render( m_pTime->GetElapsedTime() );
}

void NObject::RenderSphere()
{
	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].RenderSphere( m_controlWorld );
}

void NObject::RenderBox()
{
	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].RenderBox( m_controlWorld );
}

void NObject::RenderSphereSelect(int index)
{
	m_skinObjVec[index].RenderSphere( m_controlWorld );
}

void NObject::RenderBoxSelect(int index)
{
	m_skinObjVec[index].RenderBox( m_controlWorld );
}

void NObject::RenderObjectSphere()
{
	m_sphere.Render( m_controlWorld );
}

void NObject::RenderObjectBox()
{
	m_aabb.Render( m_controlWorld );
}

void NObject::RenderObjectOBB()
{
	m_obb.Render();
}

void NObject::RenderObjectCapsule()
{
	m_capsule.Render( m_controlWorld );
}

void NObject::RenderSelect(int index)
{
	m_skinObjVec[ index ].Render( m_pTime->GetElapsedTime() );
}

void NObject::RenderStatic()
{
	PreRender();

	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].RenderStatic();
}

void NObject::RenderStaticSelect(int index)
{
	m_skinObjVec[ index ].RenderStatic();
}

void NObject::Remove()
{
	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].Remove();
}

void NObject::Reset()
{
	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].Reset();
}

void NObject::Release()
{
	if( m_skinCount > 0 )
	{
// 		for(i = 0; i < m_skinCount; ++i)
// 			m_skinObjVec[i].Release();
		
		NSkinObjVec().swap( m_skinObjVec );
		m_skinCount = 0;
	}
}

void NObject::SetName(const TCHAR* name)
{
	m_name = name;
}

void NObject::DeleteSkin( int index )
{
	if( m_skinCount > 0 )
	{
		NSkinObjItor itor = m_skinObjVec.begin() + index;
		itor->Release();
		m_skinObjVec.erase( itor );
		--m_skinCount;
	}
}

void NObject::DeleteSkinByName(const TCHAR* name)
{
	for(NSkinObjItor itor = m_skinObjVec.begin();
		itor != m_skinObjVec.end(); ++itor)
	{
		NSkinMeshObj& curObj = (*itor);
		for( size_t i = 0; i < curObj.m_skinMeshVec.size(); ++i )
		{
			if( !_tcsicmp( curObj.m_skinMeshVec[i]->m_name.GetString(), name ) )
			{
				itor->Release();
				m_skinObjVec.erase( itor );
				--m_skinCount;
				return;
			}
		}
	}
}

void NObject::DeleteSkinAll()
{
	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].Release();
	NSkinObjVec().swap( m_skinObjVec );
	m_skinCount = 0;
}

bool NObject::SetAniMatrix(int index, NAniMeshObj* pAniObj)
{
	return m_skinObjVec[index].SetAniMatrix(pAniObj);
}

bool NObject::SetAniMatrixAll(NAniMeshObj* pAniObj)
{
	for(int i = 0; i < m_skinCount; ++i)
	{
		if( !m_skinObjVec[i].ValidateAniMatrix(pAniObj) )
		{
			CString errMsg = _T("Matrix node is not enough - [ ");
			errMsg += m_skinObjVec[i].m_name;
			errMsg += _T(" ]");
			::MessageBox( NULL, errMsg.GetString(), _T("NObject SetAniMatrix()"), MB_OK );
			return false;
		}
	}

	for(int i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].SetAniMatrix(pAniObj);

	return true;
}

void NObject::DetachAniMatrix(int index)
{
	m_skinObjVec[index].DetachAniMatrix();
}

void NObject::DetachAniMatrixAll()
{
	for(int i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].DetachAniMatrix();
}

void NObject::SetFrame( int skinIndex, int startFrame, int endFrame, bool bLoop /*= true */ )
{
	assert( skinIndex >= 0 && skinIndex < m_skinCount );

	m_skinObjVec[ skinIndex ].SetFrame( startFrame, endFrame, bLoop );
}

void NObject::SetFrameAll( int startFrame, int endFrame, bool bLoop /*= true */ )
{
	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].SetFrame( startFrame, endFrame, bLoop );
}

void NObject::SetStartFrameAll(int startFrame)
{
	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].SetStartFrame( startFrame );
}

void NObject::SetEndFrameAll(int endFrame)
{
	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].SetEndFrame( endFrame );
}

void NObject::SetCurFrameAll(int curFrame)
{
	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].SetCurFrame( curFrame );
}

void NObject::DeltaFrameAll(int deltaFrame)
{
	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].DeltaFrame( deltaFrame );
}

void NObject::SetFrameResetAll()
{
	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].SetFrameReset();
}

void NObject::SetLoopAll(bool bLoop)
{
	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].SetLoop(bLoop);
}

void NObject::SetAniSpeed( int skinIndex, float speed )
{
	assert( skinIndex >= 0 && skinIndex < m_skinCount );

	m_skinObjVec[ skinIndex ].SetSpeed( speed );
}

void NObject::SetAniSpeedAll( float speed )
{
	for(i = 0; i < m_skinCount; ++i)
		m_skinObjVec[i].SetSpeed( speed );
}

void NObject::RotateY( float radian )
{
	static NMATRIX rotMat;
	rotMat.Identity();
	rotMat.RotateY( radian );

	m_controlWorld.Multiply33WithMe( rotMat );
}

void NObject::MoveForward( float delta )
{
	m_controlWorld.GetRow3(3) += m_controlWorld.GetRow3(2) * delta;
}

void NObject::MoveSide(float delta)
{
	m_controlWorld.GetRow3(3) += m_controlWorld.GetRow3(0) * delta;
}

void NObject::SetScale( float scl )
{
	static NMATRIX sclMat;
	sclMat.Identity();
	sclMat.Scale( scl, scl, scl );

	m_controlWorld.Multiply33WithMe( sclMat );
}

void NObject::SetPosition( float x, float y, float z )
{
	m_controlWorld._41 = x;
	m_controlWorld._42 = y;
	m_controlWorld._43 = z;
}

void NObject::SetDirection( float x, float y, float z )
{
	NVECTOR3 pos = m_controlWorld.GetRow3(3);
	m_controlWorld.ObjectLookAtLHByDir(
		pos, NVECTOR3( x, y, z ), 
		NVECTOR3( 0.0f, 1.0f, 0.0f ) );
}

bool NObject::CheckAlreadyLoaded(NSkinMeshObj& obj)
{
	for(int i = 0; i < m_skinCount; ++i)
	{
		if( obj.m_skinMeshVec[0]->m_name == 
			m_skinObjVec[i].m_skinMeshVec[0]->m_name )
			return true;
	}
	return false;
}

void NObject::AddAction(const TCHAR* actionName, int start, int end)
{
	NActionFrame newAction;
	newAction.Init(actionName, start, end);
	m_actionFrame.push_back(newAction);
}

void NObject::DeleteAction(int index)
{
	m_actionFrame.erase( m_actionFrame.begin() + index );
}

void NObject::DeleteActionAll()
{
	SAFE_STL_VECTOR_WIPE( m_actionFrame );
}

void NObject::SetAction(int skinIndex, int actionIndex)
{
	assert( skinIndex >= 0 && actionIndex >= 0 );
	assert( skinIndex < m_skinCount );
	assert( actionIndex < (int)m_actionFrame.size() );

	SetFrame( skinIndex,
		m_actionFrame[actionIndex].start,
		m_actionFrame[actionIndex].end );
}

void NObject::SetActionAll(int actionIndex)
{
	assert( actionIndex >= 0 );
	assert( actionIndex < (int)m_actionFrame.size() );

	SetFrameAll(m_actionFrame[actionIndex].start,
				m_actionFrame[actionIndex].end,
				m_actionFrame[actionIndex].loop );
}

void NObject::ModifyAction(int index,const TCHAR* actionName,int start,int end)
{
	assert( index >= 0 );
	assert( index < (int)m_actionFrame.size() );

	m_actionFrame[index].Init( actionName, start, end );
}

void NObject::BuildWorldAABB()
{
	NVECTOR3 vAABBVert[8];
	NVECTOR3 vWorldAABBVert[8];

	// 좌 상 앞
	vAABBVert[0].x = m_aabb.vMin.x;
	vAABBVert[0].y = m_aabb.vMax.y;
	vAABBVert[0].z = m_aabb.vMin.z;

	// 우 상 앞
	vAABBVert[1].x = m_aabb.vMax.x;
	vAABBVert[1].y = m_aabb.vMax.y;
	vAABBVert[1].z = m_aabb.vMin.z;

	// 좌 하 앞
	vAABBVert[2].x = m_aabb.vMin.x;
	vAABBVert[2].y = m_aabb.vMin.y;
	vAABBVert[2].z = m_aabb.vMin.z;

	// 우 하 앞
	vAABBVert[3].x = m_aabb.vMax.x;
	vAABBVert[3].y = m_aabb.vMin.y;
	vAABBVert[3].z = m_aabb.vMin.z;

	// 좌 상 뒤
	vAABBVert[4].x = m_aabb.vMin.x;
	vAABBVert[4].y = m_aabb.vMax.y;
	vAABBVert[4].z = m_aabb.vMax.z;

	// 우 상 뒤
	vAABBVert[5].x = m_aabb.vMax.x;
	vAABBVert[5].y = m_aabb.vMax.y;
	vAABBVert[5].z = m_aabb.vMax.z;

	// 좌 하 뒤
	vAABBVert[6].x = m_aabb.vMin.x;
	vAABBVert[6].y = m_aabb.vMin.y;
	vAABBVert[6].z = m_aabb.vMax.z;

	// 우 하 뒤
	vAABBVert[7].x = m_aabb.vMax.x;
	vAABBVert[7].y = m_aabb.vMin.y;
	vAABBVert[7].z = m_aabb.vMax.z;

	for(int i = 0; i < 8; ++i)
		m_controlWorld.MultiplyCoord(vWorldAABBVert[i], vAABBVert[i]);

	m_aabbWorld.vMin.Set(50000.0f);
	m_aabbWorld.vMax.Set(-50000.0f);

	for(int i = 0; i < 8; ++i)
	{
		if(		m_aabbWorld.vMin.x > vWorldAABBVert[i].x)	
				m_aabbWorld.vMin.x = vWorldAABBVert[i].x;
		else if(m_aabbWorld.vMax.x < vWorldAABBVert[i].x)
				m_aabbWorld.vMax.x = vWorldAABBVert[i].x;

		if(		m_aabbWorld.vMin.y > vWorldAABBVert[i].y)	
				m_aabbWorld.vMin.y = vWorldAABBVert[i].y;
		else if(m_aabbWorld.vMax.y < vWorldAABBVert[i].y)
				m_aabbWorld.vMax.y = vWorldAABBVert[i].y;

		if(		m_aabbWorld.vMin.z > vWorldAABBVert[i].z)	
				m_aabbWorld.vMin.z = vWorldAABBVert[i].z;
		else if(m_aabbWorld.vMax.z < vWorldAABBVert[i].z)
				m_aabbWorld.vMax.z = vWorldAABBVert[i].z;
	}
}

void NObject::BuildSphere()
{
	m_sphereWorld.radius = m_sphere.radius * m_controlWorld.GetRow3(0).Length();
	m_controlWorld.MultiplyCoord( m_sphereWorld.centerPoint, m_sphere.centerPoint );
}

void NObject::BuildOBB()
{
	m_obb.Init( m_controlWorld, m_aabb );
}

void NObject::BuildCapsule()
{
	float scale = m_controlWorld.GetRow3(0).Length();
	m_capsuleWorld.height = m_capsule.height * scale;
	m_capsuleWorld.radius = m_capsule.radius * scale;
	m_controlWorld.MultiplyCoord( m_capsuleWorld.bottomPoint, m_capsule.bottomPoint );
	m_controlWorld.MultiplyNormal( m_capsuleWorld.upVector, m_capsule.upVector );
}

void NObject::SetMatrix(const NMATRIX& mat)
{
	m_controlWorld = mat;
}

void NObject::SetCapsule(NCapsule& capsule)
{
	m_capsule = capsule;
}

void NObject::SetAmbientCol(int r, int g, int b)
{
	r = min( 255, max( 0, r ) );
	g = min( 255, max( 0, g ) );
	b = min( 255, max( 0, b ) );

	float inv255 = 1.0f / 255;
	m_ambientCol.x = r * inv255;
	m_ambientCol.y = g * inv255;
	m_ambientCol.z = b * inv255;
}

void NObject::SetDiffuseCol(int r, int g, int b)
{
	r = min( 255, max( 0, r ) );
	g = min( 255, max( 0, g ) );
	b = min( 255, max( 0, b ) );

	float inv255 = 1.0f / 255;
	m_diffuseCol.x = r * inv255;
	m_diffuseCol.y = g * inv255;
	m_diffuseCol.z = b * inv255;
}

void NObject::ResetColor()
{
	m_ambientCol.Set(1.0f);
	m_diffuseCol.Set(1.0f);
}