#include <cstdio>
#include <windows.h>

#pragma comment(lib,"user32.lib")

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		printf("usage: loadlibrary dll_path [dll_name]\n测试加载dll和函数(可选)\n");
	}
	char * dll_path = "D3DCompiler_47.dll";
	char* func_name = NULL;
	if (argc > 1)
	{
		dll_path = argv[1];
	}
	if (argc > 2)
	{
		func_name = argv[2];
	}
	printf("Load %s", dll_path);
	
	HMODULE module = ::LoadLibraryA(dll_path);
	if (module)
	{
		printf(" suscceed!\n");
		
		if (func_name)
		{
				void* address = ::GetProcAddress(module, func_name);
				printf("GetProcAddress %s => 0x%x", func_name, address);	
		}
	}
	else
	{
		printf(" failed!\n");
		
	}	
	
	getchar();
}