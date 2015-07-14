#pragma once

namespace mj {
class Transform
{
public:
	Transform();

	// Transforms position from local space to world space.
	math::float3 TransformPoint( const math::float3 &point ) const;
	// Transforms direction from local space to world space.
	math::float3 TransformDirection( const math::float3 &direction ) const;
	// Transforms vector from local space to world space.
	math::float3 TransformVector( const math::float3 &vector ) const;

	// Returns the matrix that transforms a point from local space into world space.
	math::mat4 GetLocalToWorldMatrix() const;
	// Returns the matrix that transforms a point from world space into local space.
	math::mat4 GetWorldToLocalMatrix() const;

	// Returns the position of the transform relative to the parent transform.
	math::float3 GetPosition() const { return position; }
	// Returns the rotation of the transform relative to the parent transform's rotation.
	math::quat GetRotation() const { return rotation; }

	math::float3 Forward() const
	{
		return rotation * -mj::math::kAxisZ3f;
	}

	math::float3 Up() const
	{
		return rotation * mj::math::kAxisY3f;
	}

	math::float3 Right() const
	{
		return rotation * mj::math::kAxisX3f;
	}

	// Sets the position of the transform in world space.
	void SetPosition( float x, float y, float z );
	void SetPosition( const math::float3 &_position );

	// Sets the rotation of the transform in world space.
	void SetRotation( float pitch, float yaw, float roll );
	void SetRotation( const math::float3 &degrees );
	void SetRotation( const math::quat &_orientation );

private:
	math::float3 position;
	math::quat rotation;

	mutable bool matrixIsDirty = true;
	mutable math::mat4 localMatrix;
};
} // namespace mj
