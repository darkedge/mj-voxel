#pragma once
#include <limits>

#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union

namespace mj {
namespace math {
// Forward declarations
template <typename T> struct vec2_t;
template <typename T> struct vec3_t;
template <typename T> struct vec4_t;
template <typename T> struct mat4x4_t;
template <typename T> struct mat4x3_t;
template <typename T> struct mat3x3_t;
template <typename T> struct quat_t;

/************************************************************************/
/* Vector2                                                              */
/************************************************************************/
template<typename T>
struct vec2_t
{
	vec2_t() {}
	explicit vec2_t( T s ) : x( s ), y( s ) {}
	vec2_t( T x, T y ) : x( x ), y( y ) {}
	int32 Size() const
	{
		return 2;
	}
	union
	{
		T value[2];
		struct { T x, y; };
	};
	T &operator[]( const int32 &i )
	{
		assert( i >= 0 );
		assert( i < 2 );
		return value[i];
	}
	const T &operator[]( const int32 &i ) const
	{
		assert( i >= 0 );
		assert( i < 2 );
		return value[i];
	}

	template <typename T>
	vec2_t<T>& operator+=( const vec2_t<T> &v )
	{
		this->x += static_cast<T>( v.x );
		this->y += static_cast<T>( v.y );
		return *this;
	}

	template <typename T>
	vec2_t<T>& operator-=( const vec2_t<T> &v )
	{
		this->x -= static_cast<T>( v.x );
		this->y -= static_cast<T>( v.y );
		return *this;
	}
};

// Scalar multiplication (post-fix)
template <typename T>
vec2_t<T> operator*( vec2_t<T> const &v, T const &s )
{
	return vec2_t<T>(
		v.x * s,
		v.y * s );
}
// Scalar multiplication (pre-fix)
template <typename T>
vec2_t<T> operator*( T const &s, vec2_t<T> const &v )
{
	return vec2_t<T>(
		s * v.x,
		s * v.y );
}
// Component-wise multiplication
template <typename T>
vec2_t<T> operator*( vec2_t<T> const &v1, vec2_t<T> const &v2 )
{
	return vec2_t<T>(
		v1.x * v2.x,
		v1.y * v2.y );
}
// Component-wise subtraction
template <typename T>
vec2_t<T> operator-( vec2_t<T> const &v1, vec2_t<T> const &v2 )
{
	return vec2_t<T>(
		v1.x - v2.x,
		v1.y - v2.y );
}

/************************************************************************/
/* Vector3                                                              */
/************************************************************************/
template<typename T>
struct vec3_t
{
	vec3_t() {}
	explicit vec3_t( T s ) : x( s ), y( s ), z( s ) {}
	vec3_t( T x, T y, T z ) : x( x ), y( y ), z( z ) {}
	explicit vec3_t( vec4_t<T> xyz ) : x( xyz.x ), y( xyz.y ), z( xyz.z ) {}
	int32 Size() const
	{
		return 3;
	}
	union
	{
		T value[3];
		struct { T x, y, z; };
	};
	T &operator[]( const int32 &i )
	{
		assert( i >= 0 );
		assert( i < 3 );
		return value[i];
	}
	const T &operator[]( const int32 &i ) const
	{
		assert( i >= 0 );
		assert( i < 3 );
		return value[i];
	}

	template <typename T>
	vec3_t<T>& operator+=( const vec3_t<T> &v )
	{
		this->x += static_cast<T>( v.x );
		this->y += static_cast<T>( v.y );
		this->z += static_cast<T>( v.z );
		return *this;
	}

	template <typename T>
	vec3_t<T>& operator-=( const vec3_t<T> &v )
	{
		this->x -= static_cast<T>( v.x );
		this->y -= static_cast<T>( v.y );
		this->z -= static_cast<T>( v.z );
		return *this;
	}

	template <typename T>
	bool operator==( const vec3_t<T> &v ) const
	{
		return this->x == static_cast<T>( v.x )
			&& this->y == static_cast<T>( v.y )
			&& this->z == static_cast<T>( v.z );
	}

	template <typename T>
	bool operator!=( const vec3_t<T> &v ) const
	{
		return !( *this == v );
	}

	// Casting
	template <typename R>
	operator vec3_t<R>() const
	{
		return vec3_t<R>(
			static_cast<T>( x ),
			static_cast<T>( y ),
			static_cast<T>( z ) );
	}

