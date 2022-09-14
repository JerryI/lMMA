#undef FLOAT
#undef PATTERN
#undef BOOL

#include <winsock2.h>

int PORT = 2020;
#define BUFFER_SIZE 64*1024

char input_buffer[BUFFER_SIZE] = {0};

struct sockaddr_in client;
int c;
WSADATA wsa;

struct sockaddr_in server;

SOCKET initServer() {

    SOCKET s;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return 0;
	}
	
	printf("Initialised.\n");
	
	//Create a socket
	if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	printf("Socket created.\n");
	
	//setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char*)send_buffer, send_buffer_sizeof);

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	//server.sin_port = htons( 2020 );
	
	//Bind
	for (; PORT < 2040; ++PORT) {
		server.sin_port = htons( PORT );

		if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
		{
			printf("Bind failed with error code : %d" , WSAGetLastError());
			printf("Trying another...");
		} else {
			break;
		}
	}
	
	puts("Bind done");
    return s;
}

SOCKET listen(SOCKET &s) {
    SOCKET new_socket;

    listen(s , 3);
    puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);
	new_socket = accept(s , (struct sockaddr *)&client, &c);
	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d" , WSAGetLastError());
	}
	
	puts("Connection accepted");	
	//Accept and incoming connection
	return new_socket;
}

unsigned long get_n_readable_bytes(SOCKET sock) {
    unsigned long n = -1;
   if (ioctlsocket(sock, FIONREAD, &n) < 0) {
       /* look in WSAGetLastError() for the error code */
       return 0;
   }
   return n;
}

char* read(SOCKET s) { 
    memset(input_buffer, 0, BUFFER_SIZE);
	unsigned long = get_n_readable_bytes(s);
	char* p = input_buffer;

	for (;;) {
    	int res = recv(s , p, BUFFER_SIZE, 0);
    
    	if (res == SOCKET_ERROR) {
        	puts("recv failed");
    	}
		
		p = p +res;
	}
    
    return input_buffer;
}

int reply(string message, SOCKET s) {
	if( send(s , message.c_str() , strlen(message.c_str()) , 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Send\n");    
    return 0;
}
