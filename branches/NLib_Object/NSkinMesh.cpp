#include "NSkinMesh.h"
#include "NMeshRenderer.h"
#include "NSkinMeshLoader.h"
#include "NAnimateMesh.h"
#include "NL_Device.h"

NSkinMesh::NSkinMesh():
	m_PNCT_VB(0),
	m_Tan_VB(0),
	m_Weight_VB(0),
	m_optIB(0),
	m_pRenderer(0),
	m_type(-1),
	m_format(0),
	m_faceCount(0),
	m_vertCount(0),
	m_faceGroupCount(0),
	m_optMatrixCount(0),
	m_optMatTransRow0(0),
	m_optMatTransRow1(0),
	m_optMatTransRow2(0),
	m_solidIndex(-1),
	m_maxWeightCount(0),
	m_bUseNormalmap(false)
{
	
}

NSkinMesh::~NSkinMesh()
{
}

void NSkinMesh::Init(NMeshRenderer* pRenderer)
{
	m_pDevice = I_ND3d;
	m_pRenderer = pRenderer;
	m_preRender[ST_STATIC_OBJECT]	= &NSkinMesh::PreRenderStatic;
	m_preRender[ST_DYNAMIC_OBJECT]	= &NSkinMesh::PreRenderDynamic;
	m_preRender[ST_RIGID_OBJECT]	= &NSkinMesh::PreRenderRigid;
}

void NSkinMesh::Render(NAniMeshObj* pAniObj)
{
	(this->*m_preRender[ m_skinType ])( pAniObj );
	m_pRenderer->RenderSkinMesh( *this );
}

void NSkinMesh::PreRenderStatic( NAniMeshObj* pAniObj )
{
	m_pRenderer->SetMultiply(FALSE);
}
void NSkinMesh::PreRenderDynamic( NAniMeshObj* pAniObj )
{
	if(pAniObj)
	{
		m_pRenderer->SetSolidWorld( pAniObj->m_aniMesh[ m_solidIndex ]->m_matCalculate );
		m_pRenderer->SetMultiply(TRUE);
	}
	else
		m_pRenderer->SetMultiply(FALSE);
}
void NSkinMesh::PreRenderRigid(NAniMeshObj* pAniObj)
{
	if( pAniObj )
	{
		for(int i = 0; i < m_optMatrixCount; ++i)
		{
			static NMATRIX transposed;
			pAniObj->m_aniMesh[ m_optMatrixRefVec[i] ]->m_matCalculate.Transpose( transposed );
			m_optMatTransRow0[i] = transposed.GetRow4(0);
			m_optMatTransRow1[i] = transposed.GetRow4(1);
			m_optMatTransRow2[i] = transposed.GetRow4(2);
		}
	}
	else
	{
		static NMATRIX __identity;
		for(int i = 0; i < m_optMatrixCount; ++i)
		{
			m_optMatTransRow0[i] = __identity.GetRow4(0);
			m_optMatTransRow1[i] = __identity.GetRow4(1);
			m_optMatTransRow2[i] = __identity.GetRow4(2);
		}
	}
}

void NSkinMesh::DeleteBuffer()
{
	SAFE_RELEASE( m_PNCT_VB		)
	SAFE_RELEASE( m_Tan_VB		)
	SAFE_RELEASE( m_Weight_VB	)
	SAFE_RELEASE( m_optIB		)

#ifdef _DEBUG
	SAFE_STL_VECTOR_WIPE( m_normLineVec );
	SAFE_STL_VECTOR_WIPE( m_tanLineVec );
	SAFE_STL_VECTOR_WIPE( m_normLine2Vec );
	SAFE_STL_VECTOR_WIPE( m_tanLine2Vec );
#endif
}

