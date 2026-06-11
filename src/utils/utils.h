#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <ShlObj.h>
#include <cstdlib>

namespace Utils
{
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

    std::string get_appdata_roaming_path() {
        char shBuffer[MAX_PATH]{};
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, shBuffer))) {
            return shBuffer;
        }

        PWSTR shKnownBuffer = nullptr;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &shKnownBuffer))) {
            std::string path = WCharToChar(shKnownBuffer);
            CoTaskMemFree(shKnownBuffer);
            return path;
        }

        return "";
    }
}

#endif // UTILS_H