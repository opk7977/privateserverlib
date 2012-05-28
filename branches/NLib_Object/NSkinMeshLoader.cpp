#include "NSkinMeshLoader.h"
#include "NL_Device.h"
#include "NL_Texture.h"

NNOESkinLoader::NNOESkinLoader(void) :
m_maxTok(0), m_curTok(0), m_fileData(0), m_fp(0)
{
}

NNOESkinLoader::~NNOESkinLoader(void)
{
}

bool NNOESkinLoader::Load( const TCHAR* fileName )
{
	m_fp = NULL;
	errno_t ret = _tfopen_s(&m_fp, fileName, _T("rt"));
	if(0 != ret)
	{
		ErrorMsgBox(_T("Can't open file."));
		return false;
	}

	// 유효성 검사
	if( !Validate() )
	{
		fclose(m_fp);
		return false;
	}

	Tokenize(fileName);

	if( !LoadMaterial() )	return false;
	if( !LoadObject() )		return false;

	if( m_fileData )
	{
		delete [] m_fileData;
		m_fileData = NULL;
	}
	TokenVec().swap( m_token );

	return true;
}

bool NNOESkinLoader::Validate()
{
	TCHAR m_type[MAX_PATH];
	_ftscanf_s( m_fp, _T("%s %s"), m_tmpStr, MAX_PATH, m_type, MAX_PATH );

	// 유효성 점검
	if( _tcsicmp(m_tmpStr, _T("*NOBJECT_EXPORTER")) )
	{
		ErrorMsgBox(_T("This file is not NOE format."));
		return false;
	}

	// 종류 점검
	if( _tcsicmp(m_type, _T("MESH")) )
	{
		ErrorMsgBox(_T("Unsupported NOE type."));
		return false;
	}

	// 버전 점검
	_ftscanf_s( m_fp, _T("%s %d"), m_tmpStr, MAX_PATH, &m_tmpInt );
	if( m_tmpInt > NOE_VERSION )
	{
		ErrorMsgBox(_T("Unsupported NOE version."));
		return false;
	}

	fclose(m_fp);
	return true;
}

bool NNOESkinLoader::Tokenize(const TCHAR* fileName)
{
	// Open File
	HANDLE hFile = CreateFile(	fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_READONLY, NULL );
	LARGE_INTEGER fileSize;
	GetFileSizeEx(hFile, &fileSize);

	// Create Temporary buffer
	char* wholeFileMb = NULL;
	wholeFileMb = new char[ fileSize.LowPart ];

	if( !wholeFileMb )
	{
		ErrorMsgBox(_T("Out of Memory"));
		CloseHandle( hFile );
		return false;
	}

	// Read Data
	DWORD dwByteReads;
	if( !ReadFile( hFile, (LPVOID)wholeFileMb, fileSize.LowPart, &dwByteReads, NULL ) )
	{
		ErrorMsgBox(_T("Read Failed"));
		CloseHandle( hFile );
		return false;
	}
	CloseHandle( hFile );

	// Convert Wide Character
	m_fileData = NULL;
	m_fileData = new TCHAR[ fileSize.LowPart ];
	size_t convertedSize = 0;
	if( !mbstowcs_s(&convertedSize, m_fileData, fileSize.LowPart, wholeFileMb, _TRUNCATE) )
	{
		return false;
	}
	if( wholeFileMb )
	{
		delete [] wholeFileMb;
		wholeFileMb = NULL;
	}

	// Tokenize
	TCHAR* seps = _T("*");
	TCHAR* token = NULL;
	TCHAR* context = NULL;
	token = _tcstok_s( m_fileData, seps, &context );

	while( token )
	{
		m_token.push_back( token );
		token = _tcstok_s( NULL, seps, &context );		
	}
	m_curTok = 0;
	m_maxTok = (int)m_token.size();

	return true;
}

bool NNOESkinLoader::LoadMaterial()
{
	// Find "MATERIAL" Section
	if( !FindSection( _T("MATERIAL") ) )
	{
		ErrorMsgBox(_T("Can't find MATERIAL section"));
		return false;
	}
	++m_curTok;

	// Number of Mtls
	int mtlCnt = 0;
	GetInt( mtlCnt );
	if(mtlCnt == 0)
		return true;

	m_mtlVec.resize( mtlCnt );
	for( int i = 0; i < mtlCnt; ++i )
	{
		// Mtl index & name
		GetIntString( m_tmpInt, m_tmpStr );
		m_mtlVec[i].mtlName = m_tmpStr;

		// Class Type
		GetInt( m_mtlVec[i].mtlType );

		// If type is 'Standard'...
		if( m_mtlVec[i].mtlType == MT_STANDARD )
			LoadTexmap( m_mtlVec[i] );
	}

	return true;
}

