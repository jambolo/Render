/** @file *//********************************************************************************************************

                                                      Bone.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/Bone.cpp#4 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "PrecompiledHeaders.h"

#include "Bone.h"

#include "Skeleton.h"
#include "Dxx/Frame.h"


namespace Render
{

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Bone::Bone()
	: m_pParent( 0 ),
	m_Index( -1 )
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Bone::Bone( char const * sName, Dxx::Frame const & frame/* = Dxx::Frame::Identity()*/ )
	: m_Name( sName ),
	m_pParent( 0 ),
	m_LocalFrame( frame ),
	m_WorldFrame( frame ),
	m_Index( -1 )
{
	D3DXMATRIX	inverse;

	D3DXMatrixInverse( &inverse, NULL, &frame.GetTransformationMatrix() );
	m_InverseWorldFrame0.SetTransformationMatrix( inverse );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Bone::~Bone()
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

std::auto_ptr< Bone > Bone::Clone( Bone * pParent/* = 0*/ ) const
{
	Bone * const	pClone	= new Bone( m_Name.c_str() );
	if ( pClone == 0 ) throw std::bad_alloc();

	pClone->SetParent( pParent );

	for ( BoneList::const_iterator ppC = m_Children.begin(); ppC != m_Children.end(); ++ppC )
	{
		pClone->AddChild( (*ppC)->Clone( pClone ).release() );
	}

	return std::auto_ptr< Bone >( pClone );
}

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Bone::AddChild( Bone * pBone )
{
	m_Children.push_back( pBone );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Bone::RemoveChild( Bone * pBone )
{
	for ( BoneList::iterator ppC = m_Children.begin(); ppC != m_Children.end(); ++ppC )
	{
		if ( *ppC == pBone )
		{
			m_Children.erase( ppC );
			break;
		}
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Dxx::Frame const & Bone::Pose()
{
	if ( m_pParent != 0 )
	{
		D3DXMATRIX const &	local			= m_LocalFrame.GetTransformationMatrix();
		D3DXMATRIX const &	parentsWorld	= m_pParent->GetWorldFrame().GetTransformationMatrix();

		m_WorldFrame.SetTransformationMatrix( local * parentsWorld );

		for ( BoneList::iterator ppC = m_Children.begin(); ppC != m_Children.end(); ++ppC )
		{
			(*ppC)->Pose();
		}
	}
	else
	{
		m_WorldFrame = m_LocalFrame;
	}

	return m_WorldFrame;
}



} // namespace Render

