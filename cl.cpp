#include <cpr/cpr.h>
#include <iostream>

int main() {
    // Define the server URL
    std::string url = "https://localhost:5001/api/message";
    
    // Define the message payload
    std::string message = "Hello from C++ client!";
    
    // Send the POST request
    auto response = cpr::Post(cpr::Url{url},
                              cpr::Body{message},
                              cpr::Header{{"Content-Type", "application/json"}});
    
    // Check the response
    if (response.status_code == 200) {
        std::cout << "Server response: " << response.text << "\n";
    } else {
        std::cerr << "Failed to connect. Status code: " << response.status_code << "\n";
    }

    return 0;
}