void NSkinMesh::ComputeTangent()
{
	int curIndex = 0;
	TangentVec tmpVec;
	tmpVec.reserve( m_faceCount * 3 );

	bool bFound;
	NVECTOR3 *v0, *v1, *v2, *n0, *n1, *n2;
	NVECTOR2 *t0, *t1, *t2;
	NVECTOR3 e1, e2, biNormal, tangent;
	for(int i = 0; i < m_faceCount; ++i)
	{
		NFace& curFace = m_faceVec[i];
		v0 = &m_posVec[ curFace.posIndex._0 ];
		v1 = &m_posVec[ curFace.posIndex._1 ];
		v2 = &m_posVec[ curFace.posIndex._2 ];
		t0 = &m_texVec[ curFace.texIndex._0 ];
		t1 = &m_texVec[ curFace.texIndex._1 ];
		t2 = &m_texVec[ curFace.texIndex._2 ];
		n0 = &m_normVec[ curFace.normIndex._0 ];
		n1 = &m_normVec[ curFace.normIndex._1 ];
		n2 = &m_normVec[ curFace.normIndex._2 ];

		// 접선 벡터 계산
		//
		// V0
		e1 = *v1 - *v0;
		e2 = *v2 - *v0;
		biNormal = -(t2->x - t0->x)*e1 + (t1->x - t0->x)*e2;
		biNormal.Cross( tangent, *n0 );
		tangent.Normalize();
		bFound = false;
		for( int j = curIndex - 1; j >= 0; --j )
		{
			if(tmpVec[j] == tangent)
			{
				curFace.tanIndex._0 = j;
				bFound = true;
			}
		}
		if(!bFound)
		{
			tmpVec.push_back( tangent );
			curFace.tanIndex._0 = curIndex++;
		}

		// V1
		e1 = *v2 - *v1;
		e2 = *v0 - *v1;
		biNormal = -(t0->x - t1->x)*e1 + (t2->x - t1->x)*e2;
		biNormal.Cross( tangent, *n1 );
		tangent.Normalize();
		bFound = false;
		for( int j = curIndex - 1; j >= 0; --j )
		{
			if(tmpVec[j] == tangent)
			{
				curFace.tanIndex._1 = j;
				bFound = true;
			}
		}
		if(!bFound)
		{
			tmpVec.push_back( tangent );
			curFace.tanIndex._1 = curIndex++;
		}

		// V2
		e1 = *v0 - *v2;
		e2 = *v1 - *v2;
		biNormal = -(t1->x - t2->x)*e1 + (t0->x - t2->x)*e2;
		biNormal.Cross( tangent, *n2 );
		tangent.Normalize();
		bFound = false;
		for( int j = curIndex - 1; j >= 0; --j )
		{
			if(tmpVec[j] == tangent)
			{
				curFace.tanIndex._2 = j;
				bFound = true;
			}
		}
		if(!bFound)
		{
			tmpVec.push_back( tangent );
			curFace.tanIndex._2 = curIndex++;
		}
	}
	m_tanVec.swap(tmpVec);

	int a = 0;
}

bool NSkinMesh::SetRenderBuffer()
{
	if( m_faceCount == 0 || m_format == 0 )
		return false;

	DeleteBuffer();

	if( !SetPNCTBuffer() )
		return false;

	if((m_format & RS_WEIGHT4) || (m_format & RS_WEIGHT8))
	{
		if( !SetWeightBuffer() )
			return false;
	}

	if(m_format & RS_TAN)
	{
		if( !SetTangentBuffer() )
			return false;
	}

	if( !SetIndexBuffer() )
		return false;

#ifdef _DEBUG
	SetNormalLineBuffer();
	SetTangentLineBuffer();
#endif

	return true;
}

void NSkinMesh::Optimize()
{
	if( !CheckCurrentRenderType() )
		return;

	ResetOptVec();

	switch( m_format )
	{
	case RT_P3NCT:		OptimizeP3NCT(false);		break;
	case RT_P3NCTT:		OptimizeP3NCTT(false);		break;
	case RT_P3NCTW4:	OptimizeP3NCT(true);		break;
	case RT_P3NCTW4T:	OptimizeP3NCTT(true);		break;
	case RT_P3NCTW8:	OptimizeP3NCT(true, true);	break;
	case RT_P3NCTW8T:	OptimizeP3NCTT(true, true);	break;
	default:
		{
			CString tmp;
			tmp.Format(_T("Unsupported Render Format(%#4x)"), m_format);
			MessageBox(NULL, tmp.GetString(), _T("NSkinMesh"), MB_OK);
			m_type = 0;
		}
	}
}

int  NSkinMesh::CheckCurrentRenderType()
{
	m_format = 0;
	m_type = -1;

	// RS_BASE Check
	if( !m_posVec.empty() && 
		!m_texVec.empty() && 
		!m_normVec.empty() &&
		!m_colVec.empty() )
		m_format = RT_P3NCT;

	// RS_WEIGHT Check
	if( !m_weight8Vec.empty() )
		m_format |= RS_WEIGHT8;
	else if( !m_weightVec.empty() )
		m_format |= RS_WEIGHT4;
			
	// RS_TAN Check
	if( m_bUseNormalmap )
	{
		if( m_tanVec.empty() )
			ComputeTangent();
		m_format |= RS_TAN;
	}
		
	switch( m_format )
	{
	case RT_P3NCT:		m_type = RN_P3NCT;		break;
	case RT_P3NCTT:		m_type = RN_P3NCTT;		break;
	case RT_P3NCTW4:	m_type = RN_P3NCTW4;	break;
	case RT_P3NCTW4T:	m_type = RN_P3NCTW4T;	break;
	case RT_P3NCTW8:	m_type = RN_P3NCTW8;	break;
	case RT_P3NCTW8T:	m_type = RN_P3NCTW8T;	break;
	}

	return m_format;
}

