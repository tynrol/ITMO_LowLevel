#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "bmp_struct.h"

int main(int argc, char* argv[]){
    FILE* in, * out;
    struct image img;
    int angle = atoi(argv[3]);

    in = fopen(argv[1], "rb");
    out = fopen(argv[2], "wb");

    if(in == NULL || out == NULL) {
        puts("Can't open file");
        return 0;
    }

    enum read_status statusRead = read_bmp(in, &img);
    rotate(&img, angle);
    if(statusRead == READ_OK){
        enum write_status statusWrite = write_bmp(out, &img);
        print_write_error(statusWrite);
    } else{
        print_read_error(statusRead);
    }

    fclose(in);
    fclose(out);
    return 0;
}
