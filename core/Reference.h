#include "CorePCH.h"
#include "Name.h"

namespace mj {
namespace lua {


#if 0
// Helper functions for tables
void GetField(lua_State *L, lua_Integer i)
{
	// Assume table is pushed on the stack
	lua_pushinteger( L, i ); // push index on the stack
	lua_gettable( L, -2 ); // pop index, push table[i] on the stack
}

void GetField( lua_State *L, const char *name )
{
	// Assume table is pushed on the stack
	lua_getfield( L, -2, name ); // push table[name] on the stack
}

void SetField( lua_State *L, lua_Integer i )
{

}

void SetField( lua_State *L, const char *name )
{

}
#endif

/************************************************************************/
/* Reference                                                            */
/************************************************************************/
// References a Lua object.
// There are eight basic types in Lua: nil, boolean, number, string, function, userdata, thread, and table.
class Reference
{
public:
	// Because we don't store the reference directly,
	// we need a way to get the referenced value back.
	// We store a sequence of function calls to
	// get the value on the stack.
	typedef std::function<void()> Getter;

	// We use the getter in order to set a value
	typedef std::function<void( Getter )> Setter;

	// Creates a reference for a global object.
	Reference( lua_State *l, const char *name )
		: m_l( l ), m_name( Name( name ) )
	{
		m_getter = [this, name]() {
			lua_getglobal( m_l, name );
		};
		m_setter = [this, name]( Getter getter ) {
			getter();
			lua_setglobal( m_l, name );
		};
	}

	// Creates a reference for a nested object.
	Reference( lua_State *l, const char *name, Reference *bla, Getter getter, Setter setter )
		: m_l( l ), m_name( Name( name ) ), m_navList( bla->m_navList ), m_getter( getter ), m_setter( setter )
	{
		m_navList.Add( bla->m_getter );
	}

	void _traverse() const
	{
		for ( int32 i = 0; i < m_navList.Size(); i++ )
		{
			m_navList[i]();
		}
	}

	void _check_create_table() const
	{
		_traverse();
		m_getter();
		if ( lua_istable( m_l, -1 ) == 0 )
		{
			// not table
			lua_pop( m_l, 1 ); // Pop table
			auto put = [this]() {
				lua_newtable( m_l );
			};
			m_setter( put );
		}
		else
		{
			lua_pop( m_l, 1 );
		}
	}

	/************************************************************************/
	/* DEREFERENCE                                                          */
	/************************************************************************/
	operator lua_Integer() const
	{
		lua_getglobal( m_l, m_name ); // Push value
		auto ret = lua_tointeger( m_l, -1 );
		lua_pop( m_l, 1 ); // Pop value
		return ret;
	}

	operator const char *() const
	{
		lua_getglobal( m_l, m_name ); // Push value
		auto ret = lua_tostring( m_l, -1 );
		lua_pop( m_l, 1 ); // Pop value
		return ret;
	}

	operator lua_Number() const
	{
		lua_getglobal( m_l, m_name ); // Push value
		auto ret = lua_tonumber( m_l, -1 );
		lua_pop( m_l, 1 ); // Pop value
		return ret;
	}

	operator bool() const
	{
		lua_getglobal( m_l, m_name ); // Push value
		auto ret = lua_toboolean( m_l, -1 ) != 0;
		lua_pop( m_l, 1 ); // Pop value
		return ret;
	}

	// FIXME: Free function
	void Print() const
	{
		const char *str = operator const char*();
		printf( "%s = %s\n", m_name, str );
	}

	/************************************************************************/
	/* ASSIGNMENT                                                           */
	/************************************************************************/
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

	/************************************************************************/
	/* QUERY                                                                */
	/************************************************************************/
	// TODO: const versions?
	Reference operator[]( lua_Integer index )
	{
		char str[32];
		sprintf( str, "%d", index );
		Name name( { m_name, ".", str } );
		_check_create_table();
		Getter getter = [this, index]() {
			lua_pushinteger( m_l, index );
			lua_gettable( m_l, -2 );
		};

		Setter setter = [this, index]( Getter getter ) {
			lua_pushinteger( m_l, index );
			getter();
			lua_settable( m_l, -3 );
			lua_pop( m_l, 1 );
		};
	}

	Reference operator[]( const char *str )
	{
		Name name( { m_name, ".", str } );
		_check_create_table();
		Getter getter = [this, str]() {
			lua_getfield( m_l, -1, str );
		};

		Setter setter = [this, str]( Getter getter ) {
			getter();
			lua_setfield( m_l, -2, str );
			lua_pop( m_l, 1 );
		};

		return Reference( m_l, name, this, getter, setter );
	}

private:
	// Contains a list of getters in order to navigate
	// towards this variable
	Vector<Getter> m_navList;

	Getter m_getter;
	Setter m_setter;
	
	Reference *parent = nullptr;
	lua_State *m_l;
	Name m_name;
};
}
}