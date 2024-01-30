// server.cpp

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8081
#define BUFFER_SIZE 1024
using namespace std;

int clientSocket;
char buffer[BUFFER_SIZE];

void error(string msg)
{
	cout<<"[-]"<<msg<<endl;
	exit(1);
}

void success(string msg)
{
	cout<<"[+]"<<msg<<endl;
}

int main()
{
    cout << "Starting the server "
            "..............................................................."
         << endl;

    sockaddr_in srv;
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(serverSocket < 0)
	{
		error("Error creating socket");
	}
	success("socket created successfully");
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");

    int nRet = bind(serverSocket, (sockaddr *)&srv, sizeof(srv));
    if (nRet == -1)
    {
		error("Error binding the socket");
    }
	success("Addess binded with server");

    nRet = listen(serverSocket, 5);
    if (nRet == -1)
    {
        cerr << "Error listening for connections" << endl;
        return -1;
    }
	success("Server is listening on port 8080");

    while (1)
    {
        clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == -1)
        {
            cerr << "Error accepting connection" << endl;
            continue;
        }
		success("connected with client");
        while (1)
        {
            try
            {
                memset(buffer, 0, sizeof(buffer));
                int recvSize = recv(clientSocket, buffer, BUFFER_SIZE, 0);
                if (recvSize == -1)
                {
                    cerr << "Error receiving data" << endl;
                    continue;
                }
                if (recvSize == 0)
                {
                    cout << "Client disconnected" << endl;
                    break;
                }
                string s = buffer;
                cout << s << "\n";
                if (s == "exit")
                {
                    cout << "Client disconnected" << endl;
                    break;
                }
                else if (s == "upload")
                {
                    memset(buffer, 0, sizeof(buffer));
                    int recvSize = recv(clientSocket, buffer, BUFFER_SIZE, 0);
                    if (recvSize == -1)
                    {
                        cerr << "Error receiving data" << endl;
                        continue;
                    }
                    if (recvSize == 0)
                    {
                        cout << "Client disconnected" << endl;
                        break;
                    }
                    string filename = buffer;
                    ofstream file;
                    // if file exists append to it else create new file
                    // open file from clients_files folder
                    string path = "server_files/" + filename;
                    file.open(path, ios::out | ios::app | ios::binary);
                    if (!file)
                    {
                        cout << "Error creating file" << endl;
                        continue;
                    }
                    while (1)
                    {
                        memset(buffer, 0, sizeof(buffer));
                        recvSize = recv(clientSocket, buffer, BUFFER_SIZE, 0);
                        if (recvSize == -1)
                        {
                            cerr << "Error receiving data" << endl;
                            continue;
                        }
                        if (recvSize == 0)
                        {
                            cout << "Client disconnected" << endl;
                            break;
                        }
                        if (strcmp(buffer, "end") == 0)
                        {
                            break;
                        }
                        file.write(buffer, strlen(buffer));
                    }
					success("File received successfully");
                }
                else if (s == "download")
                {
                    memset(buffer, 0, sizeof(buffer));
                    int recvSize = recv(clientSocket, buffer, BUFFER_SIZE, 0);
                    if (recvSize == -1)
                    {
                        cerr << "Error receiving data" << endl;
                        continue;
                    }
                    if (recvSize == 0)
                    {
                        cout << "Client disconnected" << endl;
                        break;
                    }
                    string filename = buffer;
                    // open file from clients_files folder
                    string path = "server_files/" + filename;
                    cout << path << "\n";
                    ifstream file(path);

                    if (!file)
                    {
                        cout << "File not found" << endl;
                        continue;
                    }
                    while (!file.eof())
                    {
                        memset(buffer, 0, sizeof(buffer));
                        file.read(buffer, BUFFER_SIZE);
                        send(clientSocket, buffer, BUFFER_SIZE, 0);
                    }
                    file.close();
                    cout << "File uploaded" << endl;
                    send(clientSocket, "end", 3, 0);
                }
                else
                {
                    cout << "Invalid command" << endl;
                }
            }
            catch (...)
            {
                cout << "Error" << endl;
            }
        }
        close(clientSocket);
		success("Client disconnected");
    }

    close(serverSocket);
	success("server is closed");
    return 0;
}
