#pragma once

//===============================================================
//  NAME	  : ND3d
//  COMMENTS  : 디바이스 클래스 구현
//===============================================================
//============================
//	Constructor & Destructor
//============================
NL_INLINE
ND3d::ND3d(void) :	m_pd3dDevice(0),
					m_hWnd(0)
{
}

NL_INLINE
ND3d::~ND3d(void)
{
}

//============================
//	Function
//============================
NL_INLINE
HRESULT	ND3d::CreateDevice(	LPDIRECT3D9		pD3d,
						    HWND			hWnd,
							BOOL			bWindowed,
							UINT			presentInterval,
							D3DFORMAT		backBufferFormat,
							BOOL			bAutoDepthStencil,
							D3DFORMAT		depthStencilFormat,
							D3DSWAPEFFECT	swapEffect			)
{
	Release();

	m_hWnd = hWnd;
	ZeroMemory( &m_presentParam, sizeof( D3DPRESENT_PARAMETERS ) );
	m_presentParam.Windowed					= bWindowed;
	m_presentParam.PresentationInterval		= presentInterval;
	m_presentParam.BackBufferFormat			= backBufferFormat;
	m_presentParam.EnableAutoDepthStencil	= bAutoDepthStencil;
	m_presentParam.AutoDepthStencilFormat	= depthStencilFormat;
	m_presentParam.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	D3DPRESENT_PARAMETERS tmpParam = m_presentParam;

	HRESULT hr;
	if( FAILED( hr = pD3d->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
											D3DCREATE_HARDWARE_VERTEXPROCESSING,
											&tmpParam, &m_pd3dDevice	) ) )
	{
		Release();
		return hr;
	}

	LPDIRECT3DSURFACE9 pBackBuffer = NULL;
	m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	pBackBuffer->GetDesc( &m_backBuffer );
	pBackBuffer->Release();

	ViewPort(&m_viewPort);

	RenderState( D3DRS_ZENABLE, TRUE );
	RenderState( D3DRS_LIGHTING, (DWORD)FALSE );

	return hr;
}



NL_INLINE
HRESULT ND3d::CreateLine( LPD3DXLINE* ppLine )
{
	return D3DXCreateLine(m_pd3dDevice, ppLine);
}

NL_INLINE
HRESULT ND3d::CreateEffectFromFile( const TCHAR* filePath, LPD3DXEFFECT* ppEffect )
{
	LPD3DXBUFFER pErr;
	HRESULT hr;
	if( FAILED( hr = D3DXCreateEffectFromFileW( m_pd3dDevice, filePath, 
		NULL, NULL, 0, NULL, ppEffect, &pErr ) ) )
	{
		if(!pErr)
		{
			CString buffer = _T("Invalid .fx file - [ ");
			buffer += filePath;
			buffer += _T(" ]");
			MessageBox( NULL, buffer.GetString(), _T("CreateEffectFromFile() Error"), MB_OK );
		}
		else
		{
			size_t chgSize = 0;
			TCHAR buffer[4096] = {0};
			mbstowcs_s( &chgSize, buffer, pErr->GetBufferSize()+1, (const char*)pErr->GetBufferPointer(), _TRUNCATE );
			MessageBox( NULL, buffer, _T("CreateEffectFromFile() Error"), MB_OK );
			pErr->Release();
		}
	}

	return hr;
}

NL_INLINE
HRESULT ND3d::CreateVertexDeclaration( const D3DVERTEXELEMENT9* pElement, LPDIRECT3DVERTEXDECLARATION9* ppDecl)
{
	return m_pd3dDevice->CreateVertexDeclaration( pElement, ppDecl );
}

NL_INLINE
HRESULT ND3d::CreateVertexBuffer(	UINT						Length,
									IDirect3DVertexBuffer9**	ppVertexBuffer,
									DWORD						fvf,
									D3DPOOL						Pool,
									DWORD						Usage, 
									HANDLE*						pSharedHandle )
{
	return m_pd3dDevice->CreateVertexBuffer(Length, Usage, fvf, Pool, 
											ppVertexBuffer, pSharedHandle );
}

