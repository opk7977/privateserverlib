#pragma once

#include "NL_NeoLux.h"
#include "NFPSCamera.h"
#include "NFPSObject.h"
#include "NTerrain.h"

// Edit Type
enum EDIT_TYPE
{
	ET_TERRAIN,
	ET_TEXTURE,
	ET_OBJECT,
	ET_TESTPLAY,
	ET_OPTION
};

// Edit Terrain
enum EDIT_TERRAIN
{
	RAISE_FLAT,
	RAISE_CURVE,
	PICK_HEIGHT,
	LOWER_FLAT,
	LOWER_CURVE,
	ZERO_HEIGHT,

	EDIT_TERRAIN_MAX
};

// Edit Texture
enum EDIT_TEXTURE
{
	DRAW_LAYER,
	REMOVE_SELECTED_LAYER,
	REMOVE_ALL_LAYER
};

// Edit Circle
enum CIRCLE_SIZE
{
	CS_LARGE,
	CS_SMALL,

	CIRCLE_MAX
};

//=========================================================================
//	Name   : NMapToolCore
//	Content: 맵툴 코어 클래스
//=========================================================================
class NMapToolCore : public NeoLux
{
	//==================================
	//	Private Data
	//==================================
	NTerrain	m_map;
	NFPSCamera*	m_playCamera;
	NFPSObject	m_player;
	bool		m_bLock;
	bool		m_bRenderBox;
	bool		m_bRenderFrustumCull;

	float*		m_cosTable;
	float*		m_sinTable;
	int			m_circleDetail;
	P3C_VERTEX*	m_circle[CIRCLE_MAX];
	float		m_radius[CIRCLE_MAX];
	DWORD		m_color[CIRCLE_MAX];
	typedef std::vector<NTexture*> NTexVec;
	NTexVec		m_texVec;
	NLight		m_light;
	BOOL		m_bShowCursor;
	RECT		m_oldClipRt;
	RECT		m_curRt;

	// Edit Type
	int			m_editType;
	bool		m_editNeeded;

	// Edit Terrain
	int			m_editTerrainMode;
	float		m_editDelta;

	// Edit Texture
	int			m_editTextureMode;
	int			m_layer;
	int			m_texIndex;
	bool		m_showLayer[4];

	// Edit Object
	bool					m_bRenderObjectSphere;
	bool					m_bRenderObjectBox;
	std::vector<NObject>	m_objVec;
	bool					m_bObjSelected;
	NCallback				m_objectDeselectCall;
	NCallback				m_objectInfoUpdate;
	int						m_curSelObjIndex;
	NObject					m_curSelObj;

	bool					m_bPickMapObject;
	QUAD_PICKED_OBJECT		m_pickedMapObj;
	float					m_curObjRotX;
	float					m_curObjRotY;
	float					m_curObjRotZ;
	float					m_curObjScale;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NMapToolCore();
	~NMapToolCore();

	bool InitTool( HWND hWnd );
	bool IsInit() const	{	return this->m_hWnd != NULL;	}
	void ResetSize( int cx, int cy );
	void OnLostDevice();
	void OnResetDevice();

	//==================================
	//	External Function
	//==================================
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	//==================================
	//	Tool Function
	//==================================
	int	GetBaseTexCount() const		{	return (int)m_texVec.size();		}
	NTexture* GetBaseTex(int index)	{	return m_texVec[ index ];			}

	void CreateMap(int patchWidth, int cellWidth, float cellSize, float lodRatio, int baseTexIndex);
	void SetLock(bool bLock)	{	m_bLock = bLock;		}
	bool SaveTerrain(const TCHAR* fileName);
	bool LoadTerrain(const TCHAR* fileName);

	//==================================
	//	Edit Type
	//==================================
	void SetEditType(int type)	
	{	
		m_editType = type;
		if(m_editType == ET_TESTPLAY)	
		{	
			if(!m_player.IsInit())
				InitTestPlay();
			EnableTest();
		}
		else
		{
			EnableTest(false);
			if(m_editType == ET_OPTION)
				m_editType = ET_TERRAIN;
			else if(m_editType == ET_OBJECT)
			{
				if( m_map.IsInit() )
				{
					m_map.RestructQuadBox();
					ResetSelObject();
				}
			}
		}
	}
	int  GetEditType() const	{	return m_editType;		}
	
	//==================================
	//	Edit Circle Function
	//==================================
	void SetCircleRadius( int index, float radius )	
	{	
		m_radius[index] = radius;
		m_map.CreateCircleOnPickOptimize(
			m_circle[index], m_circleDetail, m_radius[index],
			m_cosTable, m_sinTable, m_color[index] );
	}
	void SetCircleColor( int index, DWORD color )	
	{	
		m_color[index] = color;
		m_map.CreateCircleOnPickOptimize(
			m_circle[index], m_circleDetail, m_radius[index],
			m_cosTable, m_sinTable, m_color[index] );
	}

