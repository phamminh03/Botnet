#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

int main() 
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) 
    {
        cerr << "Error creating socket!" << endl;
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(45444);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) 
    {
        cerr << "Connection failed!" << endl;
        return -1;
    }

    string request = "terminal";
    send(clientSocket, request.c_str(), request.size(), 0);

    close(clientSocket);
    return 0;
}
