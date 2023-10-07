import socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print("Socket Created")
server_address = ('127.0.0.1', 9000)
server_socket.bind(server_address)
server_socket.listen(5)

c, addr = server_socket.accept()

print('Accepted connection from', addr)
CLIENT_MSG = ""
while CLIENT_MSG != "close":
    print("Server:", end="")
    msg = input()
    c.send(msg.encode())
    CLIENT_MSG = c.recv(1024).decode()
    print(f"Message from Client : {CLIENT_MSG}")
c.close()
