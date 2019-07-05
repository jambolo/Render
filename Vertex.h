/** @file *//********************************************************************************************************

                                                      Vertex.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/Vertex.h#1 $

	$NoKeywords: $

 ********************************************************************************************************************/

#if !defined( RENDER_VERTEX_H_INCLUDED )
#define RENDER_VERTEX_H_INCLUDED

#pragma once

#include <d3dx9.h>
#include <vector>


namespace Render
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

/// A generic vertex in a Model
//
/// @ingroup	Model
///

class Vertex
{
public:

	typedef std::vector< float >		BoneWeights;		///< A list of bone weights
	typedef std::vector< D3DXVECTOR4 >	ParameterList;		///< A list of 4D vectors

	/// Default constructor
	Vertex();

	/// Constructor
	Vertex( D3DXVECTOR3 const &		position,
			D3DXCOLOR const &		color,
			BoneWeights const &		boneWeights,
			ParameterList const &	parameters );

	/// Destructor
	virtual ~Vertex();

	/// Returns the position
	D3DXVECTOR3 const &		GetPosition() const										{ return m_Position; }

	/// Sets the position
	void					SetPosition( D3DXVECTOR3 const & position )				{ m_Position = position; }

	/// Returns the vertex color
	D3DXCOLOR const &		GetColor() const										{ return m_Color; }

	/// Sets the vertex color
	void					SetColor( D3DXCOLOR const & color )						{ m_Color = color; }

	/// Returns the bone function
	BoneWeights const &		GetBoneWeights() const									{ return m_BoneWeights; }

	/// Sets the bone function
	void					SetBoneWeights( BoneWeights const & boneWeights )		{ m_BoneWeights = boneWeights; }

	/// Returns a reference to the generic parameters
	ParameterList const &	GetParameters() const									{ return m_Parameters; }

	/// Sets the generic parameters
	void					SetParameters( ParameterList const & parameters )		{ m_Parameters = parameters; }

private:

	D3DXVECTOR3			m_Position;				///< Position
	D3DXCOLOR			m_Color;				///< Vertex color
	BoneWeights			m_BoneWeights;			///< Bone function
	ParameterList		m_Parameters;			///< Generic parameters (for shaders)
};



} // namespace Render



#endif // !defined( RENDER_VERTEX_H_INCLUDED )
