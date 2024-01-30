

// creating add server which will add two numbers and run on port 8001
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;
#define PORT 8001

int serverSocket;
int clientSocket;
char buff[1024];

void success(string msg)
{
	cout<<"[+]"<<msg<<endl;
}

int main()
{
  serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serverSocket == -1)
  {
    cerr << "Error creating socket" << endl;
    return -1;
  }
	success("Socket created sucessfully");

  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) ==
      -1)
  {
    cerr << "Error binding socket" << endl;
    close(serverSocket);
    return -1;
  }
	success("Socket binded successfully");

  if (listen(serverSocket, 5) == -1)
  {
    cerr << "Error listening on socket" << endl;
    close(serverSocket);
    return -1;
  }
	success("Server is listening on port 8001");

  while (1)
  {
    clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == -1)
    {
      cerr << "Error accepting connection" << endl;
      close(serverSocket);
      return -1;
    }

    while (1)
    {
      try
      {
        // receive the add command from the server
        memset(buff, 0, sizeof(buff));
        int byteRecv = recv(clientSocket, buff, sizeof(buff), 0);
        // check if the client is still connected
        if (byteRecv == 0)
        {
          cout << "Client disconnected" << endl;
          break;
        }
        if (strcmp(buff, "exit") == 0)
        {
          break;
        }
        // buff = 2,4
        string s = buff;
        int pos = s.find(",");
        int a = stoi(s.substr(0, pos));
        int b = stoi(s.substr(pos + 1, s.length() - pos - 1));
        int sum = a + b;
        string result = to_string(sum);
        send(clientSocket, result.c_str(), sizeof(result), 0);
        cout << "Add Server: " << b << " + " << a << " = " << sum << endl;
      }
      catch (exception e)
      {
        cout << "Invalid input" << endl;
      }
    }
    close(clientSocket);
  }
  close(serverSocket);

  return 0;
}
