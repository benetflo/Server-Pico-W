import socket

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #SOCK_STREAM är TCP hade man skrivit tex SOCK_DGRAM så hade man fått UDP istället

# TCP är mer pålitligt än UDP men UDP är snabbare. UDP skickar data och "hoppas" att det kommer fram medans TCP "skakar hand" innan datan skickas så allt kommer fram.

server_socket.bind(("0.0.0.0",8080)) #HOST, PORT # HOST IP AND WHAT PORT TO LISTEN TO

server_socket.listen(10) # Sätter servern i lyssningsläge med plats för 10 anslutningar i kön

print("Server is listening on port 8080")

while True:

	client_socket, client_address = server_socket.accept() # accept returnar två värden därför kan man skriva så här. client_socket blir ett nytt socketobjekt(för aktuell klient) och client_address (IP + PORT, aktuella klientens identitet)
								# client_socket är den man använder funktioner på senare i koden ifall man vill göra det.

	print(f"New connection: {client_address}")

	message = "Welcome to my TCP-server written in Python!"

	client_socket.sendall(message.encode()) #måste omvandla string till bytes annars går det inte att skicka

	message_from_client = client_socket.recv(1024)
	print("Message from client", message_from_client.decode())

	client_socket.close()