void NSkinMesh::ResetOptVec()
{
	PosPVec().swap(	m_optPosVec );
	TexPVec().swap( m_optTexVec );
	ColorVec().swap( m_optColVec );
	NormalPVec().swap( m_optNormVec );
	TangentPVec().swap( m_optTanVec );
	IW4_PVEC().swap( m_optWeightVec );
	IW8_PVEC().swap( m_optWeight8Vec );
}

void NSkinMesh::OptimizeP3NCT(bool bWeight, bool bW8)
{
	int curIndex = 0;
	CAtlMap<CString, int>			optMap;
	CAtlMap<CString, int>::CPair*	curPair;
	TCHAR* format = _T("%u_%u_%u_%u");
	CString tmpStr;

	m_optPosVec.reserve(m_faceCount * 3);
	m_optNormVec.reserve(m_faceCount * 3);
	m_optColVec.reserve(m_faceCount * 3);
	m_optTexVec.reserve(m_faceCount * 3);
	if(bWeight)
	{
		if(bW8)
			m_optWeight8Vec.reserve(m_faceCount * 3);
		else
			m_optWeightVec.reserve(m_faceCount * 3);
	}

	// 위치, 법선, 컬러, 텍스쳐좌표 최적화
	for(int i = 0; i < m_faceCount; ++i)
	{
		NFace& curFace = m_faceVec[i];

		for(int j = 0; j < 3; ++j)
		{
			tmpStr.Format(format, 
				curFace.posIndex.v[j],
				curFace.normIndex.v[j],
				curFace.colIndex.v[j],
				curFace.texIndex.v[j]);

			// 일단 찾고
			curPair = optMap.Lookup(tmpStr);
			if(curPair)
			{
				// 있으면 해당 인덱스 대입
				curFace.optIndex.v[j] = curPair->m_value;
			}
			else
			{
				// 없으면 추가 후 인덱스 대입
				m_optPosVec.push_back( &m_posVec[ curFace.posIndex.v[j] ] );
				m_optNormVec.push_back( &m_normVec[ curFace.normIndex.v[j] ] );
				m_optColVec.push_back( m_colVec[ curFace.colIndex.v[j] ] );
				m_optTexVec.push_back( &m_texVec[ curFace.texIndex.v[j] ] );
				if(bWeight)
				{
					if(bW8)
						m_optWeight8Vec.push_back( &m_weight8Vec[ curFace.posIndex.v[j] ] );
					else
						m_optWeightVec.push_back( &m_weightVec[ curFace.posIndex.v[j] ] );
				}
				optMap[ tmpStr ] = curIndex;
				curFace.optIndex.v[j] = curIndex++;
			}
		}
	}
}
void NSkinMesh::OptimizeP3NCTT(bool bWeight, bool bW8)
{
	int curIndex = 0;
	CAtlMap<CString, int>			optMap;
	CAtlMap<CString, int>::CPair*	curPair;
	TCHAR* format = _T("%u_%u_%u_%u_%u");
	CString tmpStr;

	m_optPosVec.reserve(m_faceCount * 3);
	m_optNormVec.reserve(m_faceCount * 3);
	m_optColVec.reserve(m_faceCount * 3);
	m_optTexVec.reserve(m_faceCount * 3);
	m_optTanVec.reserve(m_faceCount * 3);
	if(bWeight)
	{
		if(bW8)
			m_optWeight8Vec.reserve(m_faceCount * 3);
		else
			m_optWeightVec.reserve(m_faceCount * 3);
	}

	// 위치, 법선, 컬러, 텍스쳐좌표, 접선 최적화
	for(int i = 0; i < m_faceCount; ++i)
	{
		NFace& curFace = m_faceVec[i];

		for(int j = 0; j < 3; ++j)
		{
			tmpStr.Format(format, 
				curFace.posIndex.v[j],
				curFace.normIndex.v[j],
				curFace.colIndex.v[j],
				curFace.texIndex.v[j],
				curFace.tanIndex.v[j]);

			// 일단 찾고
			curPair = optMap.Lookup(tmpStr);
			if(curPair)
			{
				// 있으면 해당 인덱스 대입
				curFace.optIndex.v[j] = curPair->m_value;
			}
			else
			{
				// 없으면 추가 후 인덱스 대입
				m_optPosVec.push_back( &m_posVec[ curFace.posIndex.v[j] ] );
				m_optNormVec.push_back( &m_normVec[ curFace.normIndex.v[j] ] );
				m_optColVec.push_back( m_colVec[ curFace.colIndex.v[j] ] );
				m_optTexVec.push_back( &m_texVec[ curFace.texIndex.v[j] ] );
				m_optTanVec.push_back( &m_tanVec[ curFace.tanIndex.v[j] ] );
				if(bWeight)
				{
					if(bW8)
						m_optWeight8Vec.push_back( &m_weight8Vec[ curFace.posIndex.v[j] ] );
					else
						m_optWeightVec.push_back( &m_weightVec[ curFace.posIndex.v[j] ] );
				}
				optMap[ tmpStr ] = curIndex;
				curFace.optIndex.v[j] = curIndex++;
			}
		}
	}
}

