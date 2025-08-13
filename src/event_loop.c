#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "event_loop.h"
#include "parse.h"

#define PORT 12345
#define MAX_CLIENTS 10
#define BUF_SIZE 1024

int server_fd;
int clients[MAX_CLIENTS];

void handle_sigint(int sig) {
    printf("\nServer shutting down...\n");

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] > 0) close(clients[i]);
    }

    if (server_fd > 0) close(server_fd);
    exit(0);
}

void event_loop() {
    signal(SIGINT, handle_sigint);

    struct sockaddr_in addr;
    char buf[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    for (int i = 0; i < MAX_CLIENTS; i++) clients[i] = -1;

    fd_set read_fds;
    int max_fd, activity, client_fd;

    while(1) {
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        max_fd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] > 0) FD_SET(clients[i], &read_fds);
            if (clients[i] > max_fd) max_fd = clients[i];
        }

        activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &read_fds)) {
            client_fd = accept(server_fd, NULL, NULL);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i] == -1) { clients[i] = client_fd; break; }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] > 0 && FD_ISSET(clients[i], &read_fds)) {
                int n = read(clients[i], buf, BUF_SIZE);
                if (n <= 0) { close(clients[i]); clients[i] = -1; }
                else { buf[n] = '\0'; buf[strcspn(buf, "\r\n")] = '\0'; parse(clients[i], buf); }
            }
        }
    }
}
