#ifndef _BMP_STRUCT_H_
#define _BMP_STRUCT_H_

#include <stdint.h>
#include <stdio.h>

struct __attribute__((packed)) bmp_header {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

struct image {
    uint64_t width, height;
    struct pixel *data;
};

struct __attribute__((packed)) pixel {
    unsigned char b, g, r;
};

enum read_status {
    READ_OK = 0,
    READ_INVALID_BITS,
    READ_INVALID_HEADER,
    READ_ERR
};

enum write_status {
    WRITE_OK = 0,
    WRITE_ERR
};

enum read_status read_bmp(FILE* in, struct image *image);
enum write_status write_bmp(FILE* out, struct image *image);
struct image* rotate(struct image* const in, int32_t parameter);
struct bmp_header create_bmp_header(struct image *image);
enum read_status check_bmp_header(struct bmp_header *header);
void print_read_error(enum read_status status);
void print_write_error(enum write_status status);
struct pixel get_pixel(const struct image img, uint64_t x, uint64_t y);
void set_pixel(const struct image* img, uint64_t x, uint64_t y, struct pixel p);

#endif
