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
		m_string = new char[len];
#pragma warning(suppress:4996)
		strcpy( m_string, name );
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
		m_string = new char[size];

		// Copy contents
		size = 0;
		for ( auto it = l.begin(); it != l.end(); ++it )
		{
#pragma warning(suppress:4996)
			strcpy( m_string + size, *it );
			size += strlen( *it );
		}
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

private:
	char *m_string = nullptr;
};
}
