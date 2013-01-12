#include "flim.h"
#include "server.h"

#define SOCK_PATH "/tmp/flim.sock"
#define BUFFER_SIZE 10000

void flm_server_start(struct flim *flim)
{

    int server, client;
    if ((server = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_un local, remote;
    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);

    socklen_t len = strlen(local.sun_path) + sizeof(local.sun_family) + 1;
    if (bind(server, (struct sockaddr *) &local, len) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(server, 5) == -1) {
        perror("listen");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    socklen_t sock_len;
    while (1) {
        sock_len = sizeof(remote);
        if ((server = accept(server,
                        (struct sockaddr *) &remote, &sock_len)) == -1) {

            perror("accept");
            exit(1);
        }

        int received = recv(server, buffer, BUFFER_SIZE, 0);
        if (received < 0) {
            perror("recv");
        }

        buffer[received] = 0;
        flm_eval(flim, buffer);
    }
    close(client);
}
