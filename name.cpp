#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <lmcons.h>
#else
#include <unistd.h>
#include <sys/utsname.h>
#include <pwd.h>
#endif

std::string getComputerName() {
#ifdef _WIN32
    char computerName[UNLEN + 1];
    DWORD size = sizeof(computerName);
    if (GetComputerNameA(computerName, &size)) {
        return std::string(computerName);
    } else {
        return "Error retrieving computer name";
    }
#else
    struct utsname buffer;
    if (uname(&buffer) == 0) {
        return std::string(buffer.nodename);
    } else {
        return "Error retrieving computer name";
    }
#endif
}

std::string getUserName() {
#ifdef _WIN32
    char userName[UNLEN + 1];
    DWORD size = sizeof(userName);
    if (GetUserNameA(userName, &size)) {
        return std::string(userName);
    } else {
        return "Error retrieving user name";
    }
#else
    struct passwd *pw = getpwuid(getuid());
    if (pw) {
        return std::string(pw->pw_name);
    } else {
        return "Error retrieving user name";
    }
#endif
}

int main() {
    std::cout << "Computer Name: " << getComputerName() << std::endl;
    std::cout << "User Name: " << getUserName() << std::endl;
    return 0;
}
