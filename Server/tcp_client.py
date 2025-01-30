import socket


client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_ip = "127.0.0.1"
server_port = 8080
client_socket.connect((server_ip, server_port))

message_from_server = client_socket.recv(1024)
print("Message from server: ", message_from_server.decode()) # decode för att omvandla bytes till string

message_to_send = "Hello from client"

client_socket.sendall(message_to_send.encode())


client_socket.close()
