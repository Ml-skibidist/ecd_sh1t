/* dllmain.cpp - entrypoint. */

// PreCompiled includes
#include "dllmain.h"

// Methods includes
#include "src/mappers/mappers.h"
#include "src/detects/detects.h"

// Using of this file includes
#include <wtypes.h>
#include <vector>

// Needable vectors, for known "how many we mapped methods?"
static std::vector<const char*> g_vecEcdMapUserDirFileName;
static std::vector<const char*> g_vecEcdMapGameDirFileName;
static std::vector<const char*> g_vecEcdMapPattern;

// Dll EntryPoint
void EntryPoint(HMODULE hMod)
{
    // UserDir mapping
    for (const auto& cs16_ecdMapUserDirFileNames : Detects::cs16_ecdMapUserDirFileNames) {
        if (MAP_GAMEDIR_ENABLE(cs16_ecdMapUserDirFileNames)) {
            g_vecEcdMapUserDirFileName.push_back(cs16_ecdMapUserDirFileNames);
        }
    }
    // GameDir mapping
    for (const auto& cs16_ecdMapGameDirFileName : Detects::cs16_ecdMapGameDirFileNames) {
        if (MAP_GAMEDIR_ENABLE(cs16_ecdMapGameDirFileName)) {
            g_vecEcdMapGameDirFileName.push_back(cs16_ecdMapGameDirFileName);
        }
    }
    // Pattern mapping
    for (const auto& cs16_ecdMapPatterns : Detects::cs16_ecdMapPatterns) {
        if (MAP_GAMEDIR_ENABLE(cs16_ecdMapPatterns)) {
            g_vecEcdMapPattern.push_back(cs16_ecdMapPatterns);
        }
    }

    // Take mapped size
    const size_t mappedUserDirFileCount = g_vecEcdMapUserDirFileName.size();
    const size_t mappedGameDirFileCount = g_vecEcdMapGameDirFileName.size();
    const size_t mappedPatternCount = g_vecEcdMapPattern.size();

    // Create string for message in call "MessageBoxA(...)"
    char msg[4096]{};
    sprintf(msg, "Mapped \"UserDir\" files - %lu.\nMapped \"GameDir\" files - %lu.\nMapped \"Memory\" patterns - %lu.\n", mappedUserDirFileCount, mappedGameDirFileCount, mappedPatternCount);

    // Print message
    MessageBoxA(nullptr, msg, "ecd_sh1t", MB_OK);

    // Detach dll from game
    FreeLibraryAndExitThread(hMod, 0);
}

// Callback entrypoint
BOOL APIENTRY DllMain(HMODULE hMod, DWORD Reason, LPVOID lpvReserved)
{
    if (Reason == DLL_PROCESS_ATTACH) {
        // Just start thread for entrypoint, we dont write code her...
        DisableThreadLibraryCalls(hMod);
        std::thread(EntryPoint, hMod).detach();
    }

    return TRUE;
}