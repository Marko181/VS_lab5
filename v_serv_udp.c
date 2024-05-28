/* UDP Server that sends video stream from FIFO */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXLINE     4096
#define SERVPORTNO  50000
#define FIFO_PATH   "v_fifo"

int main(void) {
    int sockfd, n;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char buffer[MAXLINE];
    int fifo_fd;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }

    // Initialize server address structure
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVPORTNO);

    // Bind socket to the server address
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind error");
        close(sockfd);
        exit(1);
    }

    // Open the FIFO for reading
    fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd == -1) {
        perror("open FIFO error");
        close(sockfd);
        exit(1);
    }

    while (1) {
        len = sizeof(cliaddr);

        // Wait for client message
        n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        if (n < 0) {
            perror("recvfrom error");
            continue;
        }

        printf("client: %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

        // Read from FIFO and send to client
        while ((n = read(fifo_fd, buffer, MAXLINE)) > 0) {
            if (sendto(sockfd, buffer, n, 0, (struct sockaddr *)&cliaddr, len) == -1) {
                perror("sendto error");
                break;
            }
        }

        // Reset FIFO read pointer for next client
        lseek(fifo_fd, 0, SEEK_SET);
    }

    close(fifo_fd);
    close(sockfd);
    return 0;
}