bool NNOESkinLoader::LoadTexmap(NMaterial& mtl)
{
	// Number of Texmaps
	int texCount;
	GetInt( texCount );
	if( texCount > 0 )
	{
		mtl.texmapCount = texCount;
		mtl.texmap.resize(texCount);
		for( int j = 0; j < texCount; ++j )
		{
			// Bitmap, type, fileName
			GetIntString( m_tmpInt, m_tmpStr );
			mtl.texmap[j].type = m_tmpInt;
			mtl.texmap[j].texture = Mgr_NTexture->GetByName( m_tmpStr );
			if( !mtl.texmap[j].texture && m_tmpInt == TM_DIFFUSE )
				mtl.texmap[j].texture = I_NDevice->GetDefaultTexture();
		}
	}

	return true;
}

bool NNOESkinLoader::LoadObject()
{
	// Find "OBJECT" Section
	--m_curTok;
	if( !FindSection( _T("OBJECT") ) )
	{
		ErrorMsgBox(_T("Can't find OBJECT section"));
		return false;
	}
	++m_curTok;

	// Bounding Sphere
	GetVectorFloat( m_sphere.centerPoint, m_sphere.radius );

	// Bounding Box
	GetVectorVector( m_bbox.vMin, m_bbox.vMax );

	// Number of Object
	int objCnt = 0;
	GetInt( objCnt );

	if(objCnt == 0)
		return true;

	m_skinMesh.resize( objCnt );
	for( int i = 0; i < objCnt; ++i )
	{
		NSkinMesh* curMesh = new NSkinMesh;
		m_skinMesh[i] = curMesh;

		LoadObjectHeader( *curMesh );
		LoadBoundingBox( *curMesh );
		LoadFace( *curMesh );
	}

	return true;
}

bool NNOESkinLoader::LoadObjectHeader(NSkinMesh& mesh)
{
	// Load Name
	GetIntString(m_tmpInt, m_tmpStr);
	
	mesh.m_name = m_tmpStr;

	// Load Type
	GetInt( mesh.m_skinType );
	if( mesh.m_skinType == ST_DYNAMIC_OBJECT )
		GetInt( mesh.m_solidIndex );
	else if( mesh.m_skinType == ST_RIGID_OBJECT )
		GetInt( mesh.m_maxWeightCount );
	
	return true;
}

bool NNOESkinLoader::LoadBoundingBox( NSkinMesh& mesh )
{
	// Load BoundingBox
	NVECTOR3 vMin, vMax;
	GetVectorVector(vMin, vMax);
	mesh.m_bbox.Init( vMin, vMax );
	mesh.m_sphere.Set( vMin, vMax );
	
	return true;
}

bool NNOESkinLoader::LoadFace( NSkinMesh& mesh )
{
	// Number of Face
	GetInt(mesh.m_faceCount);
	
	mesh.m_vertCount = 0;
	if(mesh.m_faceCount == 0)
		return false;

	mesh.m_faceVec.resize(mesh.m_faceCount);
	
	LoadFaceGroup(mesh);
	LoadPosition(mesh);
	LoadTexcoord(mesh);
	LoadColor(mesh);
	LoadNormal(mesh);
	LoadTangent(mesh);
	
	return true;
}

bool NNOESkinLoader::LoadFaceGroup(NSkinMesh& mesh)
{
	// Number of face Group
	GetInt(mesh.m_faceGroupCount);
	if(mesh.m_faceGroupCount == 0)
	{
		ErrorMsgBox(_T("NOE Mesh must have face group."));
		return false;
	}

	// Face Group [ mtlRef, faceCount ]
	int startIndex = 0;
	mesh.m_faceGroup.resize(mesh.m_faceGroupCount);
	for(int i = 0; i < mesh.m_faceGroupCount; ++i)
	{
		NFaceGroup& curGroup = mesh.m_faceGroup[i];
		
		GetIntInt( curGroup.mtlRef, curGroup.faceCount );
		curGroup.startIndex = startIndex;
		startIndex += curGroup.faceCount * 3;

		// Check Use Normalmap
		if(curGroup.mtlRef < 0)
			continue;

		int curTexmapCount = m_mtlVec[curGroup.mtlRef].texmapCount;
		NTexmapVec& curTexmapVec = m_mtlVec[curGroup.mtlRef].texmap;
		for(int j = 0; j < curTexmapCount; ++j)
		{
			if( curTexmapVec[j].type == TM_BUMP && 
				curTexmapVec[j].texture )
			{
				mesh.m_bUseNormalmap = true;
				break;
			}
		}
	}

	return true;
}