void NSkinMesh::OptimizeMatrix()
{
	if( m_skinType == ST_STATIC_OBJECT ||
		m_skinType == ST_DYNAMIC_OBJECT )
		return;

	std::vector<int>().swap( m_optMatrixRefVec );

	if( m_optMatTransRow0 )
	{
		delete []m_optMatTransRow0;
		m_optMatTransRow0 = NULL;
	}
	if( m_optMatTransRow1 )
	{
		delete []m_optMatTransRow1;
		m_optMatTransRow1 = NULL;
	}
	if( m_optMatTransRow2 )
	{
		delete []m_optMatTransRow2;
		m_optMatTransRow2 = NULL;
	}

	KeepIndexMap	indexMap;
	KeepIndexItor	curItor;

	// 메쉬에 쓰이는 바이패드만 골라서 
	// 행렬 리스트 최적화
	int curIndex = 0;
	int index;
	int weightCount;

	if( m_maxWeightCount > 4 )
	{
		weightCount = (int)m_weight8Vec.size();
		for( int i = 0; i < weightCount; ++i )
		{
			IW8_VERTEX& srcVert = m_srcWeight8Vec[i];
			IW8_VERTEX& curVert = m_weight8Vec[i];
			for(int j = 0; j < (int)srcVert.weight[7]; ++j)
			{
				index = (int)srcVert.index[j];
				curItor = indexMap.find( index );
				if(curItor != indexMap.end())
				{
					curVert.index[j] = (float)indexMap[ index ];
				}
				else
				{
					indexMap.insert( std::make_pair( index, curIndex ) );
					curVert.index[j] = (float)curIndex++;
				}
			}
		}
	}
	else
	{
		weightCount = (int)m_weightVec.size();
		for( int i = 0; i < weightCount; ++i )
		{
			IW4_VERTEX& srcVert = m_srcWeightVec[i];
			IW4_VERTEX& curVert = m_weightVec[i];
			for(int j = 0; j < (int)srcVert.weight[3]; ++j)
			{
				index = (int)srcVert.index[j];
				curItor = indexMap.find( index );
				if( curItor != indexMap.end() )
				{
					curVert.index[j] = (float)indexMap[ index ];
				}
				else
				{
					indexMap.insert( std::make_pair( index, curIndex ) );
					curVert.index[j] = (float)curIndex++;
				}
			}
		}
	}

	// 벡터에 행렬 포인터 재정렬
	KeepIndexItor curIndexItor;
	m_optMatrixCount = indexMap.size();
	
	if( !m_optMatTransRow0 )
		m_optMatTransRow0 = new NVECTOR4[ m_optMatrixCount ];
	if( !m_optMatTransRow1 )
		m_optMatTransRow1 = new NVECTOR4[ m_optMatrixCount ];
	if( !m_optMatTransRow2 )
		m_optMatTransRow2 = new NVECTOR4[ m_optMatrixCount ];
	
	m_optMatrixRefVec.resize( m_optMatrixCount );
	for( curIndexItor = indexMap.begin();
		curIndexItor != indexMap.end(); ++curIndexItor )
	{
		m_optMatrixRefVec[ curIndexItor->second ] = curIndexItor->first;
	}
}

