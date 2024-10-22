#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <windows.h>
#include <vector>
#include <ws2spi.h> // For WSCGetProviderPath

#pragma comment(lib, "ws2_32.lib")

enum Day {
    Logon = 1,    // 1
    Explorer = 2,    // 2
    InternetExplorer = 3,   // 3
    ScheduledTasks = 4,
    Services = 5, // 4
    Drivers = 6,  // 5
    Codecs = 7,    // 6
    BootExecute = 8,   // 7
    ImageHijacks = 9,    // 1
    KnownDLLs = 10,    // 2
    Winlogon = 11,   // 3
    WinsockPro = 12, // 4
    PrintMonitors = 13,  // 5
    LSAProviders = 14,    // 6
    NetworkProviders = 15,   // 7
    Office = 16
};

//Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Schedule\TaskCache\Tasks\{8A4F4A97-1540-4F87-A97D-C9D5ED92572B}

//Computer\HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\LanmanWorkstation\NetworkProvider
//Computer\HKEY_USERS\S-1-5-21-1779779872-2669267369-1413570996-1001\Software\Classes\Local Settings\MuiCache\506\52C64B7E
void PrintProtocolInfo(WSAPROTOCOL_INFO& protocolInfo) {
    std::wcout << L"Provider Name: " << (protocolInfo.szProtocol[0] ? protocolInfo.szProtocol : L"Unknown") << std::endl;
    std::wcout << L"Service Flags: 0x" << std::hex << protocolInfo.dwServiceFlags1 << std::endl;
    std::wcout << L"Protocol Chain Length: " << protocolInfo.ProtocolChain.ChainLen << std::endl;
    std::wcout << L"Address Family: " << protocolInfo.iAddressFamily << std::endl;
    std::wcout << L"Socket Type: " << protocolInfo.iSocketType << std::endl;
    std::wcout << L"Protocol: " << protocolInfo.iProtocol << std::endl;
    std::wcout << L"Max Message Size: " << protocolInfo.dwMessageSize << std::endl;
    std::wcout << L"Network Byte Order: " << protocolInfo.iNetworkByteOrder << std::endl;
    std::wcout << L"Security Scheme: " << protocolInfo.iSecurityScheme << std::endl;

    // Retrieve the provider's DLL path using WSCGetProviderPath
    WCHAR dllPath[MAX_PATH];
    int dllPathLen = MAX_PATH;  // Declare a local variable of type int
    LPINT lpDllPathLen = &dllPathLen;  // Use LPINT (which is a pointer to int)

    int errCode = WSCGetProviderPath(&protocolInfo.ProviderId, dllPath, lpDllPathLen, nullptr);
    if (errCode == 0) {
        std::wcout << L"Provider DLL Path: " << dllPath << std::endl;
    }
    else {
        std::wcout << L"Failed to get DLL path, error: " << WSAGetLastError() << std::endl;
    }

    std::wcout << L"----------------------" << std::endl;
}

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Determine the size of buffer needed
    DWORD bufferLength = 0;
    WSAEnumProtocols(nullptr, nullptr, &bufferLength);

    if (bufferLength == 0) {
        std::cerr << "Failed to get buffer size for WSAEnumProtocols." << std::endl;
        WSACleanup();
        return 1;
    }

    std::vector<BYTE> buffer(bufferLength);
    WSAPROTOCOL_INFO* protocolInfo = reinterpret_cast<WSAPROTOCOL_INFO*>(buffer.data());

    // Get the protocol information
    int protocolCount = WSAEnumProtocols(nullptr, protocolInfo, &bufferLength);
    if (protocolCount == SOCKET_ERROR) {
        std::cerr << "WSAEnumProtocols failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    std::wcout << L"Number of protocols: " << protocolCount << std::endl;
    std::wcout << L"----------------------" << std::endl;

    for (int i = 0; i < protocolCount; ++i) {
        PrintProtocolInfo(protocolInfo[i]);
    }

    WSACleanup();
    return 0;
}
