#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <curl/curl.h>

// Callback function to handle the server response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to send a request and handle the response in a separate thread
void sendRequest() {
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.86:8080/");
        std::string readBuffer;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);

        // If request was successful, handle the response in a separate thread
        if (res == CURLE_OK) {
            std::thread([=]() {
                Sleep(100000);
                std::cout << "Response received: " << readBuffer << std::endl;
                // Process response in this thread
                // (Do something with readBuffer)
                }).detach(); // Detach the thread to handle it independently
        }
        else {
            std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }
}

int main() {
    // Initialize CURL globally (only once)
    curl_global_init(CURL_GLOBAL_DEFAULT);

    while (true) {
        // Send the request every second
        std::thread(sendRequest).detach(); // Each request runs in a detached thread
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Cleanup CURL globally
    curl_global_cleanup();

    return 0;
}
