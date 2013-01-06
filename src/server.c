#include "flim.h"
#include "server.h"

#define SOCK_PATH "/tmp/flim.sock"
#define BUFFER_SIZE 10000

int server_run(struct flim *flim)
{
    int s, s2, len;
    socklen_t t;
    struct sockaddr_un local, remote;

    char str[BUFFER_SIZE];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    local.sun_family = AF_UNIX;

    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);


    // add 1 to length of sun_path 
    // (http://markmail.org/message/djecr2s4yq5hwpsp#query:+page:1+mid:djecr2s4yq5hwpsp+state:results)
    len = strlen(local.sun_path) + 1 + sizeof(local.sun_family);

    if (bind(s, (struct sockaddr *) &local, len) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(s, 5) == -1) {
        perror("listen");
        exit(1);
    }

    while (1) {
        int n;
        t = sizeof(remote);
        if ((s2 = accept(s, (struct sockaddr *) &remote, &t)) == -1) {
            perror("accept");
            exit(1);
        }

        n = recv(s2, str, BUFFER_SIZE, 0);
        if (n < 0) {
            perror("recv");
        }

        str[n] = 0;
        printf("%s", str);
        flm_eval(flim, str);
    }

    close(s2);
    printf("Exiting server\n");
    return 0;
}


