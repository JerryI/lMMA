int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;

int addrlen = sizeof(address);
char buffer[10*1024] = {0};

void initServer() {

       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

}

void listen() {
    for (;;) {
        valread = read( new_socket , buffer, 10*1024);

        if (strstr(buffer, "Exit") != NULL) {
            break;
        }
        
        json message = json::parse(buffer);
        node* tree = btree(message);
        show(*tree);
        cout << '\n' << "------------" << '\n';
        tree = evaluate(tree);
        show(*tree);
        json* reply = new json;
        reply = ftree(*tree);

        memset(buffer, 0, 10*1024);

        string s = reply->dump();
        send(new_socket , (void*)s.c_str() , strlen(s.c_str()) , 0 );
    }

}