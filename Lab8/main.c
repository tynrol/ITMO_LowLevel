#include <stdio.h>
#include <sys/resource.h>
#include <stdint.h>
#include "bmp_struct.h"
#include "sepia_c.h"
#include "sepia_sse.h"

void invoke_function(void (*function)(struct image*, struct image*), struct image*, struct image*, char* description);

int main(int argc, char* argv[]) {
    FILE* in, * out1, * out2;
    struct image img;
    struct image img_out;

    in = fopen(argv[1], "rb");
    out1 = fopen(argv[2], "wb");
    out2 = fopen(argv[3], "wb");

    if(in == NULL || out1 == NULL || out2 == NULL) {
        puts("Can't open file");
        return 0;
    }
    enum read_status statusRead = read_bmp(in, &img);

    invoke_function(sepia_c, &img, &img_out, "C");
    if(statusRead == READ_OK){
        enum write_status statusWrite = write_bmp(out1, &img_out);
        print_write_error(statusWrite);
    } else{
        print_read_error(statusRead);
    }

    invoke_function(sepia_sse, &img, &img_out, "SSE");
    if(statusRead == READ_OK){
        enum write_status statusWrite = write_bmp(out2, &img_out);
        print_write_error(statusWrite);
    } else{
        print_read_error(statusRead);
    }

    return 0;
}


void invoke_function(void (*function)(struct image*, struct image*), struct image* img_in, struct image* img_out, char* description) {
    struct rusage r;
    struct timeval start;
    struct timeval end;
    getrusage(RUSAGE_SELF, &r);
    start = r.ru_utime;

    function(img_in, img_out);

    getrusage(RUSAGE_SELF, &r);
    end = r.ru_utime;

    long res_time = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
    printf("Time elapsed in seconds for %s filter: %.2f\n", description, res_time/1000000.0);

}