bool NNOESkinLoader::LoadPosition( NSkinMesh& mesh )
{
	// Number of Position Vertex
	int posCount;
	GetInt(posCount);

	// Position Vertex List
	if(	mesh.m_skinType == ST_STATIC_OBJECT || 
		mesh.m_skinType == ST_DYNAMIC_OBJECT)
	{
		mesh.m_posVec.resize(posCount);
		for(int i = 0; i < posCount; ++i)
			GetIntVector(m_tmpInt, mesh.m_posVec[i]);
	}
	else
	{
		int numWeight;
		mesh.m_posVec.resize(posCount);
		if( mesh.m_maxWeightCount > 4 )
		{
			mesh.m_weight8Vec.resize(posCount);
			mesh.m_srcWeight8Vec.resize(posCount);
			for(int i = 0; i < posCount; ++i)
			{
				GetIntVector(m_tmpInt, mesh.m_posVec[i]);

				// Number of Weight
				GetInt( numWeight );
				for(int j = 0; j < numWeight; ++j)
				{
					GetFloatFloat(	&mesh.m_weight8Vec[i].index[j],
						&mesh.m_weight8Vec[i].weight[j] );
				}
				mesh.m_weight8Vec[i].weight[7] = (float)numWeight;
			}
			mesh.m_srcWeight8Vec = mesh.m_weight8Vec;
		}
		else
		{
			mesh.m_weightVec.resize(posCount);
			mesh.m_srcWeightVec.resize(posCount);
			for(int i = 0; i < posCount; ++i)
			{
				GetIntVector(m_tmpInt, mesh.m_posVec[i]);

				// Number of Weight
				GetInt( numWeight );
				for(int j = 0; j < numWeight; ++j)
				{
					GetFloatFloat(	&mesh.m_weightVec[i].index[j],
						&mesh.m_weightVec[i].weight[j] );
				}
				mesh.m_weightVec[i].weight[3] = (float)numWeight;
			}
			mesh.m_srcWeightVec = mesh.m_weightVec;
		}
	}
	
	// Position Face List
	for(int i = 0; i < mesh.m_faceCount; ++i)
		GetIntIndex(m_tmpInt, mesh.m_faceVec[i].posIndex);
	
	return true;
}

bool NNOESkinLoader::LoadTexcoord( NSkinMesh& mesh )
{
	// Number of Texcoord Vertex
	int texCount;
	GetInt(texCount);
	if(texCount == 0)
	{
		for(int i = 0; i < mesh.m_faceCount; ++i)
			mesh.m_faceVec[i].texIndex.Set( 0, 0, 0 );
		return false;
	}

	// Texcoord Vertex List
	mesh.m_texVec.resize(texCount);
	for(int i = 0; i < texCount; ++i)
		GetIntVector2(m_tmpInt, mesh.m_texVec[i]);

	// Texcoord Face List
	for(int i = 0; i < mesh.m_faceCount; ++i)
		GetIntIndex(m_tmpInt, mesh.m_faceVec[i].texIndex);

	return true;
}

bool NNOESkinLoader::LoadColor( NSkinMesh& mesh )
{
	// Number of Color Vertex
	int colCount;
	GetInt(colCount);
	if(colCount == 0)
	{
		for(int i = 0; i < mesh.m_faceCount; ++i)
			mesh.m_faceVec[i].colIndex.Set( 0, 0, 0 );
		mesh.m_colVec.push_back( 0xffffffff );
		return true;
	}

	// Color Vertex List
	mesh.m_colVec.resize(colCount);
	for(int i = 0; i < colCount; ++i)
		GetIntDword(m_tmpInt, mesh.m_colVec[i]);

	// Color Face List
	for(int i = 0; i < mesh.m_faceCount; ++i)
		GetIntIndex(m_tmpInt, mesh.m_faceVec[i].colIndex);

	return true;
}

