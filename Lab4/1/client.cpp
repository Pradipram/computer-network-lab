// client.cpp

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstring>
#include <fstream>
#include <netinet/in.h>
#include <pthread.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

#define PORT 8081
#define BUFFER_SIZE 1024

int Socket;
char buffer[BUFFER_SIZE];

void success(string msg)
{
	cout<<"[+]"<<msg<<endl;
}

int main()
{
    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (Socket == -1)
    {
        cerr << "Error creating socket" << endl;
        return -1;
    }
	success("Scoket created successfully");

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(Socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        cerr << "Error connecting to server" << endl;
        close(Socket);
        return -1;
    }
	success("Connected to server");
    while (1)
    {
        try
        {
            cout << "upload or download" << endl;
            string s;
            cin >> s;
            if (s == "upload")
            {
                send(Socket, s.c_str(), s.size(), 0);
                cout << "Enter file name" << endl;
                string filename;
                cin >> filename;
                // open file from clients_files folder
                string path = "clients_files/" + filename;
                ifstream file(path);
                if (!file)
                {
                    cout << "File not found" << endl;
                    continue;
                }
                send(Socket, filename.c_str(), filename.size(), 0);
                string p;
                cin >> p;
                while (!file.eof())
                {
                    memset(buffer, 0, sizeof(buffer));
                    file.read(buffer, BUFFER_SIZE);
                    send(Socket, buffer, BUFFER_SIZE, 0);
                }
                file.close();
                cout << "File uploaded" << endl;
                send(Socket, "end", 3, 0);
            }
            else if (s == "download")
            {
                send(Socket, s.c_str(), s.size(), 0);
                cout << "Enter file name" << endl;
                string filename;
                cin >> filename;
                send(Socket, filename.c_str(), filename.size(), 0);
                ofstream file;
                // if file exists, append to it else create new file
                // open file from clients_files folder
                string path = "clients_files/" + filename;
                file.open(path, ios::out | ios::app | ios::binary);
                if (!file)
                {
                    cout << "Error creating file" << endl;
                    continue;
                }

                while (1)
                {
                    memset(buffer, 0, sizeof(buffer));
                    recv(Socket, buffer, BUFFER_SIZE, 0);
                    if (strcmp(buffer, "end") == 0)
                    {
                        break;
                    }
                    file.write(buffer, strlen(buffer));
                }
                file.close();
                cout << "File downloaded" << endl;
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
	close(Socket);
    return 0;
}
