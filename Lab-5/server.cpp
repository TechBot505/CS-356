#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstring>
using namespace std;

#define PORT 8080
#define BUFFER_SIZE 100

void sendFile(int clientSocket, const char* fileName) {
    int file = open(fileName, O_RDONLY);
    if (file == -1) {
        close(clientSocket);
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = read(file, buffer, BUFFER_SIZE)) > 0) {
        send(clientSocket, buffer, bytesRead, 0);
    }

    close(file);
    close(clientSocket);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Error creating server socket." << endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Error binding socket." << endl;
        close(serverSocket);
        return -1;
    }

    listen(serverSocket, 5);
    cout << "Server listening on port " << PORT << endl;

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);
        cout << "Connection accepted from " << inet_ntoa(clientAddr.sin_addr) << endl;

        char fileName[BUFFER_SIZE];
        recv(clientSocket, fileName, BUFFER_SIZE, 0);
        cout << "Requested file: " << fileName << endl;

        sendFile(clientSocket, fileName);
    }
    return 0;
}