#pragma once

#include "NL_NeoLux.h"
#include "NObject.h"
#include "NL_Callback.h"

typedef std::vector<NAniMeshObj*>	AniMatrixVec;
typedef std::vector<NObject>		ObjVec;
typedef ObjVec::iterator			ObjItor;
typedef std::vector<bool>			RenderSkinVec;

//=========================================================================
//	Name   : NCharacterToolCore
//	Content: 캐릭터 툴 코어 클래스
//=========================================================================
class NCharacterToolCore : public NeoLux
{
	//==================================
	//	Private Data
	//==================================
	ObjVec			m_objVec;
	int				m_objCount;
	int				m_curObjIndex;
	bool			m_bPlay;

	AniMatrixVec	m_aniMatrix;
	NMeshRenderer*	m_pRenderer;
	int				m_aniCount;
	int				m_matrixSelect;
	bool			m_bApplyMatrix;
	bool			m_bRenderBone;
	bool			m_bRenderBox;
	RenderSkinVec	m_renderSkinVec;

	NCallback		m_updateInfo;
	NCallback		m_updateFrame;
	NCallback		m_resetFrame;
	NCallback		m_updateSkin;
	NCallback		m_updateAction;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NCharacterToolCore();
	virtual ~NCharacterToolCore();

	bool InitTool( HWND hWnd );

	//==================================
	//	External Function
	//==================================
	bool Init();
	bool Render();
	bool Frame();
	bool Release();

	bool IsInit() const	{	return this->m_hWnd != NULL;	}
	void ResetSize( int cx, int cy );
	void OnLostDevice();
	void OnResetDevice();

	//==================================
	//	Tool Function
	//==================================

	// Character
	int				CreateCharacter(const TCHAR* characterName);
	void			DeleteCharacter(int index);
	void			SelectCharacter(int index);
	const TCHAR*	GetCharacterName(int index) const;
	bool			SaveCharacter(const TCHAR* fileName);
	int				LoadCharacter(const TCHAR* fileName, int* pAniIndex);

	// Model
	void			AssignUpdateSkinCallback(NCallback& call);
	int				AddSkin(const TCHAR* fileName);
	void			DeleteSkin(int index);
	void			DeleteSkinAll();
	NSkinMeshObj&	GetSkin(int index);
	int				GetSkinCount();
	void			SetRenderSkin(int skinIndex, bool bRender);
	void			SetRenderSkinAll(bool bRender);
	void			SetRenderBox(bool bRender);
	bool			GetRenderBox()	{	return m_bRenderBox;	}

	// Matrix
	int				AddMatrix(const TCHAR* fileName);
	bool			SelectMatrix(int index);
	void			DeleteMatrix(int index);
	void			DeleteMatrixAll();
	NAniMeshObj&	GetMatrix(int index);
	NAniMeshObj*	GetSelMatrix();
	void			SetRenderBone(bool bRender);
	bool			SetApplyMatrix(bool bApply);

	// Frame
	void			AssignUpdateInfoCallback(NCallback& call);
	void			AssignUpdateFrameCallback(NCallback& call);
	void			AssignResetFrameCallback(NCallback& call);
	void			SetFrame( int startFrame, int endFrame );
	void			SetStartFrame( int startFrame, int endFrame );
	void			SetCurFrame( int curFrame );
	void			SetEndFrame( int endFrame );
	void			DeltaFrame( int delta );
	void			SetPlay( bool bPlay = true );
	void			SetReset();
	void			SetLoop(bool bLoop);
	void			SetAniSpeed(float speed);

	// Action
	void			AssignUpdateActionCallback(NCallback& call);
	void			AddAction( const TCHAR* name, int start, int end );
	void			DeleteAction( int index );
	void			DeleteActionAll();
	void			SetAction(int index);
	void			ModifyAction(int index, const TCHAR* name, int start, int end);
	int				GetActionCount();
	NActionFrame&	GetAction(int index);
	
private:
	//==================================
	//	Internal Function
	//==================================
};
