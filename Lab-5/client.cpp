#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstring>
#include <sstream>
#include <vector>
using namespace std;

#define PORT 8080
#define BUFFER_SIZE 100

int countWords(const char* buffer, size_t size) {
    stringstream ss(string(buffer, size));
    string word;
    int count = 0;
    while (ss >> word) {
        count++;
    }
    return count;
}

void receiveFile(int serverSocket) {
    char fileName[BUFFER_SIZE];
    cout << "Enter the file name: ";
    cin >> fileName;

    send(serverSocket, fileName, strlen(fileName), 0);

    // vector to store the file content in memory
    vector<char> fileContent;

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = recv(serverSocket, buffer, BUFFER_SIZE, 0)) > 0) {
        fileContent.insert(fileContent.end(), buffer, buffer + bytesRead);
    }

    close(serverSocket);

    int file = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (file == -1) {
        cerr << "Error creating file for writing." << endl;
        return;
    }

    write(file, fileContent.data(), fileContent.size());

    close(file);

    cout << "The file transfer is successful. Size of the file = " << fileContent.size()
              << " bytes, no. of words = " << countWords(fileContent.data(), fileContent.size()) << endl;
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Error creating client socket." << endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(PORT);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Error connecting to server." << endl;
        close(clientSocket);
        return -1;
    }

    receiveFile(clientSocket);
    return 0;
}