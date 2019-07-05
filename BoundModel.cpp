/** @file *//********************************************************************************************************

                                                     BoundModel.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/BoundModel.cpp#3 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "PrecompiledHeaders.h"

#include "BoundModel.h"

#include "Boundpart.h"
#include "BoundMaterial.h"
#include "Skeleton.h"


namespace Render
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BoundModel::BoundModel()
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BoundModel::BoundModel( PartList const &			parts,
						MaterialList const &		materials,
						std::auto_ptr< Skeleton >	qSkeleton )
	: m_Parts( parts ),
	m_Materials( materials ),
	m_pSkeleton( qSkeleton.release() )
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

BoundModel::~BoundModel()
{
	for ( PartList::iterator ppPart = m_Parts.begin(); ppPart != m_Parts.end(); ++ppPart )
	{
		delete *ppPart;
	}

	for ( MaterialList::iterator ppMaterial = m_Materials.begin(); ppMaterial != m_Materials.end(); ++ppMaterial )
	{
		delete *ppMaterial;
	}

	delete m_pSkeleton;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void BoundModel::Draw( IDirect3DDevice9 * pD3dDevice ) const
{
	// Set up skeleton-related renderstates

	pD3dDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS );

	m_pSkeleton->Apply( pD3dDevice );

	// Set up material-related renderstates

	// Draw the parts

	for ( PartList::const_iterator ppPart = m_Parts.begin(); ppPart != m_Parts.end(); ++ppPart )
	{
		BoundPart * const	pPart	= *ppPart;	// Convenience

		// Set up the material for this part

		m_Materials[ pPart->GetMaterial() ]->Apply( pD3dDevice );

		// Draw the part

		pPart->Draw( pD3dDevice );
	}
}


} // namespace Render
