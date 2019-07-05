/** @file *//********************************************************************************************************

                                                   BoundSkeleton.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/BoundSkeleton.h#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#if !defined( RENDER_BOUNDSKELETON_H_INCLUDED )
#define RENDER_BOUNDSKELETON_H_INCLUDED

#pragma once


namespace Render
{

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

/// A skeleton bound to D3D
//
/// @ingroup	SkeletalAnimation
///

class BoundSkeleton
{
public:

	/// Default Constructor
	BoundSkeleton();

	/// Destructor
	virtual ~BoundSkeleton();

	/// Applies the skeleton data for rendering
	void Apply( IDirect3DDevice9 * pD3dDevice ) const;
};


} // namespace Render



#endif // !defined( RENDER_BOUNDSKELETON_H_INCLUDED )
