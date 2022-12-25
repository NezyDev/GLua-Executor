#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>

#include "Execute.hpp"


void main()
{
	lua_shared = GetModuleHandleA("lua_shared.dll");
	if (lua_shared == NULL)
	{
		while (lua_shared == NULL)
		{
			lua_shared = GetModuleHandleA("lua_shared.dll");

			if (lua_shared != NULL)
			{
				break;
			}
		}

		glua_state = GLua::get_state(lua_shared);
		std::thread(Execution::pipe).detach();
	}
	else
	{
		glua_state = GLua::get_state(lua_shared);
		std::thread(Execution::pipe).detach();
	}
}


int __stdcall DllMain(void* Module, DWORD Reason, void* Reserve)
{
	if (Reason == 1)
	{
		std::thread(main).detach();
	}
	return true;
}