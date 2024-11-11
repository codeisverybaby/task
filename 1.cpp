#include <Windows.h>
#include <vector>
#include <stdexcept>
#include <string>

std::vector<BYTE> RunCommand(const std::wstring& command) {
    // Initialize necessary variables
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;  // Allow handle inheritance
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE hStdOutRead = NULL;
    HANDLE hStdOutWrite = NULL;

    // Create pipe for STDOUT
    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &saAttr, 0)) {
        throw std::runtime_error("Failed to create pipe");
    }

    // Ensure the read handle to the pipe is not inherited
    if (!SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0)) {
        CloseHandle(hStdOutRead);
        CloseHandle(hStdOutWrite);
        throw std::runtime_error("Failed to set handle information");
    }

    // Set up the process startup info
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.hStdOutput = hStdOutWrite;
    si.hStdError = hStdOutWrite;  // Redirect stderr to the same pipe
    si.dwFlags |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    // Convert command to a wide-character string for cmd.exe
    std::wstring cmdLine = L"cmd.exe /c " + command;

    // Create the child process
    if (!CreateProcessW(NULL, &cmdLine[0], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        CloseHandle(hStdOutRead);
        CloseHandle(hStdOutWrite);
        throw std::runtime_error("Failed to create process");
    }

    // Close the write handle to avoid blocking the process
    CloseHandle(hStdOutWrite);

    // Read output from the command
    std::vector<BYTE> output;
    DWORD bytesRead;
    const DWORD bufferSize = 4096;
    BYTE buffer[bufferSize];

    while (ReadFile(hStdOutRead, buffer, bufferSize, &bytesRead, NULL) && bytesRead > 0) {
        output.insert(output.end(), buffer, buffer + bytesRead);
    }

    // Clean up handles
    CloseHandle(hStdOutRead);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return output;
}

int main() {
    try {
        std::wstring command = L"ipconfig";
        std::vector<BYTE> output = RunCommand(command);

        // Convert the output to a wstring (assuming it's UTF-8 or compatible)
        std::wstring result(output.begin(), output.end());
        wprintf(L"Command Output:\n%s", result.c_str());
    }
    catch (const std::exception& e) {
        wprintf(L"Error: %S\n", e.what());
    }
    return 0;
}
