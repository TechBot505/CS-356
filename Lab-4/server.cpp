#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main() {
    // Create a UDP socket
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1) {
        cerr << "Error creating socket\n";
        return -1;
    }

    // Bind the socket to an address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Listen on any available interface
    serverAddress.sin_port = htons(8080); // Use port 8080

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cerr << "Error binding socket\n";
        close(serverSocket);
        return -1;
    }

    // Receive and respond to ping messages
    char buffer[1024];
    sockaddr_in clientAddress;
    socklen_t clientAddrLen = sizeof(clientAddress);

    for (int i = 0; i < 10; ++i) {
        ssize_t bytesRead = recvfrom(serverSocket, buffer, sizeof(buffer), 0,
            (struct sockaddr*)&clientAddress, &clientAddrLen);

        if (bytesRead == -1) {
            cerr << "Error receiving data\n";
            close(serverSocket);
            return -1;
        }

        // Display the received message
        cout << "Received Ping from client: " << buffer << "\n";

        // Respond with a Pong message
        const char* responseMessage = "Pong";
        ssize_t bytesSent = sendto(serverSocket, responseMessage, strlen(responseMessage), 0,
            (struct sockaddr*)&clientAddress, sizeof(clientAddress));

        if (bytesSent == -1) {
            cerr << "Error sending data\n";
            close(serverSocket);
            return -1;
        }
    }

    // Close socket
    close(serverSocket);

    return 0;
}
