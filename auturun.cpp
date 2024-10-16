#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <Shlobj.h>  // For SHGetFolderPath

using namespace std;

std::vector<std::wstring> locations = {
        L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\WinStations\\RDP-Tcp\\InitialProgram",
        L"System\\CurrentControlSet\\Control\\Terminal Server\\Wds\\rdpwd\\StartupPrograms",
        L"System\\CurrentControlSet\\Control\\Session Manager\\SetupExecute",
        L"System\\CurrentControlSet\\Control\\Session Manager\\S0InitialCommand",
        L"System\\CurrentControlSet\\Control\\Session Manager\\KnownDlls",
        L"System\\CurrentControlSet\\Control\\Session Manager\\Execute",
        L"System\\CurrentControlSet\\Control\\Session Manager\\BootExecute",
        L"System\\CurrentControlSet\\Control\\Session Manager\\AppCertDlls",
        L"SYSTEM\\CurrentControlSet\\Control\\SecurityProviders\\SecurityProviders",
        L"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\AlternateShell",
        L"SYSTEM\\CurrentControlSet\\Control\\Print\\Providers",
        L"SYSTEM\\CurrentControlSet\\Control\\Print\\Monitors",
        L"SYSTEM\\CurrentControlSet\\Control\\NetworkProvider\\Order",
        L"SYSTEM\\CurrentControlSet\\Control\\Lsa\\Notification Packages",
        L"SYSTEM\\CurrentControlSet\\Control\\Lsa\\Authentication Packages",
        L"System\\CurrentControlSet\\Control\\BootVerificationProgram\\ImagePath",
        L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\ShellServiceObjectDelayLoad",
        L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run",
        L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellServiceObjects",
        L"Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers",
        L"Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellExecuteHooks",
        L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Explorer\\SharedTaskScheduler",
        L"Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects",
        L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\Appinit_Dlls",
        L"Software\\Wow6432Node\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options",
        L"Software\\Wow6432Node\\Microsoft\\Windows NT\\CurrentVersion\\Drivers32",
        L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows CE Services\\AutoStartOnDisconnect",
        L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows CE Services\\AutoStartOnConnect",
        L"Software\\Wow6432Node\\Microsoft\\Office\\Word\\Addins",
        L"Software\\Wow6432Node\\Microsoft\\Office\\PowerPoint\\Addins",
        L"Software\\Wow6432Node\\Microsoft\\Office\\Outlook\\Addins",
        L"Software\\Wow6432Node\\Microsoft\\Office\\Onenote\\Addins",
        L"Software\\Wow6432Node\\Microsoft\\Office\\Excel\\Addins",
        L"Software\\Wow6432Node\\Microsoft\\Office\\Access\\Addins",
        L"Software\\Wow6432Node\\Microsoft\\Internet Explorer\\Toolbar",
        L"Software\\Wow6432Node\\Microsoft\\Internet Explorer\\Extensions",
        L"Software\\Wow6432Node\\Microsoft\\Internet Explorer\\Explorer Bars",
        L"Software\\Wow6432Node\\Microsoft\\Command Processor\\Autorun",
        L"SOFTWARE\\Wow6432Node\\Microsoft\\Active Setup\\Installed Components",
        L"Software\\Wow6432Node\\Classes\\Folder\\ShellEx\\PropertySheetHandlers",
        L"Software\\Wow6432Node\\Classes\\Folder\\ShellEx\\ExtShellFolderViews",
        L"Software\\Wow6432Node\\Classes\\Folder\\ShellEx\\DragDropHandlers",
        L"Software\\Wow6432Node\\Classes\\Folder\\ShellEx\\ContextMenuHandlers",
        L"Software\\Wow6432Node\\Classes\\Folder\\Shellex\\ColumnHandlers",
        L"Software\\Wow6432Node\\Classes\\Drive\\ShellEx\\ContextMenuHandlers",
        L"Software\\Wow6432Node\\Classes\\Directory\\Shellex\\PropertySheetHandlers",
        L"Software\\Wow6432Node\\Classes\\Directory\\Shellex\\DragDropHandlers",
        L"Software\\Wow6432Node\\Classes\\Directory\\Shellex\\CopyHookHandlers",
        L"Software\\Wow6432Node\\Classes\\Directory\\ShellEx\\ContextMenuHandlers",
        L"Software\\Wow6432Node\\Classes\\Directory\\Background\\ShellEx\\ContextMenuHandlers",
        L"Software\\Wow6432Node\\Classes\\CLSID\\{AC757296-3522-4E11-9862-C17BE5A1767E}\\Instance",
        L"Software\\Wow6432Node\\Classes\\CLSID\\{ABE3B9A4-257D-4B97-BD1A-294AF496222E}\\Instance",
        L"Software\\Wow6432Node\\Classes\\CLSID\\{7ED96837-96F0-4812-B211-F13C24117ED3}\\Instance",
        L"Software\\Wow6432Node\\Classes\\CLSID\\{083863F1-70DE-11d0-BD40-00A0C911CE86}\\Instance",
        L"Software\\Wow6432Node\\Classes\\AllFileSystemObjects\\ShellEx\\PropertySheetHandlers",
        L"Software\\Wow6432Node\\Classes\\AllFileSystemObjects\\ShellEx\\DragDropHandlers",
        L"Software\\Wow6432Node\\Classes\\AllFileSystemObjects\\ShellEx\\ContextMenuHandlers",
        L"Software\\Wow6432Node\\Classes\\*\\ShellEx\\PropertySheetHandlers",
        L"Software\\Wow6432Node\\Classes\\*\\ShellEx\\ContextMenuHandlers",
        L"Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Startup",
        L"Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Shutdown",
        L"Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logon",
        L"Software\\Policies\\Microsoft\\Windows\\System\\Scripts\\Logoff",
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\ShellServiceObjectDelayLoad",
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\Shell",
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\Run",
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Startup",
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Shutdown",
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logon",
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logoff",
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellServiceObjects",
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers",
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellExecuteHooks",
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\SharedTaskScheduler",
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects",
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\PLAP Providers",
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\Credential Providers",
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\Credential Provider Filters",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\VmApplet",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Userinit",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Taskman",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Shell",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\GpExtensions",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\AppSetup",
        L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\AlternateShells\\AvailableShells",
        L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\IconServiceLib",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\Appinit_Dlls",
        L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Font Drivers",
        L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Drivers32",
        L"SOFTWARE\\Microsoft\\Windows CE Services\\AutoStartOnDisconnect",
        L"SOFTWARE\\Microsoft\\Windows CE Services\\AutoStartOnConnect",
        L"Software\\Microsoft\\Office\\Word\\Addins",
        L"Software\\Microsoft\\Office\\PowerPoint\\Addins",
        L"Software\\Microsoft\\Office\\Outlook\\Addins",
        L"Software\\Microsoft\\Office\\Onenote\\Addins",
        L"Software\\Microsoft\\Office\\Excel\\Addins",
        L"Software\\Microsoft\\Office\\Access\\Addins",
        L"SOFTWARE\\Microsoft\\Office test\\Special\\Perf",
        L"Software\\Microsoft\\Internet Explorer\\Toolbar",
        L"Software\\Microsoft\\Internet Explorer\\Extensions",
        L"Software\\Microsoft\\Internet Explorer\\Explorer Bars",
        L"SYSTEM\\Setup\\CmdLine",
        L"Software\\Microsoft\\Ctf\\LangBarAddin",
        L"Software\\Microsoft\\Command Processor\\Autorun",
        L"SOFTWARE\\Microsoft\\Active Setup\\Installed Components",
        L"SOFTWARE\\Classes\\Protocols\\Handler",
        L"SOFTWARE\\Classes\\Protocols\\Filter",
        L"SOFTWARE\\Classes\\Htmlfile\\Shell\\Open\\Command\\(Default)",
        L"Software\\Classes\\Folder\\ShellEx\\PropertySheetHandlers",
        L"Software\\Classes\\Folder\\ShellEx\\ExtShellFolderViews",
        L"Software\\Classes\\Folder\\ShellEx\\DragDropHandlers",
        L"Software\\Classes\\Folder\\ShellEx\\ContextMenuHandlers",
        L"Software\\Classes\\Folder\\Shellex\\ColumnHandlers",
        L"Software\\Classes\\Filter",
        L"SOFTWARE\\Classes\\Exefile\\Shell\\Open\\Command\\(Default)",
        L"Software\\Classes\\Drive\\ShellEx\\ContextMenuHandlers",
        L"Software\\Classes\\Directory\\Shellex\\PropertySheetHandlers",
        L"Software\\Classes\\Directory\\Shellex\\DragDropHandlers",
        L"Software\\Classes\\Directory\\Shellex\\CopyHookHandlers",
        L"Software\\Classes\\Directory\\ShellEx\\ContextMenuHandlers",
        L"Software\\Classes\\Directory\\Background\\ShellEx\\ContextMenuHandlers",
        L"Software\\Classes\\CLSID\\{AC757296-3522-4E11-9862-C17BE5A1767E}\\Instance",
        L"Software\\Classes\\CLSID\\{ABE3B9A4-257D-4B97-BD1A-294AF496222E}\\Instance",
        L"Software\\Classes\\CLSID\\{7ED96837-96F0-4812-B211-F13C24117ED3}\\Instance",
        L"Software\\Classes\\CLSID\\{083863F1-70DE-11d0-BD40-00A0C911CE86}\\Instance",
        L"Software\\Classes\\AllFileSystemObjects\\ShellEx\\PropertySheetHandlers",
        L"Software\\Classes\\AllFileSystemObjects\\ShellEx\\DragDropHandlers",
        L"Software\\Classes\\AllFileSystemObjects\\ShellEx\\ContextMenuHandlers",
        L"Software\\Classes\\*\\ShellEx\\PropertySheetHandlers",
        L"Software\\Classes\\*\\ShellEx\\ContextMenuHandlers",
        L"Environment\\UserInitMprLogonScript",
        L"SOFTWARE\\Policies\\Microsoft\\Windows\\Control Panel\\Desktop\\Scrnsave.exe",
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\Protocol_Catalog9\\Catalog_Entries64",
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\Protocol_Catalog9\\Catalog_Entries",
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\NameSpace_Catalog5\\Catalog_Entries64",
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\NameSpace_Catalog5\\Catalog_Entries",
        L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\Run",
        L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\Load",
        L"Software\\Microsoft\\Internet Explorer\\UrlSearchHooks",
        L"SOFTWARE\\Microsoft\\Internet Explorer\\Desktop\\Components",
        L"Software\\Classes\\Clsid\\{AB8902B4-09CA-4bb6-B78D-A8F59079A8D5}\\Inprocserver32",
        L"Control Panel\\Desktop\\Scrnsave.exe"
};