bool NSkinMesh::SetPNCTBuffer()
{
	m_vertCount = (int)m_optPosVec.size();
	int bufferSize = m_vertCount * sizeof(P3NCT_VERTEX);

	// 위치, 법선, 컬러, 텍스쳐 좌표 정점 버퍼 생성
	if( FAILED( m_pDevice->CreateVertexBuffer( bufferSize, &m_PNCT_VB ) ) )
	{
		MessageBox( NULL, _T("Failed to create vertex buffer"), 
			_T("NSkinMesh - SetPosNormTexBuffer()"), MB_OK );
		return false;
	}
	
	
	// 락
	P3NCT_VERTEX* pVert;
	m_PNCT_VB->Lock(0, 0, (void**)&pVert, 0);
	
	for(int i = 0; i < m_vertCount; ++i)
	{
		pVert[i].Set( *(m_optPosVec[i]), *(m_optNormVec[i]), m_optColVec[i], *(m_optTexVec[i]) );
	}
	
	// 언락
	m_PNCT_VB->Unlock();

	return true;
}

bool NSkinMesh::SetWeightBuffer()
{
	int weightSize;
	if( m_maxWeightCount > 4 )
		weightSize = m_vertCount * sizeof(IW8_VERTEX);
	else
		weightSize = m_vertCount * sizeof(IW4_VERTEX);
	
	// 가중치 정점 버퍼 생성
	if( FAILED( m_pDevice->CreateVertexBuffer( weightSize, &m_Weight_VB ) ) )
	{
		MessageBox( NULL, _T("Failed to create vertex buffer"), 
			_T("NSkinMesh - SetWeightBuffer()"), MB_OK );
		return false;
	}

	// 락
	if( m_maxWeightCount > 4 )
	{
		IW8_VERTEX* pWeight;
		m_Weight_VB->Lock(0, 0, (void**)&pWeight, 0);

		for(int i = 0; i < m_vertCount; ++i)
		{
			pWeight[i] = *(m_optWeight8Vec[i]);
		}
	}
	else
	{
		IW4_VERTEX* pWeight;
		m_Weight_VB->Lock(0, 0, (void**)&pWeight, 0);

		for(int i = 0; i < m_vertCount; ++i)
		{
			pWeight[i] = *(m_optWeightVec[i]);
		}
	}
	
	// 언락
	m_Weight_VB->Unlock();

	return true;
}

bool NSkinMesh::SetTangentBuffer()
{
	int tanSize = m_vertCount * sizeof(NVECTOR3);
	
	// 접선 정점 버퍼 생성
	if( FAILED( m_pDevice->CreateVertexBuffer( tanSize, &m_Tan_VB ) ) )
	{
		MessageBox( NULL, _T("Failed to create vertex buffer"), 
			_T("NSkinMesh - SetTangentBuffer()"), MB_OK );
		return false;
	}

	// 락
	NVECTOR3* pTan;
	m_Tan_VB->Lock(0, 0, (void**)&pTan, 0);

	for(int i = 0; i < m_vertCount; ++i)
	{
		pTan[i] = *(m_optTanVec[i]);
	}
	
	// 언락
	m_Tan_VB->Unlock();

	return true;
}

bool NSkinMesh::SetIndexBuffer()
{
	int indexSize = m_faceCount * sizeof(INDEX3);

	// 인덱스 버퍼 생성
	if( FAILED( m_pDevice->CreateIndexBuffer( indexSize, &m_optIB ) ) )
	{
		MessageBox( NULL, _T("Failed to create vertex buffer"), 
			_T("NSkinMesh - SetTangentBuffer()"), MB_OK );
		return false;
	}

	// 락
	INDEX3* pIndex;
	m_optIB->Lock( 0, 0, (void**)&pIndex, 0 );

	for(int i = 0; i < m_faceCount; ++i)
	{
		pIndex[i] = m_faceVec[i].optIndex;
	}

	// 언락
	m_optIB->Unlock();

	return true;
}

void NSkinMesh::Release()
{
	DeleteBuffer();

	SAFE_STL_VECTOR_WIPE(m_posVec);
	SAFE_STL_VECTOR_WIPE(m_texVec);
	SAFE_STL_VECTOR_WIPE(m_colVec);
	SAFE_STL_VECTOR_WIPE(m_normVec);
	SAFE_STL_VECTOR_WIPE(m_tanVec);
	SAFE_STL_VECTOR_WIPE(m_weightVec);
	SAFE_STL_VECTOR_WIPE(m_weight8Vec);
	SAFE_STL_VECTOR_WIPE(m_faceVec);


	NFaceGroupVec().swap(m_faceGroup);
	if( m_optMatTransRow0 )
	{
		delete m_optMatTransRow0;
		m_optMatTransRow0 = NULL;
	}
	if( m_optMatTransRow1 )
	{
		delete m_optMatTransRow1;
		m_optMatTransRow1 = NULL;
	}
	if( m_optMatTransRow2 )
	{
		delete m_optMatTransRow2;
		m_optMatTransRow2 = NULL;
	}

#ifdef _DEBUG
	SAFE_STL_VECTOR_WIPE(m_normLineVec);
	SAFE_STL_VECTOR_WIPE(m_tanLineVec);
	SAFE_STL_VECTOR_WIPE(m_normLine2Vec);
	SAFE_STL_VECTOR_WIPE(m_tanLine2Vec);
#endif
}

