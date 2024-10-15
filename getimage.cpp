#include <windows.h>
#include <iostream>
#include <string>
#include <Shlwapi.h>  // For PathFileExistsW
#pragma comment(lib, "Shlwapi.lib")  // Link to Shlwapi.lib for PathFileExistsW

using namespace std;

// Function to retrieve the path associated with a COM object (GUID) from the registry
wstring ResolveGuidToImagePath(const wstring& guid) {
    wstring clsidPath = L"CLSID\\" + guid;
    HKEY hKey;
    wchar_t valueData[1024];
    DWORD valueDataSize = sizeof(valueData);

    // Try to open the CLSID registry key
    if (RegOpenKeyExW(HKEY_CLASSES_ROOT, clsidPath.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        // First check the LocalServer32 key (for EXE-based COM servers)
        if (RegQueryValueExW(hKey, L"LocalServer32", NULL, NULL, (LPBYTE)valueData, &valueDataSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            if (PathFileExistsW(valueData)) {
                return wstring(valueData);  // Return the EXE path if found
            }
        }

        // Next check the InprocServer32 key (for DLL-based COM servers)
        valueDataSize = sizeof(valueData);  // Reset buffer size
        if (RegQueryValueExW(hKey, L"InprocServer32", NULL, NULL, (LPBYTE)valueData, &valueDataSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            if (PathFileExistsW(valueData)) {
                return wstring(valueData);  // Return the DLL path if found
            }
        }

        RegCloseKey(hKey);
    }

    return L"(Unknown Path or COM Server Not Found)";
}

// Example usage
int main() {
    // Example GUID valueData
    wstring guid = L"{B41DB860-8EE4-11D2-9906-E49FADC173CA}";

    // Resolve the GUID to an image path
    wstring imagePath = ResolveGuidToImagePath(guid);

    // Output the resolved image path
    wcout << L"Resolved Image Path for " << guid << L": " << imagePath << endl;

    return 0;
}
