#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <signal.h>

using namespace std;

thread threadSend, threadRecv;
int clientSocket;


void sendMessage(int clientSocket);
void recvMessage(int clientSocket);

int main()
{
	if((clientSocket=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket: ");
		exit(-1);
	}

	struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_port=htons(5000);
	// client.sin_addr.s_addr=INADDR_ANY;
	bzero(&client.sin_zero,0);

	if(inet_pton ( AF_INET, "10.0.2.15", &client.sin_addr)<=0)
	{
		printf ( "\nInvalid address ! This IP Address is not supported !\n" );
		return -1;
	}

	if((connect(clientSocket,(struct sockaddr *)&client,sizeof(struct sockaddr_in)))==-1)
	{
		perror("connect: ");
		exit(-1);
	}
	char name[200];
	cout<<"Enter your name : ";
	cin.getline(name,200);
	send(clientSocket,name,sizeof(name),0);

	cout<<"\n\t  w e l c o m e  "<<endl;

	thread t1(sendMessage, clientSocket);
	thread t2(recvMessage, clientSocket);

	threadSend=move(t1);
	threadRecv=move(t2);

	if(threadSend.joinable())
		threadSend.join();
	if(threadRecv.joinable())
		threadRecv.join();
			
	return 0;
}

int eraseText(int cnt)
{
	char remove=8;
	for(int i=0; i<cnt; i++)
	{
		cout<<remove;
	}	
	return 0;
}

void sendMessage(int clientSocket)
{
	while(true)
	{
		char str[200];

		cin.getline(str,200);
		send(clientSocket,str,sizeof(str),0);

		if(strcmp(str,"/exit")==0)
		{
			threadRecv.detach();	
			close(clientSocket);
			return;
		}	
	}		
}

void recvMessage(int clientSocket)
{
	while(true) {
		char name[200];
		char str[200];

		recv(clientSocket,str,sizeof(str),0);

		eraseText(6);

		cout << str;

		cout << "You : ";
		fflush(stdout);
	}	
}
