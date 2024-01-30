

// connect to the add server at port 8001 and connect to the mul server at port
// 8003 and connect to the sub server at port 8002 and connect to the div server
// at port 8004 create a server socket and listen to the client at port 8080

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;
#define PORT 8080


void success(string msg)
{
	cout<<"[+]"<<msg<<endl;
}
// store add , sub , mul , div server sockets in map

// store client socket
int clientSocket;
// store add Server socket
int addServerSocket;
// store sub Server socket
int subServerSocket;
// store mul Server socket
int mulServerSocket;
// store div Server socket
int divServerSocket;

vector<string> convertToVector(string s)
{
  vector<string> v;
  string temp = "";
  for (int i = 0; i < s.size(); i++)
  {
    if (s[i] == ' ')
    {
      continue;
    }
    if (isdigit(s[i]))
    {
      while (isdigit(s[i]))
      {
        temp += s[i];
        i++;
      }
      v.push_back(temp);
      temp = "";
      i--;
    }
    else
    {
      temp += s[i];
      v.push_back(temp);
      temp = "";
    }
  }
  return v;
}

int precedence(string s)
{
  if (s == "*" || s == "/")
  {
    return 2;
  }
  else if (s == "+" || s == "-")
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
vector<string> convertToPrecedenceUsingParenthesis(string s)
{
  vector<string> v = convertToVector(s);
  vector<string> ans;
  stack<string> st;
  for (int i = 0; i < v.size(); i++)
  {
    if (isdigit(v[i][0]))
    {
      ans.push_back(v[i]);
    }
    else if (v[i] == "(")
    {
      st.push(v[i]);
    }
    else if (v[i] == ")")
    {
      while (!st.empty() && st.top() != "(")
      {
        ans.push_back(st.top());
        st.pop();
      }
      st.pop();
    }
    else
    {
      while (!st.empty() && precedence(st.top()) >= precedence(v[i]))
      {
        ans.push_back(st.top());
        st.pop();
      }
      st.push(v[i]);
    }
  }
  while (!st.empty())
  {
    ans.push_back(st.top());
    st.pop();
  }
  return ans;
}

int main()
{
  sockaddr_in srv;
  int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  srv.sin_family = AF_INET;
  srv.sin_port = htons(PORT);
  srv.sin_addr.s_addr = INADDR_ANY;

  int nRet = bind(serverSocket, (sockaddr *)&srv, sizeof(srv));

  nRet = listen(serverSocket, 5);

  int clientSocket = accept(serverSocket, nullptr, nullptr);

  // connect to add server
  addServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  srv.sin_family = AF_INET;
  srv.sin_port = htons(8001);
  srv.sin_addr.s_addr = INADDR_ANY;
  nRet = connect(addServerSocket, (sockaddr *)&srv, sizeof(srv));
	success("connected with add server");
  // connect to sub server
  subServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  srv.sin_family = AF_INET;
  srv.sin_port = htons(8002);
  srv.sin_addr.s_addr = INADDR_ANY;
  nRet = connect(subServerSocket, (sockaddr *)&srv, sizeof(srv));
	success("connected with subtraction server");
  // connect to mul server
  mulServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  srv.sin_family = AF_INET;
  srv.sin_port = htons(8003);
  srv.sin_addr.s_addr = INADDR_ANY;
  nRet = connect(mulServerSocket, (sockaddr *)&srv, sizeof(srv));
	success("connected with multiplication server");
  // connect to div server
  divServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  srv.sin_family = AF_INET;
  srv.sin_port = htons(8004);
  srv.sin_addr.s_addr = INADDR_ANY;
  nRet = connect(divServerSocket, (sockaddr *)&srv, sizeof(srv));
	success("connected with division server");

  char buff[1024] = {0};
  while (1)
  {
    try
    {
      // recieve the data from client
      memset(buff, 0, sizeof(buff));
      recv(clientSocket, buff, sizeof(buff), 0);
      string s = buff;
      cout <<"client: "<<  s << "\n";
      vector<string> postfix = convertToPrecedenceUsingParenthesis(s);
      stack<int> st;
      string z = "";
      for (int i = 0; i < postfix.size(); i++)
      {
        z += postfix[i];
        z += " ";
      }
      //cout << z << "\n\n";
      // string z = s;
      // send(clientSocket, z.c_str(), sizeof(z.c_str()), 0);
      for (int i = 0; i < postfix.size(); i++)
      {
        if (isdigit(postfix[i][0]))
        {
          st.push(stoi(postfix[i]));
        }
        else
        {
          int a = st.top();
          st.pop();
          int b = st.top();
          st.pop();
          if (postfix[i] == "+")
          {
            // send the data to add server
            string z = to_string(a) + "," + to_string(b);
            send(addServerSocket, z.c_str(), sizeof(z.c_str()), 0);
            // recieve the result from add server
            memset(buff, 0, sizeof(buff));
            recv(addServerSocket, buff, sizeof(buff), 0);
            int result = stoi(buff);
            st.push(result);
          }
          else if (postfix[i] == "-")
          {
            string z = to_string(a) + "," + to_string(b);
            send(subServerSocket, z.c_str(), sizeof(z.c_str()), 0);
            // recieve the result from add server
            memset(buff, 0, sizeof(buff));
            recv(subServerSocket, buff, sizeof(buff), 0);
            int result = stoi(buff);
            st.push(result);
          }
          else if (postfix[i] == "*")
          {
            string z = to_string(a) + "," + to_string(b);
            send(mulServerSocket, z.c_str(), sizeof(z.c_str()), 0);
            // recieve the result from add server
            memset(buff, 0, sizeof(buff));
            recv(mulServerSocket, buff, sizeof(buff), 0);
            int result = stoi(buff);
            st.push(result);
          }
          else if (postfix[i] == "/")
          {
            string z = to_string(a) + "," + to_string(b);
            send(divServerSocket, z.c_str(), sizeof(z.c_str()), 0);
            // recieve the result from add server
            memset(buff, 0, sizeof(buff));
            recv(divServerSocket, buff, sizeof(buff), 0);
            int result = stoi(buff);
            st.push(result);
          }
        }
      }
      // send the result to client
      string result = to_string(st.top());
      send(clientSocket, result.c_str(), sizeof(result.c_str()), 0);
    }
    catch (exception e)
    {
      cout << e.what() << endl;
    }
  }
  return 0;
}
