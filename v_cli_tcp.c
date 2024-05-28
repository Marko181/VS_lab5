//./v_cli_tcp ime_streznika | gst-launch-1.0 fdsrc ! decodebin ! videoconvert ! autovideosink

/* Povezaven - TCP odjemalec */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h> // For open
#include <errno.h>

#define PORT_NUMBER 55000

int main(int argc, char **argv)
{ 
    int n, sd, initClient(char *);
    char buf[128];

    if (argc != 2)
    {
        printf("Uporaba: %s ime_streznika\n", argv[0]);
        exit(1);
    }
    if ((sd = initClient(argv[1])) < 0)
    {
        printf("napaka init\n");
        exit(1);
    }
    else
    {
        printf("Priklopljeno na strežnik, prejemam video tok...\n");

        // Read from the socket and write to stdout
        while ((n = read(sd, buf, sizeof(buf))) > 0)
        {
            // STDOUT_FILENO is a file descriptor, using write(STDOUT_FILENO,...) writes
            // directly in terminal or any process that reads from the standard output of the v_cli_tcp program
            // we use | gst-launch-1.0 fdsrc... when running the v_cli_tcp so it gets the STDOUT_FILENO as file source
            if (write(STDOUT_FILENO, buf, n) == -1)
            {
                perror("write stdout err");
                break;
            }
        }

        if (n < 0)
        {
            perror("read err");
        }

        close(sd);
    }
    exit(0);
}

int initClient(char *hostName)
{
    struct sockaddr_in servAddr;
    struct hostent *host;
    int sd;

    if ((host = gethostbyname(hostName)) == NULL)
        return (-1);
    memset(&servAddr, 0, sizeof(servAddr));
    memcpy(&servAddr.sin_addr, host->h_addr, host->h_length);
    servAddr.sin_family = host->h_addrtype;
    servAddr.sin_port = htons(PORT_NUMBER);

    printf("streznik: %s, ", host->h_name);
    printf("%s:%d\n", inet_ntoa(servAddr.sin_addr), PORT_NUMBER);
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return (-2);
    if (connect(sd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
        return (-3);
    return (sd);
}
