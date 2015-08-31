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
private:
	class Accessor {
	public:
		enum class EType {
			Global,
			SubscriptString,
			SubscriptInteger,
			Table,
		};

		/************************************************************************/
		/* CONSTRUCTORS                                                         */
		/************************************************************************/
		// Default constructor
		Accessor() {}

		// Subscript string
		Accessor(EType type, lua_State *state, const char *str) :
			m_Type(type),
			m_State(state),
			m_Str(str) {}

		// Subscript integer
		Accessor(EType type, lua_State *state, lua_Integer idx) :
			m_Type(type),
			m_State(state),
			m_Idx(idx) {}

		void Get() const {
			switch (m_Type) {
			case EType::Global:
				lua_getglobal(m_State, m_Str);
				break;
			case EType::SubscriptInteger:
				lua_pushinteger(m_State, m_Idx);
				lua_gettable(m_State, -2);
				break;
			case EType::SubscriptString:
				lua_getfield(m_State, -1, m_Str);
				break;
			default:
				break;
			}
		}

		void Set() const {
			switch (m_Type) {
			case EType::Global:
				Get();
				lua_setglobal(m_State, m_Str);
				break;
			case EType::SubscriptInteger:
				// lua_pushinteger(m_L, index); // Needed???
				Get();
				lua_settable(m_State, -3);
				lua_pop(m_State, 1);
				break;
			case EType::SubscriptString:
				Get();
				lua_setfield(m_State, -2, m_Str);
				lua_pop(m_State, 1);
				break;
			case EType::Table:
				lua_newtable(m_State);
				break;
			default:
				break;
			}
		}

	private:
		lua_State *m_State = nullptr;
		lua_Integer m_Idx = -1;
		const char *m_Str = nullptr;
		EType m_Type;
	};

public:
	// Because we don't store the reference directly,
	// we need a way to get the referenced value back.
	// We store a sequence of function calls to
	// get the value on the stack.

	/************************************************************************/
	/* CONSTRUCTORS                                                         */
	/************************************************************************/
	// Creates a reference for a global object.
	Reference(lua_State *l, const char *name) :
		m_L(l),
		m_Name(Name(name)),
		m_Accessor(Accessor::EType::Global, l, name) {}

	// Creates a reference for a nested object.
	Reference(lua_State *l, const char *name, Reference *bla, Accessor getSet)
		: m_L(l), m_Name(Name(name)), m_NavList(bla->m_NavList), m_Accessor(getSet)
	{
		m_NavList.Add(bla->m_Accessor);
	}

	void Traverse() const
	{
		for (int32 i = 0; i < m_NavList.Size(); i++)
		{
			m_NavList[i].Get();
		}
	}

	// Creates a table. If there already is one, this function does nothing.
	void CreateTable() const
	{
		Traverse();
		m_Accessor.Get();
		if (lua_istable(m_L, -1))
		{
			// Dismiss
			lua_pop(m_L, 1);
		}
		else
		{
			// Create table
			// FIXME: m_Accessor should be of type Table here but it is never set
			lua_pop(m_L, 1);
			m_Accessor.Set();
		}
	}

	/************************************************************************/
	/* DEREFERENCE                                                          */
	/************************************************************************/
	operator lua_Integer() const
	{
		lua_getglobal(m_L, m_Name); // Push value
		auto ret = lua_tointeger(m_L, -1);
		lua_pop(m_L, 1); // Pop value
		return ret;
	}

	operator const char *() const
	{
		lua_getglobal(m_L, m_Name); // Push value
		auto ret = lua_tostring(m_L, -1);
		lua_pop(m_L, 1); // Pop value
		return ret;
	}

	operator lua_Number() const
	{
		lua_getglobal(m_L, m_Name); // Push value
		auto ret = lua_tonumber(m_L, -1);
		lua_pop(m_L, 1); // Pop value
		return ret;
	}

	operator bool() const
	{
		lua_getglobal(m_L, m_Name); // Push value
		auto ret = lua_toboolean(m_L, -1) != 0;
		lua_pop(m_L, 1); // Pop value
		return ret;
	}

	// FIXME: Free function
	void Print() const
	{
		const char *str = operator const char*();
		printf("%s = %s\n", m_Name, str);
	}

	/************************************************************************/
	/* ASSIGNMENT                                                           */
	/************************************************************************/
	Reference & operator=(const Reference & rhs)
	{
		if (this != &rhs) {
			lua_getglobal(m_L, rhs.m_Name); // Push value
			lua_setglobal(m_L, this->m_Name);
		}

		return *this;
	}

	Reference & operator=(lua_Integer i)
	{
		lua_pushinteger(m_L, i);
		lua_setglobal(m_L, m_Name);

		return *this;
	}

	Reference & operator=(const char * str)
	{
		lua_pushstring(m_L, str);
		lua_setglobal(m_L, m_Name);

		return *this;
	}

	/************************************************************************/
	/* QUERY                                                                */
	/************************************************************************/
	// TODO: const versions?
	Reference operator[](lua_Integer index)
	{
		char str[32];
		sprintf(str, "%d", index);
		Name name{ m_Name, ".", str };
		CreateTable();
		Accessor accessor(Accessor::EType::SubscriptInteger, m_L, index);
		return Reference(m_L, name, this, accessor);
	}

	Reference operator[](const char *str)
	{
		Name name{ m_Name, ".", str };
		CreateTable();
		Accessor accessor(Accessor::EType::SubscriptString, m_L, str);
		return Reference(m_L, name, this, accessor);
	}

private:
	// Contains a list of getters in order to navigate
	// towards this variable
	mj::Vector<Accessor> m_NavList;
	Accessor m_Accessor;
	Reference *m_Parent = nullptr;
	lua_State *m_L;
	Name m_Name;
};
}
}