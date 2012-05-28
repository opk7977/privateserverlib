#include "NAniMeshLoader.h"

NNOEMatrixLoader::NNOEMatrixLoader(void) :
m_maxTok(0), m_curTok(0), m_fileData(0), m_fp(0)
{
}

NNOEMatrixLoader::~NNOEMatrixLoader(void)
{
}

bool NNOEMatrixLoader::Load( const TCHAR* fileName )
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

	if( !LoadScene() )			return false;
	if( !LoadAnimateMesh() )	return false;

	if( m_fileData )
	{
		delete [] m_fileData;
		m_fileData = NULL;
	}
	TokenVec().swap( m_token );

	return true;
}

bool NNOEMatrixLoader::Validate()
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
	if( _tcsicmp(m_type, _T("MATRIX")) )
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

bool NNOEMatrixLoader::Tokenize(const TCHAR* fileName)
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
	delete [] wholeFileMb;

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

bool NNOEMatrixLoader::LoadScene()
{
	// Pass Validate Info
	if( !FindSection(_T("SCENE")) )
	{
		ErrorMsgBox(_T("Can't find SCENE section!"));
		return false;
	}

	// SCENE
	++m_curTok;

	// First Frame
	GetInt(m_tmpInt);
	m_scene.firstFrame = (float)m_tmpInt;

	// Last Frame
	GetInt(m_tmpInt);
	m_scene.lastFrame = (float)m_tmpInt;

	// Frame Speed
	GetInt(m_tmpInt);
	m_scene.frameSpeed = (float)m_tmpInt;

	// Ticks Per Frame
	GetInt(m_tmpInt);
	m_scene.ticksPerFrame = (float)m_tmpInt;

	// Inverse Ticks Per Frame
	m_scene.invTicksPerFrame = 1.0f / m_scene.ticksPerFrame;

	// First Tick
	m_scene.firstTick = m_scene.firstFrame * m_scene.ticksPerFrame;

	// Last Tick
	m_scene.lastTick = m_scene.lastFrame * m_scene.ticksPerFrame;

	// Ticks Per Sec
	m_scene.ticksPerSec = m_scene.frameSpeed * m_scene.ticksPerFrame;

	return true;
}

bool NNOEMatrixLoader::LoadAnimateMesh()
{	
	// Mesh section
	--m_curTok;
	if( !FindSection(_T("MESH")) )
	{
		ErrorMsgBox(_T("Can't find MESH section!"));
		return false;
	}
	++m_curTok;
	
	// Number of Node
	GetInt(m_tmpInt);
	if( m_tmpInt <= 0 )
	{
		ErrorMsgBox(_T("MESH is not exist(0)!"));
		return false;
	}
	m_aniMesh.resize( m_tmpInt );

	LoadObjectHeader();

	int meshCount = (int)m_aniMesh.size();
	for(int i = 0; i < meshCount; ++i)
	{
		LoadRenderInfo( *m_aniMesh[i] );
		LoadAnimKey( *m_aniMesh[i] );
	}

	return true;
}

bool NNOEMatrixLoader::LoadObjectHeader()
{
	// Load mesh type & Name
	NAnimateMesh* pNewMesh;
	int type;
	int parent;
	int meshCount = (int)m_aniMesh.size();
	for(int i = 0; i < meshCount; ++i)
	{
		pNewMesh = new NAnimateMesh;
		GetIntIntIntString(m_tmpInt, parent, type, m_tmpStr);
		pNewMesh->m_index = i;
		pNewMesh->m_type = type;
		pNewMesh->m_parent = parent;
		pNewMesh->m_name = m_tmpStr;
		
		m_aniMesh[i] = pNewMesh;
	}

	return true;
}

