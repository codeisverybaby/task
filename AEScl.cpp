#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <cryptopp/aes.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>

std::string key = "0123456789ABCDEF"; // 16 bytes key
std::string iv = "ABCDEF9876543210"; // 16 bytes IV

std::string Encrypt(const std::string& plainText)
{
    std::string cipherText;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryptor((byte*)key.c_str(), key.size(), (byte*)iv.c_str());
    CryptoPP::StringSource ss(plainText, true,
        new CryptoPP::StreamTransformationFilter(encryptor,
            new CryptoPP::StringSink(cipherText)
        )
    );
    return CryptoPP::Base64Encoder(new CryptoPP::StringSink(cipherText), false).Ref();
}

std::string Decrypt(const std::string& cipherText)
{
    std::string plainText;
    CryptoPP::Base64Decoder decoder(new CryptoPP::StringSink(cipherText));
    CryptoPP::StringSource ss(decoder, true,
        new CryptoPP::StreamTransformationFilter(
            CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption(
                (byte*)key.c_str(), key.size(), (byte*)iv.c_str()
            ),
            new CryptoPP::StringSink(plainText)
        )
    );
    return plainText;
}

int main()
{
    std::string message = "Hello, Secure Server!";
    std::string encryptedMessage = Encrypt(message);

    std::cout << "Sending encrypted message: " << encryptedMessage << "\n";

    // Send POST request
    cpr::Response response = cpr::Post(cpr::Url{"http://localhost:5000"},
                                       cpr::Body{encryptedMessage},
                                       cpr::Header{{"Content-Type", "text/plain"}});

    if (response.status_code == 200)
    {
        std::cout << "Encrypted response received: " << response.text << "\n";
        std::string decryptedResponse = Decrypt(response.text);
        std::cout << "Decrypted response: " << decryptedResponse << "\n";
    }
    else
    {
        std::cout << "Request failed. Status code: " << response.status_code << "\n";
    }

    return 0;
}
