import socket
import threading

MAX_SIZE = 1000001
isPrime = [True]*MAX_SIZE
print("\033c")

PORT = 4000
HEADER = 1024
FORMAT = "utf-8"
MAX_CLIENT = 4
DISCONNECT_MESSAGE = "!DISCONNECTED!"
FIRST_CONNECTION = "!FIRST_CONNECTION!"
SERVER = socket.gethostbyname(socket.gethostname())
ADDRESS = (SERVER, PORT)
MAX_SIZE = 1000001

user_list = {}


def sieve_of_eratosthenes():
    global isPrime
    isPrime[0] = isPrime[1] = False
    for i in range(2, MAX_SIZE):
        j = i*i
        while (j < MAX_SIZE):
            isPrime[j] = False
            j += i


try:
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error as err:
    print(f"[UNABLE TO CREATE SOCKET] : {err}...\n")
    exit(0)

# A server has a bind() method which binds it to a specific IP
# and port so that it can listen to incoming requests on that
# IP and port.

try:
    server_socket.bind(ADDRESS)
except socket.error as err:
    print(f"[UNABLE TO BIND TO THE SPECIFIC IP AND PORT] : {err}...\n")
    exit(0)


def start():
    server_socket.listen(MAX_CLIENT)
    sieve_of_eratosthenes()
    print(f"[LISTENING]  server is listening on {SERVER}\n")
    connected = True
    while connected:
        try:
            client_connection, client_address = server_socket.accept()
        except socket.error as err:
            print(f"[UNABLE TO CONNECT TO THE CLIENTS] : {err}...\n")
            exit(0)
        print("Dogg")
        client_connection, client_address = server_socket.accept()
        data, client_address = server_socket.recvfrom(1024)
        print(data)
        data_decoded = data.decode('utf-8')
        print(f"received from client : {data_decoded} ")

        # Convert the received data to an integer
        try:
            number = int(data_decoded)
        except ValueError:
            response = "Invalid input. Please send an integer."
        else:
            if isPrime[number]:
                response = "PRIME"
            else:
                response = "COMPOSITE"

        server_socket.sendto(response.encode('utf-8'), client_address)


start()