bool NNOEMatrixLoader::LoadRenderInfo( NAnimateMesh& mesh )
{
	// Pass Subject
	++m_curTok;

	// Type Check
	if(mesh.m_type == OT_GEOM || mesh.m_type == OT_BIPAD)
	{
		// If it has shape, Load vertice.
		//
		// Number of Vertex
		int vertCount;
		GetInt(vertCount);
		if(vertCount <= 0)
		{
			mesh.m_vertCount = 0;
		}
		else
		{
			mesh.m_vertCount = vertCount;
			mesh.m_faceCount = int(vertCount / 3);
			mesh.m_vertData.resize(vertCount);
			for(int i = 0; i < vertCount; ++i)
			{
				P3NC_VERTEX& v = mesh.m_vertData[i];
				GetIntVectorVectorDword(m_tmpInt, v.vp, v.vn, v.vc);
			}
		}
	}
	else
	{
		// If helper Object, Build boundingBox.
		NBoundingBox box;
		GetVectorVector( box.vMin, box.vMax );
		BuildBoundingBox( mesh, box );
	}

	return true;
}

bool NNOEMatrixLoader::LoadAnimKey( NAnimateMesh& mesh )
{
	// Number of AnimKey
	int keyCount;
	GetInt( keyCount );

	if( keyCount <= 0 )
		return true;

	// Load Anim key
	mesh.m_aniData.resize( keyCount );
	for(int i = 0; i < keyCount; ++i)
	{
		NAniData& ani = mesh.m_aniData[i];
		// *Key tick  qx qy qz qw  px py pz
		GetIntMatrix( ani.tick, ani.m );
		D3DXQuaternionRotationMatrix( 
			(D3DXQUATERNION*)&ani.q, ani.m );
		ani.q.Normalize();
		ani.t.x = ani.m._41;
		ani.t.y = ani.m._42;
		ani.t.z = ani.m._43;
	}
		
	return true;
}


void NNOEMatrixLoader::ConvertDualQuaternion(const NQUATERNION& quat, const NVECTOR3& trans)
{
	// input: unit quaternion 'q0', translation vector 't' 
	// output: unit dual quaternion 'dq'
	// non-dual part (just copy q0):
	/*for (int i=0; i<4; i++) dq[0][i] = quat.v[i];*/

	// dual part:
// 	dq[1][0] = -0.5*(t[0]*q0[1] + t[1]*q0[2] + t[2]*q0[3]);
// 	dq[1][1] = 0.5*( t[0]*q0[0] + t[1]*q0[3] - t[2]*q0[2]);
// 	dq[1][2] = 0.5*(-t[0]*q0[3] + t[1]*q0[0] + t[2]*q0[1]);
// 	dq[1][3] = 0.5*( t[0]*q0[2] - t[1]*q0[1] + t[2]*q0[0]);

}

