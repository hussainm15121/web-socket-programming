#include "h1.h"
/*****************************************************************************
* Author: Hussain Mehdi							*
* Date: 14/11/2021								*
* Purpose: TCP Client Program 						*
******************************************************************************/
int main()
{
	char remote[MAX],recvline[MAX],s[MAX],name[MAX], local[MAX] ="";		//Declaring all variables and arryas 
	char str[MAX], director[MAX], cDirectory[MAX], pwdDirectory[MAX];
	int delFname;
	int clientSocket, ret, n;
	struct sockaddr_in serverAddr;
	char buffer[1024];
	FILE *fp;
	char *testName;
	char *data = buffer;
	char *gets = "get";
	char *puts = "put";
	char *dirs = "dir";
	char *quits = "quit";
	char *cds = "cd";
	char *deletes = "delete";
	char *pwds = "pwd";
	
	int i = 0;
	
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);		//creating clientSocket
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));		//Setting memory 
	serverAddr.sin_family = AF_INET;				//Using AF_INET (for IPv4)
	serverAddr.sin_port = htons(PORT);				//Decalring the port number
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");		//using loopback IP address

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));	//using connect() function to connect the server
	if(ret < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	bzero(buffer, 1024);				//initializing buffer with bzero
	recv(clientSocket, buffer, sizeof(buffer), 0);	//Recieving welcome message
	printf("Server: %s\n", buffer);
	
	while(1){
		bzero(buffer, 1024);
		printf("SFTC: \t");
		fgets(buffer, 1024, stdin);			//getting userinput for buffer 
		send(clientSocket, buffer, strlen(buffer), 0);	//Sending data to server

		if(strstr(data, gets) != NULL)
		{
			char initial[1024];
			char first[40];
			char second[40];
			char third[40] = "\0";
			strcpy(initial, buffer);			// Copying data from buffer to initial array
			
			sscanf(initial, "%s %s %s", first, second, third);		//Parsing command line input into sections
			
			write(clientSocket,second,MAX);		

			if(third[0] == '\0')					//Checking if user entered local file name
			{ 
				while((n=read(clientSocket,recvline,MAX))!=0)
       			{
	       			if((fp=freopen(second, "w" ,stdout))==NULL) 	//Writing to a file instead of console
	       			{
						printf("Cannot open file.\n");
	   					 exit(1);
	  				}
	  				printf("%s", recvline);
	  				fclose(fp);		
				}
		       }
		       else if(third[0] != '\0');					//Checking if user entered local file name
		       {
		       	while((n=read(clientSocket,recvline,MAX))!=0)
       			{
	       			if((fp=freopen(third, "w" ,stdout))==NULL) 	//Writing to a file instead of console
	       			{
						printf("Cannot open file.\n");
	   					 exit(1);
	  				}
	  				printf("%s", recvline);
	  				fclose(fp);		
				}
		       }			
		}
		else if(strstr(data, puts) != NULL)				//Put function
		{
				bzero(str, sizeof(str));
				read(clientSocket, &str, MAX);		// Read method used to read input sent from server
				printf("\nRemote File Name : %s\n", str);
					
				fp=fopen(str, "r");				//opening file and to read
	 				
				while(fgets(buffer, 4096, fp)!=NULL)		//Reading file contents
				{
					write(clientSocket,buffer,MAX);	//Writing the data through socket to client
					printf("\n");
				}
				fclose(fp);					// Closing pointer
				bzero(str, sizeof(str));			// Setting str values to '\0'
				printf("\nFile Transferred Successfully\n");
				exit(1);	
		
		}
		else if(strstr(data, dirs) != NULL)				//Directory Function
		{
			printf("\nDirectory content of Remote Computer listed below:\n");
			while (read(clientSocket, &director, MAX)!= NULL)
			{
				printf("\n%s\n", director);			//Prints directory of remote PC on current screen
			}
			printf("\nDirectory listed\n");
				
		}
		else if(recv(clientSocket, buffer, 1024, 0) < 0)		// Error checking
		{
			printf("[-]Error in receiving data.\n");
		}
		else if(strstr(data, cds) != NULL)				//Changing direcotry
		{
			bzero(cDirectory, sizeof(cDirectory));
			read(clientSocket, &cDirectory, MAX);
			printf("\n%s", cDirectory);
		}
		else if(strstr(data, quits) != NULL)				//Ends program
		{
			printf("Client Exiting...");
			close(clientSocket);
			exit(1);
		}
		else if(strstr(data, deletes) != NULL)			//use Remove to delete file on remote pc
		{
			read(clientSocket, &delFname, MAX);
			if(delFname != 0)
			{
				printf("An error occured while deleting file");
			}
			else
			{
				printf("File deleted from remote PC");
			}
			
		}		
		else if(strstr(data, pwds) != NULL)			//Prints working directory
		{
			bzero(cDirectory, sizeof(pwdDirectory));
			read(clientSocket, &pwdDirectory, MAX);
			printf("\n%s", cDirectory);
		}
		else
		{
			printf("Server: \t%s\n", buffer);		//If none of the above, then acts as an echos command.
		}
	}

	return 0;
}
