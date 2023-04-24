#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Struct to represent the message data
struct Message
{
    std::string senderUsername;
    std::string recipientUsername;
    std::string content;
};

// Server exception class to handle errors
class ServerException : public std::exception
{
public:
    explicit ServerException(const std::string &message) : msg_(message) {}
    virtual const char *what() const noexcept override { return msg_.c_str(); }

private:
    std::string msg_;
};

// Server class to handle incoming connections
class SocketServer
{
public:
    SocketServer(int port) : port_(port), running_(false), ipaddr_("127.0.0.1") {}
    SocketServer(std::string ipaddr, int port) : ipaddr_(ipaddr), port_(port), running_(false) {}

    // Start the server and wait for incoming connections
    void start()
    {
        // Create the server socket
        serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket_ < 0)
        {
            throw ServerException("Error opening socket");
        }

        // Set socket options
        int opt = 1;
        if (setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        {
            throw ServerException("Error setting socket options");
        }

        // Bind the socket to a specific port
        sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(ipaddr_.c_str());
        serverAddr.sin_port = htons(port_);

        if (bind(serverSocket_, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            throw ServerException("Error binding socket to port");
        }

        // Listen for incoming connections
        if (listen(serverSocket_, SOMAXCONN) < 0)
        {
            throw ServerException("Error listening for incoming connections");
        }

        running_ = true;
        std::cout << "Server " << ipaddr_.c_str() << " started on port " << port_ << std::endl;

        // Wait for incoming connections in a loop
        while (running_)
        {
            // Accept incoming connection
            sockaddr_in clientAddr;
            socklen_t clientSize = sizeof(clientAddr);
            int clientSocket = accept(serverSocket_, (sockaddr *)&clientAddr, &clientSize);
            if (clientSocket < 0)
            {
                throw ServerException("Error accepting incoming connection");
            }

            // Handle incoming connection in a separate thread
            std::thread clientThread(&SocketServer::handleClient, this, clientSocket, clientAddr);
            clientThread.detach();
        }
    }

    // Stop the server and close all connections
    void stop()
    {
        running_ = false;
        for (const auto &client : clients_)
        {
            close(client.socket);
        }
        clients_.clear();
        close(serverSocket_);
        std::cout << "Server stopped" << std::endl;
    }

    bool getState()
    {
        return this->running_;
    }

private:
    // Struct to represent a connected client
    struct Client
    {
        int socket; // Client socket file descriptor
        std::string username;
        std::string masterKey;
    };

    // Handle an incoming client connection
    void handleClient(int clientSocket, sockaddr_in clientAddr)
    {
        // Receive the username and master key from the client
        std::string username, masterKey;
        receiveString(clientSocket, username);
        receiveString(clientSocket, masterKey);

        // Check if the master key is correct
        if (masterKey != "hcmuscrypto")
        {
            sendString(clientSocket, "Authentication failed");
            close(clientSocket);
            return;
        }

        // Add the client to the list of connected clients
        Client client = {clientSocket, username, masterKey};
        clients_.push_back(client);
        std::cout << "Client '" << username << "' connected from " << inet_ntoa(clientAddr.sin_addr) << std::endl;
        sendString(clientSocket, "Authentication successful");

        // Main loop to handle incoming messages from the client
        while (true)
        {
            std::string recipientUsername, content;
            receiveString(clientSocket, recipientUsername);
            if (recipientUsername.empty())
            {
                break;
            } // Connection closed
            receiveString(clientSocket, content);
            Message message = {username, recipientUsername, content};
            sendMessage(message);
        }

        // Close the client connection
        close(clientSocket);
        removeClient(client);
        std::cout << "Client '" << username << "' disconnected" << std::endl;
    }

    // Remove a client from the list of connected clients
    void removeClient(const Client &client)
    {
        auto it = std::find_if(clients_.begin(), clients_.end(), [&](const Client &c)
                               { return c.socket == client.socket; });
        if (it != clients_.end())
        {
            clients_.erase(it);
        }
    }

    // Send a message to its recipient client
    void sendMessage(const Message &message)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = std::find_if(clients_.begin(), clients_.end(), [&](const Client &c)
                               { return c.username == message.recipientUsername; });
        if (it != clients_.end())
        {
            sendString(it->socket, message.senderUsername);
            sendString(it->socket, message.content);
        }
    }

    // Receive a string from a socket
    void receiveString(int socket, std::string &str)
    {
        uint32_t size;
        if (recv(socket, &size, sizeof(size), 0) < 0)
        {
            throw ServerException("Error receiving data from client");
        }
        char *buffer = new char[size];
        if (recv(socket, buffer, size, 0) < 0)
        {
            throw ServerException("Error receiving data from client");
        }
        str = std::string(buffer, size);
        delete[] buffer;
    }

    // Send a string to a socket
    void sendString(int socket, const std::string &str)
    {
        uint32_t size = str.size();
        if (send(socket, &size, sizeof(size), 0) < 0)
        {
            throw ServerException("Error sending data to client");
        }
        if (send(socket, str.c_str(), size, 0) < 0)
        {
            throw ServerException("Error sending data to client");
        }
    }

    std::string ipaddr_;
    int port_;                    // Server port number
    int serverSocket_;            // Server socket file descriptor
    std::vector<Client> clients_; // List of connected clients
    std::mutex mutex_;            // Mutex to synchronize access to the clients list
    bool running_;                // Flag to stop the server
};
