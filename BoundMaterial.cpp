/** @file *//********************************************************************************************************

                                                   BoundMaterial.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/BoundMaterial.cpp#3 $

	$NoKeywords: $

 ********************************************************************************************************************/


#include "PrecompiledHeaders.h"

#include "BoundMaterial.h"

#include "Wx/Wx.h"


namespace Render
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BoundMaterial::BoundMaterial( IDirect3DDevice9 * pD3dDevice, D3DMATERIAL9 const & material )
	: m_Material( material )
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BoundMaterial::~BoundMaterial()
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void BoundMaterial::Apply( IDirect3DDevice9 * pD3dDevice ) const
{
	HRESULT		hr;

	hr = pD3dDevice->SetMaterial( &m_Material );
	assert_succeeded( hr );

	// Disable textures for now

//	hr = m_pD3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
//	assert_succeeded( hr );
  
//	hr = m_pD3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
//	assert_succeeded( hr );
  
//	hr = m_pD3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
//	assert_succeeded( hr );
  
//	hr = m_pD3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
//	assert_succeeded( hr );

//	pD3dDevice->SetTexture( 0, NULL );
//
//	hr = pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
//	assert_succeeded( hr );
}



} // namespace Render
