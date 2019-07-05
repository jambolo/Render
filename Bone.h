/** @file *//********************************************************************************************************

                                                       Bone.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Libraries/Render/Bone.h#5 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once

#include "Dxx/Frame.h"

#include <vector>
#include <string>
#include <boost/noncopyable.hpp>


namespace Render
{

class Skeleton;

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

/// A generic bone
//
/// @ingroup	SkeletalAnimation
///

class Bone : public boost::noncopyable
{
	friend class Skeleton;

public:

	typedef std::vector< Bone * >	BoneList;	/// An array of bone pointers

	/// Default constructor
	Bone();

	/// Constructor
	Bone( char const * sName, Dxx::Frame const & frame = Dxx::Frame::Identity() ); 

	/// Destructor
	virtual ~Bone();

	/// Clones the hierarchy rooted at this bone. Returns the hierarchy clone.
	std::auto_ptr< Bone > Clone( Bone * pParent = 0 ) const;

	/// Returns the bone's name
	std::string const & GetName() const						{ return m_Name; }

	/// Returns the bone's parent
	Bone * GetParent() const								{ return m_pParent; }

	/// Returns the bone's frame
	Dxx::Frame const & GetFrame() const						{ return m_LocalFrame; }

	/// Sets the bone's frame
	void SetFrame( Dxx::Frame const & frame )				{ m_LocalFrame = frame; }

	/// Returns the bone's children
	BoneList const & GetChildren() const					{ return m_Children; }

	/// Transforms the bone's local frame into world space. Returns the transformed frame.
	Dxx::Frame const & Pose();

	/// Returns the bone's frame in world space
	Dxx::Frame const & GetWorldFrame() const				{ return m_WorldFrame; }

	/// Returns the inverse of the bone's original frame in world space
	Dxx::Frame const & GetInverseWorldFrame0() const		{ return m_InverseWorldFrame0; }

	/// Returns this bones index
	int GetIndex() const									{ return m_Index; }

private:

	/// Adds a child to the bone
	void AddChild( Bone * pBone );

	/// Removes a child from the bone
	void RemoveChild( Bone * pBone );

	/// Sets the parent bone
	void SetParent( Bone * pParent )						{ m_pParent = pParent; }

	/// Sets the the index
	void SetIndex( int index )								{ m_Index = index; }

	std::string				m_Name;
	Bone *					m_pParent;
	Dxx::Frame				m_LocalFrame;
	Dxx::Frame				m_WorldFrame;
	Dxx::Frame				m_InverseWorldFrame0;
	BoneList				m_Children;
	int						m_Index;
};


} // namespace Render
