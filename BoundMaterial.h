/** @file *//********************************************************************************************************

                                                    BoundMaterial.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/BoundMaterial.h#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#if !defined( RENDER_BOUNDMATERIAL_H_INCLUDED )
#define RENDER_BOUNDMATERIAL_H_INCLUDED

#pragma once

#include <d3d9.h>

namespace Render
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

/// A material in a Model bound to D3D
//
/// @ingroup	Model
///

class BoundMaterial
{
public:

	/// Constructor
	BoundMaterial( IDirect3DDevice9 * pD3dDevice, D3DMATERIAL9 const & material );

	/// Destructor
	virtual ~BoundMaterial();

	/// Sets renderstates for this material
	void Apply( IDirect3DDevice9 * pD3dDevice ) const;

private:

	D3DMATERIAL9	m_Material;
};


} // namespace Render



#endif // !defined( RENDER_BOUNDMATERIAL_H_INCLUDED )
