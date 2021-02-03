#define M_PI 3.14159265358979323846
#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "bmp_struct.h"

enum read_status check_bmp_header(struct bmp_header *header){
    if(header->bfType != 0x4D42) return READ_INVALID_HEADER;
    if(header->biBitCount != 24) return READ_INVALID_BITS;
    if(header->biCompression != 0) return READ_INVALID_HEADER;
    if(header->bfReserved != 0) return READ_INVALID_HEADER;
    if(header->biSize != 40) return READ_INVALID_HEADER;

    return READ_OK;
}
struct bmp_header create_bmp_header(struct image *image){
    struct bmp_header header;
    size_t offset = 4 - (image -> width * sizeof(struct pixel)) % 4;
    offset = offset == 4 ? 0 : offset;

    header.bfType = 0x4D42;
    header.bfileSize = ((image -> width + offset)*(image -> height)) + sizeof(struct bmp_header);
    header.bfReserved = 0;
    header.bOffBits = 54;
    header.biSize = 40;
    header.biPlanes = 1;
    header.biBitCount = 24;
    header.biCompression = 0;
    header.biSizeImage = ((image -> height)*((image -> width)*sizeof(struct pixel)+offset));
    header.biXPelsPerMeter = 0; 
    header.biYPelsPerMeter = 0;
    header.biClrUsed = 0;
    header.biClrImportant = 0;
    header.biWidth = image -> width;
    header.biHeight = image -> height;

    return header;
}
enum write_status write_bmp(FILE* out, struct image *image){
    size_t i;
    uint8_t rubbish[4] = {0};
    struct bmp_header header;

    size_t offset= 4 - (image -> width * sizeof(struct pixel)) % 4;
    offset = offset == 4 ? 0 : offset;

    header = create_bmp_header(image);
    if(!fwrite(&header, sizeof(struct bmp_header), 1, out)) return WRITE_ERR;
    
    for(i=0; i < image -> height; i++){
        if(!fwrite(&image -> data[image->width*i], sizeof(struct pixel), image -> width, out)) return WRITE_ERR;
        if(offset != 0) fwrite(rubbish, sizeof(uint8_t), offset, out);
    }
    return WRITE_OK;
}
enum read_status read_bmp(FILE* in, struct image *image){
    size_t i;
    uint8_t rubbish[4];
    struct bmp_header* header = malloc(sizeof(struct bmp_header));

    if (!fread(header, sizeof(struct bmp_header), 1, in)) return READ_ERR;
    fseek(in, header -> bOffBits, SEEK_SET);

    enum read_status check = check_bmp_header(header);
    if (check != READ_OK) return check;

    image->width = header->biWidth;
    image->height = header->biHeight;

    size_t offset= 4 - (image->width * sizeof(struct pixel)) % 4;
    offset = offset == 4 ? 0 : offset;
    struct pixel* bitSet = malloc(image->height * image->width * sizeof(struct pixel));

    for(i=0; i < image -> height; i++){
        if(!fread(bitSet+i*image->width, sizeof(struct pixel), image->width , in)) return READ_ERR;
        if(offset != 0) fread(rubbish, offset, 1, in);
    }
    image -> data = bitSet;

    return READ_OK;
}
/*
struct image* rotate(struct image* const image, int32_t parameter){
    struct pixel* bitSet = image->data;
    if(parameter == 90 || parameter == 270 || parameter == -90 || parameter == -270){
    uint32_t offset_x = image->width / 2;
    uint32_t offset_y = image->height / 2;
    uint32_t width = image->width;
	uint32_t height = image->height;
    double angle = parameter*M_PI/180;
    size_t i;
    size_t j;
    struct pixel* newSet = malloc(width*height*sizeof(struct pixel));
    for(i=0; i < height; i++){
        for(j=0; j < width; j++){
            int32_t oldX = j - offset_x;
			int32_t oldY = i - offset_y;
			uint32_t newX = offset_y + round(oldX* cos(angle)+oldY*sin (angle));
			uint32_t newY = offset_x + round(-oldX* sin (angle) + oldY * cos (angle));
            *(newSet + newY*height + newX) = *(bitSet + i*width + j);
        }
    }
    image -> width = height;
    image -> height = width;
    image -> data = newSet;

    return image;
    } 
    else if(parameter == 180 || parameter == -180){
        uint64_t width = image->width;
	    uint64_t height = image->height;
        size_t i;
        size_t j;
        struct pixel* newSet = malloc(width*height*sizeof(struct pixel));
    for(i=0; i < height; i++){
        for(j=0; j < width; j++){
            *(newSet + (height-i-1)*width + (width-j-1)) = *(bitSet + i*width + j);
        }
    }
    image -> data = newSet;

    return image;
    } else {
        puts("Only 90, 180, 270 angles. Image written without rotation.");
        return image;
    }
}
*/
void print_write_error(enum write_status status){
    switch (status) {
        case WRITE_OK:
            printf("File written successfully\n");
            break;
        case WRITE_ERR:
            printf("Cant write in file\n");
            break;
    }
}

void print_read_error(enum read_status status){
    switch(status){
        case READ_OK:
            printf("File read successfully\n");
            break;
        case READ_ERR:
            printf("Read IO error\n");
            break;
        case READ_INVALID_HEADER:
            printf("Read header error\n");
            break;
        case READ_INVALID_BITS:
            printf("Image is damaged");
            break;
    }
}

struct pixel get_pixel(const struct image img, uint64_t x, uint64_t y) {
    return img.data[y * img.width + x];
}
void set_pixel(const struct image* img, uint64_t x, uint64_t y, struct pixel p) {
    img->data[y * img->width + x] = p;
}
