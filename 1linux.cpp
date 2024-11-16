#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <array>
#include <memory>
#include <cstdio>
#include <cstring> // Include for strlen

std::vector<char> RunCommand(const std::string& command) {
    // Open a pipe to run the shell command
    std::array<char, 128> buffer;
    std::vector<char> output;

    // Open a process with popen
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Failed to open pipe");
    }

    // Read the output of the command
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        output.insert(output.end(), buffer.begin(), buffer.begin() + strlen(buffer.data()));
    }

    return output;
}

int main() {
    try {
        std::string command;
        std::cout << "Enter a command to execute: ";
        std::getline(std::cin, command); // Get command from user

        std::vector<char> output = RunCommand(command);

        // Convert output to a string and print it
        std::string result(output.begin(), output.end());
        std::cout << "Command Output:\n" << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
