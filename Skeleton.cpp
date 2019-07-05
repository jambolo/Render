/** @file *//********************************************************************************************************

                                                    Skeleton.cpp

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/Skeleton.cpp#5 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "PrecompiledHeaders.h"

#include "Skeleton.h"

#include "Bone.h"
#include "BoundSkeleton.h"

#include "Dxx/Frame.h"

namespace Render
{


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Skeleton::Skeleton()
	: m_pRoot( 0 )
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Skeleton::Skeleton( std::auto_ptr< Bone > qBone )
{
	Attach( qBone, 0 );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Skeleton::Skeleton( Skeleton const & a )
{
	Attach( a.m_pRoot->Clone(), 0 );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Skeleton::~Skeleton()
{
	for ( BoneList::iterator ppBone = m_Bones.begin(); ppBone != m_Bones.end(); ++ppBone )
	{
		delete *ppBone;
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Skeleton const & Skeleton::operator =( Skeleton const & a )
{
	// Remove old skeleton

	for ( BoneList::iterator ppBone = m_Bones.begin(); ppBone != m_Bones.end(); ++ppBone )
	{
		delete *ppBone;
	}

	m_pRoot = 0;
	m_Bones.clear();

	// Build a new skeleton

	Attach( a.m_pRoot->Clone(), 0 );

	return *this;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Skeleton::Pose() const
{
	m_pRoot->Pose();
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Skeleton::Apply( IDirect3DDevice9 * pD3dDevice ) const
{
	int		index	= 0;

	for ( BoneList::const_iterator ppB = m_Bones.begin(); ppB != m_Bones.end(); ++ppB )
	{
		Bone const * const	pBone	= *ppB;
		Dxx::Frame const &	frame	= pBone->GetFrame();
		D3DXMATRIX skinningMatrix;

		D3DXMatrixMultiply( &skinningMatrix,
							&pBone->GetInverseWorldFrame0().GetTransformationMatrix(),
							&pBone->GetWorldFrame().GetTransformationMatrix() );

		pD3dDevice->SetTransform( D3DTS_WORLDMATRIX( index ), &skinningMatrix );
		++index;
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Bone * Skeleton::Grow( Bone * pParent,
					   char const * sName/* = 0*/,
					   int index/* = -1*/,
					   Dxx::Frame frame/* = Dxx::Frame::Identity()*/ )
{
	// If a root node is being added and there already is a root node, then return error.

	if ( pParent == 0 && m_pRoot != 0 )
	{
		return 0;
	}

	// If the index is not specified, then use the first unused index

	if ( index < 0 )
	{
		index = 0;
		while ( index < (int)m_Bones.size() && m_Bones[ index ] != 0 )
		{
			++index;
		}
	}

	// Grow the bone array if necessary to add a bone at this index

	if ( (int)m_Bones.size() <= index )
	{
		m_Bones.resize( index+1, 0 );
	}

	// If the index is already used, return an error

	if ( m_Bones[ index ] != 0 )
	{
		return 0;
	}

	// Create the bone

	m_Bones[ index ] = new Bone( sName, frame );

	// Set its parent bone and its index

	m_Bones[ index ]->SetParent( pParent );
	m_Bones[ index ]->SetIndex( index );

	// Add the new bone as a child of the parent (unless it is a root node)

	if ( pParent != 0 )
	{
		pParent->AddChild( m_Bones[ index ] );
	}
	else
	{
		m_pRoot = m_Bones[ index ];
	}

	return m_Bones[ index ];
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Bone * Skeleton::GetBone( int index )
{
	return const_cast< Bone * >( const_cast< Skeleton const *>(this)->GetBone( index ) );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Bone const * Skeleton::GetBone( int index ) const
{
	assert( index >= 0 && index < (int)m_Bones.size() );

	return m_Bones[ index ];
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Bone * Skeleton::GetBone( char const * sName )
{
	return const_cast< Bone * >( const_cast< Skeleton const *>(this)->GetBone( sName ) );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Bone const * Skeleton::GetBone( char const * sName ) const
{
	for ( BoneList::const_iterator ppBone = m_Bones.begin(); ppBone != m_Bones.end(); ++ppBone )
	{
		if ( strcmp( sName, (*ppBone)->GetName().c_str() ) == 0 )
		{
			return *ppBone;
		}
	}

	// Not found

	return 0;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Bone * Skeleton::Attach( Skeleton * pSkeleton, Bone * pParent )
{
	Bone * const	pRoot	= pSkeleton->m_pRoot;

	// Remove all bones from the skeleton that is being attached

	pSkeleton->m_Bones.clear();
	pSkeleton->m_pRoot = 0;

	// Attach the bones to this skeleton

	return Attach( std::auto_ptr< Bone >( pRoot ), pParent );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Bone * Skeleton::Attach( std::auto_ptr< Bone >  qBone, Bone * pParent )
{
	// If a root node is being added and there already is a root node, then return error.

	if ( pParent == 0 && m_pRoot != 0 )
	{
		return 0;
	}

	AttachBoneHierarchy( qBone.get() );

	// Set its parent bone

	qBone->SetParent( pParent );

	// Add the new bone as a child of the parent (unless it is a root node)

	if ( pParent != 0 )
	{
		pParent->AddChild( qBone.get() );
	}
	else
	{
		m_pRoot = qBone.get();
	}

	// Attach its children

	for ( BoneList::const_iterator ppChild = qBone->GetChildren().begin(); ppChild != qBone->GetChildren().end(); ++ppChild )
	{
		AttachBoneHierarchy( *ppChild );
	}

	// Take ownership and return the address to indicate success

	return qBone.release();
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Skeleton::AttachBoneHierarchy( Bone * pBone )
{
	// Find the first unused index

	int		index = 0;
	while ( index < (int)m_Bones.size() && m_Bones[ index ] != 0 )
	{
		++index;
	}

	// Grow the bone array if necessary to add a bone at this index

	if ( (int)m_Bones.size() <= index )
	{
		m_Bones.resize( index+1, 0 );
	}

	// Add the bone

	m_Bones[ index ] = pBone;
	pBone->SetIndex( index );

	// Now attach its children

	for ( BoneList::const_iterator ppChild = pBone->GetChildren().begin(); ppChild != pBone->GetChildren().end(); ++ppChild )
	{
		AttachBoneHierarchy( *ppChild );
	}
}


} // namespace Render