std::vector<std::wstring> logon = {
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
        L"System\\CurrentControlSet\\Control\\Terminal Server\\Wds\\rdpwd\\StartupPrograms",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\VmApplet",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Userinit",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Taskman",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Shell",
        L"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\AlternateShell",
        L"SOFTWARE\\Wow6432Node\\Microsoft\\Active Setup\\Installed Components",
        L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows\\IconServiceLib",
        L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run",
        L"SOFTWARE\\Microsoft\\Active Setup\\Installed Components",
};


std::vector<std::wstring> Explorer = {
        L"Software\\Classes\\*\\ShellEx\\ContextMenuHandlers",
        L"Software\\Classes\\Directory\\ShellEx\\ContextMenuHandlers",
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellServiceObjects",
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellServiceObjects",
        L"SOFTWARE\\Classes\\Protocols\\Filter",
        L"SOFTWARE\\Classes\\Protocols\\Handler",
        L"Software\\Classes\\Drive\\ShellEx\\ContextMenuHandlers",
        L"Software\\Classes\\*\\ShellEx\\PropertySheetHandlers",
        L"Software\\Classes\\AllFileSystemObjects\\ShellEx\\ContextMenuHandlers",
        L"Software\\Classes\\AllFileSystemObjects\\ShellEx\\PropertySheetHandlers",
        L"Software\\Classes\\AllFileSystemObjects\\ShellEx\\DragDropHandlers",
        L"Software\\Classes\\Directory\\Shellex\\PropertySheetHandlers",
        L"Software\\Classes\\Directory\\Shellex\\DragDropHandlers",
        L"Software\\Classes\\Directory\\Shellex\\CopyHookHandlers",
        L"Software\\Classes\\Directory\\ShellEx\\ContextMenuHandlers",
        L"Software\\Classes\\Directory\\Background\\ShellEx\\ContextMenuHandlers",
        L"Software\\Classes\\Folder\\ShellEx\\PropertySheetHandlers",
        L"Software\\Classes\\Folder\\ShellEx\\ExtShellFolderViews",
        L"Software\\Classes\\Folder\\ShellEx\\DragDropHandlers",
        L"Software\\Classes\\Folder\\ShellEx\\ContextMenuHandlers",
        L"Software\\Classes\\Folder\\Shellex\\ColumnHandlers",
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers",

        L"Software\\Wow6432Node\\Classes\\*\\ShellEx\\ContextMenuHandlers",
        L"Software\\Wow6432Node\\Classes\\Directory\\ShellEx\\ContextMenuHandlers",
        L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellServiceObjects",
        L"SOFTWARE\\Wow6432Node\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellServiceObjects",
        L"SOFTWARE\\Wow6432Node\\Classes\\Protocols\\Filter",
        L"SOFTWARE\\Wow6432Node\\Classes\\Protocols\\Handler",
        L"Software\\Wow6432Node\\Classes\\Drive\\ShellEx\\ContextMenuHandlers",
        L"Software\\Wow6432Node\\Classes\\*\\ShellEx\\PropertySheetHandlers",
        L"Software\\Wow6432Node\\Classes\\AllFileSystemObjects\\ShellEx\\ContextMenuHandlers",
        L"Software\\Wow6432Node\\Classes\\AllFileSystemObjects\\ShellEx\\PropertySheetHandlers",
        L"Software\\Wow6432Node\\Classes\\AllFileSystemObjects\\ShellEx\\DragDropHandlers",
        L"Software\\Wow6432Node\\Classes\\Directory\\Shellex\\PropertySheetHandlers",
        L"Software\\Wow6432Node\\Classes\\Directory\\Shellex\\DragDropHandlers",
        L"Software\\Wow6432Node\\Classes\\Directory\\Shellex\\CopyHookHandlers",
        L"Software\\Wow6432Node\\Classes\\Directory\\ShellEx\\ContextMenuHandlers",
        L"Software\\Wow6432Node\\Classes\\Directory\\Background\\ShellEx\\ContextMenuHandlers",
        L"Software\\Wow6432Node\\Classes\\Folder\\ShellEx\\PropertySheetHandlers",
        L"Software\\Wow6432Node\\Classes\\Folder\\ShellEx\\ExtShellFolderViews",
        L"Software\\Wow6432Node\\Classes\\Folder\\ShellEx\\DragDropHandlers",
        L"Software\\Wow6432Node\\Classes\\Folder\\ShellEx\\ContextMenuHandlers",
        L"Software\\Wow6432Node\\Classes\\Folder\\Shellex\\ColumnHandlers",
        L"Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers",
};


