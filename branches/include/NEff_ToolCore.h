#pragma once

#include "NL_NeoLux.h"
#include "NSprite.h"
#include "NCallback.h"

//=========================================================================
//	Name   : NEff_ToolCore
//	Content: 이펙트 툴 코어 클래스
//=========================================================================
class NEff_ToolCore : public NeoLux
{
	//==================================
	//	Private Data
	//==================================
	typedef std::vector<NSprite*> SPR_VEC;
	typedef SPR_VEC::iterator	  SPR_ITOR;
	
	int			m_spriteCount;
	SPR_VEC		m_sprite;
	SPR_ITOR	m_itor;
	int			m_select;

	NCallback	m_infoUpdate;
	NCallback	m_frameUpdate;
	
public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NEff_ToolCore();
	~NEff_ToolCore();

	//==================================
	//	External Function
	//==================================
	bool Init();
	bool Render();
	bool Frame();
	bool Release();
	
	void Init(HWND hWnd);
	void ToolRun();
	void ToolRelease();

	void RemoveResource();
	void SetSize( int x, int y);
	void ResetResource();

	bool IsInit() const {	return m_hWnd != NULL;	}

	//////////////////////////////////////////////////////////////////////////
	void RegisterInfoUpdateCallback(NCallback& infoUpdate);
	void RegisterFrameUpdateCallback(NCallback& frameUpdate);

	bool AddTexture(const TCHAR* fileName);
	void DeleteSpriteSel();
	void DeleteSpriteAll();
	void CreateSprite();
	bool LoadSprite(const TCHAR* fileName);
	void CreateUVTable(int row, int col, std::vector<int>* customOrder = NULL);
	void SetSelect(int sel);
	void SetFrameRateSel(float rate);
	
	int  GetTextureCount() const {	return m_sprite[ m_select ]->GetTexCount();		}
	const NSprite* GetSelSprite() const	{	return m_sprite[ m_select ];	}
	
private:
	//==================================
	//	Internal Function
	//==================================
	void ReleaseSpriteList();
};