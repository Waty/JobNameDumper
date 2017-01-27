#include <Windows.h>
#include "ZMap.h"
#include <fstream>
#include <string>

//GMS 180.2
const uint32_t map_addr = 0x29C2CE4; // B9 ? ? ? ? C7 44 24 ? ? ? ? ? E8 ? ? ? ? 8B 44 24 0C 89 74 24 18
using map_type = ZMap<unsigned, const char*>;

DWORD WINAPI DoDumping(LPVOID)
{
	map_type& map = *reinterpret_cast<map_type*>(map_addr);

	while (!map.count()) MessageBoxA(nullptr, "no job names availlable!", "nothing yet", 0);

	MessageBoxA(nullptr, (std::to_string(map.count()) + " job names are availlable!").c_str(), "found them", 0);

	std::ofstream output{ "job ids.csv" };
	map.for_each([&output](auto id, auto& name)
	{
		output << id << ",\"" << name << '"' << std::endl;
	});

	MessageBoxA(nullptr, "finished dumping!", "done", 0);

	return 0;
}

BOOL WINAPI DllMain(_In_ HINSTANCE, _In_ DWORD fdwReason, _In_ LPVOID)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(nullptr, 0, DoDumping, nullptr, 0, nullptr);
	}
	return TRUE;
}
