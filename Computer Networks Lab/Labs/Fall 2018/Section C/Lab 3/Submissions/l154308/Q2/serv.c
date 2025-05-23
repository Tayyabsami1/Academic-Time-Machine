#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr


void main()
{
		int socket_desc, client_sock, client_size; 
        struct sockaddr_in server_addr, client_addr;
        char server_message[2000], client_message[2000];

        //Cleaning the Buffers
        
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        
        //Creating Socket
        
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        
        printf("Socket Created\n");
        
        //Binding IP and Port to socket
        
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(2001);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
        {
                printf("Bind Failed. Error!!!!!\n");
                return -1;
        }        
        
        printf("Bind Done\n");
        
        //Put the socket into Listening State
        
        if(listen(socket_desc, 1) < 0)
        {
                printf("Listening Failed. Error!!!!!\n");
                return -1;
        }
        
        printf("Listening for Incoming Connections.....\n");
        
        while(1)
        {
        //Accept the incoming Connections
        
				client_size = sizeof(client_addr);
				client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
				
				if (client_sock < 0)
				{
				        printf("Accept Failed. Error!!!!!!\n");
				        return -1;
				}
				
				printf("Client Connected with IP: %s and Port No: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
				
				//Receive the message from the client
				
				if (recv(client_sock, client_message, sizeof(client_message),0) < 0)
				{
				        printf("Receive Failed. Error!!!!!\n");
				        return -1;
				}
				
				printf("Client Message: %s\n",client_message);
				
				int i = 0;
				int vowels = 0;
				
				char wordArr[20];
				int j = 0;
				for( i = 0; client_message[i] != '\0' ; i++)
				{
					if(client_message[i] != 32)
					{
						wordArr[j] = client_message[i];
						j++;
						if( client_message[i] == 65 || client_message[i] == 97 ||
							client_message[i] == 69 || client_message[i] == 101 ||
							client_message[i] == 73 || client_message[i] == 105 ||
							client_message[i] == 79 || client_message[i] == 111 ||
							client_message[i] == 85 || client_message[i] == 117)
						{
							vowels++;
						}
					}
					else
					{
						if(vowels >= 1 && j > 1)
						{
							int copy = j;
							i--;
							int k = 0;
							for (; k < j; k++)
							{
								client_message[i] = wordArr[k];
								i--;	
							}				
							i = i + j + 1;
						}
						vowels = 0;
						j = 0;
					}
				}
				//Send the message back to client
				strcpy(server_message, client_message);
				printf("Server Message: %s\n",server_message);
				//strcpy(server_message, client_message);
				
				//strcpy(server_message ,"Hello i am server. your id is ");
				//strcat(server_message , &id);
				
				if (send(client_sock, server_message, strlen(server_message),0)<0)
				{
				        printf("Send Failed. Error!!!!!\n");
				        return -1;
				}
				
				memset(server_message,'\0',sizeof(server_message));
				memset(client_message,'\0',sizeof(client_message));
		}		      
        //Closing the Socket
        
        close(client_sock);
        close(socket_desc);
}
