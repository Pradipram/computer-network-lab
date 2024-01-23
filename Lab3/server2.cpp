#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

const char* getResponse(const char* message) 
{
    //Greeting
    if(strcmp(message, "hii") == 0 || strcmp(message, "hello") == 0 || strcmp(message, "hey") == 0)
    {
        return "Hi there! How can I assist you today?";
    }

    //Product information
    if(strcmp(message,"Tell me about your latest smartphone.") == 0)
    {
        return "Our latest smartphone is the XYZ model, featuring a high-resolution camera and a long-lasting battery.";
    }
    if(strcmp(message,"What features does your latest laptop have?") == 0)
    {
        return "Our latest laptop comes with a high-performance processor, a sleek design, and a long-lasting battery.";
    }

    //FAQs
    if(strcmp(message,"What are your business hours?") == 0)
    {
        return "Our business hours are Monday to Friday, 9:00 AM to 5:00 PM.";
    }
    if(strcmp(message,"What's your return policy?") == 0)
    {
        return "Our return policy allows for returns within 30 days of purchase. Please visit our website for more details.";
    }

    //Feedback mechnism
    if(strcmp(message, "Your service is great!") == 0)
    {
        return "Thank you for your kind words! We're here to help. Is there anything specific you'd like assistance with?";
    }

    //Errors
    return "I'm sorry, I didn't understand. Could you please provide more details or ask a specific question?";
}

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
    socklen_t size = sizeof(serverAddres);
    int clientSocket = accept(serverSocket,(struct sockaddr*)&serverAddres,&size);
    if(clientSocket < 0)
    {
        error("Error accepting connecton");
    }
    success("Connection with client");

    //geting the message
    char buffer[1023];
    while(1){
        bzero(buffer,1023);
        int n = read(clientSocket,buffer,1023);
        if(n < 0)
        {
            error("Error on reading");
        }
        if(strcmp(buffer,":q") == 0){
            cout<<"Server is closing"<<endl;
            break;
        }
        cout<<"Client: "<<buffer<<endl;
        const char * response = getResponse(buffer);
        n = write(clientSocket,response,strlen(response));
        if(n < 0)
        {
            error("Error sending message to client");
        }
    }
    close(clientSocket);
    close(serverSocket);

    return 0;
}
