#include <windows.h>
#include "ExampleMod.h"
#include "Utilities/Logger.h"

//Mod* CoreMod;

void CreateMod()
{
    Log::Info("HEY THERE");
    auto CoreMod = new MultiplayerMod();
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        MessageBoxW(NULL, L"Yay!", L"MinHook Sample", MB_OK);

        CreateMod();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}