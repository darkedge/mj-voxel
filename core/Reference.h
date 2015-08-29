#include "CorePCH.h"
#include "Name.h"

namespace mj {
namespace lua {

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

	// Creates a reference for a global object.
	Reference( lua_State *l, const char *name ) :
		m_L( l ),
		m_Name( Name( name ) ),
		m_GetSet(GetSet::EType::Global, l, name) {}

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
		m_getter(); // Put reference on the stack
		if ( lua_istable( m_l, -1 ) )
		{
			lua_pop( m_l, 1 );
		}
		else
		{
			// not table
			lua_pop( m_l, 1 ); // Pop table
			auto put = [this]() {
				lua_newtable( m_l );
			};
			m_setter( put );
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
		Name name{ m_name, ".", str };
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
		Name name{ m_name, ".", str };
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

	class Getter {
	public:
		Getter(lua_State *state, const char *str) : m_State(state), m_Str(str) {}
		Getter(lua_State *state, lua_Integer index) : m_State(state), m_Idx(index) {}

		void Get() {
			if(m_Str) {
				// Table
				lua_getfield( m_State, -1, m_Str );
			} else {
				lua_pushinteger( m_State, m_Idx );
				lua_gettable( m_State, -2 );
			}
		}
	};

	// TODO
	class GetSet {
	public:
		enum class EType {
			Global,
			SubscriptString,
			SubscriptInteger,
		};
		GetSet(EType type, lua_State *state, const char *str) :
			m_Type(type),
			m_State(state),
			m_Str(str) {}

		void Get() {
			switch(m_Type) {
			case EType::Global:
				break;
			case EType::SubscriptInteger:
				break;
			case EType::SubscriptString:
				break;
			default:
				break;
			}
		}

		void Set() {
			switch(m_Type) {
			case EType::Global:
				Get();
				lua_setglobal( m_State, m_Str );
				break;
			case EType::SubscriptInteger:
				lua_pushinteger( m_State, m_Idx );
				Get();
				lua_settable( m_State, -3 );
				lua_pop( m_State, 1 );
				break;
			case EType::SubscriptString:
				Get();
				lua_setfield( m_State, -2, m_Str );
				lua_pop( m_State, 1 );
				break;
			default:
				break;
			}
		}

	private:
		lua_State *m_State = nullptr;
		lua_Integer m_Idx;
		const char *m_Str = nullptr;
		EType m_Type;
	};

	// Contains a list of getters in order to navigate
	// towards this variable
	mj::Vector<Getter> m_navList;
	GetSet m_GetSet;
	Reference *m_Parent = nullptr;
	lua_State *m_L;
	Name m_Name;
};
}
}