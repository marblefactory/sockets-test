#include <netinet/in.h>

class Server {
private:
    // The file descriptor of the socket used to listen for clients.
    int listen_socket_fd;
    // The address of the server.
    sockaddr_in server_addr;

public:
    // Create a server using the port number on which the server accepts
    // connections.
    Server(int port_num);
    ~Server();

    // Sets-up a socket to listen for clients.
    void setup();

    // Listen for a single message from a client.
    void listen();
};
