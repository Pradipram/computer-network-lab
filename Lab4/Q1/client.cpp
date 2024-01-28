#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

void error(string msg) {
    cerr << "[-]" << msg << endl;
    exit(1);
}

void success(string msg) {
    cout << "[+]" << msg << endl;
}

int main() {
    const int PORT = 8000;

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        error("Error creating client socket");
    }
    success("Client socket created successfully");

    struct sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = INADDR_ANY;
    clientAddress.sin_port = htons(PORT);

    const char* ip = "127.0.0.1";
    if (inet_pton(AF_INET, ip, &clientAddress.sin_addr) < 0) {
        error("Invalid address");
    }
    success("Address is valid");

    if (connect(clientSocket, (struct sockaddr*)&clientAddress, sizeof(clientAddress)) < 0) {
        error("Connection failed");
    }
    success("Connected with server");

    // Sending file to the server
    string filePath = "file.txt";  // Specify the path of the file to send

    ifstream file(filePath, ios::binary | ios::ate);
    if (!file) {
        error("Error opening file for reading");
    }

    int fileSize = file.tellg();
    write(clientSocket, &fileSize, sizeof(fileSize));

    file.seekg(0, ios::beg);
    char buffer[fileSize];
    file.read(buffer, fileSize);

    write(clientSocket, buffer, sizeof(buffer));

    success("File sent successfully");

    close(clientSocket);

    return 0;
}

