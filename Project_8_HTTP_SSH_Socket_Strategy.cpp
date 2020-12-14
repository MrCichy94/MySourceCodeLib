#include <iostream>
#include <string>
#include <memory>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class ISocket{
public:
    virtual void initializeConn() = 0;
    virtual void sendData() = 0;
    virtual void disconnect() = 0;
    virtual ~ISocket() = default;
};

class SocketSSH : public ISocket{
public:
    SocketSSH(std::string remoteAddress):
        address{remoteAddress}, isConnectionEstablished{false}
    {

    }
    ~SocketSSH() = default;
    void initializeConn() override {
        isConnectionEstablished = true;
        std::cout << "Initializing ssh connection" << std::endl;
    }
    void sendData() override {
        std::cout << "Send data using ssh protocol to: " << address << std::endl;
    }
    void disconnect()
    {
        isConnectionEstablished = false;
        std::cout << "Disconnecting ssh" << std::endl;
    }
private:
    std::string address;
    bool isConnectionEstablished;
};

class SocketHTTP : public ISocket{
public:
    SocketHTTP(std::string remoteAddress):
            address{remoteAddress}, isConnectionEstablished{false}
    {

    }
    ~SocketHTTP() = default;
    void initializeConn() override {
        isConnectionEstablished = true;
        std::cout << "Initializing http connection" << std::endl;
    }
    void sendData() override {
        std::cout << "Send data using http protocol to: " << address << std::endl;
    }
    void disconnect()
    {
        isConnectionEstablished = false;
        std::cout << "Disconnecting http" << std::endl;
    }
private:
    std::string address;
    bool isConnectionEstablished;
};

int solution(int choice)
{
    std::unique_ptr<ISocket> socket(nullptr);

    switch (choice)
    {
        case 1:
            socket = make_unique<SocketSSH>("192.168.0.1");
            break;
        case 2:
            socket = make_unique<SocketHTTP>("192.168.0.2");
            break;
        default:
            std::cout << "Unknown protocol";
            return false;
    }

    socket->initializeConn();
    socket->sendData();
    socket->disconnect();

    return true;
}
