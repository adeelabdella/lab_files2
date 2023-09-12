#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main()
{
    int sockfd;
    while (true)
    {
        struct sockaddr_in serverAdd;
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd == -1)
        {
            cerr << "Error creating socket" << endl;
            return 1;
        }
        memset(&serverAdd, 0, sizeof(serverAdd));
        serverAdd.sin_family = AF_INET;
        serverAdd.sin_port = htons(8090);
        serverAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
        socklen_t serverAddLen = sizeof(serverAdd);

        // char request[1024];
        // cout << "You : ";

        // char temp = 'a';
        // int i = 0;
        // do
        // {
        //     temp = getchar();
        //     request[i++] = temp;
        // } while (temp != '\n');
        // request[i - 1] = '\0';

        const char *request = "Hello server";
        if (sendto(sockfd, request, strlen(request), 0, (struct sockaddr *)&serverAdd, serverAddLen) == -1)
        {
            cout << "Error sending data" << endl;
            close(sockfd);
            return -1;
        }
        char buffer[1024];
        int bytesRead = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&serverAdd, &serverAddLen);
        if (bytesRead == -1)
        {
            cout << "Error receiving data" << endl;
            close(sockfd);
            return -1;
        }
        buffer[bytesRead] = '\0';
        cout << "Server : " << buffer << endl;
    }

    close(sockfd);
    return 0;
}