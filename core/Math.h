#pragma once

namespace mj {
namespace math {

template<typename T>
float Dot( T a, T b ) { return 1.0f; }


template<typename T>
struct tvec3 {
	T x, y, z;

	int32 length() const {
		return 3;
	}
};

template<typename T>
struct tvec4 {
	T x, y, z, w;

	int32 length() const {
		return 4;
	}
	T &operator[]( int32 i ) {
		assert( i >= 0 && i < length() );
		return &( x )[i];
	}
	T const &operator[]( int32 i ) const {
		assert( i >= 0 && i < length() );
		return &( x )[i];
	}
};

template<typename T>
class tmat4x4 {
public:
	typedef tvec4<T> col_type;
	typedef tvec4<T> row_type;
	typedef tmat4x4<T> type;
	typedef tmat4x4<T> transpose_type;
	typedef T value_type;
public:
	// TODO: Ctor

public:
	col_type columns[4];
};

// Matrix * Vector (WTF?)
template <typename T>
typename tmat4x4<T>::col_type operator*( tmat4x4<T> const &m, typename tmat4x4<T>::row_type const &v ) {
	/*
	__m128 v0 = _mm_shuffle_ps(v.data, v.data, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 v1 = _mm_shuffle_ps(v.data, v.data, _MM_SHUFFLE(1, 1, 1, 1));
	__m128 v2 = _mm_shuffle_ps(v.data, v.data, _MM_SHUFFLE(2, 2, 2, 2));
	__m128 v3 = _mm_shuffle_ps(v.data, v.data, _MM_SHUFFLE(3, 3, 3, 3));

	__m128 m0 = _mm_mul_ps(m[0].data, v0);
	__m128 m1 = _mm_mul_ps(m[1].data, v1);
	__m128 a0 = _mm_add_ps(m0, m1);

	__m128 m2 = _mm_mul_ps(m[2].data, v2);
	__m128 m3 = _mm_mul_ps(m[3].data, v3);
	__m128 a1 = _mm_add_ps(m2, m3);

	__m128 a2 = _mm_add_ps(a0, a1);

	return typename tmat4x4<T, P>::col_type(a2);
	*/

	typename tmat4x4<T>::col_type const Mov0( v[0] );
	typename tmat4x4<T>::col_type const Mov1( v[1] );
	typename tmat4x4<T>::col_type const Mul0 = m[0] * Mov0;
	typename tmat4x4<T>::col_type const Mul1 = m[1] * Mov1;
	typename tmat4x4<T>::col_type const Add0 = Mul0 + Mul1;
	typename tmat4x4<T>::col_type const Mov2( v[2] );
	typename tmat4x4<T>::col_type const Mov3( v[3] );
	typename tmat4x4<T>::col_type const Mul2 = m[2] * Mov2;
	typename tmat4x4<T>::col_type const Mul3 = m[3] * Mov3;
	typename tmat4x4<T>::col_type const Add1 = Mul2 + Mul3;
	typename tmat4x4<T>::col_type const Add2 = Add0 + Add1;
	return Add2;

	/*
	return typename tmat4x4<T, P>::col_type(
	m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3],
	m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3],
	m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3],
	m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3]);
	*/
}

// Matrix  * Matrix
template <typename T>
tmat4x4<T> operator*( tmat4x4<T> const &m1, tmat4x4<T> const &m2 ) {
	typename tmat4x4<T>::col_type const SrcA0 = m1[0];
	typename tmat4x4<T>::col_type const SrcA1 = m1[1];
	typename tmat4x4<T>::col_type const SrcA2 = m1[2];
	typename tmat4x4<T>::col_type const SrcA3 = m1[3];

	typename tmat4x4<T>::col_type const SrcB0 = m2[0];
	typename tmat4x4<T>::col_type const SrcB1 = m2[1];
	typename tmat4x4<T>::col_type const SrcB2 = m2[2];
	typename tmat4x4<T>::col_type const SrcB3 = m2[3];

	tmat4x4<T> Result;
	Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
	Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
	Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
	Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
	return Result;
}

typedef tvec3<float> Vec3;
typedef tvec4<float> Vec4;
typedef tmat4x4<float> Mat4;

} // namespace mj::math
} // namespace mj