bool NNOEMatrixLoader::BuildBoundingBox( NAnimateMesh& mesh, NBoundingBox& box )
{
	// BoundingBox
	mesh.m_vertCount = 36;
	mesh.m_faceCount = 12;
	mesh.m_vertData.resize( 36 );

	// 앞면
	mesh.m_vertData[ 0].Set(box.vMin.x, box.vMax.y, box.vMin.z, 0.0f, 0.0f, -1.0f, 0xffff4444);
	mesh.m_vertData[ 1].Set(box.vMax.x, box.vMax.y, box.vMin.z, 0.0f, 0.0f, -1.0f, 0xff44ff44);
	mesh.m_vertData[ 2].Set(box.vMin.x, box.vMin.y, box.vMin.z, 0.0f, 0.0f, -1.0f, 0xffffff44);
	mesh.m_vertData[ 3].Set(box.vMin.x, box.vMin.y, box.vMin.z, 0.0f, 0.0f, -1.0f, 0xffff4444);
	mesh.m_vertData[ 4].Set(box.vMax.x, box.vMax.y, box.vMin.z, 0.0f, 0.0f, -1.0f, 0xff44ff44);
	mesh.m_vertData[ 5].Set(box.vMax.x, box.vMin.y, box.vMin.z, 0.0f, 0.0f, -1.0f, 0xffffff44);

	// 뒷면										
	mesh.m_vertData[ 6].Set(box.vMax.x, box.vMax.y, box.vMax.z, 0.0f, 0.0f,  1.0f, 0xffff4444);
	mesh.m_vertData[ 7].Set(box.vMin.x, box.vMax.y, box.vMax.z, 0.0f, 0.0f,  1.0f, 0xff44ff44);
	mesh.m_vertData[ 8].Set(box.vMax.x, box.vMin.y, box.vMax.z, 0.0f, 0.0f,  1.0f, 0xffffff44);
	mesh.m_vertData[ 9].Set(box.vMax.x, box.vMin.y, box.vMax.z, 0.0f, 0.0f,  1.0f, 0xffff4444);
	mesh.m_vertData[10].Set(box.vMin.x, box.vMax.y, box.vMax.z, 0.0f, 0.0f,  1.0f, 0xff44ff44);
	mesh.m_vertData[11].Set(box.vMin.x, box.vMin.y, box.vMax.z, 0.0f, 0.0f,  1.0f, 0xffffff44);

	// 윗면												
	mesh.m_vertData[12].Set(box.vMin.x, box.vMax.y, box.vMax.z, 0.0f,  1.0f, 0.0f, 0xffff4444);
	mesh.m_vertData[13].Set(box.vMax.x, box.vMax.y, box.vMax.z, 0.0f,  1.0f, 0.0f, 0xff44ff44);
	mesh.m_vertData[14].Set(box.vMin.x, box.vMax.y, box.vMin.z, 0.0f,  1.0f, 0.0f, 0xffffff44);
	mesh.m_vertData[15].Set(box.vMin.x, box.vMax.y, box.vMin.z, 0.0f,  1.0f, 0.0f, 0xffff4444);
	mesh.m_vertData[16].Set(box.vMax.x, box.vMax.y, box.vMax.z, 0.0f,  1.0f, 0.0f, 0xff44ff44);
	mesh.m_vertData[17].Set(box.vMax.x, box.vMax.y, box.vMin.z, 0.0f,  1.0f, 0.0f, 0xffffff44);

	// 아랫면											
	mesh.m_vertData[18].Set(box.vMin.x, box.vMin.y, box.vMin.z, 0.0f, -1.0f, 0.0f, 0xffff4444);
	mesh.m_vertData[19].Set(box.vMax.x, box.vMin.y, box.vMin.z, 0.0f, -1.0f, 0.0f, 0xff44ff44);
	mesh.m_vertData[20].Set(box.vMin.x, box.vMin.y, box.vMax.z, 0.0f, -1.0f, 0.0f, 0xffffff44);
	mesh.m_vertData[21].Set(box.vMin.x, box.vMin.y, box.vMax.z, 0.0f, -1.0f, 0.0f, 0xffff4444);
	mesh.m_vertData[22].Set(box.vMax.x, box.vMin.y, box.vMin.z, 0.0f, -1.0f, 0.0f, 0xff44ff44);
	mesh.m_vertData[23].Set(box.vMax.x, box.vMin.y, box.vMax.z, 0.0f, -1.0f, 0.0f, 0xffffff44);

	// 왼쪽면											
	mesh.m_vertData[24].Set(box.vMin.x, box.vMax.y, box.vMax.z, -1.0f, 0.0f, 0.0f, 0xffff4444);
	mesh.m_vertData[25].Set(box.vMin.x, box.vMax.y, box.vMin.z, -1.0f, 0.0f, 0.0f, 0xff44ff44);
	mesh.m_vertData[26].Set(box.vMin.x, box.vMin.y, box.vMax.z, -1.0f, 0.0f, 0.0f, 0xffffff44);
	mesh.m_vertData[27].Set(box.vMin.x, box.vMin.y, box.vMax.z, -1.0f, 0.0f, 0.0f, 0xffff4444);
	mesh.m_vertData[28].Set(box.vMin.x, box.vMax.y, box.vMin.z, -1.0f, 0.0f, 0.0f, 0xff44ff44);
	mesh.m_vertData[29].Set(box.vMin.x, box.vMin.y, box.vMin.z, -1.0f, 0.0f, 0.0f, 0xffffff44);

	// 오른쪽면											
	mesh.m_vertData[30].Set(box.vMax.x, box.vMax.y, box.vMin.z,  1.0f, 0.0f, 0.0f, 0xffff4444);
	mesh.m_vertData[31].Set(box.vMax.x, box.vMax.y, box.vMax.z,  1.0f, 0.0f, 0.0f, 0xff44ff44);
	mesh.m_vertData[32].Set(box.vMax.x, box.vMin.y, box.vMin.z,  1.0f, 0.0f, 0.0f, 0xffffff44);
	mesh.m_vertData[33].Set(box.vMax.x, box.vMin.y, box.vMin.z,  1.0f, 0.0f, 0.0f, 0xffff4444);
	mesh.m_vertData[34].Set(box.vMax.x, box.vMax.y, box.vMax.z,  1.0f, 0.0f, 0.0f, 0xff44ff44);
	mesh.m_vertData[35].Set(box.vMax.x, box.vMin.y, box.vMax.z,  1.0f, 0.0f, 0.0f, 0xffffff44);

	return true;
}

