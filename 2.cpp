#include <windows.h>
#include <vector>
#include <string>
#include <iostream>

std::vector<BYTE> ReadFileBytes(const std::wstring& filePath) {
    std::vector<BYTE> fileData;
    HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to open file for reading, error: " << GetLastError() << std::endl;
        return fileData; 
    }

    DWORD fileSize = GetFileSize(hFile, nullptr);
    if (fileSize == INVALID_FILE_SIZE) {
        std::wcerr << L"Failed to get file size, error: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return fileData;
    }

    fileData.resize(fileSize);
    DWORD bytesRead;
    if (!ReadFile(hFile, fileData.data(), fileSize, &bytesRead, nullptr)) {
        std::wcerr << L"Failed to read file, error: " << GetLastError() << std::endl;
        fileData.clear();
    }

    CloseHandle(hFile);
    return fileData;
}

bool WriteFileBytes(const std::wstring& filePath, const std::vector<BYTE>& data) {
    HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to open file for writing, error: " << GetLastError() << std::endl;
        return false;
    }

    DWORD bytesWritten;
    if (!WriteFile(hFile, data.data(), data.size(), &bytesWritten, nullptr) || bytesWritten != data.size()) {
        std::wcerr << L"Failed to write file, error: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return false;
    }

    CloseHandle(hFile);
    return true;
}

int main() {
    // Example file path
    std::wstring filePath = L"D:\\test\\screenshot.exe";

    std::vector<BYTE> fileData = ReadFileBytes(filePath);
    if (!fileData.empty()) {
        std::wcout << L"File read successfully, size: " << fileData.size() << L" bytes." << std::endl;
    }


    std::wstring outputFilePath = L"D:\\test\\SystemInfo.exe";
    if (WriteFileBytes(outputFilePath, fileData)) {
        std::wcout << L"File written successfully to " << outputFilePath << std::endl;
    }

    return 0;
}

