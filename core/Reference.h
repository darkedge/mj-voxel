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
				lua_setglobal(m_State, m_Str);
				break;
			case EType::SubscriptInteger:
				lua_pushinteger(m_State, m_Idx);
				lua_settable(m_State, -3);
				lua_pop(m_State, 1);
				break;
			case EType::SubscriptString:
				lua_setfield(m_State, -2, m_Str);
				lua_pop(m_State, 1);
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
	/************************************************************************/
	/* CONSTRUCTORS                                                         */
	/************************************************************************/
	// Creates a reference for a global object.
	Reference(lua_State *l, const char *name) :
		m_l(l),
		m_name(Name(name)),
		m_accessor(Accessor::EType::Global, l, name) {}

	// Creates a reference for a nested object.
	Reference(lua_State *l, const char *name, Reference *bla, Accessor getSet)
		: m_l(l), m_name(Name(name)), m_navList(bla->m_navList), m_accessor(getSet)
	{
		m_navList.Add(bla->m_accessor);
	}

	void Traverse() const
	{
		for (int32 i = 0; i < m_navList.Size(); i++)
		{
			m_navList[i].Get();
		}
	}

	// Checks if this reference is a table. If not, one is created.
	void CreateTable() const
	{
		Traverse();
		m_accessor.Get();
		if (lua_istable(m_l, -1))
		{
			// Dismiss
			lua_pop(m_l, 1);
		}
		else
		{
			// Pop reference
			lua_pop(m_l, 1);
			// Push new table
			lua_newtable(m_l);
			m_accessor.Set();
		}
	}

	/************************************************************************/
	/* DEREFERENCE                                                          */
	/************************************************************************/
#if 0
	operator lua_Integer() const
	{
		lua_getglobal(m_L, m_Name); // Push value
		auto ret = lua_tointeger(m_L, -1);
		lua_pop(m_L, 1); // Pop value
		return ret;
	}
#endif

	operator const char *() const
	{
		lua_getglobal(m_l, m_name); // Push value
		auto ret = lua_tostring(m_l, -1);
		lua_pop(m_l, 1); // Pop value
		return ret;
	}

	operator lua_Number() const
	{
		lua_getglobal(m_l, m_name); // Push value
		auto ret = lua_tonumber(m_l, -1);
		lua_pop(m_l, 1); // Pop value
		return ret;
	}

	operator bool() const
	{
		lua_getglobal(m_l, m_name); // Push value
		auto ret = lua_toboolean(m_l, -1) != 0;
		lua_pop(m_l, 1); // Pop value
		return ret;
	}

	// FIXME: Free function
	void Print() const
	{
		const char *str = operator const char*();
		printf("%s = %s\n", m_name, str);
	}

	/************************************************************************/
	/* ASSIGNMENT                                                           */
	/************************************************************************/
	Reference & operator=(const Reference & rhs)
	{
		if (this != &rhs) {
			lua_getglobal(m_l, rhs.m_name); // Push value
			lua_setglobal(m_l, this->m_name);
		}

		return *this;
	}

	Reference & operator=(lua_Integer i)
	{
		lua_pushinteger(m_l, i);
		lua_setglobal(m_l, m_name);

		return *this;
	}

	Reference & operator=(const char * str)
	{
		lua_pushstring(m_l, str);
		lua_setglobal(m_l, m_name);

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
		Name name{ m_name, ".", str };
		CreateTable();
		Accessor accessor(Accessor::EType::SubscriptInteger, m_l, index);
		return Reference(m_l, name, this, accessor);
	}

	Reference operator[](const char *str)
	{
		Name name{ m_name, ".", str };
		CreateTable();
		Accessor accessor(Accessor::EType::SubscriptString, m_l, str);
		return Reference(m_l, name, this, accessor);
	}

private:
	// Contains a list of getters in order to navigate
	// towards this variable
	mj::Vector<Accessor> m_navList;
	Accessor m_accessor;
	Reference *m_parent = nullptr; // TODO: unused
	lua_State *m_l;
	Name m_name;
};
}
}