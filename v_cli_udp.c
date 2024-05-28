// ./v_cli_udp ime_streznika | gst-launch-1.0 fdsrc ! decodebin ! videoconvert ! autovideosink

/* UDP Client to receive video stream */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096
#define SERVPORTNO 50000

int main(int argc, char **argv) {
    int sockfd, n;
    struct sockaddr_in servaddr;
    char buffer[MAXLINE];
  
    if (argc != 2) {
        printf("usage: %s <IP address>\n", argv[0]);
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(SERVPORTNO);

    // Send initial message to start receiving the stream
    strcpy(buffer, "START");
    n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (n == -1) {
        perror("sendto error");
        exit(1);
    }

    // Continuously receive data from the server and write it to stdout
    while (1) {
        n = recvfrom(sockfd, buffer, MAXLINE, 0, NULL, NULL);
        if (n == -1) {
            perror("recvfrom error");
            exit(1);
        }
        if (write(STDOUT_FILENO, buffer, n) != n) {
            perror("write error");
            exit(1);
        }
    }

    close(sockfd);
    return 0;
}
