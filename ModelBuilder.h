/*********************************************************************************************************************

                                                     ModelBuilder.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/ModelBuilder.h#2 $

	$NoKeywords: $

*********************************************************************************************************************/

#if !defined( RENDER_MODELBUILDER_H_INCLUDED )
#define RENDER_MODELBUILDER_H_INCLUDED

#pragma once

#include <memory>

namespace Render
{

class Model;


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

/// Class that reads and writes models
//
/// @ingroup Render
///

class ModelBuilder
{
public:

	/// Returns a Model loaded from a MSX file
	static std::auto_ptr< Model > LoadFromMSX( char const * sPath );
};


} // namespace Render



#endif // !defined( RENDER_MODELBUILDER_H_INCLUDED )
