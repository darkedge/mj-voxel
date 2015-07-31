#pragma once

// Class for short, immutable strings.
// TODO: Create a string cache.
namespace mj {
class Name
{
public:
	Name( const char *name )
	{
		int32 len = strlen( name );
		m_string = new char[len + 1];
#pragma warning(suppress:4996)
		strcpy( m_string, name );
	}

	Name( const Name &name )
	{
		if ( name.m_string )
		{
			int32 len = strlen( name.m_string );
			m_string = new char[len + 1];
			strcpy( m_string, name.m_string );
		}
	}

	Name( std::initializer_list<const char *> l )
	{
		int32 size;

		// Calculate size
		size = 0;
		for ( auto it = l.begin(); it != l.end(); ++it )
		{
			size += strlen( *it );
		}

		// Allocate string
		m_string = new char[size + 1];

		// Copy contents
		size = 0;
		for ( auto it = l.begin(); it != l.end(); ++it )
		{
#pragma warning(suppress:4996)
			strcpy( m_string + size, *it );
			size += strlen( *it );
		}
		//m_string[size] = '\0';
	}

	~Name()
	{
		if ( m_string )
		{
			delete[] m_string;
			m_string = nullptr;
		}
	}

	operator const char *() const
	{
		return m_string;
	}

	Name & operator= ( const Name &other )
	{
		if ( this != &other ) // protect against invalid self-assignment
		{
			int32 len = strlen( other.m_string );
			char *str = new char[len + 1];
			strcpy( str, other.m_string );

			delete[] m_string;

			m_string = str;
		}
		return *this;
	}

private:
	char *m_string = nullptr;
};
}
