/** @file *//********************************************************************************************************

                                                     BoundPart.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/BoundPart.h#4 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once

#include <string>

struct IDirect3DDevice9;
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;


namespace Render
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

/// A part of a Model bound to D3D
//
/// @ingroup	Model
///

class BoundPart
{
public:

	/// Default constructor
	BoundPart( char const * sName,
			   IDirect3DVertexBuffer9 * pVB, int nVertexes, int entrySize, DWORD fvf,
			   IDirect3DIndexBuffer9 * pIB, int nIndexes,
			   int material );

	/// Destructor
	virtual ~BoundPart();

	// Copy constructor
	BoundPart( BoundPart const & a );

	// Assignment operator
	BoundPart const &  operator=( BoundPart const & a );

	/// Renders the part.
	void Draw( IDirect3DDevice9 * pD3dDevice ) const;

	/// Returns the name of this part
	char const * GetName() const							{ return m_Name.c_str(); }

	/// Returns the material index used by this part
	int GetMaterial() const									{ return m_Material; }

private:

	std::string					m_Name;						///< The name of the part
	IDirect3DVertexBuffer9 *	m_pVB;
	int							m_nVertexes;
	int							m_VBEntrySize;
	DWORD						m_Fvf;
	IDirect3DIndexBuffer9 *		m_pIB;
	int							m_nIndexes;
	int							m_Material;					///< Index of the material for this part

};


} // namespace Render
