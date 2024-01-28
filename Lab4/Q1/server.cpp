#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
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
    const string SAVE_DIRECTORY = "/path/to/save/directory/";  // Specify the directory where files will be saved

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
        error("Error creating server socket");
    success("Server created successfully");

    struct sockaddr_in serverAddres;
    serverAddres.sin_family = AF_INET;
    serverAddres.sin_addr.s_addr = INADDR_ANY;
    serverAddres.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddres, sizeof(serverAddres)) < 0) {
        error("Error binding server socket");
    }
    success("Address binded successfully");

    if (listen(serverSocket, 3) < 0) {
        error("Error listening for connection");
    }
    cout << "Server is listening on Port:" << PORT << endl;

    socklen_t addrlen = sizeof(serverAddres);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&serverAddres, &addrlen);
    if (clientSocket < 0) {
        error("Error connecting with client");
    }
    success("Client connected successfully");

    // Reading file size
    int fileSize;
    read(clientSocket, &fileSize, sizeof(fileSize));

    // Reading file content
    char buffer[fileSize];
    read(clientSocket, buffer, sizeof(buffer));

    // Saving received file
    ofstream outfile("received_file.txt", ios::binary);
    if (!outfile) {
        error("Error opening file for writing");
    }
    outfile.write(buffer, fileSize);
    outfile.close();

    success("File received and saved successfully");

    close(clientSocket);
    close(serverSocket);

    return 0;
}

