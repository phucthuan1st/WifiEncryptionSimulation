#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Client exception class to handle errors
class ClientException : public std::exception
{
public:
    explicit ClientException(const std::string &message) : msg_(message) {}
    virtual const char *what() const noexcept override { return msg_.c_str(); }

private:
    std::string msg_;
};

// Client class to connect to a server and send/receive messages
class SocketClient
{
public:
    SocketClient(const std::string &host, int port) : host_(host), port_(port), running_(false) {}

    // Connect to the server and authenticate with a username and master key
    void establishConnection(const std::string &username, const std::string &masterKey)
    {
        // Create the client socket
        clientSocket_ = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket_ < 0)
        {
            throw ClientException("Error opening socket");
        }

        // Connect to the server
        sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(host_.c_str());
        serverAddr.sin_port = htons(port_);

        if (connect(clientSocket_, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            throw ClientException("Error connecting to server");
        }

        // Authenticate with the server by sending the username and master key
        sendString(clientSocket_, username);
        sendString(clientSocket_, masterKey);
        std::string response;
        receiveString(clientSocket_, response);
        if (response != "Authentication successful")
        {
            throw ClientException("Error authentication failed");
        }

        running_ = true;
        std::cout << "Connected to server" << std::endl;

        // Start a separate thread to handle incoming messages from the server
        std::thread serverThread(&SocketClient::handleServer, this);
        serverThread.detach();
    }

    // Disconnect from the server and close the connection
    void closeConnection()
    {
        running_ = false;
        close(clientSocket_);
        std::cout << "Disconnected from server" << std::endl;
    }

    // Send a message to a specific recipient client
    void sendMessage(const std::string &recipientUsername, const std::string &content)
    {
        sendString(clientSocket_, recipientUsername);
        sendString(clientSocket_, content);
    }

private:
    // Handle incoming messages from the server
    void handleServer()
    {
        while (running_)
        {
            std::string senderUsername, content;
            receiveString(clientSocket_, senderUsername);
            receiveString(clientSocket_, content);
            std::cout << "Received message from " << senderUsername << ": " << content << std::endl;
        }
    }

    // Receive a string from the server
    void receiveString(int socket, std::string &str)
    {
        uint32_t size;
        if (recv(socket, &size, sizeof(size), 0) < 0)
        {
            throw ClientException("Error receiving data from server");
        }
        char *buffer = new char[size];
        if (recv(socket, buffer, size, 0) < 0)
        {
            throw ClientException("Error receiving data from server");
        }
        str = std::string(buffer, size);
        delete[] buffer;
    }

    // Send a string to the server
    void sendString(int socket, const std::string &str)
    {
        uint32_t size = str.size();
        if (send(socket, &size, sizeof(size), 0) < 0)
        {
            throw ClientException("Error sending data to server");
        }
        if (send(socket, str.c_str(), size, 0) < 0)
        {
            throw ClientException("Error sending data to server");
        }
    }

    std::string host_; // Server host name or IP address
    int port_;         // Server port number
    int clientSocket_; // Client socket file descriptor
    bool running_;     // Flag to stop the client
};