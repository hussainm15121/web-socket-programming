#include "h1.h"
/************************************************************
*Name: getFunc                                              *
*Date: 14/11/2021                                           *
*Import: char *buffer & newSocket(Int)                      *
*Export: None                                		      *
*Purpose: Implements the Get function from server side      *
*************************************************************/
void getFunc(char *buffer, int newSocket)
{
	FILE *f1;						//File pointer
	char str[MAX];						// Str Array created to MAX size
	
			bzero(str, sizeof(str));		//initializing array with '\0'
			read(newSocket, &str, MAX);		// Read method used to read input sent from client
			printf("\nRemote File Name : %s\n", str);	
					
			f1=fopen(str, "r");			//opening file and to read
	 				
			while(fgets(buffer, 4096, f1)!=NULL)	//Reading file contents
			{
				write(newSocket,buffer,MAX);	//Writing the data through socket to client
				printf("\n");
			}
			fclose(f1);			// Closing pointer
			bzero(str, sizeof(str));	// Setting str values to '\0'
}

/************************************************************
*Name: putFunc                                              *
*Date: 14/11/2021                                           *
*Import: char *buffer & newSocket(Int)                      *
*Export: None                                		      *
*Purpose: Implements the Put function from server side      *
*************************************************************/

void putFunc(char *buffer, int newSocket)
{
	char initial[1024];					//Creating arrays
	char recvline[MAX];
	char first[40];
	char second[40];
	char third[40] = "\0";					//Initializing array with "\0"
	strcpy(initial, buffer);				// Copying data from buffer to initial array
	FILE *fp;						//Creating file pointer
	int n;
					
		sscanf(initial, "%s %s %s", first, second, third);		//Parsing command line input into sections 
					
		write(newSocket,second,MAX);					//Writing to the client with the name of file

		if(third[0] == '\0')						//Checking if user entered local file name
		{ 
			while((n=read(newSocket,recvline,MAX))!=0)
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
				 else if(third[0] != '\0');			//Checking if user entered local file name
				 {
				     	while((n=read(newSocket,recvline,MAX))!=0)	//Writing to a file instead of console
		       		{
			       		if((fp=freopen(third, "w" ,stdout))==NULL) 
			       		{
							printf("Cannot open file.\n");
			   				exit(1);
			  			}
			  				printf("%s", recvline);
			  				fclose(fp);		
					}
				 }
}
/************************************************************
*Name: dirFunc                                              *
*Date: 14/11/2021                                           *
*Import: char *buffer & newSocket(Int)                      *
*Export: None                                		      *
*Purpose: Implements the dir function from server side      *
*************************************************************/
void dirFunc(char *buffer, int newSocket)
{
	DIR* dir = opendir(".");	//"." is for current directory
	if(dir == NULL)
	{
		printf("EMPTY");		
	}
	
	struct dirent* entity;			//creating a struct dirent with entity
	entity = readdir(dir);
	while (entity != NULL)
	{
		printf("%s\n", entity->d_name);		//prints current directory
		write(newSocket,entity->d_name,MAX);		//sends diretory to client
		entity = readdir(dir);	
	}
		closedir(dir);					
}
/************************************************************
*Name: cdFunc                                               *
*Date: 14/11/2021                                           *
*Import: char *buffer & newSocket(Int)                      *
*Export: None                                		      *
*Purpose: Implements the cd function from server side       *
*************************************************************/
void cdFunc(char *buffer, int newSocket)
{
	char initial[1024];				//Declaring arrays
	char first[40];
	char second[40];
	strcpy(initial, buffer);			//Copying from buffer into initial
	
	sscanf(initial, "%s %s", first, second);	//Parsing contents
	
	char s[100];

    	printf("%s\n", getcwd(s, 100));		// printing current working directory
    	chdir(second);					// using the command to change the directory
  	write(newSocket,getcwd(s, 100),MAX);
    	printf("%s\n", getcwd(s, 100));		// printing current working directory
}
/************************************************************
*Name: deleteFunc                                           *
*Date: 14/11/2021                                           *
*Import: char *buffer & newSocket(Int)                      *
*Export: None                                		      *
*Purpose: Implements the delete function from server side   *
*************************************************************/
void deleteFunc(char *buffer, int newSocket)
{
	char initial[1024];					//Declaring arrays
	char first[40];
	char second[40];	
	int delFname;
	strcpy(initial, buffer);				//Copying from buffer into initial
	
	sscanf(initial, "%s %s", first, second);
	
	if (remove(second) == 0) 				//Removing file
	{
		delFname =0;
		write(newSocket, &delFname, MAX);	
        	printf("The file is deleted successfully.");
    	} 
    	else 
    	{
    		delFname =-1;
    		write(newSocket, &delFname, MAX);
        	printf("The file is not deleted.");
    	}
}
/************************************************************
*Name: pwdFunc                                              *
*Date: 14/11/2021                                           *
*Import: char *buffer & newSocket(Int)                      *
*Export: None                                		      *
*Purpose: Implements the pwd function from server side      *
*************************************************************/
void pwdFunc(char *buffer, int newSocket)
{
	char s[100];
	
    	printf("%s\n", getcwd(s, 100));			// printing current working directory
    
  	write(newSocket,getcwd(s, 100),MAX);
}
/*****************************************************************************
* Author: Hussain Mehdi							*
* Date: 14/11/2021								*
* Purpose: TCP Server Program 						*
*										*
******************************************************************************/
int main(int argc, char *argv[])
{
	int max_clients = argv[1];
	int cNumber, exit, epExit;
	int sockfd, ret;				//Declaring socket descriptors and initializing variables
	struct sockaddr_in serverAddr;		
	struct sockaddr_in newAddr;
	
	char t[MAX], str[MAX], here[MAX];
	char buffer[1024];
	char *welcome = "Welcome this is Server!";
	char *data = buffer;
	char *gets = "get";
	char *puts = "put";
	char *dirs = "dir";
	char *cds = "cd";
	char *deletes = "delete";
	char *pwds = "pwd";
	char *quits = "quit";
	
	int newSocket;
	FILE *fp;
	int n;
	
	socklen_t addr_size;			//Socket length, address size
	
	char databuf[1024];			// Declaring buffer
	pid_t childpid;			// using pid_t to create child pids

	sockfd = socket(AF_INET, SOCK_STREAM, 0);		//Creating the socket 
	if(sockfd < 0)
	{
		printf("[-]Error in connection.\n");
	}
	else
	{
		printf("[+]Server Socket is created.\n");

		memset(&serverAddr, '\0', sizeof(serverAddr));		//Setting memory address
		serverAddr.sin_family = AF_INET;				//Using AF_INET (for IPv4)
		serverAddr.sin_port = htons(PORT);				//Decalring the port number
		serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");		//using loopback IP address

		ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));	//using bind() function to bind the socket
		if(ret < 0)
		{
			printf("[-]Error in binding.\n");
			
		}
		else
		{
		printf("[+]Bind to port %d\n", 62001);

		if(listen(sockfd, 10) == 0)					//Listening for new connections
		{
			printf("[+]Listening....\n");
		}
		else
		{
			printf("[-]Error in binding.\n");
		}
			
		while(exit<1)
		{
			newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);		//Accepting socket connection
			if(newSocket < 0)
			{
				printf("[-] No Clients Connected");
				exit++;
			}
			else
			{
				printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));									//displaying connected address
				
				bzero(buffer, 1024);						//initializing buffer with bzero	
				send(newSocket, welcome, strlen(welcome), 0);		//Sending welcome message to client

				if((childpid = fork()) == 0)		//Calling fork() to create child processes
				{
					close(sockfd);

					while(epExit<1)
					{
						recv(newSocket, buffer, 1024, 0);	//recieving buffer input from client
						if(strstr(data, gets) != NULL)	//Get function
						{
							getFunc(buffer, newSocket);
						       printf("\nFile Transferred Successfully\n");
						}
						else if(strstr(buffer, puts) != NULL)		//Put function
						{
							putFunc(buffer, newSocket);
							printf("\nFile Recieved Successfully\n");
						} 
						else if(strstr(buffer, dirs) != NULL)		//directory function
						{
							dirFunc(buffer, newSocket);
							printf("\nAll Files and Directories Listed\n");
						}
						else if(strstr(buffer, cds) != NULL)		//cd function
						{
							cdFunc(buffer, newSocket);
							printf("\nDirectory Changed\n");
						}
						else if(strstr(buffer, deletes) != NULL)	//delete function
						{
							deleteFunc(buffer, newSocket);
							printf("\nFile Deleted\n");
						}
						else if(strstr(buffer, quits) != NULL)	//quit function
						{
							printf("Server Ending...");
							printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
							close(newSocket);
							epExit++;
							exit++;
							
						}
						else if(strstr(buffer, pwds) != NULL)		//pwd function
						{
							pwdFunc(buffer, newSocket);
						}
						else
						{
							printf("Client: %s\n", buffer);
							send(newSocket, buffer, strlen(buffer), 0);
							bzero(buffer, sizeof(buffer));
						}
					}
					
				}
			}
		}
			close(newSocket);
	}
	}




	return 0;
}