	template<typename R, typename T>
	vec3_t<T> Call( R( *Func )( T ) ) const
	{
		return vec3_t<T>( Func( x ), Func( y ), Func( z ) );
	}
};
// Dot product
template <typename T>
T Dot( vec3_t<T> const &x, vec3_t<T> const &y )
{
	vec3_t<T> tmp( x * y );
	return tmp.x + tmp.y + tmp.z;
}
// Cross product
template<typename T>
vec3_t<T> Cross( vec3_t<T> x, vec3_t<T> y )
{
	static_assert( std::numeric_limits<T>::is_iec559, "'Cross' only accepts floating-point inputs!" );
	return vec3_t<T>(
		x.y * y.z - y.y * x.z,
		x.z * y.x - y.z * x.x,
		x.x * y.y - y.x * x.y );
}
// Scalar multiplication (post-fix)
template <typename T>
vec3_t<T> operator*( vec3_t<T> const &v, T const &s )
{
	return vec3_t<T>(
		v.x * s,
		v.y * s,
		v.z * s );
}
// Scalar multiplication (pre-fix)
template <typename T>
vec3_t<T> operator*( T const &s, vec3_t<T> const &v )
{
	return vec3_t<T>(
		s * v.x,
		s * v.y,
		s * v.z );
}
// Component-wise multiplication
template <typename T>
vec3_t<T> operator*( vec3_t<T> const &v1, vec3_t<T> const &v2 )
{
	return vec3_t<T>(
		v1.x * v2.x,
		v1.y * v2.y,
		v1.z * v2.z );
}
// Component-wise addition
template <typename T>
vec3_t<T> operator+( vec3_t<T> const &v1, vec3_t<T> const &v2 )
{
	return vec3_t<T>(
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.z );
}
// Component-wise subtraction
template <typename T>
vec3_t<T> operator-( vec3_t<T> const &v1, vec3_t<T> const &v2 )
{
	return vec3_t<T>(
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z );
}
// Unary negation
template <typename T>
vec3_t<T> operator-( vec3_t<T> const &v )
{
	return vec3_t<T>(
		-v.x,
		-v.y,
		-v.z );
}
/************************************************************************/
/* Vector4                                                              */
/************************************************************************/
template<typename T>
struct vec4_t
{
	vec4_t() {}
	explicit vec4_t( T s ) : x( s ), y( s ), z( s ), w( s ) {}
	explicit vec4_t( vec3_t<T> xyz, T w ) : x( xyz.x ), y( xyz.y ), z( xyz.z ), w( w ) {}
	vec4_t( T x, T y, T z, T w ) : x( x ), y( y ), z( z ), w( w ) {}
	int32 Size() const
	{
		return 4;
	}
	union
	{
		T value[4];
		struct { T x, y, z, w; };
	};
	T &operator[]( const int32 &i )
	{
		assert( i >= 0 );
		assert( i < 4 );
		return value[i];
	}
	const T &operator[]( const int32 &i ) const
	{
		assert( i >= 0 );
		assert( i < 4 );
		return value[i];
	}
	template<typename R, typename T>
	void Call( R( *Func )( T ) ) const
	{
		return vec3_t( Func( x ), Func( y ), Func( z ), Func( w ) );
	}
};
// Scalar multiplication (post-fix)
template <typename T>
vec4_t<T> operator*( vec4_t<T> const &v, T const &s )
{
	return vec4_t<T>(
		v.x * s,
		v.y * s,
		v.z * s,
		v.w * s );
}
// Scalar multiplication (pre-fix)
template <typename T>
vec4_t<T> operator*( T const &s, vec4_t<T> const &v )
{
	return vec4_t<T>(
		s * v.x,
		s * v.y,
		s * v.z,
		s * v.w );
}
// Component-wise multiplication
template <typename T>
vec4_t<T> operator*( vec4_t<T> const &v1, vec4_t<T> const &v2 )
{
	return vec4_t<T>(
		v1.x * v2.x,
		v1.y * v2.y,
		v1.z * v2.z,
		v1.w * v2.w );
}
// Component-wise addition
template <typename T>
vec4_t<T> operator+( vec4_t<T> const &v1, vec4_t<T> const &v2 )
{
	return vec4_t<T>(
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.z,
		v1.w + v2.w );
}
// Component-wise subtraction
template <typename T>
vec4_t<T> operator-( vec4_t<T> const &v1, vec4_t<T> const &v2 )
{
	return vec4_t<T>(
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z,
		v1.w - v2.w );
}
template <typename T>
T Dot( vec4_t<T> const &x, vec4_t<T> const &y )
{
	vec4_t<T> tmp( x * y );
	return ( tmp.x + tmp.y ) + ( tmp.z + tmp.w );
}
/************************************************************************/
/* Matrix3x3                                                            */
/************************************************************************/
template<typename T>
struct mat3x3_t
{
	typedef vec3_t<T> Column;
	mat3x3_t( quat_t<T> q )
	{
		T x2( q.x * q.x );
		T y2( q.y * q.y );
		T z2( q.z * q.z );
		T xz( q.x * q.z );
		T xy( q.x * q.y );
		T yz( q.y * q.z );
		T wx( q.w * q.x );
		T wy( q.w * q.y );
		T wz( q.w * q.z );

		value[0][0] = 1 - 2 * ( y2 + z2 ); // neg?
		value[0][1] = 2 * ( xy + wz );
		value[0][2] = 2 * ( xz - wy );

		value[1][0] = 2 * ( xy - wz );
		value[1][1] = 1 - 2 * ( x2 + z2 );
		value[1][2] = 2 * ( yz + wx );

		value[2][0] = 2 * ( xz + wy );
		value[2][1] = 2 * ( yz - wx );
		value[2][2] = 1 - 2 * ( x2 + y2 ); // neg?
	}

