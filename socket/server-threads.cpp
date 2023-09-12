#include <iostream>
#include <arpa/inet.h> // socket related
#include <unistd.h>    // close
#include <cstring>
#include <thread> // threading
#include <vector> // for managing threads

using namespace std;

// Function to handle communication with a client
void handleClient(int clientSocket)
{
    char request[1024];
    ssize_t bytesRead = recv(clientSocket, request, sizeof(request), 0);
    if (bytesRead <= 0)
    {
        cerr << "Error receiving data from client" << endl;
        close(clientSocket);
        return;
    }

    cout << "Client : " << request << endl;

    char response[1024];
    char temp = 'a';
    int i = 0;
    do
    {
        temp = getchar();
        response[i++] = temp;
    } while (temp != '\n');
    response[i - 1] = '\0';
    send(clientSocket, response, strlen(response), 0);
    cout << "You : " << response << endl;
    close(clientSocket);
}

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1)
    {
        cerr << "Error creating socket" << endl;
        return 1;
    }

    cout << "Server Socket : " << serverSocket << endl;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int bindResult = bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    cout << "Bind Result : " << bindResult << endl;

    if (bindResult == -1)
    {
        cerr << "Error binding socket" << endl;
        close(serverSocket);
        return 1;
    }

    // Listening
    if (listen(serverSocket, 5) == -1)
    {
        cerr << "Error listening" << endl;
        close(serverSocket);
        return 1;
    }

    socklen_t clientAddrSize = sizeof(clientAddr);

    vector<thread> threads; // Store threads for clients

    while (true)
    {
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket == -1)
        {
            cerr << "Error accepting connections." << endl;
            continue;
        }

        // Create a new thread to handle the client
        threads.emplace_back(handleClient, clientSocket);
    }

    // Close the server socket
    close(serverSocket);

    // Wait for all threads to finish
    for (auto &t : threads)
    {
        t.join();
    }

    return 0;
}
