#pragma once

#include "NL_NeoLux.h"
#include "NMeshRenderer.h"
#include "NAnimateMesh.h"
#include "NObj.h"
#include "NMonitor.h"
#include "NCallback.h"

//=========================================================================
//	Name   : NObjToolCore
//	Content: 오브젝트 툴 코어 클래스
//=========================================================================
class NObjToolCore : public NeoLux
{
	//==================================
	//	Private Data
	//==================================
	typedef std::vector<NAniMeshObj> NAniVec;
	NMeshRenderer*	m_pRenderer;
	NAniVec			m_aniMatrix;
	NObj			m_obj;
	NSyncObj		m_sync;

	std::vector<int>	m_skinSelList;
	std::vector<int>::iterator	m_tmpItor;
	int					m_matrixSelect;

	bool			m_bRenderBone;
	bool			m_bUseMatrix;
	int				m_bRenderNormal;

	NCallback		m_updateInfo;
	NCallback		m_updateFrame;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NObjToolCore();
	~NObjToolCore();

	//==================================
	//	External Function
	//==================================
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	void Init(HWND hWnd);
	void ToolRun();
	void SetSize( UINT newSizeX, UINT newSizeY );
	void ToolRelease();
	bool IsInit()	{	return m_hWnd != NULL;	}

	//==================================
	//	Tool Function
	//==================================
	void RegisterUpdateInfoCallback(NCallback& call);
	void RegisterUpdateFrameCallback(NCallback& call);

	int  LoadSkin(const TCHAR* fileName);
	int	 LoadMatrix( const TCHAR* fileName);
	void SetSkinSelectList(std::vector<int>& selList);
	void SetMatrixSelectList(int sel);

	bool SaveCharacter( const TCHAR* fileName );
	bool LoadCharacter( const TCHAR* fileName );

	void DeleteSkin(int index);
	void DeleteSkinAll();
	void DeleteAniMatrixSel();
	void DeleteAniMatrixAll();
	void ToggleBoneRender();
	bool SetUseAniMatrix(bool bUse);

	void SetSelStartFrame(int frame);
	void SetSelEndFrame(int frame);

	NSkinMeshObj&	GetSkin(int index)			{	return m_obj.GetSkin(index);			}
	NAniMeshObj&	GetAniMatrix(int index)		{	return m_aniMatrix[index];				}
	NAniMeshObj&	GetSelAniMatrix()			{	return m_aniMatrix[ m_matrixSelect ];	}
		
private:
	//==================================
	//	Internal Function
	//==================================
	
};