NL_INLINE
HRESULT ND3d::CreateIndexBuffer(UINT					Length, 
								IDirect3DIndexBuffer9**	ppIndexBuffer, 
								D3DPOOL					Pool, 
								D3DFORMAT				Format, 
								DWORD					Usage, 
								HANDLE*					pSharedHandle )
{
	return m_pd3dDevice->CreateIndexBuffer( Length, Usage, Format,
											Pool, ppIndexBuffer, pSharedHandle );
}

NL_INLINE
HRESULT ND3d::CreateTexture(UINT				Width,
							UINT				Height,
							IDirect3DTexture9** ppTexture, 
							D3DFORMAT			Format,
							D3DPOOL				Pool,
							UINT				Levels,
							DWORD				Usage,
							HANDLE*				pSharedHandle)
{
	return m_pd3dDevice->CreateTexture( Width, Height, Levels, Usage, Format, 
										Pool, ppTexture, pSharedHandle );
}

NL_INLINE
HRESULT ND3d::CreateTextureFromFile( const TCHAR* filePath, LPDIRECT3DTEXTURE9* ppTex )
{
	return D3DXCreateTextureFromFile( m_pd3dDevice, filePath, ppTex );
}

NL_INLINE
HRESULT ND3d::CreateDepthStencilSurface(UINT				Width, 
										UINT				Height, 
										IDirect3DSurface9** ppSurface, 
										D3DFORMAT			Format,
										D3DMULTISAMPLE_TYPE	MultiSample,
										DWORD				MultisampleQuality,
										BOOL				Discard,
										HANDLE*				pSharedHandle		)
{
	return m_pd3dDevice->CreateDepthStencilSurface( Width, Height, Format, MultiSample, 
													MultisampleQuality,	Discard, 
													ppSurface, pSharedHandle );
}

NL_INLINE
HRESULT ND3d::BeginScene()
{
	return m_pd3dDevice->BeginScene();
}

NL_INLINE
HRESULT ND3d::Clear(DWORD			Flags, 
					D3DCOLOR		Color, 
					DWORD			Count, 
					const D3DRECT*	pRects, 
					float			z, 
					DWORD			Stencil)
{
	return m_pd3dDevice->Clear( Count, pRects, Flags, Color, z, Stencil );
}

NL_INLINE
HRESULT ND3d::Draw(UINT				PrimitiveCount, 
				   UINT				StartVertex, 
				   D3DPRIMITIVETYPE	PrimitiveType	)
{
	return m_pd3dDevice->DrawPrimitive( PrimitiveType, StartVertex, PrimitiveCount );
}

NL_INLINE
HRESULT ND3d::DrawUP(const void*		pVertexStreamZeroData, 
					 UINT				VertexStreamZeroStride, 
					 UINT				PrimitiveCount, 
					 D3DPRIMITIVETYPE	PrimitiveType)
{
	return m_pd3dDevice->DrawPrimitiveUP(	PrimitiveType, PrimitiveCount, 
											pVertexStreamZeroData, VertexStreamZeroStride	);
}

NL_INLINE
HRESULT ND3d::DrawIndexed(UINT				NumVertices, 
						  UINT				PrimitiveCount, 
						  UINT				StartIndex, 
						  INT				BaseVertexIndex, 
						  UINT				MinIndex, 
						  D3DPRIMITIVETYPE	PrimitiveType)
{
	return m_pd3dDevice->DrawIndexedPrimitive(	PrimitiveType, BaseVertexIndex, MinIndex,
												NumVertices, StartIndex, PrimitiveCount );
}

