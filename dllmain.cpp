#include "dllmain.h"
#include "src/mappers/mappers.h"
#include "src/detects/detects.h"

#include <wtypes.h>
#include <vector>

static std::vector<const char*> g_vecEcdMapUserDirFileName;
static std::vector<const char*> g_vecEcdMapGameDirFileName;
static std::vector<const char*> g_vecEcdMapPattern;

void EntryPoint(HMODULE hMod)
{
    for (const auto& ecdMapUserDirFileName : Detects::ecdMapUserDirFileNames) {
        if (MAP_USERDIR_ENABLE(ecdMapUserDirFileName)) {
            g_vecEcdMapUserDirFileName.push_back(ecdMapUserDirFileName);
        }
    }
    for (const auto& ecdMapGameDirFileName : Detects::ecdMapGameDirFileNames) {
        if (MAP_GAMEDIR_ENABLE(ecdMapGameDirFileName)) {
            g_vecEcdMapGameDirFileName.push_back(ecdMapGameDirFileName);
        }
    }
    for (const auto& ecdMapPattern : Detects::ecdMapPatterns) {
        if (MAP_PATTERN_ENABLE(ecdMapPattern)) {
            g_vecEcdMapPattern.push_back(ecdMapPattern);
        }
    }

    const size_t mappedUserDirFileCount = g_vecEcdMapUserDirFileName.size();
    const size_t mappedGameDirFileCount = g_vecEcdMapGameDirFileName.size();
    const size_t mappedPatternCount = g_vecEcdMapPattern.size();

    char msg[4096]{};
    sprintf(msg, "Mapped \"UserDir\" files - %lu.\nMapped \"GameDir\" files - %lu.\nMapped \"Memory\" patterns - %lu.\n", mappedUserDirFileCount, mappedGameDirFileCount, mappedPatternCount);

    MessageBoxA(nullptr, msg, "ecd_sh1t", MB_OK);
}

BOOL APIENTRY DllMain(HMODULE hMod, DWORD Reason, LPVOID lpvReserved)
{
    if (Reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hMod);
        std::thread(EntryPoint, hMod).detach();
    }

    return TRUE;
}