bool NNOESkinLoader::LoadNormal( NSkinMesh& mesh )
{
	// Number of Normal Vertex
	int normCount;
	GetInt(normCount);
	if(normCount == 0)
	{
		for(int i = 0; i < mesh.m_faceCount; ++i)
			mesh.m_faceVec[i].normIndex.Set( 0, 0, 0 );
		return false;
	}

	// Normal Vertex List
	mesh.m_normVec.resize(normCount);
	for(int i = 0; i < normCount; ++i)
		GetIntVector(m_tmpInt, mesh.m_normVec[i]);

	// Normal Face List
	for(int i = 0; i < mesh.m_faceCount; ++i)
		GetIntIndex(m_tmpInt, mesh.m_faceVec[i].normIndex);

	return true;
}

bool NNOESkinLoader::LoadTangent(NSkinMesh& mesh)
{
	// Number of Tangent Vertex
	int tanCount;
	GetInt(tanCount);
	if(tanCount == 0)
	{
		for(int i = 0; i < mesh.m_faceCount; ++i)
			mesh.m_faceVec[i].tanIndex.Set( 0, 0, 0 );
		return false;
	}

	// Tangent Vertex List
	mesh.m_tanVec.resize(tanCount);
	for(int i = 0; i < tanCount; ++i)
		GetIntVector(m_tmpInt, mesh.m_tanVec[i]);

	// Tangent Face List
	for(int i = 0; i < mesh.m_faceCount; ++i)
		GetIntIndex(m_tmpInt, mesh.m_faceVec[i].tanIndex);

	return true;
}

bool NNOESkinLoader::FindSection( const TCHAR* section )
{
	while( m_curTok + 1 < m_maxTok )
	{
		_stscanf_s( m_token[ ++m_curTok ], _T("%s"), m_tmpStr, MAX_PATH );
		if( !_tcsicmp( m_tmpStr, section ) )
			return true;
	}

	return false;
}

void NNOESkinLoader::GetInt( int& data )
{
	static TCHAR* strFormat = _T("%s %d");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, m_tmpStr, MAX_PATH, 
		&data );
}

void NNOESkinLoader::GetIntInt(int& num1, int& num2)
{
	static TCHAR* strFormat = _T("%s %d %d");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, m_tmpStr, MAX_PATH, 
		&num1, &num2 );
}

void NNOESkinLoader::GetIntDword( int& num, DWORD& data )
{
	static TCHAR* strFormat = _T("%s %d %x");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, m_tmpStr, MAX_PATH, 
		&num, &data );
}

void NNOESkinLoader::GetIntIndex( int& data, INDEX3& index )
{
	static TCHAR* strFormat = _T("%s %d %u %u %u");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, m_tmpStr, MAX_PATH, 
		&data, &index._0, &index._1, &index._2 );
}

void NNOESkinLoader::GetIntString( int& num, TCHAR* str )
{
	static TCHAR* strFormat = _T("%s %d %s");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, m_tmpStr, MAX_PATH, 
		&num, str, MAX_PATH );
}

void NNOESkinLoader::GetIntVector2( int& num, NVECTOR2& vec )
{
	static TCHAR* strFormat = _T("%s %d %f %f");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, m_tmpStr, MAX_PATH, 
		&num, &vec.x, &vec.y );
}

void NNOESkinLoader::GetIntVector( int& num, NVECTOR3& vec )
{
	static TCHAR* strFormat = _T("%s %d %f %f %f");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, m_tmpStr, MAX_PATH, 
		&num, &vec.x, &vec.y, &vec.z );
}

void NNOESkinLoader::GetVectorVector( NVECTOR3& vec1, NVECTOR3& vec2 )
{
	static TCHAR* strFormat = _T("%s %f %f %f %f %f %f");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, m_tmpStr, MAX_PATH, 
		&vec1.x, &vec1.y, &vec1.z, &vec2.x, &vec2.y, &vec2.z );
}

void NNOESkinLoader::GetVectorFloat(NVECTOR3& vec, float& val)
{
	static TCHAR* strFormat = _T("%s %f %f %f %f");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, m_tmpStr, MAX_PATH, 
		&vec.x, &vec.y, &vec.z, &val );
}

void NNOESkinLoader::GetFloatFloat(float* num1, float* num2)
{
	static TCHAR* strFormat = _T("%f %f");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, 
		num1, num2 );
}

void NNOESkinLoader::ErrorMsgBox( const TCHAR* errStr )
{
	MessageBox( NULL, errStr, _T("NNOEMatrixLoader"), MB_OK );
}

