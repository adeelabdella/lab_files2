import random
import socket
import threading
import json

print("\033c")

PORT = 4000
HEADER = 1024
FORMAT = "utf-8"
MAX_CLIENT = 2
DISCONNECT_MESSAGE = "!DISCONNECTED!"
FIRST_CONNECTION = "!FIRST_CONNECTION!"
SERVER = socket.gethostbyname(socket.gethostname())
ADDRESS = (SERVER, PORT)
MAX_SIZE = 1000001

user_list = {}


def is_number(number):
    try:
        int(number)
        return True
    except ValueError:
        return False


try:
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error as err:
    print(f"[UNABLE TO CREATE SOCKET] : {err}...\n")
    exit(0)

try:
    server.bind(ADDRESS)
except socket.error as err:
    print(f"[UNABLE TO BIND TO THE SPECIFIC IP AND PORT] : {err}...\n")
    exit(0)


def sendMessage(msg, client_connection, client_address):
    try:
        client_connection.send(msg.encode(FORMAT))
    except socket.error as err:
        global user_list
        print(
            f"[UNABLE TO SEND MESSAGE TO THE {user_list[client_address]['name']}] : {err}...\n")
        del user_list[client_address]
        exit(0)


def decode_message(str, client_connection, client_address):
    client_object = json.loads(str)
    if client_object['msg'] == FIRST_CONNECTION:
        global user_list
        user_list[client_address] = {
            "name":  client_object['name'],
            "number": 2,
        }
        return "Joined the server."
    else:
        if client_object['msg'] == 'start':
            num = random.randrange(2, MAX_SIZE)
            num_bits = format(num, 'b')
            user_list[client_address]['number'] = num.bit_count()
            print(num_bits)
            sendMessage(num_bits, client_connection, client_address)
        elif client_object["msg"] == DISCONNECT_MESSAGE:
            return client_object["msg"]
        else:
            num = user_list[client_address]['number']
            if (not is_number(num)):
                sendMessage("Invalid Option!",
                            client_connection, client_address)
            elif (int(client_object['msg']) == num):
                sendMessage("Your answer is correct!",
                            client_connection, client_address)
            else:
                sendMessage("Your answer is incorrect!",
                            client_connection, client_address)

        return client_object['msg']


def handle_client(client_connection, client_address):
    print(f"[NEW CONNECTION] {client_address} connected.\n")
    global user_list
    connected = True
    while connected:
        try:
            str = client_connection.recv(HEADER).decode(FORMAT)
        except socket.error as err:
            print(
                f"[UNABLE TO RECIVE MESSAGE FROM THE {user_list[client_address]['name']}] : {err}...\n")
            del user_list[client_address]
            exit(0)

        if len(str) == 0:
            continue

        msg = decode_message(str, client_connection, client_address)
        if msg == DISCONNECT_MESSAGE:
            connected = False
            print(f"{user_list[client_address]['name']} is offline now.")
            continue

        print(f"{user_list[client_address]['name']} : {msg}")

    del user_list[client_address]
    client_connection.close()


def start():
    server.listen(MAX_CLIENT)
    print(f"[LISTENING]  server is listening on {SERVER}\n")
    connected = True
    while connected:
        try:
            client_connection, client_address = server.accept()
        except socket.error as err:
            print(f"[UNABLE TO CONNECT TO THE CLIENTS] : {err}...\n")
            exit(0)

        try:
            thread = threading.Thread(target=handle_client, args=(
                client_connection, client_address))
            thread.start()
        except socket.error as err:
            print(f"[UNABLE TO CREATE THREAD] : {err}...\n")
            exit(0)

        print(f"[ACTIVE CONNECTIONS] {threading.active_count()-1}\n")


print("[STARTING] server is starting...\n")
start()
