#pragma once

#include "NL_Base.h"
#include "NL_Math.h"
#include "NAnimateMesh.h"

static const int NOE_VERSION = 100;

//====================================
//	Name	: NNOEMatrixLoader
//	Content	: NOE 파일 로더 클래스
//====================================
class  NNOEMatrixLoader
{
	//==================
	//	Private Data
	//==================
	enum OBJECT_TYPE
	{
		OT_GEOM,
		OT_BONE,
		OT_DUMMY,
		OT_BIPAD
	};
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
	NQUATERNION	m_tmpQuat;

	NScene			m_scene;
	NAniMeshPVec	m_aniMesh;

public:
	//==================
	//	Ctor & Dtor
	//==================
	NNOEMatrixLoader();
	~NNOEMatrixLoader();

	//==================
	//	Public Method
	//==================
	bool Load(const TCHAR* fileName);
	NAniMeshPVec&	GetMeshObj()
	{
		return m_aniMesh;
	}
	NScene&	GetScene()
	{
		return m_scene;
	}

private:
	//==================
	//	Private Method
	//==================
	bool Validate();
	bool Tokenize(const TCHAR* fileName);
	bool LoadScene();
	bool LoadAnimateMesh();
	bool LoadObjectHeader();
	bool LoadRenderInfo(NAnimateMesh& mesh);
	bool LoadAnimKey(NAnimateMesh& mesh);
	bool BuildBoundingBox(NAnimateMesh& mesh, NBoundingBox& box);
	void ConvertDualQuaternion(const NQUATERNION& quat, const NVECTOR3& trans);

	bool FindSection(const TCHAR* section);
	void GetInt(int& data);
	void GetIntIntIntString(int& num1, int& num2, int& num3, TCHAR* str);
	void GetVectorVector(NVECTOR3& vec1, NVECTOR3& vec2);
	void GetIntVectorVectorDword(int& num, NVECTOR3& vec1, NVECTOR3& vec2, DWORD& dwd);
	void GetIntMatrix(int& num, NMATRIX& mat);
	
	void ErrorMsgBox(const TCHAR* errStr);
};