#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

FILE *ptr;
FILE *write_ptr;

int main( ){

	int fi, fo, d = 1280, v = 1024;
	int SIZEI = 640*2;
	int SIZE_SCREEN = d * 2 - 1280;
	char *buf, *buf_frame;
	buf = (char *) malloc(sizeof(char) * SIZEI);
	buf_frame = (char *) malloc(sizeof(char) * SIZE_SCREEN);
	
	fi = open("v_fifo", O_RDONLY);
	fo = open("/dev/fb0", O_WRONLY);

	// Read and process each row
    for (size_t i = 0; i < 480; i++) {
        ssize_t n_read = read(fi, buf + i * SIZEI, SIZEI);
        if (n_read == -1) {
            perror("Error reading from video device");
            break;
        } else if (n_read != SIZEI) {
            fprintf(stderr, "Incomplete read: expected %zu bytes, got %zd bytes\n", SIZEI, n_read);
            break;
        }
    }

    // Write the complete frame to the output file
    ssize_t n_written = write(fo, buf_frame, SIZE_SCREEN);
    if (n_written != SIZE_SCREEN) {
        perror("Error writing to output file");
    }

	lseek(fo,SIZE_SCREEN,SEEK_CUR);
		
	if((lseek(fo,0,SEEK_CUR)) >= (d * 2 * 480)){
		lseek(fo,0,SEEK_SET);
	}
    
	close(fi);
	close(fo);
	
        free(buf);
		free(buf_frame);

        printf("Done!\n");

        return 0;
}
