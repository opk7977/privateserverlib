#pragma once

#include "NL_Base.h"
#include "NL_D3DFormat.h"
#include <vector>

//==================================
//	UV_Table 구조체
//==================================
struct UV_TABLE
{
	NVECTOR2 uv[4];
};
typedef std::vector<UV_TABLE>	UV_Vec;

enum SPR_TYPE
{
	ST_SINGLE_TEX,
	ST_MULTI_TEX,

	SPR_TYPE_MAX
};

//=========================================================================
//	Name   : NSprite
//	Content: 스프라이트 클래스
//=========================================================================
class NTime;
class NTexture;
class ND3d;
class NEffectRenderer;
class NSprite
{
	//==================================
	//	Private Data
	//==================================
	friend class NEffectRenderer;

	typedef std::vector<NTexture*>					SprTexVec;
	typedef std::vector<LPDIRECT3DVERTEXBUFFER9>	VB_Vec;
	
	int						m_type;
	LPDIRECT3DVERTEXBUFFER9	m_pP3VB;
		
	// For Single Texture
	SprTexVec				m_texVec;
	LPDIRECT3DVERTEXBUFFER9	m_pSingleTexVB;
	
	// For Multi Texture
	NTexture*				m_multiTex;
	UV_Vec					m_uvVec;
	VB_Vec					m_uvBufferVec;

	// Sprite Data
	int						m_startFrame;
	int						m_endFrame;
	int						m_frameCount;
	int						m_texCount;
	NMATRIX					m_matWorld;
	float					m_frameRate;
	float					m_controlSpeed;
	float					m_curFrame;
	NTime*					m_pTime;
	ND3d*					m_pDev;
	NEffectRenderer*		m_pRenderer;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NSprite();
	~NSprite();

	//==================================
	//	External Function
	//==================================
	void Init();
	void Frame();
	void Render();
	void Remove();
	bool Reset();
	void Release();

	bool Load(const TCHAR* fileName);
	void AddTexture(NTexture* pTex);
	bool CreateUVTable(int row, int col, std::vector<int>* customOrder = NULL);
	void SetFrame(int startFrame, int endFrame);
	void SetStartFrame(int startFrame);
	void SetEndFrame(int endFrame);
	void SetFrameRate(float rate);
	bool SetRenderBuffer();

	bool  IsNull()			const	{	return m_texCount == 0;	}
	int   GetTexCount()		const	{	return m_texCount;		}
	int   GetFrameCount()	const	{	return m_frameCount;	}
	int	  GetStartFrame()	const	{	return m_startFrame;	}
	int	  GetEndFrame()		const	{	return m_endFrame;		}
	int   GetType()			const	{	return m_type;			}
	int	  GetCurFrame()		const	{	return (int)m_curFrame;	}
	float GetFrameRate()	const	{	return m_frameRate;		}
	float GetControlSpeed()	const	{	return m_controlSpeed;	}
	
private:
	//==================================
	//	Internal Function
	//==================================
};