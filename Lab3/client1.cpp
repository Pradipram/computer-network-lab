#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

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

int main()
{
    const int PORT = 8000;

    //creating socket
    int clientSocket = socket(AF_INET,SOCK_STREAM,0);
    if(clientSocket < 0)
    {
        error("Error creating client socket");
    }
    success("Client socket created successfully");
    struct sockaddr_in ClientAddress;
    ClientAddress.sin_family = AF_INET;
    ClientAddress.sin_addr.s_addr = INADDR_ANY;
    ClientAddress.sin_port = htons(PORT);

    //checking address
    const char* ip = "127.0.0.1";
    if(inet_pton(AF_INET,ip,&ClientAddress.sin_addr) < 0)
	{
		error("Invalid address");
	}
    success("Address is valid");

    //connecting to server
    if(connect(clientSocket,(struct sockaddr*)&ClientAddress,sizeof(ClientAddress)) < 0)
    {
        error("Connection failed");
    }

    char msg[1023];
    // cin>>msg;
    while(1)
    {
        bzero(msg,1023);
        cout<<"Enter message:(press :q to quit) ";
        cin.getline(msg,1023);
        int n = write(clientSocket,msg,sizeof(msg));
        if(n < 0)
        {
            error("Error on sending message");
        }
        if(strcmp(msg,":q") == 0){
            cout<<"Client server is closing"<<endl;
            break;
        }
        bzero(msg,1023);
        n = read(clientSocket,msg,1023);
        cout<<"Chatbot: "<<msg<<endl<<endl;
    }
    close(clientSocket);

    return 0;
}