NL_INLINE
HRESULT ND3d::DrawIndexedUP(const void*			pIndexData, 
							const void*			pVertexStreamZeroData, 
							UINT				VertexStreamZeroStride, 
							UINT				NumVertices, 
							UINT				PrimitiveCount, 
							UINT				MinVertexIndex, 
							D3DFORMAT			IndexDataFormat,
							D3DPRIMITIVETYPE	PrimitiveType	)
{
	return m_pd3dDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices,
												PrimitiveCount, pIndexData, IndexDataFormat, 
												pVertexStreamZeroData, VertexStreamZeroStride );
}

NL_INLINE
HRESULT ND3d::Present(const RECT*		pSourceRect, 
					  const RECT*		pDestRect, 
					  HWND				hDestWindowOverride, 
					  const RGNDATA*	pDirtyRegion		)
{
	return m_pd3dDevice->Present( pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion );
}

NL_INLINE
HRESULT ND3d::EndScene()
{
	return m_pd3dDevice->EndScene();
}

//============================
//	Getter
//============================
NL_INLINE
LPDIRECT3DDEVICE9 ND3d::GetDevice(void) const
{
	return m_pd3dDevice;
}

NL_INLINE
const D3DPRESENT_PARAMETERS& ND3d::GetPresentParam(void) const
{
	return m_presentParam;
}

NL_INLINE
const D3DSURFACE_DESC&	ND3d::GetDesc(void)	const
{
	return m_backBuffer;
}

NL_INLINE
const D3DVIEWPORT9& ND3d::GetViewport(void) const
{
	return m_viewPort;
}

NL_INLINE
HRESULT ND3d::WorldMatrix(NMATRIX* pMatWorld)
{
	return m_pd3dDevice->GetTransform( D3DTS_WORLD, *pMatWorld );
}

NL_INLINE
HRESULT ND3d::ViewMatrix(NMATRIX* pMatView)
{
	return m_pd3dDevice->GetTransform( D3DTS_VIEW, *pMatView );
}

NL_INLINE
HRESULT ND3d::ProjectionMatrix(NMATRIX* pMatProjection)
{
	return m_pd3dDevice->GetTransform( D3DTS_PROJECTION, *pMatProjection );
}

NL_INLINE
HRESULT ND3d::ViewPort(D3DVIEWPORT9* pViewPort)
{
	return m_pd3dDevice->GetViewport( pViewPort );
}

NL_INLINE
HRESULT ND3d::TextureMatrix(NMATRIX* pMatTexture, UINT Stage)
{
	return m_pd3dDevice->GetTransform(	D3DTRANSFORMSTATETYPE(D3DTS_TEXTURE0 + Stage), 
										*pMatTexture );
}

NL_INLINE
HRESULT ND3d::Texture(IDirect3DBaseTexture9** ppTexture, DWORD Stage)
{
	return m_pd3dDevice->GetTexture( Stage, ppTexture );
}

NL_INLINE
HRESULT ND3d::RenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
{
	return m_pd3dDevice->GetRenderState( State, pValue );
}

NL_INLINE
HRESULT ND3d::SamplerState(D3DSAMPLERSTATETYPE Type, DWORD* pValue, DWORD Sampler)
{
	return m_pd3dDevice->GetSamplerState( Sampler, Type, pValue );
}

NL_INLINE
HRESULT ND3d::TextureStageState(D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue, DWORD Stage)
{
	return m_pd3dDevice->GetTextureStageState( Stage, Type, pValue );
}

NL_INLINE
HRESULT ND3d::FVF(DWORD* pfvf)
{
	return m_pd3dDevice->GetFVF( pfvf );
}

NL_INLINE
HRESULT ND3d::StreamSource(IDirect3DVertexBuffer9** ppData, UINT* pStride, UINT* pOffsetInBytes, UINT Number)
{
	return m_pd3dDevice->GetStreamSource( Number, ppData, pOffsetInBytes, pStride );
}

NL_INLINE
HRESULT ND3d::Indices(IDirect3DIndexBuffer9** ppIndexData)
{
	return m_pd3dDevice->GetIndices( ppIndexData );
}

