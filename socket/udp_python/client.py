import socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
serverAddr = ('127.0.0.1', 9000)

print("""Enter message or "exit" to quit\n""")
while True:
    message = input("You : ")
    if message == 'exit' :
        break

    client_socket.sendto(message.encode('utf-8'), serverAddr)
    data, serverAddr = client_socket.recvfrom(1024)
    print(f"Server : {data.decode('utf-8')} ")
    client_socket.close()
