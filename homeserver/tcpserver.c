#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080
#define BUF_SIZE 256

void *handle_client(void *arg);

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    pthread_t thread_id;

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // bind socket to port
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    // listen for connections
    listen(sockfd, 5);

    printf("Server started. Waiting for connections...\n");

    while (1) {
        // accept a new connection
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            exit(1);
        }

        // spawn a new thread to handle the connection
        if (pthread_create(&thread_id, NULL, handle_client, &newsockfd) < 0) {
            perror("ERROR creating thread");
            exit(1);
        }
    }

    close(sockfd);

    return 0;
}

void *handle_client(void *arg) {
    int sockfd = *(int *)arg;
    char buffer[BUF_SIZE];
    int n;

    while(1) {
        bzero(buffer, BUF_SIZE);
        n = read(sockfd, buffer, BUF_SIZE-1);
        if (n <= 0) {
            perror("ERROR reading from socket or client disconnected");
            close(sockfd);
            pthread_exit(NULL);
        }

        printf("Received message: %s\n", buffer);

        // Echo back the message
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("ERROR writing to socket");
            close(sockfd);
            pthread_exit(NULL);
        }
    }
}