NL_INLINE
HRESULT ND3d::RenderTarget(IDirect3DSurface9** ppRenderTarget, DWORD RenderTargetIndex)
{
	return m_pd3dDevice->GetRenderTarget( RenderTargetIndex, ppRenderTarget );
}

NL_INLINE
HRESULT ND3d::DepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
	return m_pd3dDevice->GetDepthStencilSurface( ppZStencilSurface );
}

NL_INLINE
HRESULT	ND3d::VertexDeclaration( IDirect3DVertexDeclaration9** ppDecl )
{
	return m_pd3dDevice->GetVertexDeclaration( ppDecl );
}

//============================
//	Setter
//============================
NL_INLINE
void ND3d::SetPresentParam( const D3DPRESENT_PARAMETERS& presentParam )
{
	m_presentParam = presentParam;
}

NL_INLINE
HRESULT ND3d::WorldMatrix(const NMATRIX& matWorld)
{
	return m_pd3dDevice->SetTransform( D3DTS_WORLD, matWorld );
}

NL_INLINE
HRESULT ND3d::ViewMatrix(const NMATRIX& matView)
{
	return m_pd3dDevice->SetTransform( D3DTS_VIEW, matView );
}

NL_INLINE
HRESULT ND3d::ProjectionMatrix(const NMATRIX& matProjection)
{
	return m_pd3dDevice->SetTransform( D3DTS_PROJECTION, matProjection );
}

NL_INLINE
HRESULT ND3d::ViewPort(const D3DVIEWPORT9& viewPort)
{
	return m_pd3dDevice->SetViewport( &viewPort );
}

NL_INLINE
HRESULT ND3d::TextureMatrix(const NMATRIX& matTexture, UINT Stage)
{
	return m_pd3dDevice->SetTransform(	D3DTRANSFORMSTATETYPE(D3DTS_TEXTURE0 + Stage), 
										matTexture );
}

NL_INLINE
HRESULT ND3d::Texture(IDirect3DBaseTexture9* pTexture, DWORD Stage)
{
	return m_pd3dDevice->SetTexture( Stage, pTexture );
}

NL_INLINE
HRESULT ND3d::RenderState(D3DRENDERSTATETYPE State, DWORD value)
{
	return m_pd3dDevice->SetRenderState( State, value );
}

NL_INLINE
HRESULT ND3d::SamplerState(D3DSAMPLERSTATETYPE Type, DWORD Value, DWORD Sampler)
{
	return m_pd3dDevice->SetSamplerState( Sampler, Type, Value );
}

NL_INLINE
HRESULT ND3d::TextureStageState(D3DTEXTURESTAGESTATETYPE Type, DWORD Value, DWORD Stage)
{
	return m_pd3dDevice->SetTextureStageState( Stage, Type, Value );
}

NL_INLINE
HRESULT ND3d::FVF(DWORD fvf)
{
	return m_pd3dDevice->SetFVF( fvf );
}

NL_INLINE
HRESULT ND3d::StreamSource(IDirect3DVertexBuffer9* pData, UINT Stride, UINT OffsetInBytes, UINT Number)
{
	return m_pd3dDevice->SetStreamSource( Number, pData, OffsetInBytes, Stride );
}

NL_INLINE
HRESULT ND3d::Indices(IDirect3DIndexBuffer9* pIndexData)
{
	return m_pd3dDevice->SetIndices( pIndexData );
}

NL_INLINE
HRESULT ND3d::RenderTarget(IDirect3DSurface9* pRenderTarget, DWORD RenderTargetIndex)
{
	return m_pd3dDevice->SetRenderTarget( RenderTargetIndex, pRenderTarget );
}

NL_INLINE
HRESULT ND3d::DepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
	return m_pd3dDevice->SetDepthStencilSurface( pNewZStencil );
}

NL_INLINE
HRESULT	ND3d::VertexDeclaration( IDirect3DVertexDeclaration9* pDecl )
{
	return m_pd3dDevice->SetVertexDeclaration( pDecl );
}