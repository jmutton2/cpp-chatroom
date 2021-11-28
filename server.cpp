#include <string.h>
#include <sys/socket.h>
#include <vector>


//open its own socket and ports to be connected to
// announse server is running
// listen for conenctions
// if connection received, ask client for name, connect by sending name
// broadcast message 
// broadcast loop through clients in the vector
//  send to the socket, print out message and author to everyone		
// another thread for outgoing messgages
// one prepared to send, one recv

// error checking
	// if socket can't be made
	// if error binding to socket
	// error user conencting
	// error listening
	// user disconnecting
	
use namespace std;

struct client {
	string name;
	int id;
	int thread;
	int socket;
}

vector<client> clients;

void broadcast(string text) {
	for (int i =0; i< clients.length; i++) {
		send(clients[i].socket, &text, sizeof(text), 0);
	}
}

void handleClient(int clientSocket, int id) {

	char name [200];
	char text [200];
	
	recv(clientSocket, name, sizeof(name), 0);
	setName(id, name);
	
	string welcome = string(name) + string(" has joined");
	broadcast(welcome);
	
	while(true) {
		
		acceptedString = recv(clientSocket , text, sizeof(text), 0 );
		
		if(strcmp(text, "done")) {
			string endMsg = string(name) + string(" has left");
			broadcast(endMsg);
			break;
		}
		
		broadcast(acceptedString);
		
	}
	
}

void setName(int id, string name) {
	clients[id].name = name;
}

void main() {
	int id = 0;
	string done;
	
	struct sockaddr_in server; 
	server.sin_family = AF_INET;
	server.sin_port = htons(5000);
	server.sin_addr.s_addr = INADDR_ANY; // defines interfaces to be bound to (binds to all interfaces)
	int serverSocket =  socket(AF_INET, SOCK_STREAM, 0);
	
	
	
	// binds created socket to the interfaces
	int bind(serverSocket, (struct sockaddr *) & server, sizeof (struct sockaddr_in));
	int listen(int serverSocket, int backlog) // keep listening
	
	
	cout << "welcommen to server";
	
	while(true) {
	
		++id;
		// accept connection
		
		clientSocket = accept(serverSocket, (str5uct sockaddr *) & address,sizeof (struct sockaddr_in) );
		
		// make new thread
		thread th(handleClient,clientSocket,id)
		
		// push to client array
		clients.push_back({"anon", id , move(th), clientSocket});
		
		cin >> done;
		
		if(done.compare("done")) {
			string closeMsg = string("server is closing");
			
			broadcast(closeMsg);
			
			for (int i = 0; i < clients.length; i++) {
				close(clients[i].socket);
			}
		}
	}			
}
