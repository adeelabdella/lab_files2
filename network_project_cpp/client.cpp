#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main()
{
    int clientSocket;
    while (true)
    {
        struct sockaddr_in serverAdd;
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1)
        {
            cerr << "Error creating socket" << endl;
            return 1;
        }
        serverAdd.sin_family = AF_INET;
        serverAdd.sin_port = htons(9000);
        serverAdd.sin_addr.s_addr = inet_addr("127.0.0.1");

        if (connect(clientSocket, (struct sockaddr *)&serverAdd, sizeof(serverAdd)) == -1)
        {
            cerr << "Error connecting to server" << endl;
            close(clientSocket);
            return 1;
        }

        char request[1024];
        cout << "You : ";
        // cin.getline(request, sizeof(request));
        // cout << sizeof(request) << endl;
        char temp = 'a';
        int i = 0;
        do
        {
            temp = getchar();
            request[i++] = temp;
        } while (temp != '\n');
        request[i - 1] = '\0';
        // int requestLen = strlen(request);
        send(clientSocket, request, i, 0);
        char response[1024];
        recv(clientSocket, response, sizeof(response), 0);
        cout << "Server : " << response << endl;
    }

    close(clientSocket);
    return 0;
}