#include "SocketClient.h"

int main()
{
    std::string serverHost = "127.0.0.1"; // Or get it from user input
    int serverPort = 12345;               // Or get it from user input
    std::string username;
    std::cout << "Your name is: ";
    std::cin >> username;                  // Or get it from user input
    std::string masterKey = "hcmuscrypto"; // Or get it from user input
    SocketClient client(serverHost, serverPort);
    client.establishConnection(username, masterKey);

    std::cout << "Send message to: " << std::endl;
    std::string recipientUsername;
    std::cin >> recipientUsername;       // Or get it from user input
    std::string content = "Hello, Bob!"; // Or get it from user input
    client.sendMessage(recipientUsername, content);

    return 0;
}