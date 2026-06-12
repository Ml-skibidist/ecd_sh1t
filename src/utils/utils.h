/* utils.h - using for src and e.t.c. */

#ifndef UTILS_H
#define UTILS_H

// Using of this file includes
#include <string>
#include <ShlObj.h>
#include <cstdlib>

namespace Utils
{
    // Converts wchar_t -> char
    std::string WCharToChar(const wchar_t* wstr) {
        if (!wstr || lstrlenW(wstr) == 0) {
            return "";
        }

        int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
        if (size <= 0) {
            return "";
        }

        std::string chrStr(size - 1, 0);
        if (WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &chrStr[0], size, NULL, NULL) <= 0) {
            return "";
        }

        return chrStr;
    }
    std::string WCharToChar(const std::wstring& wstr) {
        return WCharToChar(wstr.c_str());
    }

    // Getters of file paths
    std::string get_appdata_roaming_path() {
        // Getting by SH
        char shBuffer[MAX_PATH]{};
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, shBuffer))) {
            return shBuffer;
        }

        // Fallbacks
        PWSTR shKnownBuffer = nullptr;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &shKnownBuffer))) {
            std::string path = WCharToChar(shKnownBuffer);
            CoTaskMemFree(shKnownBuffer);
            return path;
        }

        return "";
    }
    std::string get_game_path_cs16() {
        // !!! It`s hardcoded gamepath, for use other game`s u need create new function
        // !!! If u create new function, take ONLY with game module!

        // Getting game module
        HMODULE hlBase = GetModuleHandleA("hl.exe");
        if (!hlBase) {
            // Fallbacks
            hlBase = GetModuleHandleA("hw.dll");
            if (!hlBase) {
                return "";
            }
        }

        // Getting path to game module
        char dllPath[MAX_PATH]{};
        if (!GetModuleFileNameA(hlBase, dllPath, sizeof(dllPath))) {
            return "";
        }

        return dllPath;
    }
}

#endif // UTILS_H