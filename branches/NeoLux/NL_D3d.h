#pragma once

#include "NL_Base.h"
#include "NL_Math.h"

//===============================================================
//  NAME	  : ND3d
//  COMMENTS  : 디바이스 클래스 선언
//===============================================================
class ND3d
{
	//============================
	//	ND3d Data
	//============================
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	D3DPRESENT_PARAMETERS	m_presentParam;
	D3DSURFACE_DESC			m_backBuffer;
	HWND					m_hWnd;
	D3DVIEWPORT9			m_viewPort;
	
	//============================
	//	Constructor & Destructor
	//============================
public:
	ND3d(void);
	~ND3d(void);

	//============================
	//	Function
	//============================
	HRESULT	CreateDevice(	LPDIRECT3D9		pD3d,
							HWND			hWnd,
							BOOL			bWindowed			= TRUE, 
							UINT			presentInterval		= D3DPRESENT_INTERVAL_IMMEDIATE,
							D3DFORMAT		backBufferFormat	= D3DFMT_UNKNOWN,
							BOOL			bAutoDepthStencil	= TRUE,
							D3DFORMAT		depthStencilFormat	= D3DFMT_D24S8,
							D3DSWAPEFFECT	swapEffect			= D3DSWAPEFFECT_DISCARD	);
	HRESULT Reset(LPDIRECT3D9 pD3d);
	HRESULT Release(void);

	HRESULT CreateLine( LPD3DXLINE* ppLine );
	HRESULT CreateEffectFromFile( const TCHAR* filePath, LPD3DXEFFECT* ppEffect );
	HRESULT CreateVertexDeclaration( const D3DVERTEXELEMENT9* pElement, LPDIRECT3DVERTEXDECLARATION9* ppDecl);
	HRESULT	CreateVertexBuffer( UINT					 Length,
								IDirect3DVertexBuffer9** ppVertexBuffer,
								DWORD					 fvf			= 0,
								D3DPOOL					 Pool			= D3DPOOL_DEFAULT,
								DWORD					 Usage			= 0,
								HANDLE*					 pSharedHandle	= NULL );
	HRESULT	CreateIndexBuffer(	UINT					 Length,
								IDirect3DIndexBuffer9**	 ppIndexBuffer,
								D3DPOOL					 Pool			= D3DPOOL_DEFAULT,
								D3DFORMAT				 Format			= D3DFMT_INDEX32,
								DWORD					 Usage			= 0,
								HANDLE*					 pSharedHandle	= NULL );
	HRESULT	CreateTexture(	UINT				Width,
							UINT				Height,
							IDirect3DTexture9** ppTexture,
							D3DFORMAT			Format			= D3DFMT_A8R8G8B8,
							D3DPOOL				Pool			= D3DPOOL_DEFAULT,
							UINT				Levels			= 1,
							DWORD				Usage			= 0,
							HANDLE*				pSharedHandle	= NULL );
	HRESULT CreateTextureFromFile( const TCHAR* filePath, LPDIRECT3DTEXTURE9* ppTex );
	HRESULT	CreateDepthStencilSurface(	UINT				Width,
										UINT				Height,
										IDirect3DSurface9** ppSurface,
										D3DFORMAT			Format				= D3DFMT_D24S8,
										D3DMULTISAMPLE_TYPE	MultiSample			= D3DMULTISAMPLE_NONE,
										DWORD				MultisampleQuality	= 0,
										BOOL				Discard				= FALSE,
										HANDLE*				pSharedHandle		= NULL );
	HRESULT	BeginScene();
	HRESULT	Clear(	DWORD			Flags,
					D3DCOLOR		Color,
					DWORD			Count	= 0,
					const D3DRECT*	pRects	= NULL,
					float			z		= 1.0f,
					DWORD			Stencil = 0 );
	HRESULT	Draw(	UINT				PrimitiveCount,
					UINT				StartVertex		= 0,
					D3DPRIMITIVETYPE	PrimitiveType	= D3DPT_TRIANGLELIST );
	HRESULT	DrawUP(	const void*			pVertexStreamZeroData,
					UINT				VertexStreamZeroStride,
					UINT				PrimitiveCount,
					D3DPRIMITIVETYPE	PrimitiveType	= D3DPT_TRIANGLELIST	);
	HRESULT	DrawIndexed(UINT				NumVertices,
						UINT				PrimitiveCount,
						UINT				StartIndex		= 0,
						INT					BaseVertexIndex	= 0,
						UINT				MinIndex		= 0,
						D3DPRIMITIVETYPE	PrimitiveType	= D3DPT_TRIANGLELIST	);
	HRESULT	DrawIndexedUP(	const void*			pIndexData,
							const void*			pVertexStreamZeroData,
							UINT				VertexStreamZeroStride,
							UINT				NumVertices,
							UINT				PrimitiveCount,
							UINT				MinVertexIndex	= 0,
							D3DFORMAT			IndexDataFormat = D3DFMT_INDEX32,
							D3DPRIMITIVETYPE	PrimitiveType	= D3DPT_TRIANGLELIST	);
	HRESULT	Present(const RECT*		pSourceRect			= 0,
					const RECT*		pDestRect			= 0,
					HWND			hDestWindowOverride = 0,
					const RGNDATA*	pDirtyRegion		= 0);
	HRESULT	EndScene();
									
