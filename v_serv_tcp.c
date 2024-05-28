#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h> // for open
#include <sys/stat.h> // for mkfifo

#define PORT_NUMBER 55000
#define FIFO_NAME "v_fifo"

int main( int argc, char **argv )
{ 
  int    n, sd, initClient( char * );
  char   buf[128];
  int    fifo_fd;

  if( argc != 2 ){
    printf("Usage: %s server_name\n", argv[0]);
    exit( 1 );
  }

  // Initialize the client connection
  if( (sd = initClient( argv[1] )) < 0 ){
    printf("Initialization error\n");  
    exit( 1 );
  }
  else{
    // Open FIFO for reading
    if ((fifo_fd = open(FIFO_NAME, O_RDONLY)) < 0) {
      perror("FIFO open error");
      exit(1);
    }

    printf("Reading from FIFO and sending video stream to server\n");

    // Read from FIFO and send data to the server
    while((n = read(fifo_fd, buf, sizeof(buf))) > 0) {
      if( write(sd, buf, n) == -1) {
        perror("write error");
        break;
      }
    }

    if (n == -1) {
      perror("read error");
    }

    close(fifo_fd);
    close(sd);
  }

  exit( 0 );
}

int initClient( char *hostName )
{
  struct sockaddr_in  servAddr;
  struct hostent     *host;
  int    sd;
  
  if( ( host = gethostbyname( hostName )) == NULL) return( -1 );
  memset( &servAddr, 0, sizeof(servAddr));
  memcpy( &servAddr.sin_addr, host->h_addr, host->h_length );
  servAddr.sin_family = host->h_addrtype;
  servAddr.sin_port   = htons( PORT_NUMBER );

  printf("Server: %s, ", host -> h_name);
  printf("%s:%d\n", inet_ntoa( servAddr.sin_addr ), PORT_NUMBER);
  if( (sd = socket(AF_INET,SOCK_STREAM,0)) < 0 ) return( -2 );
  if( connect(sd, (struct sockaddr *)&servAddr,sizeof(servAddr)) < 0) return( -3 );
  return( sd );
}
