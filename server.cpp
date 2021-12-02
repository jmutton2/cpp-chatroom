#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

#define MAX_LEN 200

using namespace std;

struct client {

	int id;
	int socket;
    string name;
	thread th;
};

vector<client> clients;

//Sets the user's name
void setName(int id, char name[]) {

	for(int i=0; i<clients.size(); i++)
	{
			if(clients[i].id==id)	
			{
				clients[i].name=string(name);
			}
	}	
}

//Broadcast for all messages
int broadcast(string message, int id) {

	char temp[200];

    string message1 = clients[id].name + " : " + message + "\n";

	strcpy(temp,message1.c_str());

	for(int i=0; i<clients.size(); i++)
	{
        if (clients[i].id != id) {

            send(clients[i].socket,temp,sizeof(temp),0);

        }   
	}		
}

//Broadcast displayed on new user join
int welcomeBroadcast(string name, int sender_id) {

	char temp[200];

    name = name + " has joined the chatroom.\n";

	strcpy(temp,name.c_str());

	for(int i=0; i<clients.size(); i++)
	{
        if (clients[i].id != sender_id) {

            send(clients[i].socket,temp,sizeof(temp),0);
        }
	}	
}

//Broadcast displayed on user leave
int leaveBroadcast(int id) {

	char temp[200];

    string stemp = clients[id].name + " has left the chatroom.\n";

	strcpy(temp,stemp.c_str());

	for(int i=0; i < clients.size(); i++) {
        
        if (clients[i].id != id) {

            send(clients[i].socket,temp,sizeof(temp),0);
        }
	}	
}

void clientHandler(int clientSocket, int id) {

	char name[200];
    char str[200];
    
	recv(clientSocket,name,sizeof(name),0);

	setName(id,name);									
    welcomeBroadcast(name,id);	
	
	bool running = true;
    while(running) {

		//Awaits client message, rebroadcasts to all connected users.
		recv(clientSocket,str,sizeof(str),0);

        if(strcmp(str,"/exit")==0) {				

			leaveBroadcast(id);		
			running = false;		
			break;
		} else  {
			broadcast(string(str),id);
		}
	}	
}

int main() {

    int id = -1;
	int serverSocket;
	if((serverSocket=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket: ");
		exit(-1);
	}

	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(5000);
	server.sin_addr.s_addr=INADDR_ANY;
	bzero(&server.sin_zero,0);

	if((bind(serverSocket,(struct sockaddr *)&server,sizeof(struct sockaddr_in)))==-1)
	{
		perror("bind error: ");
		exit(-1);
	}

	if((listen(serverSocket,8))==-1)
	{
		perror("listen error: ");
		exit(-1);
	}

	struct sockaddr_in client;
	int clientSocket;
	unsigned int len = sizeof(sockaddr_in);

	cout<<"\n\t c h a t r o o m - o n l i n e"<<endl;

	while(true) {

		//Checks for new user connection
		if((clientSocket = accept(serverSocket,(struct sockaddr *)&client,&len))==-1) {

			perror("accept error: ");
			exit(-1);
		}

		id++;

		//Creates a new thread to deal with the client, then pushes it to the vector of clients
		thread th(clientHandler,clientSocket,id);

		clients.push_back({id, clientSocket, string("Anon"), (move(th))});
	}

	close(serverSocket);
	return 0;
}
