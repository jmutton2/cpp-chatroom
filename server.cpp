#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
#include <string>
#include <string.h> 
#include <stdio.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

//Define structure for a client object
//Includes a the user's name, a unique id to identify the client, the thread that deals with that client, and the socket the client is connected to
struct client {
	string name;
	int id;
	thread th;
	int socket;
};

//Contains a vector of clients
vector<client> clients;

//Broadcast a string to all connected clients in the vector
void broadcast(string text) {
	for (int i =0; i< clients.size(); i++) {
		send(clients[i].socket, &text, sizeof(text), 0);
	}
};

//Assigns the name given by the user, to the user's name
void setName(int id, string name) {
	clients[id].name = name;
};

//Handles the client interaction
void clientHandler(int clientSocket, int id) {

	char name [200];
	char text [200];
	
	//On start it grabs the user's name and sets it in the vector of connected clients
	recv(clientSocket, name, sizeof(name), 0);
	setName(id, name);
	
	//Displays a welcome message for the user who just joined, to all users.
	string welcome = string(name) + string(" has joined");
	broadcast(welcome);
	
	while(true) {
		
		//Get the user's typed text
		recv(clientSocket , text, sizeof(text), 0 );
		
		//Checks if the user typed ##done to leave
		//Displays an exit message if they left
		if(!strcmp(text, "##done")) {
			string endMsg = string(name) + string(" has left");
			broadcast(endMsg);
			break;
		}

		//Display the received text to everyone
		broadcast(text);
	}
};

int main() {

	int id = 0;
	string done;
	
	//Defines the socket for the server
	struct sockaddr_in server; 
	server.sin_family = AF_INET;
	server.sin_port = htons(5000);
	server.sin_addr.s_addr = INADDR_ANY; // defines interfaces to be bound to (binds to all interfaces)
	int serverSocket =  socket(AF_INET, SOCK_STREAM, 0);

	//Binds the socket to the system
	if (bind(serverSocket, ( struct sockaddr * )&server, sizeof(sockaddr_in)) < 0) {

		perror ( "Binding of socket failed !" );
		exit(EXIT_FAILURE);
	};

	//Listens for connections
	if((listen(serverSocket,5)) < 0)
	{
		perror("listen error: ");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in client;
	int clientSocket;
	unsigned int len = sizeof(sockaddr_in);
	
	cout << "welcommen to server";

	while(true) {
	
		++id;

		// accept connection
		if(accept(serverSocket, (struct sockaddr *)&server, &len) < 0) {
			
			perror ( "Binding of socket failed !" );
		};
		
		//Makes a new thread to deal with the client
		thread th(clientHandler,clientSocket,id);
		
		//Add a new client to the vector of clients
		clients.push_back({"anon", id , move(th), clientSocket});
		
		cin >> done;
		
		//Checks if the server owner types done to close the server
		//Displays server closing message to all users
		//Loops through and closes all connections if server closed.
		//Currently only works after a user has connected
		if(done.compare("done")) {
			string closeMsg = string("server is closing");
			
			broadcast(closeMsg);
			
			for (int i = 0; i < clients.size(); i++) {
				close(clients[i].socket);
			}
		}
	}			
}
