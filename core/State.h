#include "CorePCH.h"
#include "Reference.h"

namespace mj {
namespace lua {
class State
{
public:
	// Default constructor
	State() {
		m_l = luaL_newstate();
		if ( !m_l )
		{
			throw 0;
		}

		// FIXME: For now, we enable all libs
		// This is probably not necessary/secure
		luaL_openlibs( m_l );
	}

	// Destructor
	~State()
	{
		if ( m_l )
		{
			lua_close( m_l );
		}
		m_l = nullptr;
	}

	void error( lua_State *L, const char *fmt, ... )
	{
		va_list argp;
		va_start( argp, fmt );
		vfprintf( stderr, fmt, argp );
		va_end( argp );
		lua_close( L );
		exit( EXIT_FAILURE );
	}

	void LoadFile( const char *path )
	{
		// luaL_loadfile loads the file
		// lua_pcall runs it
		if ( luaL_loadfile( m_l, path ) || lua_pcall( m_l, 0, 0, 0 ) )
		{
			error( m_l, "cannot run configuration file: %s",
				lua_tostring( m_l, -1 ) );
		}
	}

	operator lua_State*( )
	{
		return m_l;
	}

	Reference operator[]( const char *name )
	{
		// Create a Value that remembers the request
		return Reference( m_l, name );
	}

	// https://stackoverflow.com/questions/8936369/compile-lua-code-store-bytecode-then-load-and-execute-it
	// bool LoadByteCode() {}

	// bool LoadString() {}

private:
	lua_State *m_l = nullptr;
};
}
}

static void stackDump( lua_State *L )
{
	int i;
	int top = lua_gettop( L );
	for ( i = 1; i <= top; i++ )
	{  /* repeat for each level */
		int t = lua_type( L, i );
		switch ( t )
		{

		case LUA_TSTRING:  /* strings */
			printf( "`%s'", lua_tostring( L, i ) );
			break;

		case LUA_TBOOLEAN:  /* booleans */
			printf( lua_toboolean( L, i ) ? "true" : "false" );
			break;

		case LUA_TNUMBER:  /* numbers */
			printf( "%g", lua_tonumber( L, i ) );
			break;

		default:  /* other values */
			printf( "%s", lua_typename( L, t ) );
			break;

		}
		printf( "  " );  /* put a separator */
	}
	printf( "\n" );  /* end the listing */
}