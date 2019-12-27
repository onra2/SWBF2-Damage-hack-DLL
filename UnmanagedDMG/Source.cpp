#include <Windows.h>
#include <iostream>
#include "Usefull.h"

bool FirstTime = true;
DWORD64 XDMG;
HANDLE hProc;

extern "C" __declspec(dllexport) DWORD64 getOffset(){
	LPCSTR sigOFFSET_XDMG = "\x8b\x8a\x50\x01\x00\x00\x49\x8b\x50\x20\xe9";
	LPCSTR maskOFFSET_XDMG = "xxxxxxxxxxx";

	if (GetProcess(L"starwarsbattlefrontii.exe"))
	{
		module mod = GetModule(L"starwarsbattlefrontii.exe");
		DWORD64 XDMG = FindSignature(mod.dwBase, mod.dwSize, sigOFFSET_XDMG, maskOFFSET_XDMG);
		return XDMG;
	}
	return 0;
}

extern "C" __declspec(dllexport) void damage(int damage)
{
	if (FirstTime)
	{
		XDMG = getOffset();
		DWORD pid;
		GetWindowThreadProcessId(FindWindowA(nullptr, "star wars battlefront ii"), &pid);
		hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		FirstTime = false;
	}
	byte* shellcode = new byte[6]{ 0xb9, 0x00, 0x00, 0x00, 0x00, 0x90 };
	memcpy(shellcode + 1, &damage, 4);
	WriteProcessMemory(hProc, reinterpret_cast<LPVOID>(XDMG), shellcode, 6, nullptr);
	delete[] shellcode;
}