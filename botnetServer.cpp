#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  // for close() and sleep() functions
#include <thread>
#include <string>
#include <cstdlib>
using namespace std;

static int PORT = 45444;

int serverSocket;  // Using int instead of SOCKET in POSIX

string currCommand = "";

// Clear screen for Linux (WSL)
void clearScreen() {
    cout << "\033[2J\033[1;1H";  // ANSI escape code to clear terminal
}

void TaskInput()
{
    while (true)
    {
        clearScreen();
        cout << "Current Command: " << currCommand << endl;
        cout << "Type a new command: ";

        string response;
        getline(cin, response);

        currCommand = response;

        usleep(1000);  // Sleep for 1 millisecond (in microseconds)
    }
}

int main()
{
    thread threadInput(TaskInput);  // Start the input thread

    // Initialize socket
    struct sockaddr_in sockAddr;
    sockAddr.sin_port = htons(PORT);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Socket creation failed" << endl;
        return 1;
    }

    if (bind(serverSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0) {
        cerr << "Socket bind failed" << endl;
        close(serverSocket);
        return -1;
    }

    if (listen(serverSocket, 10) < 0) {
        cerr << "Socket listen failed" << endl;
        close(serverSocket);
        return 1;
    }

    cout << "Server started on port: " << PORT << endl;

    while (true)
    {
        struct sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientSize);

        if (clientSocket < 0) {
            cerr << "Failed to accept connection" << endl;
            continue;
        }

        int bufferLength = 4096;
        char buffer[4096];

        int iResult = recv(clientSocket, buffer, bufferLength, 0);
        if (iResult < 0) {
            cerr << "Error receiving data" << endl;
            close(clientSocket);
            continue;
        }

        string request = string(buffer, buffer + iResult);

        if (request == "poll")
        {
            cout << "Sending command: " << currCommand << endl;
            send(clientSocket, currCommand.c_str(), currCommand.length(), 0);
        }

        close(clientSocket);  // Close client socket (use close() in POSIX)
        usleep(1000);  // Sleep for 1 millisecond
    }

    close(serverSocket);  // Close server socket (use close() in POSIX)
    return 0;
}

// #include <iostream>
// #include <thread>
// #include <cstring>
// #include <unistd.h>       // Thư viện cho Linux (để dùng hàm sleep và close)
// #include <arpa/inet.h>    // Thư viện socket cho Linux
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <cstdlib>        // Thư viện cho hàm system()

// using namespace std;

// static int PORT = 45444;
// int serverSocket;
// string currCommand = "";

// void TaskInput() {
//     while (true) {
//         system("clear");
//         cout << "Current Command: " << currCommand << endl;
//         cout << "Type a new command: ";

//         string response;
//         getline(cin, response);

//         currCommand = response;

//         sleep(1);  // Dừng lại trong 1 giây
//     }
// }

// int main() {
//     thread threadInput(TaskInput);

//     // Tạo socket
//     serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (serverSocket == -1) {
//         cerr << "Error creating socket!" << endl;
//         return -1;
//     }

//     sockaddr_in serverAddr;
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(PORT);
//     serverAddr.sin_addr.s_addr = INADDR_ANY;

//     // Gán socket với địa chỉ IP và cổng
//     if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
//         cerr << "Bind failed!" << endl;
//         return -1;
//     }

//     // Lắng nghe kết nối
//     listen(serverSocket, 10);

//     while (true) {
//         sockaddr_in clientAddr;
//         socklen_t clientSize = sizeof(clientAddr);
//         int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientSize);

//         if (clientSocket < 0) {
//             cerr << "Failed to accept connection!" << endl;
//             continue;
//         }

//         char buffer[4096];
//         memset(buffer, 0, 4096);

//         // Nhận yêu cầu từ client
//         int bytesReceived = recv(clientSocket, buffer, 4096, 0);
//         if (bytesReceived > 0) {
//             string request(buffer, bytesReceived);
//             cout << "Received: " << request << endl;

//             // Kiểm tra nếu lệnh từ client là "open_terminal"
//             if (request == "open_terminal") {
//                 cout << "Opening terminal..." << endl;
//                 system("gnome-terminal");  // Mở terminal
//                 system("sudo apt install hping3");
//                 system("y");
//                 system("sudo hping3 192.168.1.20");
//             } else if (request == "poll") {
//                 send(clientSocket, currCommand.c_str(), currCommand.length(), 0);
//             }
//         }

//         close(clientSocket);
//         sleep(1);
//     }

//     close(serverSocket);
//     return 0;
// }
