#include "mappers.h"
#include "../utils/utils.h"

#include <fstream>

bool UserDirMapper::Map(MapperMode map_mode, const char* detect) const
{
    if (!detect) {
        return false;
    }

    size_t fileNameLen = strlen(detect) + 1;
    if (fileNameLen <= 1) {
        return false;
    }

    std::string userDirPath = Utils::get_appdata_roaming_path();
    if (userDirPath.empty() || userDirPath.length() <= 1) {
        return false;
    }

    std::string fullUserDirPath = userDirPath + "\\" + detect;

    switch (map_mode)
    {
    case (MAPPER_MODE_ENABLE): {
        if (!CreateDirectoryA(fullUserDirPath.c_str(), NULL)) {
            if (GetLastError() != ERROR_ALREADY_EXISTS) {
                return false;
            }
        }

        break;
    }
    case (MAPPER_MODE_DISABLE): {
        if (!RemoveDirectoryA(fullUserDirPath.c_str())) {
            return false;
        }

        break;
    }
    default: {
        return false;
    }
    }

    return true;
}
bool GameDirMapper::Map(MapperMode map_mode, const char* detect) const
{
    if (!detect) {
        return false;
    }

    const size_t fileNameLen = strlen(detect) + 1;
    if (fileNameLen <= 1) {
        return false;
    }

    HMODULE hwMod = GetModuleHandleA("hl.exe");
    if (!hwMod) {
        hwMod = GetModuleHandleA("hw.dll");
        if (!hwMod) {
            return false;
        }
    }

    char dllPath[MAX_PATH]{};
    if (!GetModuleFileNameA(hwMod, dllPath, sizeof(dllPath))) {
        return false;
    }

    std::string fullPath = static_cast<std::string>(dllPath) + "\\" + detect;

    size_t fullPathInLastPoints = fullPath.find_last_of("./");
    if (fullPathInLastPoints != std::string::npos) {
        fullPath.erase(fullPathInLastPoints, 2);
    }
    fullPathInLastPoints = fullPath.find_last_of("../");
    if (fullPathInLastPoints != std::string::npos) {
        fullPath.erase(fullPathInLastPoints, 3);
    }

    switch (map_mode)
    {
    case (MAPPER_MODE_ENABLE): {
        size_t lastSlashInFull = fullPath.find_last_of("\\/");
        if (lastSlashInFull != std::string::npos) {
            std::string dirPath = fullPath.substr(0, lastSlashInFull);

            std::string currPath{};
            for (char c : dirPath) {
                currPath += c;
                if (c == '\\') {
                    if (!CreateDirectoryA(currPath.c_str(), NULL)) {
                        if (GetLastError() != ERROR_ALREADY_EXISTS) {
                            return false;
                        }
                    }
                }
            }
            if (!CreateDirectoryA(dirPath.c_str(), NULL)) {
                if (GetLastError() != ERROR_ALREADY_EXISTS) {
                    return false;
                }
            }
        }

        std::fstream gameFile(fullPath, std::ios::app);
        if (!gameFile.is_open()) {
            return false;
        }

        gameFile.close();
        break;
    }
    case (MAPPER_MODE_DISABLE): {
        if (!DeleteFileA(fullPath.c_str())) {
            if (GetLastError() != ERROR_FILE_NOT_FOUND) {
                return false;
            }
        }

        size_t lastSlashInFull = fullPath.find_last_of("\\/");
        if (lastSlashInFull != std::string::npos) {
            std::string dirPath = fullPath.substr(0, lastSlashInFull);
            if (!RemoveDirectoryA(dirPath.c_str())) {
                if (GetLastError() != ERROR_DIR_NOT_EMPTY && GetLastError() != ERROR_FILE_NOT_FOUND) {
                    return false;
                }
            }
        }

        break;
    }
    default: {
        return false;
    }
    }

    return true;
}
bool PatternMapper::Map(MapperMode map_mode, const char* detect) const
{
    if (!detect) {
        return false;
    }

    size_t patternLen = strlen(detect) + 1;
    if (patternLen <= 1) {
        return false;
    }

    switch (map_mode)
    {
    case (MAPPER_MODE_ENABLE): {
        char* patternMem = reinterpret_cast<char*>(VirtualAlloc(nullptr, patternLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
        if (!patternMem) {
            return false;
        }

        memcpy(patternMem, detect, patternLen);

        break;
    }
    case (MAPPER_MODE_DISABLE): {
        MEMORY_BASIC_INFORMATION mbi{};

        char* addr = nullptr;
        while (VirtualQuery(addr, &mbi, sizeof(mbi))) {
            if (mbi.State == MEM_COMMIT && mbi.Protect == PAGE_READWRITE && mbi.Type == MEM_PRIVATE && mbi.RegionSize == patternLen) {
                if (memcmp(mbi.BaseAddress, detect, patternLen - 1) == 0) {
                    if (!VirtualFree(mbi.BaseAddress, 0, MEM_RELEASE)) {
                        return false;
                    }
                }
            }
            addr = static_cast<char*>(mbi.BaseAddress) + mbi.RegionSize;
        }

        break;
    }
    default: {
        return false;
    }
    }

    return true;
}

IMapper* BaseMapper::GetMapper()
{
    return mapperPtr.get();
}
void BaseMapper::SetMapper(std::unique_ptr<IMapper> mapper_ptr)
{
    mapperPtr = std::move(mapper_ptr);
}

BaseMapper& BaseMapper::Get()
{
    static BaseMapper instance{ std::make_unique<UserDirMapper>() };
    return instance;
}