#ifdef _DEBUG

void NSkinMesh::SetNormalLineBuffer()
{
	int curIndex = 0;
	if( m_maxWeightCount > 4 )
	{
		LINE_RENDER2 tmp;
		m_normLine2Vec.resize( m_faceCount * 6 );
		for( int i = 0; i < m_faceCount; ++i )
		{
			for( int j = 0; j < 3; ++j )
			{
				NVECTOR3& pos = m_posVec[ m_faceVec[i].posIndex.v[j] ];
				NVECTOR3& norm = m_normVec[ m_faceVec[i].normIndex.v[j] ];

				tmp.iw = m_weight8Vec[ m_faceVec[i].posIndex.v[j] ];
										
				tmp.pos = pos;
				tmp.norm = norm;
				tmp.col = 0xffff0000;
				tmp.u = 1.0f;
				m_normLine2Vec[ curIndex++ ] = tmp;

				tmp.u = 0.0f;
				tmp.col = 0xff00ff00;
				m_normLine2Vec[ curIndex++ ] = tmp;
			}
		}
	}
	else
	{
		LINE_RENDER tmp;
		m_normLineVec.resize( m_faceCount * 6 );
		for( int i = 0; i < m_faceCount; ++i )
		{
			for( int j = 0; j < 3; ++j )
			{
				NVECTOR3& pos = m_posVec[ m_faceVec[i].posIndex.v[j] ];
				NVECTOR3& norm = m_normVec[ m_faceVec[i].normIndex.v[j] ];

				if( m_skinType == ST_RIGID_OBJECT )
					tmp.iw = m_weightVec[ m_faceVec[i].posIndex.v[j] ];
										
				tmp.pos = pos;
				tmp.norm = norm;
				tmp.col = 0xffff0000;
				tmp.u = 1.0f;
				m_normLineVec[ curIndex++ ] = tmp;
				tmp.u = 0.0f;
				tmp.col = 0xff00ff00;
				m_normLineVec[ curIndex++ ] = tmp;
			}
		}
	}
}

void NSkinMesh::SetTangentLineBuffer()
{
	if(m_tanVec.empty())
		return;

	int curIndex = 0;
	if( m_maxWeightCount > 4 )
	{
		LINE_RENDER2 tmp;
		m_tanLine2Vec.resize( m_faceCount * 6 );
		for( int i = 0; i < m_faceCount; ++i )
		{
			for( int j = 0; j < 3; ++j )
			{
				NVECTOR3& pos = m_posVec[ m_faceVec[i].posIndex.v[j] ];
				NVECTOR3& vTan = m_tanVec[ m_faceVec[i].tanIndex.v[j] ];
	
				tmp.iw = m_weight8Vec[ m_faceVec[i].posIndex.v[j] ];

				tmp.pos = pos;
				tmp.norm = vTan;
				tmp.col = 0xff0000ff;

				tmp.u = 1.0f;
				m_tanLine2Vec[ curIndex++ ] = tmp;
				tmp.col = 0xff00ffff;
				tmp.u = 0.0f;
				m_tanLine2Vec[ curIndex++ ] = tmp;
			}
		}
	}
	else
	{
		LINE_RENDER tmp;
		m_tanLineVec.resize( m_faceCount * 6 );
		for( int i = 0; i < m_faceCount; ++i )
		{
			for( int j = 0; j < 3; ++j )
			{
				NVECTOR3& pos = m_posVec[ m_faceVec[i].posIndex.v[j] ];
				NVECTOR3& vTan = m_tanVec[ m_faceVec[i].tanIndex.v[j] ];
				if(m_skinType == ST_RIGID_OBJECT)
					tmp.iw = m_weightVec[ m_faceVec[i].posIndex.v[j] ];

				tmp.pos = pos;
				tmp.norm = vTan;
				tmp.col = 0xff0000ff;

				tmp.u = 1.0f;
				m_tanLineVec[ curIndex++ ] = tmp;
				tmp.col = 0xff00ffff;
				tmp.u = 0.0f;
				m_tanLineVec[ curIndex++ ] = tmp;
			}
		}
	}
}

