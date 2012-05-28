#pragma once

#include "NL_Base.h"
#include "NSkinMesh.h"

static const int NOE_VERSION = 200;

//====================================
//	Name	: NNOESkinLoader
//	Content	: NOE 파일 로더 클래스
//====================================
class  NNOESkinLoader
{
	//==================
	//	Private Data
	//==================
	typedef std::vector<TCHAR*>	TokenVec;
	TokenVec	m_token;
	int			m_maxTok;
	int			m_curTok;
	TCHAR*		m_fileData;
	FILE*		m_fp;

	TCHAR		m_tmpStr[MAX_PATH];
	int			m_tmpInt;
	float		m_tmpFloat;
	NVECTOR3	m_tmpVector;

	NMtlVec			m_mtlVec;
	NSkinMeshPVec	m_skinMesh;
	NBoundingBox	m_bbox;
	NSphere			m_sphere;

public:
	//==================
	//	Ctor & Dtor
	//==================
	NNOESkinLoader();
	~NNOESkinLoader();

	//==================
	//	Public Method
	//==================
	bool Load(const TCHAR* fileName);
	NSkinMeshPVec&	GetMeshObj()
	{
		return m_skinMesh;
	}
	NMtlVec& GetMtlVec()
	{
		return m_mtlVec;
	}
	NSphere& GetSphere()
	{
		return m_sphere;
	}
	NBoundingBox& GetBoundingBox()
	{
		return m_bbox;
	}

private:
	//==================
	//	Private Method
	//==================
	bool Validate();
	bool Tokenize(const TCHAR* fileName);
	bool LoadMaterial();
	bool LoadTexmap(NMaterial& mtl);
	bool LoadObject();
	bool LoadObjectHeader(NSkinMesh& mesh);
	bool LoadBoundingBox(NSkinMesh& mesh);
	bool LoadFace(NSkinMesh& mesh);
	bool LoadFaceGroup(NSkinMesh& mesh);
	bool LoadPosition(NSkinMesh& mesh);
	bool LoadTexcoord(NSkinMesh& mesh);
	bool LoadColor(NSkinMesh& mesh);
	bool LoadNormal(NSkinMesh& mesh);
	bool LoadTangent(NSkinMesh& mesh);

	bool FindSection(const TCHAR* section);
	void GetInt(int& data);
	void GetIntInt(int& num1, int& num2);
	void GetIntDword(int& num, DWORD& data);
	void GetIntIndex(int& data, INDEX3& index);
	void GetIntString(int& num, TCHAR* str);
	void GetIntVector2(int& num, NVECTOR2& vec);
	void GetIntVector(int& num, NVECTOR3& vec);
	void GetVectorVector(NVECTOR3& vec1, NVECTOR3& vec2);
	void GetVectorFloat(NVECTOR3& vec, float& val);
	void GetFloatFloat(float* num1, float* num2);
	
	void ErrorMsgBox(const TCHAR* errStr);
};