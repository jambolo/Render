/** @file *//********************************************************************************************************

                                                       Model.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/Model.h#4 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once

#include "Dxx/TextureManager.h"
#include <d3d9.h>
#include <vector>
#include <memory>
#include <boost/noncopyable.hpp>


/// @defgroup	Model	Models
/// @ingroup	Render

namespace Render
{

class Part;
class Material;
class Skeleton;
class BoundModel;


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

/// A generic model
//
/// @ingroup	Model
///

class Model : public boost::noncopyable
{
public:

	typedef std::vector< Part * >		PartList;		///< A list of parts
	typedef std::vector< Material * >	MaterialList;	///< A list of materials

	/// Default constructor
	Model();

	/// Constructor
	Model( PartList const &		parts,
		   MaterialList const &	materials,
		   Skeleton *			pSkeleton );

	/// Destructor
	virtual ~Model();

	/// Creates a version of the model that can be rendered directly
	std::auto_ptr< BoundModel > Bind( IDirect3DDevice9 * pD3dDevice, Dxx::TextureManager * pTextureManager ) const;

	/// Renders the model.
	void Draw( IDirect3DDevice9 * pD3dDevice ) const;
	
	/// Returns a reference to the parts of the model
	PartList const &		GetParts() const										{ return m_Parts; }

	/// Sets the list of parts
	void					SetParts( PartList const & parts )						{ m_Parts = parts; }

	/// Returns a reference to the materials in the part
	MaterialList const &	GetMaterials() const									{ return m_Materials; }

	/// Sets the list of materials in the part
	void					SetMaterials( MaterialList const & materials )			{ m_Materials = materials; }

	/// Returns a pointer to the skeleton used by this model
	Skeleton *				GetSkeleton() const										{ return m_pSkeleton; }

	/// Sets the skeleton used by this model
	void					AttachSkeleton( std::auto_ptr< Skeleton > qSkeleton )	{ m_pSkeleton = qSkeleton.release(); }

private:

	PartList				m_Parts;		///< All the parts of the model
	MaterialList			m_Materials;	///< All the materials in the model
	Skeleton *				m_pSkeleton;	///< The skeleton used by this model
	mutable BoundModel *	m_pBoundModel;
};


} // namespace Render
