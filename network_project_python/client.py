import socket
import json
import signal

print("\033c")

# Define ANSI escape codes for text colors
RESET = "\033[0m"
RED = "\033[31m"
GREEN = "\033[32m"
YELLOW = "\033[33m"
BLUE = "\033[34m"
BOLD = "\033[1m"

PORT = 4001
# size of data in bytes that can go in one packets
HEADER = 1024
FORMAT = "utf-8"
DISCONNECT_MESSAGE = "!DISCONNECTED!"

# get the server ip address

SERVER = socket.gethostbyname(socket.gethostname())
# socket.gethostbyname(socket.gethostname())
ADDRESS = (SERVER, PORT)

# Function to print colored text


def colored_print(text, color):
    print(color + text + RESET)


# Here we made a socket instance and passed it two parameters. The first
# parameter is AF_INET and the second one is SOCK_STREAM. AF_INET refers
# to the address-family ipv4. The SOCK_STREAM means connection-oriented
# TCP protocol.

try:
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error as err:
    colored_print(f"[UNABLE TO CREATE SOCKET] : {err}...\n", RED)
    exit(0)
try:
    # coneecting the client to the server
    client.connect(ADDRESS)
except socket.error as err:
    colored_print(f"[UNABLE TO CONNECT TO THE SERVER] : {err}...\n", RED)
    exit(0)


# function to send message to the server
def send_message(input_msg_copy):
    json_object_x = {'msg': input_msg_copy}
    msg_to_send = json.dumps(json_object_x)
    try:
        client.send(msg_to_send.encode(FORMAT))
    except socket.error as err_x:
        colored_print(
            f"[UNABLE TO SEND MESSAGE TO THE SERVER] : {err_x}...\n", RED)
        exit(0)


def receive_message():
    try:
        server_msg_x = client.recv(HEADER).decode('utf8')
    except socket.error as err_x:
        colored_print(
            f"[UNABLE TO RECIEVE MESSAGE FROM THE SERVER] : {err_x}...\n", RED)
        exit(0)
    colored_print(f"Server : {server_msg_x}", GREEN)
    return server_msg_x


def close_client_connection():
    # closing the connection fromt the server
    colored_print("Connection Closed!", RED)
    send_message(DISCONNECT_MESSAGE)
    client.close()
    exit(0)

# This is the first message sent to the server from the client side to know
# that another client is connected
# so we have to store the client information and don't ask the info again
# and again from the client side


user_name = input(f"{BOLD}{BLUE}Enter your name : {RESET}")
json_object = {'name': user_name, 'msg': '!FIRST_CONNECTION!'}
msg = json.dumps(json_object)
print(RESET)
client.send(msg.encode(FORMAT))

CONNECTED = True

# Signal handler
EXIT_FLAG = False


def signal_handler(signal, frame):
    global EXIT_FLAG
    global CONNECTED
    EXIT_FLAG = True
    CONNECTED = False
    close_client_connection()


signal.signal(signal.SIGINT, signal_handler)
signal.signal(signal.SIGTSTP, signal_handler)

while CONNECTED:
    print("\033c")
    colored_print(
        "Enter a number to check whether its prime.", BLUE)
    input_msg = input(f"{BOLD}{BLUE}Number = {RESET}")
    send_message(input_msg)
    server_msg = receive_message()
    colored_print("Press Enter to continue or exit to close...", YELLOW)
    CONNECTED = not "exit" == input()

close_client_connection()
