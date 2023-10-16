import socket
import json

print("\033c")

PORT = 4000
HEADER = 1024
FORMAT = "utf-8"
DISCONNECT_MESSAGE = "!DISCONNECTED!"


SERVER = socket.gethostbyname(socket.gethostname())
ADDRESS = (SERVER, PORT)


"""
Here we made a socket instance and passed it two parameters. The first parameter is AF_INET and the second one is SOCK_STREAM. AF_INET refers to the address-family ipv4. The SOCK_STREAM means connection-oriented TCP protocol.
"""

try:
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error as err:
    print(f"[UNABLE TO CREATE SOCKET] : {err}...\n")
    exit(0)
try:
    client.connect(ADDRESS)
except socket.error as err:
    print(f"[UNABLE TO CONNECT TO THE SERVER] : {err}...\n")
    exit(0)


def sendMessage(msg):
    json_object = {'msg': msg}
    msg = json.dumps(json_object)
    try:
        client.send(msg.encode(FORMAT))
    except socket.error as err:
        print(f"[UNABLE TO SEND MESSAGE TO THE SERVER] : {err}...\n")
        exit(0)


def reciveMessage():
    try:
        server_msg = client.recv(HEADER).decode('utf8')
    except socket.error as err:
        print(f"[UNABLE TO RECIEVE MESSAGE FROM THE SERVER] : {err}...\n")
        exit(0)
    print(f"Server : {server_msg}")
    return server_msg


"""
This is the first message sent to the server from the client side to know that another client is CONNECTED 
so we have to store the client information and don't ask the info again and again from the client side
"""

user_name = input("Enter your name : ")
json_object = {'name': user_name, 'msg': '!FIRST_CONNECTION!'}
msg = json.dumps(json_object)
client.send(msg.encode(FORMAT))

CONNECTED = True
while CONNECTED:
    print("\033c")
    msg = input("Type (y) to get the number\n")

    if (msg != 'y'):
        print("Exiting the connection...")
        msg = DISCONNECT_MESSAGE
        CONNECTED = False
        sendMessage(msg)
        continue

    sendMessage('start')

    server_msg = reciveMessage()

    msg = input("Enter number of set bits in the string : ")
    sendMessage(msg)

    server_msg = reciveMessage()

    input("Press Enter to continue...")

print("Connection Closed!")
client.close()
