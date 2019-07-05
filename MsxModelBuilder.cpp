/*********************************************************************************************************************

                                                  MsxModelBuilder.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/MsxModelBuilder.cpp#5 $

	$NoKeywords: $

*********************************************************************************************************************/

#include "PrecompiledHeaders.h"

#include "ModelBuilder.h"

#include "Model.h"
#include "Part.h"
#include "Face.h"
#include "Material.h"
#include "Vertex.h"
#include "Skeleton.h"
#include "Bone.h"
#include "D3d.h"
#include "Misc/Trace.h"
#include "Msxmlx/Msxmlx.h"


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

namespace
{

	struct BoneInfo
	{
		int				index;
		std::string		name;
		D3DXVECTOR3		position;
		std::string		parent;
		D3DXVECTOR3		rotation;

	};

	typedef std::vector< BoneInfo > BoneInfoList;

	bool ProcessScene( IXMLDOMDocument2 * pDocument, Render::Model * pModel );
	bool ProcessModel( IXMLDOMElement * pElement, uint32 context );
	bool ProcessParts( IXMLDOMElement * pElement, uint32 context );
	bool ProcessPart( IXMLDOMElement * pElement, uint32 context );
	bool ProcessVertexes( IXMLDOMElement * pElement, uint32 context );
	bool ProcessVertex( IXMLDOMElement * pElement, uint32 context );
	bool ProcessVertexNormals( IXMLDOMElement * pElement, uint32 context );
	bool ProcessVertexNormal( IXMLDOMElement * pElement, uint32 context );
	bool ProcessFaces( IXMLDOMElement * pElement, uint32 context );
	bool ProcessFace( IXMLDOMElement * pElement, uint32 context );
	bool ProcessMaterials( IXMLDOMElement * pElement, uint32 context );
	bool ProcessMaterial( IXMLDOMElement * pElement, uint32 context );
	bool ProcessTextureNames( IXMLDOMElement * pElement, uint32 context );
	bool ProcessTexture( IXMLDOMElement * pElement, uint32 context );
	bool ProcessBones( IXMLDOMElement * pElement, uint32 context );
	bool ProcessBone( IXMLDOMElement * pElement, uint32 context );

	std::auto_ptr< Render::Skeleton > BuildSkeleletonFromBoneInfoList( BoneInfoList const & bones );

	D3DXVECTOR3 GetVector3SubElement( IXMLDOMElement * pElement, char const * sName );
	D3DXCOLOR GetRgbaSubElement( IXMLDOMElement * pElement, char const * sName );
	D3DXQUATERNION GetQuatSubElement( IXMLDOMElement * pElement, char const * sName );
	D3DXVECTOR4 GetUVSubElement( IXMLDOMElement * pElement, char const * sName );
	void GetTriVertexIndexSubElement( IXMLDOMElement * pElement, char const * sName, int aIndexes[3] );

} // anonymous namespace


namespace Render
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

std::auto_ptr< Model > ModelBuilder::LoadFromMSX( char const * sPath )
{
	HRESULT			hr;

	// Load the XML document

	CComPtr< IXMLDOMDocument2 >		pDocument;

	hr = pDocument.CoCreateInstance( CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER );
	if ( FAILED( hr ) )
	{
		trace( "ModelBuilder::ProcessModelFromMSX: CoCreateInstance failed.\n" );
		return std::auto_ptr< Model >();
	}

	VARIANT_BOOL	status;

	hr = pDocument->put_async( VARIANT_FALSE );
	if ( FAILED( hr ) )
	{
		trace( "ModelBuilder::ProcessModelFromMSX: Failed to set async property\n" );
		return std::auto_ptr< Model >();
	}

	hr = pDocument->load( CComVariant( sPath ), &status );
	if ( FAILED( hr ) )
	{
		trace( "ModelBuilder::ProcessModelFromMSX: load( \"%s\" ) failed.\n", sPath );
		return std::auto_ptr< Model >();
	}

	if ( status != VARIANT_TRUE )
	{
#if defined( _DEBUG )
		CComPtr< IXMLDOMParseError >	pObjError;
		CComBSTR						reason;

		hr = pDocument->get_parseError( &pObjError );
		hr = pObjError->get_reason( &reason );
		trace( "ModelBuilder::ProcessModelFromMSX: Failed to load DOM from '%s'\n%S\n", sPath, reason );
#endif // defined( _DEBUG )

		return std::auto_ptr< Model >();
	}

	CComPtr< IXMLDOMElement >	pRoot;
	hr = pDocument->get_documentElement( &pRoot );
	if ( FAILED( hr ) )
	{
		trace( "ModelBuilder::ProcessModelFromMSX: Failed to get document element\n" );
		return std::auto_ptr< Model >();
	}

	Model * const	pModel	= new Model();

	if ( pModel != 0 )
	{
		ProcessScene( pDocument, pModel );		// Process the model from the document
	}

	return std::auto_ptr< Model >( pModel );
}


} // namespace Render


