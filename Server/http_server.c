#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>



void* thread_function(void *arg){    // körs när en ny tråd skapas

		char buffer[1024]; //store in data
					//is 1024 enough or do we want to allocate memory? malloc()

                int* client_socket = (int*)arg; // void* arg needed, need to convert to pointer of the exact type because this is written in C (Socket == int)
		

		int read_sucess = read(*client_socket, buffer, sizeof(buffer) - 1);  	//parameter 1 == int pointer to current socket
											//parameter 2 == pointer to storagebuffer
											//parameter 3 == bytes to read, match it with buffersize - 1 leave room for NULL. 
											//parameter 4 == position in file to start reading from. not relevant in this case
											//!!!read function if succesful returns the amount of bytes read.		 
		if(read_sucess == -1){
			perror("Read data failed");
		}
		else if(read_sucess == 0){
			printf("Client disconnected\n");		
		}
		else{
			buffer[read_sucess] = '\0'; // if read data does not contain NULL-terminator. NULL is added to the end of read data
			printf("Data recieved %s\n", buffer); //print data recieved
		}
		
		close(*client_socket); // close current socket
		

		return NULL; //return NULL if no value from thread.
        }

//använder tcp-socket

int main(){

	const int PORT = 8080;

	struct sockaddr_in address = {
		.sin_family = AF_INET, //IPv4
		.sin_addr.s_addr = INADDR_ANY, // Accept connections for any IP address
		.sin_port = htons(PORT) // Convert to network byte order
	};

	//Create socket file descriptor
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd == -1){ //If socketfunction fails
		perror("Socket creation failed"); //Custom message followed by errno msg
		return 1;
	}
	//Bind the socket to the specified port
	int did_bind = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
	if(did_bind == -1){ //If bindfunction fails
		perror("Binding failed");
		close(server_fd); // Close socket to prevent leaks
		return 1;
	}
	while(1){

		//Start listening for incoming connections
		if(listen(server_fd,10) == -1){          //max 10 i kön
			perror("Listen failed");
			close(server_fd); // Close socket to prevent leaks
			return 1;
		}
		else{
			printf("Server is listening on port %d\n", PORT);
		}

		//Accept incoming connection
		int socket = accept(server_fd, (struct sockaddr *)&address, &(socklen_t){sizeof(address)});


		if(socket == -1){
			perror("Accept failed");
			return 1;
		}
		else{
			// Multithreading here?
			pthread_t new_thread; // store new thread here
			int if_thread_fail = pthread_create(&new_thread, NULL, thread_function, (void*)&socket); //create new thread, specify what function to execute
			if(if_thread_fail != 0){ //if threadfunction failed
				perror("Thread creation failed");
			}
		}
	}

	//close socket
	close(server_fd);

	return 0;
}

// ! == RELEVANT 
// !! == MYCKET RELEVANT 


// Multiplexing lättare skalbart, framtidstänk. Men valde multithreading eftersom vi har ett satt antal anslutningar i gruppen


//TODO !!: Parsa HTTP-förfrågningar:Programmet skickar alltid samma svar oavsett vilken HTTP-förfrågan som tas emot. För att servern ska kunna hantera olika förfrågningar (som GET, POST etc.) 
//måste du implementera kod för att läsa och tolka HTTP-förfrågningar (t.ex. från klienten), och sedan svara med relevant innehåll beroende på förfrågningens typ och URI.

//TODO: Stöd för flera HTTP-metoder:
//För närvarande hanterar servern bara GET-förfrågningar implicit (och svarar med en statisk text). Du kan lägga till stöd för andra HTTP-metoder som POST, PUT, DELETE, etc., 
//och svara på olika sätt beroende på metoden som används.

//TODO: Hantera URL-parametrar:När servern får en GET-förfrågan kan den också ta emot parametrar i URL:en (som ?id=123). 
//För att servern ska kunna svara på dynamiska förfrågningar, behöver den kunna parsa och hantera dessa parametrar.

//TODO: Dynamiskt innehåll (t.ex. filservrar):
//Du kan också göra servern kapabel att servera filer (HTML, bilder, CSS, etc.) genom att läsa filer från disken och sända dem som svar. 
//För att göra detta kan du implementera funktioner för att läsa filinnehåll och skicka det i HTTP-svaret.

//TODO !!: Error handling och loggning:
//Även om programmet hanterar vissa fel (som socketfel och bindfel), kan du förbättra felhanteringen genom att skapa mer specifika felmeddelanden och kanske även logga dessa fel för felsökning.

//TODO !: HTTPS-stöd:
//För att lägga till säkerhet kan du överväga att stödja HTTPS (SSL/TLS). Detta skulle innebära att du implementerar SSL/TLS för att kryptera kommunikationen mellan servern och klienten.

//TODO: Headerhantering: För att kunna svara på mer avancerade HTTP-förfrågningar kan servern behöva hantera HTTP-headers (t.ex. Accept, Content-Type, User-Agent). 
//Du kan skapa kod som analyserar dessa headers och använder informationen för att bestämma vilket innehåll som ska skickas som svar.
