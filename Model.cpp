/** @file *//********************************************************************************************************

                                                      Model.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/Model.cpp#5 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "PrecompiledHeaders.h"

#include "Model.h"

#include "Skeleton.h"
#include "Part.h"
#include "Face.h"
#include "Vertex.h"
#include "Material.h"
#include "BoundModel.h"
#include "BoundPart.h"
#include "BoundMaterial.h"
#include "Dxx/TextureManager.h"


namespace Render
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Model::Model()
	: m_pSkeleton( 0 ), m_pBoundModel( 0 )
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Model::Model( PartList const &		parts,
			  MaterialList const &	materials,
			  Skeleton *			pSkeleton )
	: m_Parts( parts ), m_Materials( materials ), m_pSkeleton( pSkeleton ), m_pBoundModel( 0 )
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Model::~Model()
{
	delete m_pSkeleton;
	delete m_pBoundModel;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

std::auto_ptr< BoundModel >	Model::Bind( IDirect3DDevice9 * pD3dDevice, Dxx::TextureManager * pTextureManager ) const
{
	BoundModel::PartList		parts;
	BoundModel::MaterialList	materials;
	Skeleton *					pSkeleton;

	// Bind the parts

	for ( PartList::const_iterator ppPart = m_Parts.begin(); ppPart != m_Parts.end(); ++ppPart )
	{
		parts.push_back( (*ppPart)->Bind( pD3dDevice ).release() );
	}

	// Bind the materials

	for ( MaterialList::const_iterator ppMaterial = m_Materials.begin(); ppMaterial != m_Materials.end(); ++ppMaterial )
	{
		materials.push_back( (*ppMaterial)->Bind( pD3dDevice, pTextureManager ).release() );
	}

	// Clone the skeleton

	pSkeleton = new Skeleton( *m_pSkeleton );

	return std::auto_ptr< BoundModel >( new BoundModel( parts, materials, std::auto_ptr< Skeleton >( pSkeleton ) ) );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Model::Draw( IDirect3DDevice9 * pD3dDevice ) const
{
	// If the model has not been bound yet, then bind it now

	if ( m_pBoundModel == 0 )
	{
		m_pBoundModel = Bind( pD3dDevice, 0 ).release();
	}

	// Draw the bound model

	m_pBoundModel->Draw( pD3dDevice );
}

} // namespace Render