#endif

//////////////////////////////////////////////////////////////////////////

NSkinMeshObj::NSkinMeshObj(): 
	m_pAniMeshObj(0), 
	m_pRenderer(0),
	m_highestMatIndex(0),
	m_faceCount(0)
{

}
NSkinMeshObj::~NSkinMeshObj()
{

}

void NSkinMeshObj::Init()
{
	TCHAR* fileName = _T("MeshRender.fx");
	m_pRenderer = Mgr_NRenderer->GetByName_T< NMeshRenderer >( fileName );
}

bool NSkinMeshObj::Load( const TCHAR* fileName )
{
	NNOESkinLoader loader;
	if( !loader.Load( fileName ) )
		return false;

	Release();

	m_sphere	= loader.GetSphere();
	m_bbox		= loader.GetBoundingBox();

	CountingFaceTotal();
	SetNameFromPath(fileName);
	SplitPath(fileName);

	NSkinMeshPVec& loadedMesh = loader.GetMeshObj();
	m_skinMeshVec = loadedMesh;
	m_mtlVec = loader.GetMtlVec();

	OptimizeAllMatrix();
		
	int newMeshSize = m_skinMeshVec.size();
	for( int i = 0; i < newMeshSize; ++i )
	{
		m_skinMeshVec[i]->Init( m_pRenderer );
		m_skinMeshVec[i]->Optimize();
		m_skinMeshVec[i]->OptimizeMatrix();
		m_skinMeshVec[i]->SetRenderBuffer();
	}
	return true;
}

void NSkinMeshObj::CountingFaceTotal()
{
	m_faceCount = 0;

	for(size_t i = 0; i < m_skinMeshVec.size(); ++i)
		m_faceCount += m_skinMeshVec[i]->m_faceCount;
}

void NSkinMeshObj::OptimizeAllMatrix()
{
	// 모든 사용하는 매트릭스 리스트 검색
	KeepIndexMap	tmpMap;
	KeepIndexItor	tmpItor;
	
	int curIndex = 0;
	int weightCount;
	m_highestMatIndex = 0;
	int meshCount = m_skinMeshVec.size();
	for( int i = 0; i < meshCount; ++i )
	{
		NSkinMesh& mesh = *m_skinMeshVec[i];
		if( mesh.m_skinType == ST_DYNAMIC_OBJECT )
		{
			if( m_highestMatIndex < mesh.m_solidIndex )
				m_highestMatIndex = mesh.m_solidIndex;

			tmpItor = tmpMap.find( mesh.m_solidIndex );
			if( tmpItor == tmpMap.end() )
				tmpMap.insert( std::make_pair( mesh.m_solidIndex, curIndex++) );
			continue;
		}
		weightCount = (int)mesh.m_weightVec.size();
		for( int j = 0; j < weightCount; ++j )
		{
			IW4_VERTEX& vert = mesh.m_srcWeightVec[j];

			for( int k = 0; k < (int)vert.weight[3]; ++k )
			{
				if( m_highestMatIndex < vert.index[k] )
					m_highestMatIndex = (int)vert.index[k];

				tmpItor = tmpMap.find( (int)vert.index[k] );
				if( tmpItor == tmpMap.end() )
					tmpMap.insert( std::make_pair( (int)vert.index[k], curIndex++) );
			}
		}
	}

	// 최적화 매트릭스 인덱스 벡터로 재구성
	if(!curIndex)
		return;

	MatrixIndexVec().swap( m_optMatrixIndexVec );
	m_optMatrixIndexVec.resize(curIndex);
	for(tmpItor = tmpMap.begin();
		tmpItor != tmpMap.end(); ++tmpItor)
		m_optMatrixIndexVec[ tmpItor->second ] = tmpItor->first;
}

void NSkinMeshObj::Animation(float elapsed)
{
	if(m_pAniMeshObj)
	{
		m_frame.elapsed = elapsed;
		m_pAniMeshObj->Frame( m_frame, m_optMatrixIndexVec );
	}
}

void NSkinMeshObj::Render(float elapsed)
{
	Animation(elapsed);
	RenderStatic();
}

void NSkinMeshObj::RenderStatic()
{
	m_pRenderer->SetMaterial(&m_mtlVec);

	int meshCount = (int)m_skinMeshVec.size();
	for(int i = 0; i < meshCount; ++i)
		m_skinMeshVec[i]->Render(m_pAniMeshObj);
}

