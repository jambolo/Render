/** @file *//********************************************************************************************************

                                                      Vertex.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/Vertex.cpp#1 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "PrecompiledHeaders.h"

#include "Vertex.h"

namespace Render
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Vertex::Vertex()
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Vertex::Vertex( D3DXVECTOR3 const &		position,
				D3DXCOLOR const &		color,
				BoneWeights const &		boneWeights,
				ParameterList const &	parameters )
	: m_Position( position ), m_Color( color ), m_BoneWeights( boneWeights ), m_Parameters( parameters )
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Vertex::~Vertex()
{
}


} // namespace Render