#pragma once

namespace mj {
class Transform {
public:
	// Transforms position from local space to world space.
	math::Vector3 TransformPoint( const math::Vector3 &point ) const;
	// Transforms direction from local space to world space.
	math::Vector3 TransformDirection( const math::Vector3 &direction ) const;
	// Transforms vector from local space to world space.
	math::Vector3 TransformVector( const math::Vector3 &vector ) const;

	// Returns the matrix that transforms a point from local space into world space.
	math::Matrix4x4 GetLocalToWorldMatrix() const;
	// Returns the matrix that transforms a point from world space into local space.
	math::Matrix4x4 GetWorldToLocalMatrix() const;

	// Returns the position of the transform relative to the parent transform.
	math::Vector3 GetPosition() const { return position; }
	// Returns the rotation of the transform relative to the parent transform's rotation.
	math::Quaternion GetRotation() const { return rotation; }

	math::Vector3 Forward() const {
		return rotation * mj::math::kAxisZ3f;
	}

	math::Vector3 Up() const {
		return rotation * mj::math::kAxisY3f;
	}

	math::Vector3 Right() const {
		return rotation * mj::math::kAxisX3f;
	}

	// Sets the position of the transform in world space.
	void SetPosition( float x, float y, float z );
	void SetPosition( const math::Vector3 &_position );

	// Sets the rotation of the transform in world space.
	void SetRotation( float pitch, float yaw, float roll );
	void SetRotation( const math::Vector3 &degrees );
	void SetRotation( const math::Quaternion &_orientation );

private:
	math::Vector3 position;
	math::Quaternion rotation;

	mutable bool matrixIsDirty = true;
	mutable math::Matrix4x4 localMatrix;
};
} // namespace mj
