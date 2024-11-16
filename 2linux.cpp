#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<char> ReadFileBytes(const std::string& filePath) {
    std::vector<char> fileData;

    // Open file in binary mode
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for reading: " << filePath << std::endl;
        return fileData; // Return an empty vector
    }

    // Determine file size
    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read file content
    fileData.resize(fileSize);
    if (!file.read(fileData.data(), fileSize)) {
        std::cerr << "Failed to read file: " << filePath << std::endl;
        fileData.clear();
    }

    return fileData;
}

bool WriteFileBytes(const std::string& filePath, const std::vector<char>& data) {
    // Open file in binary mode
    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return false;
    }

    // Write data to the file
    if (!file.write(data.data(), data.size())) {
        std::cerr << "Failed to write to file: " << filePath << std::endl;
        return false;
    }

    return true;
}

int main() {
    // Example file path (modify as needed)
    std::string filePath = "/mnt/d/test/file/hehe.txt";
    std::string outputFilePath = "/mnt/d/test/file/huhu.txt";

    // Read the file
    std::vector<char> fileData = ReadFileBytes(filePath);
    if (!fileData.empty()) {
        std::cout << "File read successfully, size: " << fileData.size() << " bytes." << std::endl;
    }

    // Write the file
    if (WriteFileBytes(outputFilePath, fileData)) {
        std::cout << "File written successfully to " << outputFilePath << std::endl;
    }

    return 0;
}