void NSkinMeshObj::RenderSphere(NMATRIX& matWorld)
{
	m_sphere.Render(matWorld);
}

void NSkinMeshObj::RenderBox(NMATRIX& matWorld)
{
	m_bbox.Render(matWorld);
}

void NSkinMeshObj::Remove()
{
	for(size_t i = 0; i < m_skinMeshVec.size(); ++i)
		m_skinMeshVec[i]->DeleteBuffer();
}

bool NSkinMeshObj::Reset()
{
	for(size_t i = 0; i < m_skinMeshVec.size(); ++i)
		m_skinMeshVec[i]->SetRenderBuffer();

	return true;
}

bool NSkinMeshObj::ValidateAniMatrix(NAniMeshObj* aniObj)
{
	if( !aniObj || aniObj->m_meshCount < m_highestMatIndex + 1)
		return false;

	return true;
}

bool NSkinMeshObj::SetAniMatrix( NAniMeshObj* aniObj )
{
	if( !ValidateAniMatrix(aniObj) )
		return false;

	m_pAniMeshObj = aniObj;
	m_frame = aniObj->m_frame;
	m_frame.stopped = false;

	return true;
}

void NSkinMeshObj::DetachAniMatrix()
{
	m_pAniMeshObj = NULL;
}

void NSkinMeshObj::SetNameFromPath( const TCHAR* filePath )
{
	TCHAR tmpStr[MAX_PATH];
	TCHAR tmpName[MAX_PATH];
	
	_tsplitpath_s( filePath, tmpStr, MAX_PATH,
							tmpStr, MAX_PATH,
							tmpName, MAX_PATH,
							tmpStr, MAX_PATH );
	m_name = tmpName;
}

void NSkinMeshObj::SetFrame(int startFrame, int lastFrame, bool bLoop)
{
	m_frame.startFrame = startFrame;
	m_frame.lastFrame = lastFrame;
	m_frame.bLoop = bLoop;
	m_frame.stopped = false;
	m_frame.curFrameF = (float)startFrame;
	m_frame.curFrameI = startFrame;
}

void NSkinMeshObj::SetStartFrame(int startFrame)
{
	m_frame.startFrame = startFrame;
}

void NSkinMeshObj::SetEndFrame(int endFrame)
{
	m_frame.lastFrame = endFrame;
}

void NSkinMeshObj::SetCurFrame(int curFrame)
{
	m_frame.curFrameI = curFrame;
	m_frame.curFrameF = float(curFrame);
	m_frame.delta = 0.0f;
	bool oldLoop = m_frame.bLoop;
	m_frame.bLoop = false;

	Animation(0.0f);

	m_frame.bLoop = oldLoop;
}

void NSkinMeshObj::DeltaFrame(int delta)
{
	m_frame.curFrameI += delta;

	if( m_frame.curFrameI < m_frame.startFrame )
		m_frame.curFrameI = m_frame.startFrame;
	else if( m_frame.curFrameI > m_frame.lastFrame )
		m_frame.curFrameI = m_frame.lastFrame;

	SetCurFrame( m_frame.curFrameI );
}

void NSkinMeshObj::SetFrameReset()
{
	SetCurFrame( m_frame.startFrame );
}

void NSkinMeshObj::SetLoop(bool bLoop)
{
	m_frame.bLoop = bLoop;
	m_frame.stopped = false;
}

void NSkinMeshObj::SetSpeed(float speed)
{
	m_frame.controlSpeed = speed;
}

void NSkinMeshObj::SetTopFrame(int startFrame, int lastFrame, bool bLoop)
{
	m_frameTop.startFrame = startFrame;
	m_frameTop.lastFrame = lastFrame;
	m_frameTop.bLoop = bLoop;
	m_frameTop.stopped = false;
	m_frameTop.curFrameF = (float)startFrame;
	m_frameTop.curFrameI = startFrame;
}

void NSkinMeshObj::SetBottomFrame(int startFrame, int lastFrame, bool bLoop)
{
	m_frameBottom.startFrame	= startFrame;
	m_frameBottom.lastFrame		= lastFrame;
	m_frameBottom.bLoop			= bLoop;
	m_frameBottom.stopped		= false;
	m_frameBottom.curFrameF		= (float)startFrame;
	m_frameBottom.curFrameI		= startFrame;
}

void NSkinMeshObj::Release()
{
	SAFE_STL_VECTOR_DELETE_AFTER_RELEASE( m_skinMeshVec );
}