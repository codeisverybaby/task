#include <iostream>
#include <string>
#include <vector>

// RC4 key scheduling and encryption/decryption function
std::string RC4(const std::string& key, const std::string& input) {
    if (key.empty()) {
        throw std::invalid_argument("Key cannot be empty");
    }

    std::vector<unsigned char> keySchedule(256);
    for (int i = 0; i < 256; ++i) {
        keySchedule[i] = static_cast<unsigned char>(i);
    }

    int j = 0;
    for (int i = 0; i < 256; ++i) {
        j = (j + keySchedule[i] + static_cast<unsigned char>(key[i % key.length()])) % 256;
        std::swap(keySchedule[i], keySchedule[j]);
    }

    std::string output(input.size(), '\0');
    int i1 = 0, j1 = 0;
    for (size_t k = 0; k < input.size(); ++k) {
        i1 = (i1 + 1) % 256;
        j1 = (j1 + keySchedule[i1]) % 256;
        std::swap(keySchedule[i1], keySchedule[j1]);
        unsigned char ksj = keySchedule[(keySchedule[i1] + keySchedule[j1]) % 256];
        output[k] = input[k] ^ ksj;
    }

    return output;
}

int main() {
    std::string key = "supersecretkey";  // Your key here
    std::string message = "Hello, World!";

    // Encrypt the message
    std::string encrypted = RC4(key, message);

    // Decrypt the message (RC4 is symmetric)
    std::string decrypted = RC4(key, encrypted);

    std::cout << "Original: " << message << std::endl;
    std::cout << "Encrypted: " << encrypted << std::endl;
    std::cout << "Decrypted: " << decrypted << std::endl;

    return 0;
}
