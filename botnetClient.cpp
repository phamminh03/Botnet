// #include <bits/stdc++.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <netdb.h>
// #include <unistd.h>  // for close() and usleep() functions
// #include <string>
// #include <cstdlib>

// using namespace std;

// int skt;  // Use int instead of SOCKET on Linux

// void ConnectSocket()
// {
//     // Initialize socket (no WSAStartup needed on Linux)
//     skt = socket(AF_INET, SOCK_STREAM, 0);
//     if (skt < 0) {
//         cerr << "Socket creation failed" << endl;
//         return;
//     }

//     struct hostent* host;
//     host = gethostbyname("127.0.0.1");  // Resolve hostname

//     struct sockaddr_in sockAddr;
//     sockAddr.sin_port = htons(45444);
//     sockAddr.sin_family = AF_INET;
//     sockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);  // Set IP address

//     if (connect(skt, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) != 0) {
//         cerr << "Socket connection failed" << endl;
//         return;
//     }
// }

// void CloseSocket()
// {
//     close(skt);  // Use close() instead of closesocket() on Linux
// }

// string PoolCommand()
// {
//     ConnectSocket();

//     if (skt > 0)  // Check if socket is valid
//     {
//         string req = "poll";
//         send(skt, req.c_str(), req.length(), 0);

//         int bufferLength = 4096;
//         char buffer[4096];

//         int received = recv(skt, buffer, bufferLength, 0);
//         if (received > 0) {
//             cout << "Received data: " << string(buffer, buffer + received) << endl;
//             CloseSocket();
//         }
//         else
//         {
//             cout << "No data received or error occurred." << endl;
//         }
//     }
//     CloseSocket();
//     return "";
// }

// int main()
// {
//     string lastCommand = "";
    
//     while (true)
//     {
//         string command = PoolCommand();
//         if (command.length() > 0 && command != lastCommand)
//         {
//             lastCommand = command;

//             if (command.find("say") == 0)
//             {
//                 string title = command.substr(3, command.find("\\") - 3);
//                 string msg = command.substr(command.find("\\") + 1);
//                 // Print to console instead of using MessageBoxA
//                 cout << "Title: " << title << endl;
//                 cout << "Message: " << msg << endl;
//             }
//         }
//         usleep(5000 * 1000);  // Sleep for 5000 milliseconds (usleep uses microseconds)
//     }

//     return 0;
// }

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

    // Gửi lệnh mở terminal
    string request = "terminal";
    send(clientSocket, request.c_str(), request.size(), 0);

    close(clientSocket);
    return 0;
}
