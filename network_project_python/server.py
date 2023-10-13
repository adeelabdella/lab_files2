import socket
import threading
import json
import uuid

print("\033c")

PORT = 4000
# size of data in bytes that can go in one packets
HEADER = 1024
FORMAT = "utf-8"
MAX_CLIENT = 4
DISCONNECT_MESSAGE = "!DISCONNECTED!"
FIRST_CONNECTION = "!FIRST_CONNECTION!"
SERVER = socket.gethostbyname(socket.gethostname())
ADDRESS = (SERVER, PORT)
MAX_SIZE: int = 10000000

# Define ANSI escape codes for text colors
RESET = "\033[0m"
RED = "\033[31m"
GREEN = "\033[32m"
YELLOW = "\033[33m"
BLUE = "\033[34m"
BOLD = "\033[1m"

# Function to print colored text


def colored_print(text, color):
    print(color + text + RESET)


# stores the client information like username
user_list = {}

# store the information that number is prime or composite
# True means prime and False means composite
isPrime = [True] * MAX_SIZE


# precomputes all prime and composite in the range of 2, MAX_SIZE

def sieve_of_eratosthenes():
    global isPrime
    isPrime[0] = isPrime[1] = False
    for i in range(2, MAX_SIZE):
        j = i * i
        while (j < MAX_SIZE):
            isPrime[j] = False
            j += i


def check_prime(number):
    if number < 2:
        return False
    for i in range(2, int(number ** 0.5) + 1):
        if number % i == 0:
            return False
    return True

# Here we made a socket instance and passed it two parameters. The first
# parameter is AF_INET and the second one is SOCK_STREAM. AF_INET refers
# to the address-family ipv4. The SOCK_STREAM means connection-oriented
# TCP protocol.


try:
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error as err:
    colored_print(f"[UNABLE TO CREATE SOCKET] : {err}...\n", RED)
    exit(0)

# A server has a bind() method which binds it to a specific IP and port so
# that it can listen to incoming requests on that IP and port.

while True:
    try:
        ADDRESS = (SERVER, PORT)
        server.bind(ADDRESS)
        break
    except socket.error as err:
        colored_print(f"[UNABLE TO BIND TO THE {ADDRESS}] : {err}...\n", RED)
        PORT += 1


# send message to the client


def send_message(msg, client_connection, client_address):
    try:
        client_connection.send(msg.encode(FORMAT))
    except socket.error as err_x:
        global user_list
        colored_print(
            f"[UNABLE TO SEND MESSAGE TO THE {user_list[client_address]['name']}] : {err_x}...\n", RED)
        del user_list[client_address]
        # exit the helper thread created not the main thread
        exit(0)

# decode the message if it was the first message or the other message and
# respond accordingly


def decode_message(str_received, client_connection, client_address):
    client_object = json.loads(str_received)
    if client_object['msg'] == FIRST_CONNECTION:
        # for first connection stores the name of the user corresponding
        # to the client address and the number which is to be send
        global user_list, isPrime
        user_list[client_address] = {
            "id": str(uuid.uuid4()),
            "name": client_object['name'],
        }
        return "joined the server."
    else:
        if client_object['msg'] == DISCONNECT_MESSAGE:
            return client_object['msg']
        try:
            number = int(client_object['msg'])
        except ValueError:
            colored_print(
                "Invalid message. Please enter a valid integer.", RED)
            send_message("You have entered an INVALID number",
                         client_connection, client_address)
            return
        if number > MAX_SIZE - 1:
            send_message(f"Enter number smaller than {MAX_SIZE}",
                         client_connection, client_address)
        elif (isPrime[number]):
            send_message("You have entered a PRIME number",
                         client_connection, client_address)
        else:
            send_message("You have entered a COMPOSITE number",
                         client_connection, client_address)
        return client_object['msg']

# handle's client queries


def handle_client(client_connection, client_address):
    colored_print(f"[NEW CONNECTION] {client_address} connected.\n", GREEN)
    global user_list
    connected = True
    while connected:
        # reciveing response from client
        try:
            str_received = client_connection.recv(HEADER).decode(FORMAT)
        except socket.error as err_x:
            colored_print(
                f"[UNABLE TO RECIVE MESSAGE FROM THE {user_list[client_address]['name']}] : {err_x}...\n", RED)
            del user_list[client_address]
            # exit the helper thread created not the main thread
            exit(0)

        if len(str_received) == 0:
            continue

        msg = decode_message(str_received, client_connection, client_address)
        if msg == DISCONNECT_MESSAGE:
            # disconnect the client from the server if message is
            # !DISCONNECTED!
            connected = False
            colored_print(
                f"{user_list[client_address]['name']} is offline now.", RED)
            colored_print(
                f"[ACTIVE CONNECTIONS] {threading.active_count()-2}\n", YELLOW)
            continue

        colored_print(f"{user_list[client_address]['name']} : {msg}", BLUE)

    # removing the client from the list after he/she get disconnected
    del user_list[client_address]
    client_connection.close()


def start():
    server.listen(MAX_CLIENT)
    sieve_of_eratosthenes()
    colored_print(f"[LISTENING]  server is listening on {ADDRESS}\n", GREEN)
    connected = True
    while connected:
        # And last a server has an accept() and close() method. The accept
        # method initiates a connection with the client and the close method
        # closes the connection with the client.
        try:
            client_connection, client_address = server.accept()
        except socket.error as err_x:
            colored_print(
                f"[UNABLE TO CONNECT TO THE CLIENTS] : {err_x}...\n", RED)
            exit(0)

        try:
            thread = threading.Thread(target=handle_client, args=(
                client_connection, client_address))
            thread.start()
        except socket.error as err_x:
            colored_print(f"[UNABLE TO CREATE THREAD] : {err_x}...\n", RED)
            exit(0)

        # -1 bcoz one thread is running the server
        colored_print(
            f"[ACTIVE CONNECTIONS] {threading.active_count()-1}\n", YELLOW)


colored_print("[STARTING] server is starting...\n", YELLOW)
start()
