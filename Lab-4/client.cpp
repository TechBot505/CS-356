#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>
#include <cstring>
using namespace std;

int main() {
    // Create a UDP socket
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        cerr << "Error creating socket\n";
        return -1;
    }

    // Set up the server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server's IP address
    serverAddress.sin_port = htons(8080); // Server's port

    for (int i = 0; i < 10; ++i) {
        // Send a ping message
        const char* message = "Ping";
        ssize_t bytesSent = sendto(clientSocket, message, strlen(message), 0,
            (struct sockaddr*)&serverAddress, sizeof(serverAddress));

        if (bytesSent == -1) {
            cerr << "Error sending data\n";
            close(clientSocket);
            return -1;
        }

        // Receive the pong message
        char buffer[1024];
        sockaddr_in serverResponseAddress;
        socklen_t serverResponseAddrLen = sizeof(serverResponseAddress);

        time_t startTime = time(nullptr);
        ssize_t bytesRead = recvfrom(clientSocket, buffer, sizeof(buffer), 0,
            (struct sockaddr*)&serverResponseAddress, &serverResponseAddrLen);

        if (bytesRead == -1) {
            cerr << "Error receiving data\n";
            close(clientSocket);
            return -1;
        }

        time_t endTime = time(nullptr);
        double rtt = difftime(endTime, startTime) * 1000; // Calculate RTT in milliseconds
        cout << "Received Pong from server, RTT: " << rtt << " ms\n";
    }

    // Close socket
    close(clientSocket);

    return 0;
}
