#pragma once

namespace mj {
#if 0
	// https://home.comcast.net/~tom_forsyth/blog.wiki.html#[[Resizable%20arrays%20without%20using%20STL]]
	template<class T>
	class Vector
	{
	public:
		Vector( int32 numElements = 0 )
		{
			Init( numElements );
		}

		Vector( const Vector<T> &other )
		{
			int32 numElements = other.Size();

			m_Array = nullptr;
			m_Size = 0;
			m_AllocatedSize = 0;
			Resize( numElements );
			for ( int32 i = 0; i < numElements; i++ )
			{
				this[i] = other[i];
			}
		}

		~Vector()
		{
			Destroy();
		}

		void Init( int32 numElements = 0, int32 numReservedElements = 0 )
		{
			m_Array = nullptr;
			m_Size = 0;
			m_AllocatedSize = 0;
			if ( numReservedElements > numElements )
			{
				Reserve( numReservedElements, true );
				Resize( numElements, false );
			}
			else if ( numElements > 0 )
			{
				Resize( numElements, true, true );
			}
		}

		void Destroy()
		{
			if ( !m_Array )
			{
				assert( m_AllocatedSize == 0 );
				assert( m_Size == 0 );
			}
			else
			{
				assert( m_AllocatedSize > 0 );
				assert( m_Size >= 0 );
				delete[] m_Array;
				m_AllocatedSize = 0;
				m_Size = 0;
				m_Array = nullptr;
			}
		}

		T &operator []( const int32 idx )
		{
			assert( idx < m_Size );
			return m_Array[idx];
		}

		const T &operator []( const int32 idx ) const
		{
			assert( idx < m_Size );
			return m_Array[idx];
		}

		T *Begin()
		{
			return m_Array;
		}

		int32 Size() const
		{
			return m_Size;
		}

		T *End()
		{
			return m_Array + m_Size;
		}

		T *Resize( int32 size, bool bBinAtZero = true, bool bAllocExactly = false )
		{
			assert( size >= 0 );
			int32 oldSize = m_Size;
			m_Size = size;

			if ( size == m_AllocatedSize )
			{
				if ( m_AllocatedSize == 0 )
				{
					assert( !m_Array );
				}
				else
				{
					assert( m_Array );
				}
				return m_Array;
			}
			else if ( size < m_AllocatedSize )
			{
				if ( ( size == 0 ) && bBinAtZero )
				{
					delete[] m_Array;
					m_Array = nullptr;
					m_AllocatedSize = 0;
					return nullptr;
				}
				if ( !bAllocExactly || ( size == m_AllocatedSize ) )
				{
					return m_Array;
				}
			}

			assert( size > 0 );
			int32 newSize = size;
			if ( !bAllocExactly )
			{
				// Usually grow by 50% more than needed to avoid constant regrows.
				newSize = ( size * 3 ) >> 1;
				if ( newSize < 8 )
				{
					newSize = 8;
				}
				assert( newSize > m_AllocatedSize );
			}
			if ( !m_Array )
			{
				assert( m_AllocatedSize == 0 );
				m_Array = new T[newSize];
			}
			else
			{
				assert( m_AllocatedSize > 0 );
				T *oldArray = m_Array;
				m_Array = new T[newSize];
				int32 length = oldSize;
				if ( length > newSize )
				{
					length = newSize;
				}
				for ( int32 i = 0; i < length; i++ )
				{
					m_Array[i] = oldArray[i];
				}
				delete[] oldArray;
			}
			assert( m_Array );
			m_AllocatedSize = newSize;
			return m_Array;
		}

		void Reserve( int32 size, bool bAllocExactly = false )
		{
			assert( size >= m_Size );
			if ( size <= m_AllocatedSize )
			{
				return;
			}
			int32 oldSize = m_Size;
			Resize( size, false, bAllocExactly );
			m_Size = oldSize;
		}

		void FreeMem()
		{
			Resize( 0, true );
		}

		// Removes the element at the specified location
		// by swapping it with the last element and shrinking it.
		// Faster, but order is not maintained.
		void RemoveSwap( int32 pos )
		{
			assert( pos >= 0 );
			assert( pos < m_Size );
			m_Array[pos] = m_Array[m_Size - 1];
			Resize( m_Size - 1 );
		}

		// Removes the element at the specified location
		// And shifts the rest (order is maintained)
		void Remove( int32 pos )
		{
			assert( pos >= 0 );
			assert( pos < m_Size );

			const int32 length = m_Size - 1;
			for ( int32 i = pos; i < length; i++ )
			{
				m_Array[i] = m_Array[i + 1];
			}
			Resize( m_Size - 1 );
		}

		// TODO: Construct in-place and return reference?
		// See NVIDIA docs
		void Add( T const &t )
		{
			Resize( m_Size + 1 );
			m_Array[m_Size - 1] = t;
		}

		void Insert( int32 pos, T &value )
		{
			assert( pos >= 0 && pos <= m_Size );
			Resize( m_Size + 1 );
			for ( int32 i = m_Size - 1; i >= pos + 1; i-- )
			{
				m_Array[i] = m_Array[i - 1];
			}
			&m_Array[pos] = value;
		}

		int32 Find( const T &value )
		{
			for ( int32 i = 0; i < m_Size; i++ )
			{
				if ( m_Array[i] == value )
				{
					return i;
				}
			}
			return -1;
		}

		void ShrinkToFit()
		{
			Resize( m_Size, true, true );
		}

	private:
		int32	m_AllocatedSize;
		int32	m_Size;
		T		*m_Array;
	};
#endif
	template<class T>
	class Vector
	{
	public:
		Vector(); // TODO: other constructors
		~Vector();

		T& Add(); // Adds one element and returns a reference to it.
		T* Add( const T* ptr, int32 size );

		void Resize( int32 size );
		int32 Size() const;

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
	Vector<T>::~Vector()
	{
		delete m_ptr;
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

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	void Vector<T>::ReallocRound( int32 size )
	{
		assert( size >= 0 );
		int32 rounded = math::Max( (int32) (MinBytes / sizeof( T )), 1 );
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
				newPtr[i] = m_ptr[i]; // Copy!
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
		assert( idx >= 0 && idx < m_size );
		return m_ptr[idx];
	}
} // namespace mj