	//============================
	//	Getter
	//============================
	LPDIRECT3D9						GetD3D(void)			const;
	LPDIRECT3DDEVICE9				GetDevice(void)			const;
	const D3DPRESENT_PARAMETERS&	GetPresentParam(void)	const;
	const D3DSURFACE_DESC&			GetDesc(void)			const;
	const D3DVIEWPORT9&				GetViewport(void)		const;

	HRESULT	WorldMatrix( NMATRIX* pMatWorld );
	HRESULT	ViewMatrix( NMATRIX* pMatView );
	HRESULT	ProjectionMatrix( NMATRIX* pMatProjection );
	HRESULT	ViewPort( D3DVIEWPORT9* pViewPort );
	HRESULT	TextureMatrix( NMATRIX* pMatTexture, UINT Stage = 0 );
	HRESULT	Texture( IDirect3DBaseTexture9** ppTexture, DWORD Stage = 0 );
	HRESULT	RenderState( D3DRENDERSTATETYPE State, DWORD* pValue );
	HRESULT	SamplerState( D3DSAMPLERSTATETYPE Type, DWORD* pValue, DWORD Sampler = 0 );
	HRESULT	TextureStageState( D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue, DWORD Stage = 0 );
	HRESULT	FVF( DWORD* pfvf );
	HRESULT	StreamSource(IDirect3DVertexBuffer9** ppData, UINT* pStride, UINT* OffsetInBytes, UINT Number = 0 );
	HRESULT Indices( IDirect3DIndexBuffer9** ppIndexData );
	HRESULT	RenderTarget( IDirect3DSurface9** ppRenderTarget, DWORD RenderTargetIndex = 0 );
	HRESULT DepthStencilSurface( IDirect3DSurface9** ppZStencilSurface );
	HRESULT	VertexDeclaration( IDirect3DVertexDeclaration9* pDecl );

	//============================
	//	Setter
	//============================
	void	SetPresentParam( const D3DPRESENT_PARAMETERS& presentParam );

	HRESULT	WorldMatrix( const NMATRIX& matWorld );
	HRESULT	ViewMatrix( const NMATRIX& matView );
	HRESULT	ProjectionMatrix( const NMATRIX& matProjection );
	HRESULT	ViewPort( const D3DVIEWPORT9& viewPort );
	HRESULT	TextureMatrix( const NMATRIX& matTexture, UINT Stage = 0 );
	HRESULT	Texture( IDirect3DBaseTexture9* pTexture, DWORD Stage = 0 );
	HRESULT	RenderState( D3DRENDERSTATETYPE State, DWORD value );
	HRESULT	SamplerState( D3DSAMPLERSTATETYPE Type, DWORD Value, DWORD Sampler = 0 );
	HRESULT	TextureStageState( D3DTEXTURESTAGESTATETYPE Type, DWORD Value, DWORD Stage = 0 );
	HRESULT	FVF( DWORD fvf );
	HRESULT	StreamSource(IDirect3DVertexBuffer9* pData, UINT Stride, UINT OffsetInBytes = 0, UINT Number = 0 );
	HRESULT Indices( IDirect3DIndexBuffer9* pIndexData );
	HRESULT	RenderTarget( IDirect3DSurface9* pRenderTarget, DWORD RenderTargetIndex = 0 );
	HRESULT DepthStencilSurface( IDirect3DSurface9* pNewZStencil );
	HRESULT	VertexDeclaration( IDirect3DVertexDeclaration9** ppDecl );

};

#include "NL_D3d.inl"