	Column value[3];

	Column &operator[]( const int32 &i )
	{
		assert( i >= 0 );
		assert( i < 3 );
		return value[i];
	}
	const Column &operator[]( const int32 &i ) const
	{
		assert( i >= 0 );
		assert( i < 3 );
		return value[i];
	}
};
/************************************************************************/
/* Matrix4x4                                                            */
/************************************************************************/
template<typename T>
struct mat4x4_t
{
	typedef vec4_t<T> Column;
	// Creates a 4x4 matrix with uninitialized values.
	mat4x4_t() {}
	// Creates a 4x4 rotation matrix from a quaternion.
	mat4x4_t( quat_t<T> q )
	{
		*this = mat4x4_t<T>( mat3x3_t<T>( q ) );
	}
	// Creates a 4x4 matrix from a 3x3 matrix.
	mat4x4_t( const mat3x3_t<T> &m )
	{
		this->value[0] = Column( m[0], 0 );
		this->value[1] = Column( m[1], 0 );
		this->value[2] = Column( m[2], 0 );
		this->value[3] = Column( 0, 0, 0, T( 1 ) );
	}
	// Identity matrix, scaled by s.
	mat4x4_t( T s )
	{
		this->value[0] = Column( s, 0, 0, 0 );
		this->value[1] = Column( 0, s, 0, 0 );
		this->value[2] = Column( 0, 0, s, 0 );
		this->value[3] = Column( 0, 0, 0, s );
	}
	mat4x4_t( const Column &a, const Column &b, const Column &c, const Column &d )
	{
		this->value[0] = a;
		this->value[1] = b;
		this->value[2] = c;
		this->value[3] = d;
	}
	Column value[4];
	Column &operator[]( const int32 &i )
	{
		assert( i >= 0 );
		assert( i < 4 );
		return value[i];
	}
	const Column &operator[]( const int32 &i ) const
	{
		assert( i >= 0 );
		assert( i < 4 );
		return value[i];
	}
};
template <typename T>
mat4x4_t<T> operator*( const mat4x4_t<T> &m, const T &s )
{
	return mat4x4_t<T>(
		m[0] * s,
		m[1] * s,
		m[2] * s,
		m[3] * s );
}
template <typename T>
mat4x4_t<T> operator*( const T &s, const mat4x4_t<T> &m )
{
	return mat4x4_t<T>(
		m[0] * s,
		m[1] * s,
		m[2] * s,
		m[3] * s );
}
/************************************************************************/
/* Quaternion                                                           */
/************************************************************************/
template<typename T>
struct quat_t
{
	quat_t() {}
	quat_t( T x, T y, T z, T w ) : x( x ), y( y ), z( z ), w( w ) {}
	// Construct a Quaternion from Euler angles (radians)
	quat_t( vec3_t<T> eulerAngles )
	{
		vec3_t<T> c = Cos( eulerAngles * T( 0.5 ) );
		vec3_t<T> s = Sin( eulerAngles * T( 0.5 ) );

		this->x = s.x * c.y * c.z - c.x * s.y * s.z;
		this->y = c.x * s.y * c.z + s.x * c.y * s.z;
		this->z = c.x * c.y * s.z - s.x * s.y * c.z;
		this->w = c.x * c.y * c.z + s.x * s.y * s.z;
	}
	T x, y, z, w;
};

template <typename T>
quat_t<T> operator/( quat_t<T> const & q, T const & s )
{
	return quat_t<T>( q.x / s, q.y / s, q.z / s, q.w / s );
}

// Quaternion * Quaternion
template <typename T>
quat_t<T> operator*( quat_t<T> const &p, quat_t<T> const &q )
{
	quat_t<T> qr;
	qr.x = p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y;
	qr.y = p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z;
	qr.z = p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x;
	qr.w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
	return qr;
}

// Quaternion * Axis
template <typename T>
vec3_t<T> operator*( quat_t<T> const &q, vec3_t<T> const &v )
{
	vec3_t<T> const QuatVector( q.x, q.y, q.z );
	vec3_t<T> const uv( Cross( QuatVector, v ) );
	vec3_t<T> const uuv( Cross( QuatVector, uv ) );

	return v + ( ( uv * q.w ) + uuv ) * static_cast<T>( 2 );
}

#if 0
template <typename T>
quat_t<T> Inverse( quat_t<T> const &q )
{
	return Conjugate( q ) / Dot( q, q );
}

template <typename T>
quat_t<T> Conjugate( quat_t<T> const & q )
{
	return quat_t<T>( -q.x, -q.y, -q.z, q.w );
}

template <typename T>
T Dot( quat_t<T> const & x, quat_t<T> const & y )
{
	vec4_t<T> tmp( x.x * y.x, x.y * y.y, x.z * y.z, x.w * y.w );
	return ( tmp.x + tmp.y ) + ( tmp.z + tmp.w );
}

template <typename T>
quat_t<T> operator-( quat_t<T> const & q )
{
	return quat_t<T>( -q.x, -q.y, -q.z, -q.w );
}

template <typename T>
T Length( quat_t<T> const & q )
{
	return sqrt( Dot( q, q ) );
}

template<typename T>
quat_t<T> Normalize( const quat_t<T> &q )
{
	T len = Length( q );
	assert( len > 0 );
	T oneOverLen = T( 1 ) / len;
	return quat_t<T>( q.w * oneOverLen, q.x * oneOverLen, q.y * oneOverLen, q.z * oneOverLen );
}
#endif

/************************************************************************/
/* TYPEDEFS                                                             */
/************************************************************************/
typedef vec2_t<float> float2;
typedef vec3_t<float> float3;
typedef vec4_t<float> float4;
typedef vec2_t<int32> int2;
typedef vec3_t<int32> int3;
typedef vec4_t<int32> int4;
typedef mat4x4_t<float> mat4;
typedef quat_t<float> quat;

/************************************************************************/
/* CONSTANTS                                                            */
/************************************************************************/
static const float2 kZeros2f( 0.0f, 0.0f );
static const float2 kOnes2f( 1.0f, 1.0f );
static const float2 kAxisX2f( 1.0f, 0.0f );
static const float2 kAxisY2f( 0.0f, 1.0f );

static const float3 kZeros3f( 0.0f, 0.0f, 0.0f );
static const float3 kOnes3f( 1.0f, 1.0f, 1.0f );
static const float3 kAxisX3f( 1.0f, 0.0f, 0.0f );
static const float3 kAxisY3f( 0.0f, 1.0f, 0.0f );
static const float3 kAxisZ3f( 0.0f, 0.0f, 1.0f );

static const float4 kZeros4f( 0.0f, 0.0f, 0.0f, 0.0f );
static const float4 kOnes4f( 1.0f, 1.0f, 1.0f, 1.0f );
static const float4 kAxisX4f( 1.0f, 0.0f, 0.0f, 0.0f );
static const float4 kAxisY4f( 0.0f, 1.0f, 0.0f, 0.0f );
static const float4 kAxisZ4f( 0.0f, 0.0f, 1.0f, 0.0f );
static const float4 kAxisW4f( 0.0f, 0.0f, 0.0f, 1.0f );

// TODO: Maybe specialize these using the values above
template<typename T>
T Zero()
{
	return T( 0 );
}

template<typename T>
T One()
{
	return T( 1 );
}

template<typename T = float>
T Pi()
{
	return T( 3.14159265358979323846264338327950288 );
}

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
// radians
template <typename T>
T Deg2Rad( T degrees )
{
	static_assert( std::numeric_limits<T>::is_iec559, "'Deg2Rad' only accepts floating-point input!" );
	return degrees * static_cast<T>( 0.01745329251994329576923690768489 );
}

template <typename T, template <typename> class V>
V<T> Deg2Rad( V<T> const &v )
{
	return v.Call( Deg2Rad<T> );
}

// degrees
template <typename T>
T Rad2Deg( T radians )
{
	static_assert( std::numeric_limits<T>::is_iec559, "'Rad2Deg' only accepts floating-point input!" );
	return radians * static_cast<T>( 57.295779513082320876798154814105 );
}

template <typename T, template <typename> class V>
V<T> Rad2Deg( V<T> const &v )
{
	return v.Call( Rad2Deg<T> );
}

template <typename T>
T Sin( T s )
{
	static_assert( std::numeric_limits<T>::is_iec559, "'Sin' only accepts floating-point input!" );
	return std::sin( s );
}

template <typename T, template <typename> class V>
V<T> Sin( V<T> const &v )
{
	return v.Call( Sin<T> );
}

template <typename T>
T Cos( T s )
{
	static_assert( std::numeric_limits<T>::is_iec559, "'Cos' only accepts floating-point input!" );
	return std::cos( s );
}

template <typename T, template <typename> class V>
V<T> Cos( V<T> const &v )
{
	return v.Call( Cos<T> );
}

template <typename T>
T InverseSqrt( T x )
{
	return static_cast<T>( 1 ) / std::sqrt( x );
}

template<typename T, template <typename> class V>
V<T> Normalize( const V<T> &x )
{
	static_assert( std::numeric_limits<T>::is_iec559, "'normalize' only accepts floating-point inputs!" );
	return x * InverseSqrt( Dot( x, x ) );
}

// Matrix inverse
template <typename T>
mat4x4_t<T> Inverse( mat4x4_t<T> const &m )
{
	static_assert( std::numeric_limits<T>::is_iec559, "'inverse' only accepts floating-point inputs!" );
	T Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	T Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	T Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

	T Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	T Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	T Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

	T Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	T Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	T Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

	T Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	T Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	T Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

	T Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	T Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	T Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

	T Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	T Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	T Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	vec4_t<T> fac0( Coef00, Coef00, Coef02, Coef03 );
	vec4_t<T> fac1( Coef04, Coef04, Coef06, Coef07 );
	vec4_t<T> fac2( Coef08, Coef08, Coef10, Coef11 );
	vec4_t<T> fac3( Coef12, Coef12, Coef14, Coef15 );
	vec4_t<T> fac4( Coef16, Coef16, Coef18, Coef19 );
	vec4_t<T> fac5( Coef20, Coef20, Coef22, Coef23 );

	vec4_t<T> vec0( m[1][0], m[0][0], m[0][0], m[0][0] );
	vec4_t<T> vec1( m[1][1], m[0][1], m[0][1], m[0][1] );
	vec4_t<T> vec2( m[1][2], m[0][2], m[0][2], m[0][2] );
	vec4_t<T> vec3( m[1][3], m[0][3], m[0][3], m[0][3] );

	vec4_t<T> inv0( vec1 * fac0 - vec2 * fac1 + vec3 * fac2 );
	vec4_t<T> inv1( vec0 * fac0 - vec2 * fac3 + vec3 * fac4 );
	vec4_t<T> inv2( vec0 * fac1 - vec1 * fac3 + vec3 * fac5 );
	vec4_t<T> inv3( vec0 * fac2 - vec1 * fac4 + vec2 * fac5 );

	vec4_t<T> signA( +1, -1, +1, -1 );
	vec4_t<T> signB( -1, +1, -1, +1 );
	mat4x4_t<T> inverse( inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB );

	vec4_t<T> row0( inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0] );

