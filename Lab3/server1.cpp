#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

void error(string msg)
{
    cerr<<"[-]"<<msg<<endl;
    exit(1);
}

void success(string msg)
{
    cout<<"[+]"<<msg<<endl;
}

vector<int> client_sockets;

//Broadcasting the message
void broadcast(string message) {
    for (int client_socket : client_sockets) {
        send(client_socket, message.c_str(), message.size(), 0);
    }
}

//handeling incoming client connection
void handle_client(int client_socket) {
    char buffer[2048];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            cout<<"[-]"<<client_socket<<"Disconnected"<<endl;
            close(client_socket);
            client_sockets.erase(remove(client_sockets.begin(), client_sockets.end(), client_socket), client_sockets.end());
            break;
        } else {
            broadcast(string(buffer, bytes_received));
        }
    }
}

int main()
{
    const int PORT = 8000;

    int serverSocket = socket(AF_INET,SOCK_STREAM,0);
    if(serverSocket < 0)
        error("Error creating server socket");
    success("Sever created successfully");

    //binding sever address
    struct sockaddr_in serverAddres;
    serverAddres.sin_family = AF_INET;
    serverAddres.sin_addr.s_addr = INADDR_ANY;
    serverAddres.sin_port = htons(PORT);

    if(bind(serverSocket,(struct sockaddr*)&serverAddres,sizeof(serverAddres)) < 0)
    {
        error("Error binding server socket");
    }
    success("Address binded successfully");

    //listening
    if(listen(serverSocket,3) < 0)
    {
        error("Error listening for connection");
    }
    cout<<"Server is listening on Port:"<<PORT<<endl;

    //accepting
    while(1)
    {
        socklen_t addrlen = sizeof(serverAddres);
        int client_socket = accept(serverSocket,(struct sockaddr*)&serverAddres,&addrlen);
        if(client_socket < 0)
        {
            error("Error connecting with client");
        }
        cout<<"[+]"<<client_socket<<" connected"<<endl;
        client_sockets.push_back(client_socket);
        thread(handle_client,client_socket).detach();
    }

    // socklen_t size = sizeof(serverAddres);
    // int clientSocket = accept(serverSocket,(struct sockaddr*)&serverAddres,&size);
    // if(clientSocket < 0)
    // {
    //     error("Error accepting connecton");
    // }
    // success("Connection with client");

    // //geting the message
    // char buffer[1023];
    // while(1){
    //     bzero(buffer,1023);
    //     int n = read(clientSocket,buffer,1023);
    //     if(n < 0)
    //     {
    //         error("Error on reading");
    //     }
    //     if(strcmp(buffer,":q") == 0){
    //         cout<<"Server is closing"<<endl;
    //         break;
    //     }
    //     cout<<"Client: "<<buffer<<endl;
    //     // const char * response = getResponse(buffer);
    //     const char *response = "Hello client!. How are you?";
    //     n = write(clientSocket,response,strlen(response));
    //     if(n < 0)
    //     {
    //         error("Error sending message to client");
    //     }
    // }
    // close(clientSocket);
    close(serverSocket);

    return 0;
}
