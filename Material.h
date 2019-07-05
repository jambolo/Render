/** @file *//********************************************************************************************************

                                                      Material.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/Material.h#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#if !defined( RENDER_MATERIAL_H_INCLUDED )
#define RENDER_MATERIAL_H_INCLUDED

#pragma once

#include "Dxx/D3d.h"
#include "Dxx/TextureManager.h"
#include <d3dx9.h>
#include <vector>
#include <memory>
#include <string>


namespace Render
{

class BoundMaterial;

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

/// A generic material in a Model
//
/// @ingroup	Model
///

class Material
{
public:

	typedef std::vector< std::string >	TextureNameList;	///< A list of texture names

	/// Constructor
	Material( char const *				sName			= "",
			  TextureNameList const &	textures		= TextureNameList(),
			  D3DXCOLOR const &			ambient			= Dxx::White(),
			  D3DXCOLOR const &			diffuse			= Dxx::White(),
			  D3DXCOLOR const &			specular		= Dxx::NoColor(),
			  float						shininess		= 0.0f,
			  D3DXCOLOR const &			emissive		= Dxx::NoColor() );

	/// Destructor
	virtual ~Material();

	/// Returns a bound version of the material
	std::auto_ptr< BoundMaterial > Bind( IDirect3DDevice9 * pD3dDevice, Dxx::TextureManager * pTextureManager ) const;

	/// Returns the name of the material
	char const *		GetName() const										{ return m_Name.c_str(); }

	/// Sets the name of the material
	void				SetName( char const * sName )						{ m_Name = sName; }
									 
	/// Returns the ambient color
	D3DCOLORVALUE const &	GetAmbient() const								{ return m_D3dMaterial.Ambient; }

	/// Sets the ambient color
	void				SetAmbient( D3DXCOLOR const & ambient )				{ m_D3dMaterial.Ambient = ambient; }

	/// Returns the diffuse color
	D3DCOLORVALUE const &	GetDiffuse() const								{ return m_D3dMaterial.Diffuse; }

	/// Sets the diffuse color
	void				SetDiffuse( D3DXCOLOR const & diffuse )				{ m_D3dMaterial.Diffuse = diffuse; }

	/// Returns the specular color
	D3DCOLORVALUE const &	GetSpecular() const								{ return m_D3dMaterial.Specular; }

	/// Sets the specular color
	void				SetSpecular( D3DXCOLOR const & specular )			{ m_D3dMaterial.Specular = specular; }

	/// Returns the emissive color
	D3DCOLORVALUE const &	GetEmissive() const								{ return m_D3dMaterial.Emissive; }

	/// Sets the emissive color
	void				SetEmissive( D3DXCOLOR const & emissive )			{ m_D3dMaterial.Emissive = emissive; }

	/// Returns the specular power
	float				GetShininess() const								{ return m_D3dMaterial.Power; }

	/// Sets the specular power
	void				SetShininess( float shininess )						{ m_D3dMaterial.Power = shininess; }

	/// Returns a reference to the textures used in this material
	TextureNameList		GetTextures() const									{ return m_Textures; }

	/// Sets the textures used in this material
	void				SetTextures( TextureNameList const & textures )		{ m_Textures = textures; }

private:

	std::string			m_Name;					///< The name of the material
	D3DMATERIAL9		m_D3dMaterial;
	TextureNameList		m_Textures;				///< Textures used in this material
};


} // namespace Render



#endif // !defined( RENDER_MATERIAL_H_INCLUDED )