std::vector<std::wstring> InternetExplorer = {
        L"Software\\Microsoft\\Internet Explorer\\UrlSearchHooks",
        L"Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects",
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects",
        L"Software\\Wow6432Node\\Microsoft\\Internet Explorer\\Extensions",
        L"Software\\Microsoft\\Internet Explorer\\Extensions",

};

std::vector<std::wstring> Services = {
        
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\Protocol_Catalog9\\Catalog_Entries64",
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\Protocol_Catalog9\\Catalog_Entries",
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\NameSpace_Catalog5\\Catalog_Entries64",
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\NameSpace_Catalog5\\Catalog_Entries",
        L"System\\CurrentControlSet\\Services",
};

std::vector<std::wstring> Drivers = {

        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\Protocol_Catalog9\\Catalog_Entries64",
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\Protocol_Catalog9\\Catalog_Entries",
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\NameSpace_Catalog5\\Catalog_Entries64",
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\NameSpace_Catalog5\\Catalog_Entries",
        L"System\\CurrentControlSet\\Services",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Font Drivers",

};

std::vector<std::wstring> Codecs = {
        L"Software\\Wow6432Node\\Microsoft\\Windows NT\\CurrentVersion\\Drivers32",
        L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Drivers32",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Font Drivers",
        L"Software\\Wow6432Node\\Classes\\CLSID\\{AC757296-3522-4E11-9862-C17BE5A1767E}\\Instance",
        L"Software\\Wow6432Node\\Classes\\CLSID\\{ABE3B9A4-257D-4B97-BD1A-294AF496222E}\\Instance",
        L"Software\\Wow6432Node\\Classes\\CLSID\\{7ED96837-96F0-4812-B211-F13C24117ED3}\\Instance",
        L"Software\\Wow6432Node\\Classes\\CLSID\\{083863F1-70DE-11d0-BD40-00A0C911CE86}\\Instance",
        L"Software\\Classes\\CLSID\\{AC757296-3522-4E11-9862-C17BE5A1767E}\\Instance",
        L"Software\\Classes\\CLSID\\{ABE3B9A4-257D-4B97-BD1A-294AF496222E}\\Instance",
        L"Software\\Classes\\CLSID\\{7ED96837-96F0-4812-B211-F13C24117ED3}\\Instance",
        L"Software\\Classes\\CLSID\\{083863F1-70DE-11d0-BD40-00A0C911CE86}\\Instance",


};