	vec4_t<T> dot0( m[0] * row0 );
	T dot1 = ( dot0.x + dot0.y ) + ( dot0.z + dot0.w );

	T oneOverDeterminant = static_cast<T>( 1 ) / dot1;

	return inverse * oneOverDeterminant;
}

template <typename T>
mat4x4_t<T> Transpose( mat4x4_t<T> const &m )
{
	mat4x4_t<T> result;
	result[0][0] = m[0][0];
	result[0][1] = m[1][0];
	result[0][2] = m[2][0];
	result[0][3] = m[3][0];

	result[1][0] = m[0][1];
	result[1][1] = m[1][1];
	result[1][2] = m[2][1];
	result[1][3] = m[3][1];

	result[2][0] = m[0][2];
	result[2][1] = m[1][2];
	result[2][2] = m[2][2];
	result[2][3] = m[3][2];

	result[3][0] = m[0][3];
	result[3][1] = m[1][3];
	result[3][2] = m[2][3];
	result[3][3] = m[3][3];
	return result;
}

template<typename T>
mat4x4_t<T> TranslationMatrix( const vec3_t<T> &v )
{
	mat4x4_t<T> result( T( 1 ) );
	result[3] = result[0] * v[0] + result[1] * v[1] + result[2] * v[2] + result[3];
	return result;
}

template<typename T>
mat4x4_t<T> RotationMatrix( const quat_t<T> &q )
{
	mat4x4_t<T> result( T( 1 ) );
	T qxx( q.x * q.x );
	T qyy( q.y * q.y );
	T qzz( q.z * q.z );
	T qxz( q.x * q.z );
	T qxy( q.x * q.y );
	T qyz( q.y * q.z );
	T qwx( q.w * q.x );
	T qwy( q.w * q.y );
	T qwz( q.w * q.z );

	result[0][0] = 1 - 2 * ( qyy - qzz );
	result[0][1] = 2 * ( qxy + qwz );
	result[0][2] = 2 * ( qxz - qwy );

	result[1][0] = 2 * ( qxy - qwz );
	result[1][1] = 1 - 2 * ( qxx - qzz );
	result[1][2] = 2 * ( qyz + qwx );

	result[2][0] = 2 * ( qxz + qwy );
	result[2][1] = 2 * ( qyz - qwx );
	result[2][2] = 1 - 2 * ( qxx - qyy );

	return result;
}

mat4 Perspective( float fovy, float aspect, float zNear, float zFar );

/************************************************************************/
/* OPERATORS                                                            */
/************************************************************************/

// Matrix * Vector
template<typename T>
typename vec4_t<T> operator*( mat4x4_t<T> const &m, typename vec4_t<T> const &v )
{
	const vec4_t<T> Mov0( v[0] );
	const vec4_t<T> Mov1( v[1] );
	const vec4_t<T> Mul0 = m[0] * Mov0;
	const vec4_t<T> Mul1 = m[1] * Mov1;
	const vec4_t<T> Add0 = Mul0 + Mul1;
	const vec4_t<T> Mov2( v[2] );
	const vec4_t<T> Mov3( v[3] );
	const vec4_t<T> Mul2 = m[2] * Mov2;
	const vec4_t<T> Mul3 = m[3] * Mov3;
	const vec4_t<T> Add1 = Mul2 + Mul3;
	const vec4_t<T> Add2 = Add0 + Add1;
	return Add2;
}

// Matrix * Matrix
template<typename T>
mat4x4_t<T>operator*( const mat4x4_t<T> &m1, const mat4x4_t<T> &m2 )
{
	const vec4_t<T> SrcA0 = m1[0];
	const vec4_t<T> SrcA1 = m1[1];
	const vec4_t<T> SrcA2 = m1[2];
	const vec4_t<T> SrcA3 = m1[3];

	const vec4_t<T> SrcB0 = m2[0];
	const vec4_t<T> SrcB1 = m2[1];
	const vec4_t<T> SrcB2 = m2[2];
	const vec4_t<T> SrcB3 = m2[3];

	mat4x4_t<T> result;
	result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
	result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
	result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
	result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
	return result;
}

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/

template<typename T>
T Min( T a, T b ) { return ( a < b ) ? a : b; }
template<typename T>
T Max( T a, T b ) { return ( a > b ) ? a : b; }

} // namespace mj::math
} // namespace mj
