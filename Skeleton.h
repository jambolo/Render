/** @file *//********************************************************************************************************

                                                     Skeleton.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/Skeleton.h#6 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once

#include "Dxx/Frame.h"
#include <vector>
#include <string>
#include <memory>

struct IDirect3DDevice9;


/// @defgroup	SkeletalAnimation	Skeletal Animation
/// @ingroup	Render


namespace Render
{

class Bone;
class BoundSkeleton;

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

/// A generic skeleton
//
/// @ingroup	SkeletalAnimation
///
/// A skeleton is a container of Bones with the following properties:
///
///		- The Bones form a hierarchy.
///		- A Bone can be referenced by its index.
///		- A Bone can be referenced by its name.
///		- A Skeleton can be attached. Attaching a skeleton creates a copy of it.
///		- A Bone can be added.
///		- A Bone can be removed.
///		- When a Bone is removed, all Bones in its sub-hierarchy are removed.
///		- Indexes are not in any particular order.
///		- Some indexes may be unused.
///		- An Bone's index only changes when a Skeleton containing it is attached to another Skeleton.

class Skeleton
{
public:

	typedef std::vector< Bone * >	BoneList;

	/// Default Constructor
	Skeleton();

	/// Constructor
	Skeleton( std::auto_ptr< Bone > qBone );

	/// Copy constructor
	Skeleton( Skeleton const & a );

	/// Destructor
	virtual ~Skeleton();

	/// Assignment operator
	Skeleton const & operator=( Skeleton const & a );

	/// Updates all the bones
	void Pose() const;

	/// Sets up D3D to use this skeleton for rendering
	void Apply( IDirect3DDevice9 * pD3dDevice ) const;

	/// Returns the root bone
	Bone * GetRoot()										{ return m_pRoot; }

	/// Returns the root bone
	Bone const * GetRoot() const							{ return m_pRoot; }

	/// Adds a bone to the skeleton. Returns a pointer to the added bone or 0.
	Bone * Grow( Bone * pParent, char const * sName = 0, int index = -1, Dxx::Frame frame = Dxx::Frame::Identity() );

	/// Attaches a skeleton
	Bone * Attach( Skeleton * pSkeleton, Bone * pParent );

	/// Attaches a bone hierarchy
	Bone * Attach( std::auto_ptr< Bone > qBone, Bone * pParent );

	/// Returns the bone at the specified index
	Bone * GetBone( int index );

	/// Returns the bone at the specified index
	Bone const * GetBone( int index ) const;

	/// Returns the bone with the specified name
	Bone * GetBone( char const * sName );

	/// Returns the bone with the specified name
	Bone const * GetBone( char const * sName ) const;

private:

	/// Attaches a bone hierarchy
	void Skeleton::AttachBoneHierarchy( Bone * pBone );

	Bone *			m_pRoot;
	BoneList		m_Bones;
};


} // namespace Render
