#include <iostream>
#include <arpa/inet.h> // socket related
#include <unistd.h>    // close
#include <cstring>
using namespace std;

int main()
{
    int sockfd, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    memset(&clientAddr, 0, sizeof(clientAddr));

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        cerr << "Error creating socket" << endl;
        return 1;
    }

    cout << "Server Socket : " << sockfd << endl;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8090);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int bindResult = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    cout << "Bind Result : " << bindResult << endl;

    if (bindResult == -1)
    {
        cerr << "Error binding socket" << endl;
        close(sockfd);
        return 1;
    }

    while (true)
    {
        for(long long i = 0; i < 5000000000; i++);

        socklen_t clientAddrSize = sizeof(clientAddr);
        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
        {
            cerr << "Error sending socket options." << endl;
            close(sockfd);
            return -1;
        }

        char request[1024];
        int bytesRead = recvfrom(clientSocket, request, sizeof(request), 0, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (bytesRead == -1)
        {
            if (errno == EWOULDBLOCK)
            {
                cerr << "Reveive timeout, no data received" << endl;
            }
            else
            {
                cerr << "Error receiving data" << endl;
            }
        }
        else
        {
            request[bytesRead] = '\0';

            cout << "Client : " << request << endl;
            const char *response = "Hello UDP client";
            sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&clientAddr, clientAddrSize);
        }
    }
    // not needed though
    close(sockfd);
    return 0;
}