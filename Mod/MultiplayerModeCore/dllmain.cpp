#include <windows.h>
#include "MultiplayerMod.h"
#include "Utilities/Logger.h"

//Mod* CoreMod;

void CreateMod()
{
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
        CreateMod();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}