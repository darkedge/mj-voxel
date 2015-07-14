#pragma once

namespace mj {
	template<class T>
	class Vector
	{
	public:
		Vector(); // TODO: other constructors
		explicit Vector( int32 size );
		~Vector();

		T& Add(); // Adds one element and returns a reference to it.
		T* Add( const T* ptr, int32 size ); // Adds an array of elements. Returns a pointer to the first added element.
		T& Add( T &&element ); // Adds an element using move semantics.
		T* Add( std::initializer_list<T> l );

		void Resize( int32 size );
		int32 Size() const;
		void Clear();

		const T& operator[]( int32 idx ) const;
		T& operator[]( int32 idx );

	private:
		enum
		{
			MinBytes = 256,
		};

		void ReallocRound( int32 size );
		void Realloc( int32 size );

		T *m_ptr;
		int32 m_size;
		int32 m_alloc;
	};

	//////////////////////////////////////////////////////////////////////////

	// Constructors/Destructor

	template<class T>
	Vector<T>::Vector() :
		m_ptr( nullptr ),
		m_size( 0 ),
		m_alloc( 0 )
	{
	}

	template<class T>
	Vector<T>::Vector( int32 size )
	{
		Resize( size );
	}

	template<class T>
	Vector<T>::~Vector()
	{
		delete[] m_ptr;
	}

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	int32 Vector<T>::Size() const
	{
		return m_size;
	}

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	T& Vector<T>::Add()
	{
		return *Add( nullptr, 1 );
	}

	template<class T>
	T* Vector<T>::Add( const T* src, int32 size )
	{
		int32 oldSize = m_size;
		Resize( m_size + size ); // changes m_size
		T* dst = m_ptr + oldSize;
		// Copy source to array
		if ( src )
		{
			for ( int32 i = 0; i < size; i++ )
			{
				dst[i] = src[i];
			}
		}
		return dst;
	}

	template<class T>
	T& Vector<T>::Add( T &&element )
	{
		int32 oldSize = m_size;
		Resize( m_size + 1 ); // changes m_size
		T &dst = *( m_ptr + oldSize );
		dst = std::move( element );
		return dst;
	}

	template<class T>
	T* Vector<T>::Add( std::initializer_list<T> l )
	{
		int32 oldSize = m_size;
		Resize( m_size + l.size() ); // changes m_size
		T* dst = m_ptr + oldSize;
		int32 i = 0;
		for ( auto it = l.begin(); it != l.end(); ++it )
		{
			dst[i] = std::move( *it );
			i++;
		}
		return dst;
	}

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	void Vector<T>::Resize( int32 size )
	{
		assert( size >= 0 );
		if ( size > m_alloc )
		{
			ReallocRound( size );
		}
		m_size = size;
	}

	template<class T>
	void Vector<T>::Clear()
	{
		delete[] m_ptr;
		m_ptr = nullptr;
		m_size = 0;
		m_alloc = 0;
	}

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	void Vector<T>::ReallocRound( int32 size )
	{
		assert( size >= 0 );
		int32 rounded = math::Max( (int32) ( MinBytes / sizeof( T ) ), 1 );
		while ( size > rounded )
			rounded <<= 1;
		Realloc( rounded );
	}

	template<class T>
	void Vector<T>::Realloc( int32 size )
	{
		assert( size >= 0 );

		T* newPtr = nullptr;
		if ( size )
		{
			newPtr = new T[size];
			int32 s = math::Min( size, m_size );
			for ( int32 i = 0; i < s; i++ )
			{
				newPtr[i] = std::move( m_ptr[i] );
			}
		}
		if ( m_ptr )
		{
			delete[] m_ptr;
		}
		m_ptr = newPtr;
		m_alloc = size;
	}

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	const T& Vector<T>::operator[]( int32 idx ) const
	{
		assert( idx >= 0 && idx < m_size );
		return m_ptr[idx];
	}

	template<class T>
	T& Vector<T>::operator[]( int32 idx )
	{
		assert( m_ptr && idx >= 0 && idx < m_size );
		return m_ptr[idx];
	}
} // namespace mj
