#include "sepia_sse.h"

extern void _sepia_asm(struct pixel* input_data, size_t input_data_len, struct pixel* output_data);

void sepia_sse(struct image* in, struct image* out) {
    out->width = in->width;
    out->height = in->height;
    out->data = malloc(sizeof(struct pixel) * out->width * out->height);
    _sepia_asm(in->data, in->height * in->width, out->data);
}
