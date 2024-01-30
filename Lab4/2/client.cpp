

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstring>
#include <unistd.h>

using namespace std;

#define PORT 8080

int clientSocket;
char buff[1024];

int main()
{
  clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (clientSocket == -1)
  {
    cerr << "Error creating socket" << endl;
    return -1;
  }

  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr =
      inet_addr("127.0.0.1"); // Replace with the actual server IP address

  if (connect(clientSocket, (struct sockaddr *)&serverAddr,
              sizeof(serverAddr)) == -1)
  {
    cerr << "Error connecting to server" << endl;
    close(clientSocket);
    return -1;
  }
  // send to the calc server that this is the client
  // send(clientSocket, "client", sizeof("client"), 0);

  while (1)
  {
    cout << "Enter Expression: ";
    cin.getline(buff, sizeof(buff));

    // Send the message to the server
    send(clientSocket, buff, strlen(buff), 0);

    if (strcmp(buff, "exit") == 0)
    {
      break;
    }
    // receive the result from server
    memset(buff, 0, sizeof(buff));
    recv(clientSocket, buff, sizeof(buff), 0);
    cout << "Result: " << buff << endl;
  }

  return 0;
}
