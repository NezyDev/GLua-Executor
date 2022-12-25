#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>

#include "GLua.hpp"


namespace Execution
{
	void run_script(void* glua_state, std::string script)
	{
		if (GLua::glual_loadbuffer(glua_state, script.c_str(), script.size(), "@GLuaExecutor"))
		{
			uintptr_t* n = 0;
			std::string error = GLua::glua_tolstring(glua_state, -1, n);
			MessageBoxA(NULL, error.c_str(), "ERROR:", NULL);
		}
		else 
		{
			GLua::glua_pcall(glua_state, 0, 0, 0);
		}
	}


	void pipe()
	{
		std::string ScriptBuffer;
		char Buffer[999999];
		DWORD ReadWords;
		void* PipeHandle = CreateNamedPipe(TEXT("\\\\.\\pipe\\GLuaExecutor"), PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, PIPE_WAIT, 1, 999999, 999999, NMPWAIT_USE_DEFAULT_WAIT, 0);
		while (true)
		{
			if (ConnectNamedPipe(PipeHandle, 0))
			{
				while (ReadFile(PipeHandle, Buffer, sizeof(Buffer) - 1, &ReadWords, 0))
				{
					Buffer[ReadWords] = '\0';
					ScriptBuffer.append(Buffer);
				}

				run_script((void*)glua_state, ScriptBuffer);
				ScriptBuffer.clear();
			}
			DisconnectNamedPipe(PipeHandle);
		}
	}
}