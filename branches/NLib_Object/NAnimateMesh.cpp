#include "NAnimateMesh.h"
#include "NMeshRenderer.h"
#include "NAniMeshLoader.h"

NAnimateMesh::NAnimateMesh(void)
{
}

NAnimateMesh::~NAnimateMesh(void)
{
}

void NAnimateMesh::Init(NMeshRenderer*	pRenderer)
{
	m_pRenderer = pRenderer;
}

void NAnimateMesh::Frame( NFrameInfo& frame )
{
	static NQUATERNION	tmpQ;
	static NVECTOR3		tmpT;
	static int			curFrame;
	static int			nextFrame;
	
	// 다음 프레임이 마지막 프레임과 같거나 클때
	if( frame.curFrameI >= frame.lastFrame )
	{
		if( !frame.bLoop )
		{
			if( frame.stopped )
				return;
			
			curFrame = frame.lastFrame;
			nextFrame = frame.lastFrame;
			frame.delta = 0.0f;
			frame.stopped = true;
		}
		else
		{
			if( frame.curFrameI > frame.lastFrame )
			{
				frame.curFrameI = frame.startFrame;
				frame.curFrameF = (float)frame.startFrame;
				curFrame  = frame.startFrame;
				nextFrame = frame.startFrame + 1;
			}
			else
			{
				curFrame  = frame.lastFrame;
				nextFrame = frame.startFrame;
			}
		}
	}
	else
	{
		curFrame  = frame.curFrameI;
		nextFrame = frame.curFrameI + 1;
	}

	NAniData& aniPrev = m_aniData[ curFrame  ];
	NAniData& aniNext = m_aniData[ nextFrame ];

	//aniPrev.q.SLerp( tmpQ, aniNext.q, frame.elapsed );//frame.delta );
	//tmpQ.GetMatrix( m_matCalculate );
	D3DXQuaternionSlerp( tmpQ, aniPrev.q, aniNext.q, frame.delta );
	D3DXMatrixRotationQuaternion( m_matCalculate, tmpQ );
	
	aniPrev.t.Lerp( tmpT, aniNext.t, frame.delta );
	m_matCalculate._41 = tmpT.x;
	m_matCalculate._42 = tmpT.y;
	m_matCalculate._43 = tmpT.z;
	//m_matCalculate = aniPrev.m;
}



void NAnimateMesh::Render()
{
	m_pRenderer->RenderAniMesh(*this);
}

void NAnimateMesh::Release()
{
	
}

//////////////////////////////////////////////////////////////////////////
NAniMeshObj::NAniMeshObj():
	m_meshCount(0)
{

}
void NAniMeshObj::Init()
{
	TCHAR* fileName = _T("MeshRender.fx");
	m_pRenderer = Mgr_NRenderer->GetByName_T< NMeshRenderer >( fileName );
}
bool NAniMeshObj::Load( const TCHAR* fileName )
{
	NNOEMatrixLoader loader;
	if( !loader.Load( fileName ) )
		return false;

	Release();

	SetNameFromPath(fileName);
	SplitPath(fileName);

	m_scene = loader.GetScene();
	m_aniMesh = loader.GetMeshObj();

	SetFrame(	(int)m_scene.firstFrame,
				(int)m_scene.lastFrame );
	SetSpeed( 1.0f );

	m_meshCount = (int)m_aniMesh.size();
	for( int i = 0; i < m_meshCount; ++i )
		m_aniMesh[i]->Init( m_pRenderer );

	return true;
}

void NAniMeshObj::Remove()
{
	
}

bool NAniMeshObj::Reset()
{
	return true;
}

void NAniMeshObj::Frame( NFrameInfo& frame, MatrixIndexVec& optIndexVec )
{
	if( !frame.bLoop && frame.curFrameI >= frame.lastFrame )
		return;

	frame.curFrameF +=	frame.elapsed * 
					m_scene.frameSpeed * 
					frame.controlSpeed;

	frame.curFrameI = int(frame.curFrameF);
	frame.delta = frame.curFrameF - (float)frame.curFrameI;
	
	int optCount = (int)optIndexVec.size();
	for( int i = 0; i < optCount; ++i )
		m_aniMesh[ optIndexVec[i] ]->Frame( frame );
}

void NAniMeshObj::FrameSelf(float elapsed)
{
	if( !m_frame.bLoop && m_frame.curFrameI >= m_frame.lastFrame )
		return;

	m_frame.curFrameF +=	elapsed * 
							m_scene.frameSpeed * 
							m_frame.controlSpeed;

	m_frame.curFrameI = int(m_frame.curFrameF);
	m_frame.delta = m_frame.curFrameF - (float)m_frame.curFrameI;
}

void NAniMeshObj::Render(bool bRender)
{
	for( int i = 0; i < m_meshCount; ++i )
	{
		m_aniMesh[i]->Frame( m_frame );
	}

	if( bRender )
		RenderStatic();
}

void NAniMeshObj::RenderStatic()
{
	for( int i = 0; i < m_meshCount; ++i )
		m_aniMesh[i]->Render();
}

void NAniMeshObj::RenderSelect(int index)
{
	m_aniMesh[index]->Frame( m_frame );
	m_aniMesh[index]->Render();
}

void NAniMeshObj::SetNameFromPath( const TCHAR* filePath )
{
	TCHAR tmpStr[MAX_PATH];
	TCHAR tmpName[MAX_PATH];

	_tsplitpath_s( filePath, tmpStr, MAX_PATH,
		tmpStr, MAX_PATH,
		tmpName, MAX_PATH,
		tmpStr, MAX_PATH );
	m_name = tmpName;
}

void NAniMeshObj::SetFrame( int startFrame, int endFrame, bool bLoop )
{
	m_frame.startFrame = startFrame;
	m_frame.lastFrame = endFrame;
	m_frame.bLoop = bLoop;
	m_frame.stopped = false;
	m_frame.curFrameF = (float)startFrame;
}

void NAniMeshObj::SetStartFrame(int frame)
{
	m_frame.startFrame = frame;
}

void NAniMeshObj::SetLastFrame(int frame)
{
	m_frame.lastFrame = frame;
}

void NAniMeshObj::SetCurFrame(int frame)
{
	m_frame.curFrameF = (float)frame;
	m_frame.curFrameI = frame;
	m_frame.delta = 0.0f;
	bool oldLoop = m_frame.bLoop;
	m_frame.bLoop = false;
	m_frame.stopped = false;

	for( int i = 0; i < m_meshCount; ++i )
		m_aniMesh[i]->Frame( m_frame );

	m_frame.bLoop = oldLoop;
}

void NAniMeshObj::DeltaFrame(int frame)
{
	m_frame.curFrameI = m_frame.curFrameI + frame;

	if( m_frame.curFrameI < m_frame.startFrame )
		m_frame.curFrameI = m_frame.startFrame;
	else if( m_frame.curFrameI > m_frame.lastFrame )
		m_frame.curFrameI = m_frame.lastFrame;

	SetCurFrame( m_frame.curFrameI );
}

void NAniMeshObj::SetFrameReset()
{
	SetCurFrame( m_frame.startFrame );
}

void NAniMeshObj::SetLoop(bool bLoop)
{
	m_frame.bLoop = bLoop;
	m_frame.stopped = false;
}

void NAniMeshObj::SetSpeed( float speed )
{
	m_frame.controlSpeed = speed;
}

void NAniMeshObj::Release()
{
	SAFE_STL_VECTOR_DELETE_AFTER_RELEASE( m_aniMesh );
	m_meshCount = 0;
}