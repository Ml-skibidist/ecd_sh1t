/* mappers.cpp - code for detects */

// Code includes
#include "mappers.h"
#include "../utils/utils.h"

// Using of this file includes
#include <fstream>

// UserDir map-code
bool UserDirMapper::Map(MapperMode map_mode, const char* detect) const
{
    // Check detect-file
    if (!detect) {
        return false;
    }

    // Check length of filename or filedir...
    size_t fileNameLen = strlen(detect) + 1;
    if (fileNameLen <= 1) {
        return false;
    }

    // Getting userdir path (appdata/roaming)
    std::string userDirPath = Utils::get_appdata_roaming_path();
    if (userDirPath.empty() || userDirPath.length() <= 1) {
        return false;
    }

    // Set fulldir path (appdata/roaming/...)
    std::string fullUserDirPath = userDirPath + "\\" + detect;

    // ENABLE/DISABLE modes
    switch (map_mode)
    {
    case (MAPPER_MODE_ENABLE): {
        // Just create new directory...
        if (!CreateDirectoryA(fullUserDirPath.c_str(), NULL)) {
            if (GetLastError() != ERROR_ALREADY_EXISTS) {
                return false;
            }
        }

        break;
    }
    case (MAPPER_MODE_DISABLE): {
        // Just delete new directory...
        if (!RemoveDirectoryA(fullUserDirPath.c_str())) {
            return false;
        }

        break;
    }
    default: {
        // Oops, unknown mode.
        return false;
    }
    }

    return true;
}
// GameDir map-code
bool GameDirMapper::Map(MapperMode map_mode, const char* detect) const
{
    // Check detect-file
    if (!detect) {
        return false;
    }

    // Check length of filename or filedir...
    const size_t fileNameLen = strlen(detect) + 1;
    if (fileNameLen <= 1) {
        return false;
    }

    // Getting full gamedir path
    std::string gamePath = Utils::get_game_path_cs16();
    if (gamePath.empty() || gamePath.length() <= 1) {
        return false;
    }

    // Find only dir
    size_t lastSlash = gamePath.find_last_of("\\/");
    if (lastSlash == std::string::npos) {
        return false;
    }

    // Create only dir path
    std::string gameDir = gamePath.substr(0, lastSlash);

    // Check for "./", "../"
    std::string detectStr(detect);
    std::string fullPath;
    if (detectStr.substr(0, 3) == "../") {
        // Jmp up lvl to "game/"
        size_t parentSlash = gameDir.find_last_of("\\/");
        if (parentSlash != std::string::npos) {
            gameDir = gameDir.substr(0, parentSlash);
        }
        detectStr = detectStr.substr(3);
        fullPath = gameDir + "\\" + detectStr;
    }
    else if (detectStr.substr(0, 2) == "./") {
        // Not jmp to up lvl
        detectStr = detectStr.substr(2);
        fullPath = gameDir + "\\" + detectStr;
    }
    /*
    else {
        fullPath = gameDir + "\\cstrike\\" + detectStr;
    }
    */


    // Change dir "/" to "\" for Windows
    for (auto& c : fullPath) {
        if (c == '/') {
            c = '\\';
        }
    }

    // Remove double-slashes
    size_t pos;
    while ((pos = fullPath.find("\\\\")) != std::string::npos) {
        fullPath.erase(pos, 1);
    }

    // Check corrected path
    if (fullPath.length() <= 3) {
        return false;
    }

    // ENABLE/DISABLE modes
    switch (map_mode)
    {
    case (MAPPER_MODE_ENABLE): {
        // Create full dirs
        size_t lastSlashInFull = fullPath.find_last_of("\\/");
        if (lastSlashInFull != std::string::npos) {
            std::string dirPath = fullPath.substr(0, lastSlashInFull);
            std::string currPath;
            for (size_t i = 0; i < dirPath.length(); ++i) {
                currPath += dirPath[i];
                if (dirPath[i] == '\\' || i == dirPath.length() - 1) {
                    if (currPath.length() >= 3) { // Пропускаем "C:"
                        CreateDirectoryA(currPath.c_str(), NULL);
                    }
                }
            }
        }

        // Create file
        std::fstream gameFile(fullPath, std::ios::app);
        if (!gameFile.is_open()) {
            return false;
        }
        gameFile.close();
        break;
    }
    case (MAPPER_MODE_DISABLE): {
        // Delete file
        DeleteFileA(fullPath.c_str());

        // Remove full dirs
        size_t lastSlashInFull = fullPath.find_last_of("\\/");
        if (lastSlashInFull != std::string::npos) {
            std::string dirPath = fullPath.substr(0, lastSlashInFull);
            RemoveDirectoryA(dirPath.c_str());
        }
        break;
    }
    default: {
        // Oops, unknown mode
        return false;
    }
    }

    return true;
}
// Pattern map-code
bool PatternMapper::Map(MapperMode map_mode, const char* detect) const
{
    // Check detect-file
    if (!detect) {
        return false;
    }

    // Check length of pattern...
    size_t patternLen = strlen(detect) + 1;
    if (patternLen <= 1) {
        return false;
    }

    // ENABLE/DISABLE modes
    switch (map_mode)
    {
    case (MAPPER_MODE_ENABLE): {
        // Alloc memory for pattern
        char* patternMem = reinterpret_cast<char*>(VirtualAlloc(nullptr, patternLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
        if (!patternMem) {
            return false;
        }

        // Put in memory pattern
        memcpy(patternMem, detect, patternLen);

        break;
    }
    case (MAPPER_MODE_DISABLE): {
        // Run on all memory
        MEMORY_BASIC_INFORMATION mbi{};
        char* addr = nullptr;
        while (VirtualQuery(addr, &mbi, sizeof(mbi))) {
            // Check MEM_PRIVATE (Allocated)
            if (mbi.State == MEM_COMMIT && mbi.Protect == PAGE_READWRITE && mbi.Type == MEM_PRIVATE && mbi.RegionSize == patternLen) {
                // Check memory bytes to pattern with need len
                if (memcmp(mbi.BaseAddress, detect, patternLen - 1) == 0) {
                    // Free memory
                    if (!VirtualFree(mbi.BaseAddress, 0, MEM_RELEASE)) {
                        return false;
                    }
                }
            }
            // Jmp to next addr
            addr = static_cast<char*>(mbi.BaseAddress) + mbi.RegionSize;
        }

        break;
    }
    default: {
        // Oops, unknown mode
        return false;
    }
    }

    return true;
}

// Interactive methods
IMapper* BaseMapper::GetMapper()
{
    // Just push unique ptr...
    return mapperPtr.get();
}
void BaseMapper::SetMapper(std::unique_ptr<IMapper> mapper_ptr)
{
    // Set strategy
    mapperPtr = std::move(mapper_ptr);
}

// Singleton
BaseMapper& BaseMapper::Get()
{
    // Our singleton
    static BaseMapper instance{ std::make_unique<UserDirMapper>() };
    return instance;
}