bool NNOEMatrixLoader::FindSection( const TCHAR* section )
{
	while( m_curTok + 1 < m_maxTok )
	{
		_stscanf_s( m_token[ ++m_curTok ], _T("%s"), m_tmpStr, MAX_PATH );
		if( !_tcsicmp( m_tmpStr, section ) )
			return true;
	}

	return false;
}

void NNOEMatrixLoader::GetInt( int& data )
{
	static TCHAR* strFormat = _T("%s %d");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, m_tmpStr, MAX_PATH, 
		&data );
}

void NNOEMatrixLoader::GetVectorVector( NVECTOR3& vec1, NVECTOR3& vec2 )
{
	static TCHAR* strFormat = _T("%s %f %f %f %f %f %f");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, m_tmpStr, MAX_PATH, 
		&vec1.x, &vec1.y, &vec1.z, &vec2.x, &vec2.y, &vec2.z );
}

void NNOEMatrixLoader::GetIntMatrix( int& num, NMATRIX& mat )
{
	static TCHAR* strFormat = _T("%s %d %f %f %f %f %f %f %f %f %f %f %f %f");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, m_tmpStr, MAX_PATH, 
		&num,	&mat._11, &mat._12, &mat._13, &mat._21, &mat._22, &mat._23,
				&mat._31, &mat._32, &mat._33, &mat._41, &mat._42, &mat._43 );
}

void NNOEMatrixLoader::GetIntVectorVectorDword(int& num, NVECTOR3& vec1, NVECTOR3& vec2, DWORD& dwd )
{
	static TCHAR* strFormat = _T("%s %d %f %f %f %f %f %f %x");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, m_tmpStr, MAX_PATH, 
		&num, &vec1.x, &vec1.y, &vec1.z, &vec2.x, &vec2.y, &vec2.z, &dwd );
}

void NNOEMatrixLoader::GetIntIntIntString( int& num1, int& num2, int& num3, TCHAR* str )
{
	static TCHAR* strFormat = _T("%s %d %d %d %s");
	_stscanf_s( m_token[ m_curTok++ ], strFormat, m_tmpStr, MAX_PATH, 
		&num1, &num2, &num3, str, MAX_PATH );
}

void NNOEMatrixLoader::ErrorMsgBox( const TCHAR* errStr )
{
	MessageBox( NULL, errStr, _T("NNOEMatrixLoader"), MB_OK );
}
