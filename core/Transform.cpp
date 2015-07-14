#include "CorePCH.h"
#include "Transform.h"

mj::Transform::Transform() :
position( 0, 0, 0 ),
rotation( 0, 0, 1, 0 ),
localMatrix( 1 )
{
}

void mj::Transform::SetPosition( float x, float y, float z )
{
	SetPosition( mj::math::float3( x, y, z ) );
}

void mj::Transform::SetPosition( const mj::math::float3 &position )
{
	this->position = position;
	matrixIsDirty = true;
}

void mj::Transform::SetRotation( const mj::math::quat &orientation )
{
	this->rotation = orientation;
	matrixIsDirty = true;
}

// Degrees
void mj::Transform::SetRotation( float pitch, float yaw, float roll )
{
	SetRotation( mj::math::float3( pitch, yaw, roll ) );
}

// Degrees
void mj::Transform::SetRotation( const mj::math::float3 &degrees )
{
	SetRotation( mj::math::quat( mj::math::Deg2Rad( degrees ) ) );
}

mj::math::mat4 mj::Transform::GetLocalToWorldMatrix() const
{
	if ( matrixIsDirty )
	{
		localMatrix = mj::math::TranslationMatrix( position )
			* mj::math::RotationMatrix( rotation );
		matrixIsDirty = false;
	}

	return localMatrix;
}

mj::math::mat4 mj::Transform::GetWorldToLocalMatrix() const
{
	return mj::math::Inverse( GetLocalToWorldMatrix() );
}

mj::math::float3 mj::Transform::TransformPoint( const mj::math::float3 &point ) const
{
	return mj::math::float3( GetLocalToWorldMatrix() * mj::math::float4( point, 1 ) );
}

mj::math::float3 mj::Transform::TransformDirection( const mj::math::float3 &direction ) const
{
	return mj::math::Normalize( GetRotation() * direction );
}

mj::math::float3 mj::Transform::TransformVector( const mj::math::float3 &vector ) const
{
	return mj::math::float3( GetLocalToWorldMatrix() * mj::math::float4( vector, 0 ) );
}
