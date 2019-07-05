/** @file *//********************************************************************************************************

                                                    Material.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/Material.cpp#4 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "PrecompiledHeaders.h"

#include "Material.h"

#include "BoundMaterial.h"
#include "Dxx/TextureManager.h"

namespace Render
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Material::Material( char const *			sName/*		= ""*/,
					TextureNameList const &	textures/*	= TextureNameList()*/,
					D3DXCOLOR const &		ambient/*	= Dxx::White()*/,
					D3DXCOLOR const &		diffuse/*	= Dxx::White()*/,
					D3DXCOLOR const &		specular/*	= Dxx::NoColor()*/,
					float					shininess/*	= 0.0f*/,
					D3DXCOLOR const &		emissive/*	= Dxx::NoColor()*/ )
	: m_Name( sName ),
	m_Textures( textures )
{
	  m_D3dMaterial.Ambient = ambient;
	  m_D3dMaterial.Diffuse = diffuse;
	  m_D3dMaterial.Specular = specular;
	  m_D3dMaterial.Emissive = emissive;
	  m_D3dMaterial.Power = shininess;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Material::~Material()
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

std::auto_ptr< BoundMaterial > Material::Bind( IDirect3DDevice9 * pD3dDevice, Dxx::TextureManager * pTextureManager ) const
{
	BoundMaterial * const	pMaterial	= new BoundMaterial( pD3dDevice, m_D3dMaterial );

	return std::auto_ptr< BoundMaterial >( pMaterial );
}


} // namespace Render
