#include "sepia_c.h"

static unsigned char sat(uint64_t x){
    if (x < 256) return x; 
    return 255;
}

static struct pixel sepia_one( struct pixel const pixel_in ) {
    static const float c[3][3] = {
            {.393f, .769f, .189f},
            {.349f, .686f, .168f},
            {.272f, .543f, .131f}};
    struct pixel pixel_out;
    pixel_out.r = sat(
            pixel_in.r * c[0][0] + pixel_in.g * c[0][1] + pixel_in.b * c[0][2]
    );
    pixel_out.g = sat(
            pixel_in.r * c[1][0] + pixel_in.g * c[1][1] + pixel_in.b * c[1][2]
    );
    pixel_out.b = sat(
            pixel_in.r * c[2][0] + pixel_in.g * c[2][1] + pixel_in.b * c[2][2]
    );
    return pixel_out;
}

void sepia_c( struct image* img, struct image* res ) {
    uint32_t x,y;
    res->width = img->width;
    res->height = img->height;
    res->data = malloc(sizeof(struct pixel) * res->width * res->height);
    for( y = 0; y < img->height; y++ )
        for( x = 0; x < img->width; x++ ) {
            struct pixel processed = sepia_one(get_pixel(*img, x, y));
            set_pixel(res, x, y, processed);
        }
}
