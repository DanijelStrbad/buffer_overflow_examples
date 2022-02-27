void printTerminalOptions(int argc, char *argv[]);

int Getaddrinfo(const char *restrict node, const char *restrict service, const struct addrinfo *restrict hints, struct addrinfo **restrict res);
int Socket(int family, int type, int proto);
int Bind(int sockfd, const struct sockaddr *myaddr, int addrlen);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr* cliaddr, socklen_t *addrlen);
int Connect(int sockfd, const struct sockaddr *server, socklen_t addrlen);
ssize_t Send(int sockfd, const void *buf, size_t len, int flags);
ssize_t Recv(int sockfd, void *buf, size_t len, int flags);
