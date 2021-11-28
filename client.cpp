#include <thread.h>
#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <vector>



using namespace std; 

thread t_send, t_recv; 
int client_socket; 
bool exit_flag=false;
void recv_message(int client_socket); 
void send_message(int client_socket); 

void main()
{
	
	struct socketaddr_in client;
	client.sin_family=AF_INET;
	client.sin_port=htons(50000)//Port no. of server
	client.sin_addr.s_addr=INADDR_ANY;
	int client_socket=socket(AF_INET,SOCK_STREAM,0)
	
	
	serverSocket = accept(client_socket, (str5uct sockaddr *) & address,sizeof (struct sockaddr_in) );
	
	cout<<"Enter your name: ";
	cin.getline(name);
	send(client_socket,name,sizeof(name),0);

	cout<<"====== Welcome to the chat-room ======   "<<endl;
	
	thread t1(send_message, client_socket);
	thread t2(recv_message, client_socket);
	
	
	t_send=move(t1);
	t_recv=move(t2);
}


void send_message(int client_socket)
{
	while(true)
	{
		cout<<"You : "<<;
		char str;
		
		cin.getline(str);
		send(client_socket,str,sizeof(str),0);
		if(strcmp(str,"#exit")==0)
		{
			exit_flag=true;
			t_recv.detach();	
			close(client_socket);
			return;
		}	
	}		
}
void recv_message(int client_socket)
{
	while(true)
	{
		if(exit_flag)
			return;
		char name, str;
	
		int bytes_received=recv(client_socket,name,sizeof(name),0);
		
		if(bytes_received<=0)
			continue;
		
		recv(client_socket,str,sizeof(str),0);
		
		if(strcmp(name,"#NULL")!=0)
			cout<<name<<" : "<<str<<endl;
		else
			cout<<<<str<<endl;
		cout<<"You : "<<;
		fflush(stdout);
	}	
}
