/** @file *//********************************************************************************************************

                                                       Face.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/Face.h#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#if !defined( RENDER_FACE_H_INCLUDED )
#define RENDER_FACE_H_INCLUDED

#pragma once

#include <d3dx9.h>


namespace Render
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

/// A generic face in a Model
//
/// @ingroup	Model
///

class Face
{
public:

	enum
	{
		NUM_VERTEXES	= 3
	};

	/// Default constructor
	Face();

	/// Constructor
	Face( int v0, int v1, int v2, int n0, int n1, int n2 );

	/// Destructor
	virtual ~Face();

	/// Returns the indexes of the face's vertexes
	int const *			GetVertexIndexes() const					{ return m_V; }

	/// Sets the indexes of the face's vertexes
	void				SetVertexIndexes( int v0, int v1, int v2 )	{ m_V[0] = v0; m_V[1] = v1; m_V[2] = v2; }

	/// Returns the indexes of the face's normals
	int const *			GetNormalIndexes() const					{ return m_N; }

	/// Sets the indexes of the face's normals
	void				SetNormalIndexes( int n0, int n1, int n2 )	{ m_N[0] = n0; m_N[1] = n1; m_N[2] = n2; }

private:

	int			m_V[NUM_VERTEXES];				///< The indexes of the vertexes
	int			m_N[NUM_VERTEXES];				///< The indexes of the vertex normals
};


} // namespace Render



#endif // !defined( RENDER_FACE_H_INCLUDED )
