/** @file *//********************************************************************************************************

                                                     BoundModel.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/BoundModel.h#4 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once

#include <vector>
#include <memory>
#include <boost/noncopyable.hpp>

struct IDirect3DDevice9;


namespace Render
{

class BoundPart;
class BoundMaterial;
class Skeleton;


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

/// A model bound to D3D
//
/// @ingroup	Model
///

class BoundModel : public boost::noncopyable
{
public:

	typedef std::vector< BoundPart * >			PartList;		///< A list of parts
	typedef std::vector< BoundMaterial * >		MaterialList;	///< A list of materials

	/// Default constructor
	BoundModel();

	/// Constructor
	BoundModel( PartList const &			parts,
				MaterialList const &		materials,
				std::auto_ptr< Skeleton >	qSkeleton );

	/// Destructor
	virtual ~BoundModel();

	/// Renders the model.
	void Draw( IDirect3DDevice9 * pD3dDevice ) const;
	
	/// Returns a reference to the parts of the model
	PartList const &		GetParts() const											{ return m_Parts; }

	/// Sets the list of parts
	void					SetParts( PartList const & parts )							{ m_Parts = parts; }

	/// Returns a reference to the materials in the part
	MaterialList const &	GetMaterials() const										{ return m_Materials; }

	/// Sets the list of materials in the part
	void					SetMaterials( MaterialList const & materials )				{ m_Materials = materials; }

	/// Returns a pointer to the skeleton used by this model
	Skeleton *				GetSkeleton() const											{ return m_pSkeleton; }

	/// Sets the skeleton used by this model
	void					AttachSkeleton( std::auto_ptr< Skeleton > qSkeleton )		{ m_pSkeleton = qSkeleton.release(); }

private:

	PartList				m_Parts;		///< All the parts of the model
	MaterialList			m_Materials;	///< All the materials in the model
	Skeleton *				m_pSkeleton;	///< The skeleton used by this model
};


} // namespace Render
