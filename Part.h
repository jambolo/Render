/** @file *//********************************************************************************************************

                                                       Part.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/Part.h#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#if !defined( RENDER_PART_H_INCLUDED )
#define RENDER_PART_H_INCLUDED

#pragma once

#include <vector>
#include <string>
#include <memory>

struct IDirect3DDevice9;


namespace Render
{

class Face;
class Vertex;
class BoundPart;

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

/// A generic part of a Model
//
/// @ingroup	Model
///

class Part
{
public:

	typedef std::vector< Face * >		FaceList;			///< A list of faces
	typedef std::vector< Vertex * >		VertexList;			///< A list of vertexes
	typedef std::vector< D3DXVECTOR3 >	NormalList;			///< A list of normals

	/// Default constructor
	Part();

	/// Constructor
	Part( char const *			sName,
		  int					material,
		  FaceList const &		faces,
		  VertexList const &	vertexes,
		  NormalList const &	normals );

	/// Destructor
	virtual ~Part();

	/// Creates a version of the part that can be rendered directly
	std::auto_ptr< BoundPart > Bind( IDirect3DDevice9 * pD3dDevice ) const;

	/// Renders the part.
	void Draw( IDirect3DDevice9 * pD3dDevice ) const;

	/// Returns the name of the part
	char const *		GetName() const								{ return m_Name.c_str(); }

	/// Sets the name of the part
	void				SetName( char const * sName )				{ m_Name = sName; }

	/// Returns the material index for the part
	int					GetMaterial() const							{ return m_Material; }

	/// Sets the material index for the part
	void				SetMaterial( int material )					{ m_Material = material; }

	/// Returns a reference to the faces in the part
	FaceList const &	GetFaces() const							{ return m_Faces; }

	/// Sets the faces in the part
	void				SetFaces( FaceList const & faces )			{ m_Faces = faces; }

	/// Returns a reference to the vertexes in the part
	VertexList const &	GetVertexes() const							{ return m_Vertexes; }

	/// Sets the vertexes in the part
	void				SetVertexes( VertexList const & vertexes )	{ m_Vertexes = vertexes; }

	/// Returns a reference to the normals in the part
	NormalList const &	GetNormals() const							{ return m_Normals; }

	/// Sets the normals in the part
	void				SetNormals( NormalList const & normals )	{ m_Normals = normals; }

private:

	std::string				m_Name;						///< The name of the part
	int						m_Material;					///< Index of the material for this part
	FaceList				m_Faces;					///< The faces in the part
	VertexList				m_Vertexes;					///< All the vertexes in the part
	NormalList				m_Normals;					///< All the normals in the part
	mutable BoundPart *		m_pBoundPart;
};


} // namespace Render



#endif // !defined( RENDER_PART_H_INCLUDED )
