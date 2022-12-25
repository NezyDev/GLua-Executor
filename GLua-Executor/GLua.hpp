#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>

#include "Retcheck.hpp"


typedef void* (*CreateInterface)(const char* IName, int* IReturnCode);

enum LuaInterfaceType
{
	LUAINTERFACE_CLIENT = 0,
	LUAINTERFACE_SERVER = 1,
	LUAINTERFACE_MENU = 2
};

class CLuaShared
{
public:
	virtual void            padding00() = 0;
	virtual void* padding01() = 0;
	virtual void* padding02() = 0;
	virtual void* padding03() = 0;
	virtual void* padding04() = 0;
	virtual void* padding05() = 0;
	virtual DWORD			GetLuaInterface(LuaInterfaceType type) = 0;
};


CLuaShared* ILuaShared;
HMODULE lua_shared;
uintptr_t glua_state;


namespace GLua
{
	uintptr_t get_state(HMODULE lua_shared)
	{
		uintptr_t state = 0;
		if (lua_shared != NULL)
		{
			CreateInterface CreateInterface_src = (CreateInterface)GetProcAddress(lua_shared, "CreateInterface");
			ILuaShared = (CLuaShared*)CreateInterface_src("LUASHARED003", NULL);
			uintptr_t CLuaInterface = ILuaShared->GetLuaInterface(LUAINTERFACE_CLIENT);
			state = *(uintptr_t*)(CLuaInterface + 0x4);
		}
		return state;
	}


	int glua_pcall(void* glua_state, int nargs, int nresult, int errfunction)
	{
		if (lua_shared != NULL)
		{
			typedef int(__cdecl* _lua_pcall)(void* lua_state, int nargs, int nresult, int errfunction);
			_lua_pcall lua_pcall = (_lua_pcall)(retcheckBypass(GetProcAddress(lua_shared, "lua_pcall")));

			lua_pcall(glua_state, nargs, nresult, errfunction);
		}
		return 0;
	}


	int glual_loadbuffer(void* glua_state, const char* buffer, size_t len, const char* name)
	{
		int result;
		if (lua_shared != NULL)
		{
			typedef int(__cdecl* _lual_loadbuffer)(void* lua_state, const char* buffer, size_t len, const char* name);
			_lual_loadbuffer lual_loadbuffer = (_lual_loadbuffer)(retcheckBypass(GetProcAddress(lua_shared, "luaL_loadbuffer")));

			result = lual_loadbuffer(glua_state, buffer, len, name);
		}
		else
		{
			result = NULL;
		}
		return result;
	}


	const char* glua_tolstring(void* glua_state, int index, size_t* len)
	{
		const char* string;
		if (lua_shared != NULL)
		{
			typedef const char* (__cdecl* _lua_tolstring)(void* lua_state, int index, size_t* len);
			_lua_tolstring lua_tolstring = (_lua_tolstring)(retcheckBypass(GetProcAddress(lua_shared, "lua_tolstring")));

			string = lua_tolstring(glua_state, index, len);
		}
		else
		{
			string = "";
		}
		return string;
	}
}