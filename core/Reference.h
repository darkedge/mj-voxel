#include "CorePCH.h"

namespace mj {
namespace lua {
/************************************************************************/
/* VALUE                                                                */
/************************************************************************/
// There are eight basic types in Lua: nil, boolean, number, string, function, userdata, thread, and table.

// References a global Lua variable.
class Reference
{
public:
	Reference(lua_State *l, const char *name) : m_l(l), m_name(name)
	{

	}

	operator lua_Integer() const
	{
		lua_getglobal( m_l, m_name ); // Push value
		auto ret = lua_tointeger( m_l, -1 );
		lua_settop( m_l, -2 ); // Pop value
		return ret;
	}

	operator const char *() const
	{
		lua_getglobal( m_l, m_name ); // Push value
		auto ret = lua_tostring( m_l, -1 );
		lua_settop( m_l, -2 ); // Pop value
		return ret;
	}

	operator lua_Number() const
	{
		lua_getglobal( m_l, m_name ); // Push value
		auto ret = lua_tonumber( m_l, -1 );
		lua_settop( m_l, -2 ); // Pop value
		return ret;
	}

	operator bool() const
	{
		lua_getglobal( m_l, m_name ); // Push value
		auto ret = lua_toboolean( m_l, -1 ) != 0;
		lua_settop( m_l, -2 ); // Pop value
		return ret;
	}

	void Print() const
	{
		const char *str = operator const char*();
		printf( "%s = %s\n", m_name, str );
	}

	Reference & operator=( const Reference & rhs )
	{
		if (this != &rhs) {
			lua_getglobal( m_l, rhs.m_name ); // Push value
			lua_setglobal( m_l, this->m_name );
		}

		return *this;
	}

	Reference & operator=( lua_Integer i )
	{
		lua_pushinteger( m_l, i );
		lua_setglobal( m_l, m_name );

		return *this;
	}

	Reference & operator=( const char * str )
	{
		lua_pushstring( m_l, str );
		lua_setglobal( m_l, m_name );

		return *this;
	}

private:
	lua_State *m_l;
	const char *m_name;
};
}
}