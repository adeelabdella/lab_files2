import socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_address = ('127.0.0.1', 9000)
server_socket.bind(server_address)

print("UDP server is listening...")
while True:
    data, client_address = server_socket.recvfrom(1024)
    print(f"received from client : {data.decode('utf-8')} ")
    response = input("Enter message : ")
    server_socket.sendto(response.encode('utf-8'), client_address)
