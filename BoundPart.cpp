/** @file *//********************************************************************************************************

                                                     BoundPart.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/BoundPart.cpp#2 $

	$NoKeywords: $

 ********************************************************************************************************************/


#include "PrecompiledHeaders.h"

#include "BoundPart.h"

#include "Wx/Wx.h"

namespace Render
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BoundPart::BoundPart( char const * sName,
			   IDirect3DVertexBuffer9 * pVB, int nVertexes, int entrySize, DWORD fvf,
			   IDirect3DIndexBuffer9 * pIB, int nIndexes,
			   int material )
	: m_Name( sName ),
	  m_pVB( pVB ),
	  m_nVertexes( nVertexes ),
	  m_VBEntrySize( entrySize ),
	  m_Fvf( fvf ),
	  m_pIB( pIB ),
	  m_nIndexes( nIndexes ),
	  m_Material( material )
{
	assert( pVB != 0 );
	assert( pIB != 0 );

	m_pVB->AddRef();
	m_pIB->AddRef();
}

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BoundPart::~BoundPart()
{
	m_pVB->Release(); 
	m_pIB->Release();
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BoundPart::BoundPart( BoundPart const & a )
{
	m_Name			= a.m_Name;
	m_pVB			= a.m_pVB;
	m_nVertexes		= a.m_nVertexes;
	m_VBEntrySize	= a.m_VBEntrySize;
	m_Fvf			= a.m_Fvf;
	m_pIB			= a.m_pIB;
	m_nIndexes		= a.m_nIndexes;
	m_Material		= a.m_Material;

	m_pVB->AddRef();
	m_pIB->AddRef();
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BoundPart const &  BoundPart::operator =( BoundPart const & a )
{
	if ( &a != this )
	{
		m_pVB->Release();
		m_pIB->Release();

		m_Name			= a.m_Name;
		m_pVB			= a.m_pVB;
		m_nVertexes		= a.m_nVertexes;
		m_VBEntrySize	= a.m_VBEntrySize;
		m_Fvf			= a.m_Fvf;
		m_pIB			= a.m_pIB;
		m_nIndexes		= a.m_nIndexes;
		m_Material		= a.m_Material;

		m_pVB->AddRef();
		m_pIB->AddRef();
	}

	return *this;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void BoundPart::Draw( IDirect3DDevice9 * pD3dDevice ) const
{
	HRESULT		hr;

	hr = pD3dDevice->SetFVF( m_Fvf );
	assert_succeeded( hr );

	hr = pD3dDevice->SetStreamSource( 0, m_pVB, 0, m_VBEntrySize );
	assert_succeeded( hr );

	hr = pD3dDevice->SetIndices( m_pIB );
	assert_succeeded( hr );

	hr = pD3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_nVertexes, 0, m_nIndexes / 3 );
	assert_succeeded( hr );
}


} // namespace Render