namespace
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessScene( IXMLDOMDocument2 * pDocument, Render::Model * pModel )
{
	HRESULT		hr;

	// Get the Scene node

	CComPtr< IXMLDOMElement >	pScene;
	hr = pDocument->get_documentElement( &pScene );
	if ( FAILED( hr ) )
	{
		trace( "ProcessScene: Failed to get document element\n" );
		return false;
	}

	Msxmlx::ForEachSubElement( pScene, ProcessModel, (uint32)pModel );	// Process the first model in the scene

	return true;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessModel( IXMLDOMElement * pElement, uint32 context )
{
	Render::Model * const	pModel	= reinterpret_cast< Render::Model * >( context );

	HRESULT		hr;
	CComBSTR	tag;
	hr = pElement->get_tagName( &tag );

	if ( tag == CComBSTR( "Model" ) )
	{
		// Get the parts

		Render::Model::PartList		parts;

		Msxmlx::ForEachSubElement( pElement, ProcessParts, (uint32)&parts );
		pModel->SetParts( parts );

		// Get the materials

		Render::Model::MaterialList		materials;

		Msxmlx::ForEachSubElement( pElement, ProcessMaterials, (uint32)&materials );
		pModel->SetMaterials( materials );

		// Get the skeleton

		BoneInfoList	bones;

		Msxmlx::ForEachSubElement( pElement, ProcessBones, (uint32)&bones );
		pModel->AttachSkeleton( BuildSkeleletonFromBoneInfoList( bones ) );

		return false;	// Only once
	}
	else
	{
		return true;
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessParts( IXMLDOMElement * pElement, uint32 context )
{
	Render::Model::PartList * const	pParts	= reinterpret_cast< Render::Model::PartList * >( context );

	HRESULT		hr;
	CComBSTR	tag;
	hr = pElement->get_tagName( &tag );

	if ( tag == CComBSTR( "MeshList" ) )
	{
		CComPtr< IXMLDOMNodeList >	pSubNodeList;
		long						length;

		hr = pElement->get_childNodes( &pSubNodeList );
		hr = pSubNodeList->get_length( &length );

		pParts->resize( length );

		Msxmlx::ForEachElement( pSubNodeList, ProcessPart, (uint32)pParts );
	}

	return true;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessPart( IXMLDOMElement * pElement, uint32 context )
{
	Render::Model::PartList * const	pParts	= reinterpret_cast< Render::Model::PartList * >( context );

	HRESULT		hr;
	CComBSTR	tag;

	hr = pElement->get_tagName( &tag );

	if ( tag == CComBSTR( "Mesh" ) )
	{
		int const	index	= Msxmlx::GetIntAttribute( pElement, "index" );

		if ( index >= 0 && index < (int)pParts->size() )
		{
			int const					material	=	Msxmlx::GetIntSubElement( pElement, "Material", -1 );
			Render::Part::VertexList	vertexes;
			Render::Part::NormalList	normals;
			Render::Part::FaceList		faces;

			Msxmlx::ForEachSubElement( pElement, ProcessVertexes, (uint32)&vertexes );
			Msxmlx::ForEachSubElement( pElement, ProcessVertexNormals, (uint32)&normals );
			Msxmlx::ForEachSubElement( pElement, ProcessFaces, (uint32)&faces );

			(*pParts)[ index ] = new Render::Part( "", material, faces, vertexes, normals );
			if ( (*pParts)[ index ] == 0 )	throw std::bad_alloc();
		}
	}

	return true;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessVertexes( IXMLDOMElement * pElement, uint32 context )
{
	Render::Part::VertexList * const	pVertexes	= reinterpret_cast< Render::Part::VertexList * >( context );

	HRESULT		hr;
	CComBSTR	tag;
	hr = pElement->get_tagName( &tag );

	if ( tag == CComBSTR( "VertexList" ) )
	{
		CComPtr< IXMLDOMNodeList >	pSubNodeList;
		long						length;

		hr = pElement->get_childNodes( &pSubNodeList );
		hr = pSubNodeList->get_length( &length );

		pVertexes->resize( length );

		Msxmlx::ForEachElement( pSubNodeList, ProcessVertex, (uint32)pVertexes );
	}

	return true;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessVertex( IXMLDOMElement * pElement, uint32 context )
{
	Render::Part::VertexList * const	pVertexes	= reinterpret_cast< Render::Part::VertexList * >( context );

	HRESULT		hr;
	CComBSTR	tag;
	hr = pElement->get_tagName( &tag );

	if ( tag == CComBSTR( "Vertex" ) )
	{
		int const	index	= Msxmlx::GetIntAttribute( pElement, "index" );

		if ( index >= 0 && index < (int)pVertexes->size() )
		{
			int const						bone		= Msxmlx::GetIntAttribute( pElement, "Bone", -1 );
			D3DXVECTOR4 const				texcoords	= GetUVSubElement( pElement, "TexCoords" );
			D3DXVECTOR3 const				position	= GetVector3SubElement( pElement, "Position" );

			Render::Vertex::ParameterList	parameters( 1, texcoords );
			Render::Vertex::BoneWeights		boneWeights( std::min( bone+1, 1 ), 0.0f );
			boneWeights.push_back( 1.0f );

			(*pVertexes)[ index ] = new Render::Vertex( position, Dxx::NoColor(), boneWeights, parameters );
			if ( (*pVertexes)[ index ] == 0 ) throw std::bad_alloc();
		}
	}

	return true;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessVertexNormals( IXMLDOMElement * pElement, uint32 context )
{
	Render::Part::NormalList * const	pNormals	= reinterpret_cast< Render::Part::NormalList * >( context );

	HRESULT		hr;
	CComBSTR	tag;
	hr = pElement->get_tagName( &tag );

	if ( tag == CComBSTR( "VertexNormalList" ) )
	{
		CComPtr< IXMLDOMNodeList >	pSubNodeList;
		long						length;

		hr = pElement->get_childNodes( &pSubNodeList );
		hr = pSubNodeList->get_length( &length );

		pNormals->resize( length );

		Msxmlx::ForEachElement( pSubNodeList, ProcessVertexNormal, (uint32)pNormals );
	}

	return true;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessVertexNormal( IXMLDOMElement * pElement, uint32 context )
{
	Render::Part::NormalList * const	pNormals	= reinterpret_cast< Render::Part::NormalList * >( context );

	HRESULT		hr;
	CComBSTR	tag;
	hr = pElement->get_tagName( &tag );

	if ( tag == CComBSTR( "VertexNormal" ) )
	{
		int const			index		= Msxmlx::GetIntAttribute( pElement, "index" );

		if ( index >= 0 && index < (int)pNormals->size() )
		{
			float const		x	= Msxmlx::GetFloatSubElement( pElement, "X" );
			float const		y	= Msxmlx::GetFloatSubElement( pElement, "Y" );
			float const		z	= Msxmlx::GetFloatSubElement( pElement, "Z" );

			(*pNormals)[ index ] = D3DXVECTOR3( x, y, z );
		}
	}

	return true;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessFaces( IXMLDOMElement * pElement, uint32 context )
{
	Render::Part::FaceList * const	pFaces	= reinterpret_cast< Render::Part::FaceList * >( context );

	HRESULT		hr;
	CComBSTR	tag;

	hr = pElement->get_tagName( &tag );

	if ( tag == CComBSTR( "FaceList" ) )
	{
		CComPtr< IXMLDOMNodeList >	pSubNodeList;
		long						length;

		hr = pElement->get_childNodes( &pSubNodeList );
		hr = pSubNodeList->get_length( &length );

		pFaces->resize( length );

		Msxmlx::ForEachElement( pSubNodeList, ProcessFace, (uint32)pFaces );
	}

	return true;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessFace( IXMLDOMElement * pElement, uint32 context )
{
	Render::Part::FaceList * const	pFaces	= reinterpret_cast< Render::Part::FaceList * >( context );

	HRESULT		hr;
	CComBSTR	tag;

	hr = pElement->get_tagName( &tag );

	if ( tag == CComBSTR( "Face" ) )
	{
		int const		index		= Msxmlx::GetIntAttribute( pElement, "index" );

		if ( index >= 0 && index < (int)pFaces->size() )
		{
			int		aVertexes[3];
			int		aNormals[3];

			GetTriVertexIndexSubElement( pElement, "VertexIndexes", aVertexes );
			GetTriVertexIndexSubElement( pElement, "NormalIndexes", aNormals );

			Render::Face * const	pFace	= new Render::Face( aVertexes[0], aVertexes[1], aVertexes[2],
																aNormals[0], aNormals[1], aNormals[2] );
			(*pFaces)[ index ] = pFace;
		}
	}

	return true;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessMaterials( IXMLDOMElement * pElement, uint32 context )
{
	Render::Model::MaterialList * const	pMaterials	= reinterpret_cast< Render::Model::MaterialList * >( context );

	HRESULT		hr;
	CComBSTR	tag;
	hr = pElement->get_tagName( &tag );

	if ( tag == CComBSTR( "MaterialList" ) )
	{
		CComPtr< IXMLDOMNodeList >	pSubNodeList;
		long						length;

		hr = pElement->get_childNodes( &pSubNodeList );
		hr = pSubNodeList->get_length( &length );

		pMaterials->resize( length );

		Msxmlx::ForEachElement( pSubNodeList, ProcessMaterial, (uint32)pMaterials );
	}

	return true;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessMaterial( IXMLDOMElement * pElement, uint32 context )
{
	Render::Model::MaterialList * const	pMaterials	= reinterpret_cast< Render::Model::MaterialList * >( context );

	HRESULT		hr;
	CComBSTR	tag;
	hr = pElement->get_tagName( &tag );

	if ( tag == CComBSTR( "Material" ) )
	{
		int const	index	= Msxmlx::GetIntAttribute( pElement, "index" );

		if ( index >= 0 && index < (int)pMaterials->size() )
		{
			std::string const	name			= Msxmlx::GetStringAttribute( pElement, "name" );
			D3DXCOLOR const		ambient			= GetRgbaSubElement( pElement, "AmbientColor" );
			D3DXCOLOR const		diffuse			= GetRgbaSubElement( pElement, "DiffuseColor" );
			D3DXCOLOR const		specular		= GetRgbaSubElement( pElement, "SpecularColor" );
			D3DXCOLOR const		emissive		= GetRgbaSubElement( pElement, "EmissiveColor" );
			float const			shininess		= Msxmlx::GetFloatSubElement( pElement, "Shininess" );
			std::string const	diffuseTexture	= Msxmlx::GetStringSubElement( pElement, "DiffuseTexture" );
			std::string const	alphaTexture	= Msxmlx::GetStringSubElement( pElement, "AlphaTexture" );

			// Get the textures

			Render::Material::TextureNameList		textures;

			if ( !diffuseTexture.empty() || !alphaTexture.empty() )
			{
				textures.push_back( diffuseTexture );
				textures.push_back( alphaTexture );
			}
			else
			{
				CComPtr< IXMLDOMNodeList >	pSubNodeList;
				long						length;

				hr = pElement->get_childNodes( &pSubNodeList );
				hr = pSubNodeList->get_length( &length );

				textures.resize( length );

				Msxmlx::ForEachElement( pSubNodeList, ProcessTextureNames, (uint32)&textures );
			}

	//		float const			transparency	= Msxmlx::GetFloatSubElement( pElement, "Transparency", 1.0f );
	//		pMaterial->SetTransparency( transparency );

			(*pMaterials)[ index ] = new Render::Material( name.c_str(),
														   textures,
														   ambient,
														   diffuse,
														   specular,
														   shininess,
														   emissive );
			if ( (*pMaterials)[ index ] == 0 )
			{
				throw std::bad_alloc();
			}
		}
	}

	return true;
}

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessTextureNames( IXMLDOMElement * pElement, uint32 context )
{
	Render::Material::TextureNameList * const	pTextures	= reinterpret_cast< Render::Material::TextureNameList * >( context );

	HRESULT		hr;
	CComBSTR	tag;
	hr = pElement->get_tagName( &tag );

	if ( tag == CComBSTR( "Texture" ) )
	{
		int const	index	= Msxmlx::GetIntAttribute( pElement, "index" );

		if ( index >= 0 && index < (int)pTextures->size() )
		{
			(*pTextures)[ index ] = Msxmlx::GetStringAttribute( pElement, "name" );
		}
	}

	return true;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessBones( IXMLDOMElement * pElement, uint32 context )
{
	BoneInfoList * const	pBoneInfo	= reinterpret_cast< BoneInfoList * >( context );

	HRESULT		hr;
	CComBSTR	tag;
	hr = pElement->get_tagName( &tag );

	if ( tag == CComBSTR( "BoneList" ) )
	{
		CComPtr< IXMLDOMNodeList >	pSubNodeList;
		long						length;

		hr = pElement->get_childNodes( &pSubNodeList );
		hr = pSubNodeList->get_length( &length );

		pBoneInfo->resize( length );

		Msxmlx::ForEachElement( pSubNodeList, ProcessBone, (uint32)pBoneInfo );
	}

	return true;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool ProcessBone( IXMLDOMElement * pElement, uint32 context )
{
	BoneInfoList * const	pBoneInfo	= reinterpret_cast< BoneInfoList * >( context );

	HRESULT		hr;
	CComBSTR	tag;
	hr = pElement->get_tagName( &tag );

	if ( tag == CComBSTR( "Bone" ) )
	{
		int const	index	= Msxmlx::GetIntAttribute( pElement, "index" );

		if ( index >= 0 && index < (int)pBoneInfo->size() )
		{
			BoneInfo * const	pBone	= &(*pBoneInfo)[ index ];	// Convenience

			pBone->name		= Msxmlx::GetStringAttribute( pElement, "name" );
			pBone->parent	= Msxmlx::GetStringSubElement( pElement, "Parent" );
			pBone->position	= GetVector3SubElement( pElement, "Position" );
			pBone->rotation	= GetVector3SubElement( pElement, "Rotation" );
		}
	}

	return true;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

std::auto_ptr< Render::Skeleton > BuildSkeleletonFromBoneInfoList( BoneInfoList const & bones )
{
	Render::Skeleton * const	pSkeleton	= new Render::Skeleton();
	if ( pSkeleton == 0 ) throw std::bad_alloc();

	// First - find the root node and add it to the skeleton

	for ( BoneInfoList::const_iterator pBI = bones.begin(); pBI != bones.end(); ++pBI )
	{
		if ( pBI->parent.empty() )
		{
			D3DXQUATERNION	rotation;
			D3DXQuaternionRotationYawPitchRoll( &rotation, pBI->rotation[1], pBI->rotation[0], pBI->rotation[2] );
			Dxx::Frame const	frame( pBI->position, rotation );

			pSkeleton->Grow( 0, pBI->name.c_str(), pBI->index, frame );
			break;
		}
	}

	// Second - For each bone not already in the skeleton, if its parent is in the skeleton, then add it to the
	// skeleton. Do this until no more bones are added (because they have all been added).

	bool boneWasAdded;
	do
	{
		boneWasAdded = false;

		for ( BoneInfoList::const_iterator pBI = bones.begin(); pBI != bones.end(); ++pBI )
		{
			// If this bone is not already skeleton, then add it if its parent is in the skeleton

			if ( pSkeleton->GetBone( pBI->name.c_str() ) == 0 )
			{
				Render::Bone * const	pParent	= pSkeleton->GetBone( pBI->parent.c_str() );

				// If the bone's parent is in the skeleton, then the bone can be added.

				if ( pParent != 0 )
				{
					D3DXQUATERNION	rotation;
					D3DXQuaternionRotationYawPitchRoll( &rotation, pBI->rotation[1], pBI->rotation[0], pBI->rotation[2] );
					Dxx::Frame const	frame( pBI->position, rotation );

					pSkeleton->Grow( pParent, pBI->name.c_str(), pBI->index, frame );

					boneWasAdded = true;
				}
			}
		}

	} while ( boneWasAdded );

	return std::auto_ptr< Render::Skeleton >( pSkeleton );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

D3DXVECTOR3 GetVector3SubElement( IXMLDOMElement * pElement, char const * sName )
{
	HRESULT						hr;
	CComPtr< IXMLDOMElement >	pSubElement;

	hr = Msxmlx::GetSubElement( pElement, sName, &pSubElement );
	if ( pSubElement != NULL )
	{
		//	<xsd:complexType name="vector3">
		//		<xsd:all>
		//			<xsd:element name="X" type="xsd:float" />
		//			<xsd:element name="Y" type="xsd:float" />
		//			<xsd:element name="Z" type="xsd:float" />
		//		</xsd:all>
		//	</xsd:complexType>

		D3DXVECTOR3 const	v( Msxmlx::GetFloatSubElement( pSubElement, "X" ),
							   Msxmlx::GetFloatSubElement( pSubElement, "Y" ),
							   Msxmlx::GetFloatSubElement( pSubElement, "Z" ) );

		return v;
	}
	else
	{
		return Dxx::Vector3Origin();
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

D3DXCOLOR GetRgbaSubElement( IXMLDOMElement * pElement, char const * sName )
{
	HRESULT						hr;
	CComPtr< IXMLDOMElement >	pSubElement;

	hr = Msxmlx::GetSubElement( pElement, sName, &pSubElement );
	if ( pSubElement != NULL )
	{
		D3DXCOLOR const		rgba( Msxmlx::GetFloatSubElement( pSubElement, "R" ),
								  Msxmlx::GetFloatSubElement( pSubElement, "G" ),
								  Msxmlx::GetFloatSubElement( pSubElement, "B" ),
								  Msxmlx::GetFloatSubElement( pSubElement, "A", 1.0f ) );

		return rgba;
	}
	else
	{
		return D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

D3DXQUATERNION GetQuatSubElement( IXMLDOMElement * pElement, char const * sName )
{
	HRESULT						hr;
	CComPtr< IXMLDOMElement >	pSubElement;

	hr = Msxmlx::GetSubElement( pElement, sName, &pSubElement );
	if ( pSubElement != NULL )
	{
		D3DXQUATERNION	q( Msxmlx::GetFloatSubElement( pSubElement, "X" ),
						   Msxmlx::GetFloatSubElement( pSubElement, "Y" ),
						   Msxmlx::GetFloatSubElement( pSubElement, "Z" ),
						   Msxmlx::GetFloatSubElement( pSubElement, "W" ) );

		return q;
	}
	else
	{
		return Dxx::QuaternionIdentity();
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

D3DXVECTOR4 GetUVSubElement( IXMLDOMElement * pElement, char const * sName )
{
	HRESULT						hr;
	CComPtr< IXMLDOMElement >	pSubElement;

	hr = Msxmlx::GetSubElement( pElement, sName, &pSubElement );
	if ( pSubElement != NULL )
	{
		D3DXVECTOR4 const	uv( Msxmlx::GetFloatSubElement( pSubElement, "U" ),
							    Msxmlx::GetFloatSubElement( pSubElement, "V", 0.0f ),
							    Msxmlx::GetFloatSubElement( pSubElement, "S", 0.0f ),
							    Msxmlx::GetFloatSubElement( pSubElement, "T", 0.0f ) );

		return uv;
	}
	else
	{
		return Dxx::Vector4Origin();
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void GetTriVertexIndexSubElement( IXMLDOMElement * pElement, char const * sName, int aIndexes[3] )
{
	HRESULT						hr;
	CComPtr< IXMLDOMElement >	pSubElement;

	hr = Msxmlx::GetSubElement( pElement, sName, &pSubElement );
	if ( pSubElement != NULL )
	{
		aIndexes[0]	= Msxmlx::GetIntSubElement( pSubElement, "V0", -1 );
		aIndexes[1]	= Msxmlx::GetIntSubElement( pSubElement, "V1", -1 );
		aIndexes[2]	= Msxmlx::GetIntSubElement( pSubElement, "V2", -1 );
	}
	else
	{
		aIndexes[0]	= -1;
		aIndexes[1]	= -1;
		aIndexes[2]	= -1;
	}
}


} // anonymous namespace