std::vector<std::wstring> BootExecute = {
        L"System\\CurrentControlSet\\Control\\Session Manager\\SetupExecute",
        L"System\\CurrentControlSet\\Control\\Session Manager\\S0InitialCommand",
        L"System\\CurrentControlSet\\Control\\Session Manager\\KnownDlls",
        L"System\\CurrentControlSet\\Control\\Session Manager\\Execute",
        L"System\\CurrentControlSet\\Control\\Session Manager\\BootExecute",
        L"System\\CurrentControlSet\\Control\\Session Manager\\AppCertDlls",


};



std::vector<std::wstring> ImageHijacks = {
        L"SOFTWARE\\Classes\\Exefile\\Shell\\Open\\Command\\(Default)",



};


std::vector<std::wstring> KnownDLLs = {
        L"System\\CurrentControlSet\\Control\\Session Manager\\KnownDlls",

};

std::vector<std::wstring> Winlogon = {
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\Credential Providers",
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\Credential Provider Filters",
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\PLAP Providers",
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\GpExtensions",


};

std::vector<std::wstring> WinsockPro = {
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\Protocol_Catalog9\\Catalog_Entries64",
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\Protocol_Catalog9\\Catalog_Entries",
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\NameSpace_Catalog5\\Catalog_Entries64",
        L"System\\CurrentControlSet\\Services\\WinSock2\\Parameters\\NameSpace_Catalog5\\Catalog_Entries",

};

std::vector<std::wstring> PrintMonitors = {
        L"SYSTEM\\CurrentControlSet\\Control\\Print\\Providers",
        L"SYSTEM\\CurrentControlSet\\Control\\Print\\Monitors",

};

std::vector<std::wstring> LSA = {
        L"SYSTEM\\CurrentControlSet\\Control\\Lsa\\Notification Packages",
        L"SYSTEM\\CurrentControlSet\\Control\\Lsa\\Authentication Packages",
        L"SYSTEM\\CurrentControlSet\\Control\\SecurityProviders\\SecurityProviders",

};

std::vector<std::wstring> Network = {
        L"SYSTEM\\CurrentControlSet\\Control\\NetworkProvider\\Order",

};

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
