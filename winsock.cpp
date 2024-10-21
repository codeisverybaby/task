#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <windows.h>
#include <vector>
#include <ws2spi.h> // For WSCGetProviderPath

#pragma comment(lib, "ws2_32.lib")
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
