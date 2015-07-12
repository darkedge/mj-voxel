#include "CorePCH.h"
#include "MathUtil.h"

mj::math::mat4 mj::math::Perspective( float fovy, float aspect, float zNear, float zFar )
{
	//assert( abs( aspect - std::numeric_limits<T>::epsilon() ) > static_cast<T>( 0 ) );

	float const tanHalfFovy = tan( fovy / 2.0f );

	mj::math::mat4 result( 0.0f );
	result[0][0] = 1.0f / ( aspect * tanHalfFovy );
	result[1][1] = 1.0f / ( tanHalfFovy );
	result[2][2] = -( zFar + zNear ) / ( zFar - zNear );
	result[2][3] = -1.0f;
	result[3][2] = -( 2.0f * zFar * zNear ) / ( zFar - zNear );
	return result;
}
