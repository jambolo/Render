/** @file *//********************************************************************************************************

                                                     Part.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/Part.cpp#3 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "PrecompiledHeaders.h"

#include "Part.h"

#include "BoundPart.h"
#include "Face.h"
#include "Vertex.h"
#include "Material.h"
#include "Dxx/VertexBufferProxy.h"
#include "Dxx/VertexBufferLock.h"


namespace Render
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Part::Part()
	: m_pBoundPart( 0 )
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Part::Part( char const *		sName,
			int					material,
			FaceList const &	faces,
			VertexList const &	vertexes,
			NormalList const &	normals )
	: m_Name( sName ),
	  m_Material( material ),
	  m_Faces( faces ),
	  m_Vertexes( vertexes ),
	  m_Normals( normals ),
	  m_pBoundPart( 0 )
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Part::~Part()
{
	delete m_pBoundPart;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

std::auto_ptr< BoundPart > Part::Bind( IDirect3DDevice9 * pD3dDevice ) const
{
	HRESULT		hr;

	// Calculate the FVF from the data available for rendering

	DWORD	fvf		= 0;
	int		vsize	= 0;

//	fvf |= D3DFVF_XYZ;				vsize += 3 * sizeof( float );	// Position
	fvf |= D3DFVF_XYZB1;			vsize += 4 * sizeof( float );	// Number of bone weights (this is hard-coded to 1 weight for now)
	fvf |= D3DFVF_LASTBETA_UBYTE4;	vsize += 0;						// Use indexed matrixes
	fvf |= D3DFVF_NORMAL;			vsize += 3 * sizeof( float );	// Normal
//	fvf |= D3DFVF_DIFFUSE;			vsize += 1 * sizeof( DWORD );	// Diffuse vertex color
	fvf |= D3DFVF_TEX0;				vsize += 0;						// No textures for now

	// Set up the vertex buffer

	IDirect3DVertexBuffer9 *	pVB;

	hr = pD3dDevice->CreateVertexBuffer( vsize * m_Vertexes.size(), D3DUSAGE_WRITEONLY, fvf, D3DPOOL_MANAGED, &pVB, NULL );
	if ( FAILED( hr ) )
	{
		return std::auto_ptr< BoundPart >();
	}

	// Copy the vertex data to the vertex buffer

	try
	{
		Dxx::VertexBufferProxy	vbp( pVB );
		vbp.Lock( 0, 0, 0 );

		for ( int i = 0; i < (int)m_Vertexes.size(); i++ )
		{
			vbp[i].Position() = m_Vertexes[i]->GetPosition();

			vbp[i].BlendIndexes() = 0;
			for ( Vertex::BoneWeights::const_iterator pW = m_Vertexes[i]->GetBoneWeights().begin(); pW != m_Vertexes[i]->GetBoneWeights().end(); ++pW )
			{
				if ( *pW != 0.0f )
				{
					vbp[i].BlendIndexes() = pW - m_Vertexes[i]->GetBoneWeights().begin();
					break;
				}
			}

//			vbp[i].Diffuse() = m_Vertexes[i]->GetColor();
			vbp[i].Normal() = m_Normals[i];
//			vbp[i].TexCoord0()[0] = m_Vertexes[i].GetParameters()[0].x;
//			vbp[i].TexCoord0()[1] = m_Vertexes[i].GetParameters()[0].y;
		}

		vbp.Unlock();
	}
	catch ( ... )
	{
		pVB->Release();
		return std::auto_ptr< BoundPart >();
	}

	// Set up the index buffer

	IDirect3DIndexBuffer9 *		pIB;

	hr = pD3dDevice->CreateIndexBuffer( m_Faces.size() * 3 * sizeof( __int16 ),
										D3DUSAGE_WRITEONLY,
										D3DFMT_INDEX16,
										D3DPOOL_MANAGED,
										&pIB,
										NULL );
	if ( FAILED( hr ) )
	{
		return std::auto_ptr< BoundPart >();
	}

	// Copy the indexes to the index buffer

	try
	{
		Dxx::IndexBufferLock	lock( pIB, 0, 0, 0 );

		__int16 *	pB	= reinterpret_cast< __int16 * >( lock.GetLockedBuffer() );

		for ( FaceList::const_iterator ppF = m_Faces.begin(); ppF != m_Faces.end(); ++ppF )
		{
			*pB++ = (*ppF)->GetVertexIndexes()[0];
			*pB++ = (*ppF)->GetVertexIndexes()[1];
			*pB++ = (*ppF)->GetVertexIndexes()[2];
		}
	}
	catch ( ... )
	{
		pIB->Release();
		return std::auto_ptr< BoundPart >();
	}

	BoundPart * const	pPart	= new BoundPart( m_Name.c_str(),
												 pVB, m_Vertexes.size(), vsize, fvf,
												 pIB, m_Faces.size() * 3,
												 m_Material );
	if ( pPart == 0 ) throw std::bad_alloc();

	return std::auto_ptr< BoundPart >( pPart );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Part::Draw( IDirect3DDevice9 * pD3dDevice ) const
{
	if ( m_pBoundPart == 0 )
	{
		m_pBoundPart = Bind( pD3dDevice ).release();
	}

	m_pBoundPart->Draw( pD3dDevice );
}


} // namespace Render
