#pragma once
#include <string>
#include "Logger.h"
#include "include/MinHook.h"
#include "Globals.h"
#ifdef UNREALENGINEMODLOADER_EXPORTS //Stops dumb errors from the ExampleMod shit
#pragma comment(lib,"../../Minhook/lib/libMinHook-x64-v141-mtd.lib")
#endif

namespace MinHook
{
	static void Init()
	{
		if (MH_Initialize() != MH_OK)
		{
			Log::Error("Failed to initialize MinHook");
		}
	}

	template <typename T>
	static void Add(DWORD_PTR pTarget, LPVOID pDetour, T** ppOriginal, std::string displayName = "")
	{
		int createResult = MH_CreateHook((LPVOID)pTarget, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
		if (createResult != MH_OK)
		{
			Log::Error("Failed to create hook: %s (%d)", displayName.c_str(), createResult);
			return;
		}

		int enableResult = MH_EnableHook((LPVOID)pTarget);
		if (enableResult != MH_OK)
		{
			Log::Error("Failed to enable hook: %s (%d)", displayName.c_str(), enableResult);
			return;
		}
		Log::Info("Added hook: %s", displayName.c_str());
	}
}