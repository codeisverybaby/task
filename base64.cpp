#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

const std::string base64Chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

std::vector<unsigned char> base64Decode(const std::string& base64String) {
    std::vector<unsigned char> decodedData;
    int val = 0, valb = -8;
    for (unsigned char c : base64String) {
        if (isspace(c)) continue; // Ignore whitespace
        if (c == '=') break;      // Padding
        int index = base64Chars.find(c);
        if (index == std::string::npos) {
            throw std::invalid_argument("Invalid character in Base64 string");
        }
        val = (val << 6) + index;
        valb += 6;
        if (valb >= 0) {
            decodedData.push_back((val >> valb) & 0xFF);
            valb -= 8;
        }
    }
    return decodedData;
}

int main() {
    std::string base64String = "SGVsbG8sIFdvcmxkIQ=="; // "Hello, World!" in Base64
    try {
        std::vector<unsigned char> binaryData = base64Decode(base64String);
        std::cout << "Decoded binary data: ";
        for (unsigned char c : binaryData) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