	//==================================
	//	Edit Terrain Function
	//==================================
	void SetEditTerrainMode(int mode)		{	m_editTerrainMode	= mode;		}
	void SetEditDelta(float delta)			{	m_editDelta			= delta;	}
	void SetUseMinHeight(bool bUse)			{	m_map.SetUseMinHeight(bUse);	}
	void SetUseMaxHeight(bool bUse)			{	m_map.SetUseMaxHeight(bUse);	}
	void SetMinHeight(float height)			{	m_map.SetMinHeight(height);		}
	void SetMaxHeight(float height)			{	m_map.SetMaxHeight(height);		}
	void SetLODRatio(float ratio)			{	m_map.SetLODRatio(ratio);		}

	int	  GetEditTerrainMode(void)	const	{	return m_editTerrainMode;		}
	float GetEditDelta(void)		const	{	return m_editDelta;				}
	bool  GetUseMinHeight(void)		const	{	return m_map.GetUseMinHeight();	}
	bool  GetUseMaxHeight(void)		const	{	return m_map.GetUseMaxHeight();	}
	float GetMinHeight(void)		const	{	return m_map.GetMinHeight();	}
	float GetMaxHeight(void)		const	{	return m_map.GetMaxHeight();	}
	float GetLODRatio(void)			const	{	return m_map.GetLODRatio();		}

	//==================================
	//	Edit Texture Function
	//==================================
	void SetEditTextureMode(int mode)		{	m_editTextureMode	= mode;		}
	void SetTextureLayer(int layer)			{	m_layer				= layer;	}
	void SetTextureIndex(int index)			{	m_texIndex			= index;	}
	void SetShowLayer(int index, bool bShow){	m_showLayer[index]	= bShow;	}

	int  GetEditTextureMode()	const		{	return m_editTextureMode;		}
	int  GetTextureLayer()		const		{	return m_layer;					}
	int  GetTextureIndex()		const		{	return m_texIndex;				}
	bool GetShowLayer(int index)const		{	return m_showLayer[index];		}

	//==================================
	//  Edit Object Function
	//==================================
	void AssignObjectDeselectCallback(NCallback& call);
	void AssignObjectInfoUpdate(NCallback& call);
	int  AddObject(const TCHAR* fileName);
	void DeleteObject(int index);
	void DeleteObjectAll();
	void SetCurObject(int index);
	void SetRenderObjectSphere(BOOL bRender);
	void SetRenderObjectBox(BOOL bRender);
	void ResetSelObject();
	void UpdateObject(bool pickedObj = true);
	void UpdateObjectEditValue();
	void ProcessKeyForTransformInEditObject(bool pickedObj = true);
	void SetObjRotateX(float val);
	void SetObjRotateY(float val);
	void SetObjRotateZ(float val);
	void SetObjScale(float val);
	
	NObject& GetNObject(int index)	{	return m_objVec[index];			}
	NObject* GetSelObject();
	float	 GetObjectRotX()		{	return m_curObjRotX;			}
	float	 GetObjectRotY()		{	return m_curObjRotY;			}
	float	 GetObjectRotZ()		{	return m_curObjRotZ;			}
	float	 GetObjectScale()		{	return m_curObjScale;			}

	//==================================
	//	Test Play
	//==================================
	void InitTestPlay();
	void EnableTest(bool bPlay = true);
	void TestPlay();
	void CaptureCursor();
	void ReleaseCursor();
	void SetPivot(float x, float y, float z)	{	m_player.SetPivot(NVECTOR3(x, y, z));	}
	void SetCamHeight(float height)				{	m_playCamera->SetBaseHeight(height);	}
	void SetMoveSpeed(float speed)				{	m_playCamera->SetMoveSpeed(speed);		}
	void SetRotateSpeed(float speed)			{	m_playCamera->SetRotateSpeed(speed);	}
	void SetFovYLimitTop(int top)				{	m_playCamera->SetFovYLimitTop(top);		}
	void SetFovYLimitBottom(int bot)			{	m_playCamera->SetFovYLimitBottom(bot);	}

	float GetPivotX()		const	{	return m_player.GetPivotX();				}
	float GetPivotY()		const	{	return m_player.GetPivotY();				}
	float GetPivotZ()		const	{	return m_player.GetPivotZ();				}
	float GetCamHeight()	const	{	return m_playCamera->GetBaseHeight();		}
	float GetMoveSpeed()	const	{	return m_playCamera->GetMoveSpeed();		}
	float GetRotateSpeed()	const	{	return m_playCamera->GetRotateSpeed();		}
	int   GetFovYLimitTop()	const	{	return m_playCamera->GetFovYLimitTop();		}
	int   GetFovYLimitBot()	const	{	return m_playCamera->GetFovYLimitBottom();	}

private:
	//==================================
	//	Internal Function
	//==================================
	bool LoadDefaultTexture();
	void SetRenderBox( bool bRender );
	void AddBaseTexture( const TCHAR* bRender );
	void ProcessKey();
	void DebugFrustumCull();
	void PreCreateCircleTable();
	void Edit();
	void EditTerrain();
	void EditTexture();
	void EditObject();
};