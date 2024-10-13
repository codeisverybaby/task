#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <Shlobj.h>  // For SHGetFolderPath

using namespace std;

// Function to query registry key and return values (using wide strings)
vector<wstring> QueryRegistryAutoRun(HKEY hKey, const wstring& subKey) {
    HKEY key;
    vector<wstring> autoRunPrograms;

    if (RegOpenKeyEx(hKey, subKey.c_str(), 0, KEY_READ, &key) == ERROR_SUCCESS) {
        wchar_t valueName[1024];
        wchar_t valueData[1024];
        DWORD valueNameSize, valueDataSize, valueType;
        DWORD index = 0;

        while (true) {
            valueNameSize = sizeof(valueName) / sizeof(wchar_t);
            valueDataSize = sizeof(valueData);
            LONG result = RegEnumValue(key, index, valueName, &valueNameSize, NULL, &valueType, (LPBYTE)valueData, &valueDataSize);

            if (result == ERROR_NO_MORE_ITEMS) {
                break;
            }

            if (result == ERROR_SUCCESS && valueType == REG_SZ) {
                wstring entry = wstring(valueName) + L" -> " + wstring(valueData);
                autoRunPrograms.push_back(entry);
            }
            index++;
        }

        RegCloseKey(key);
    }

    return autoRunPrograms;
}

// Function to display startup folder programs (wide strings)
void ListStartupFolderPrograms() {
    wchar_t startupPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_STARTUP, NULL, 0, startupPath))) {
        wcout << L"Startup Folder Path: " << startupPath << endl;

        WIN32_FIND_DATAW fileData;
        HANDLE hFind = FindFirstFileW((wstring(startupPath) + L"\\*").c_str(), &fileData);

        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    wcout << L"Startup Program: " << fileData.cFileName << endl;
                }
            } while (FindNextFileW(hFind, &fileData) != 0);
            FindClose(hFind);
        }
    }
    else {
        wcout << L"Failed to retrieve startup folder path." << endl;
    }
}

int wmain() {
    // List of autorun registry locations
    vector<wstring> locations = {
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        L"Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"
    };

    // Query HKCU and HKLM for autorun entries
    wcout << L"Auto-Start Programs in Current User (HKCU):" << endl;
    for (const auto& location : locations) {
        vector<wstring> results = QueryRegistryAutoRun(HKEY_CURRENT_USER, location);
        for (const auto& result : results) {
            wcout << result << endl;
        }
    }

    wcout << L"\nAuto-Start Programs in Local Machine (HKLM):" << endl;
    for (const auto& location : locations) {
        vector<wstring> results = QueryRegistryAutoRun(HKEY_LOCAL_MACHINE, location);
        for (const auto& result : results) {
            wcout << result << endl;
        }
    }

    // List startup folder programs
    wcout << L"\nPrograms in Startup Folder:" << endl;
    ListStartupFolderPrograms();

    return 0;
}
