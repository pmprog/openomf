#include "shadowdive/sprite_image.h"
#include "shadowdive/rgba_image.h"
#include "shadowdive/palette.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

sd_sprite_image* sd_sprite_image_create(unsigned int w, unsigned int h, unsigned int len) {
    sd_sprite_image *img = (sd_sprite_image*)malloc(sizeof(sd_sprite_image));
    img->w = w;
    img->h = h;
    img->len = len;
    img->data = (char*)malloc(len);
    memset(img->data, 0, len);
    return img;
}

void sd_sprite_image_delete(sd_sprite_image *img, int missing) {
    if (!missing) {
        free(img->data);
    }
    free(img);
}

char color_to_palette_index(uint8_t r, uint8_t g, uint8_t b, sd_palette *pal) {
    /*for(int i = 255; i >= 0; i--) {*/
    for(int i = 0; i < 256; i++) {
        uint8_t red = pal->data[i][0] & 0xff;
        uint8_t blue = pal->data[i][1] & 0xff;
        uint8_t green = pal->data[i][2] & 0xff;
        if (red == r && blue == b && green == g) {
            /*if (i != 63) {*/
                /*printf("Red %u == %u; blue %u == %u; green %u == %u -- %u\n", red, r, blue, b, green, g, i);*/
            /*}*/
            return i;
        }
    }
    return 0;
}

sd_sprite_image* sd_sprite_image_encode(sd_rgba_image *img, sd_palette *pal, int remapping) {
    int lastx = -1;
    int lasty = -1;
    unsigned int rgb_size = (img->w * img->h * 4);
    // allocate a buffer plenty big enough, we will trim it later
    char *buf = (char*)malloc(img->w * img->h * 4);
    int i = 0;
    int j = 0; // where we need to insert pixel count
    int rowlen = 0;
    uint16_t c;
    for(int pos = 0; pos <= rgb_size; pos+= 4) {
        uint8_t r = img->data[pos];
        uint8_t g = img->data[pos+1];
        uint8_t b = img->data[pos+2];
        uint8_t a = img->data[pos+3];

        // ignore anytjhing but fully opaque pixels
        if (a == 255) {
            int16_t x = (pos/4) % img->w;
            int16_t y = (pos/4) / img->w;
            if (y != lasty) {
                // new row
                c = (y*4)+2;
                // write little endian unsigned word
                buf[i++] = c & 0x00ff;
                buf[i++] = (c & 0xff00) >> 8;
                lasty = y;
            }
            if (x != lastx+1) {
                // dont write X coordinate if we just wrote a row and the nex X coordinate is 0
                // because the decoder resets X coordinate to 0 after each row
                if (!(rowlen && x == 0)) {
                    // we skipped some columns
                    c = (x*4);
                    // write little endian unsigned word
                    buf[i++] = c & 0x00ff;
                    buf[i++] = (c & 0xff00) >> 8;
                }
                // write out the length of the previous row, if there was one
                if (rowlen) {
                    // go back and write in the width of the row of pixels
                    c = (rowlen*4)+1;
                    // write little endian unsigned word
                    buf[j] = c & 0x00ff;
                    buf[j+1] = (c & 0xff00) >> 8;
                    rowlen = 0;
                    lastx = -1;
                }
                j = i;
                i += 2; // leave room for the byte counting how many pixels in a row there are
            }
            lastx = x;
            // write byte
            buf[i++] = color_to_palette_index(r, g, b, pal);
            rowlen++;
            /*printf("rgba at pos %u X: %u Y: %u %u %u %u %u\n", pos, x, y, r, g, b, a);*/
        }
    }
    // update the length of the last row
    if (rowlen) {
        // go back and write in the width of the row of pixels
        c = (rowlen*4)+1;
        // write little endian unsigned word
        buf[j] = c & 0x00ff;
        buf[j+1] = (c & 0xff00) >> 8;
        rowlen = 0;
    }

    // End of sprite marker, a WORD of value 7
    buf[i++] = 7; 
    buf[i++] = 0;
    sd_sprite_image *sprite = sd_sprite_image_create(img->w, img->h, i);
    memcpy(sprite->data, buf, i);
    free(buf);
    return sprite;
}

sd_rgba_image* sd_sprite_image_decode(sd_sprite_image *img, sd_palette *pal, int remapping) {
    sd_rgba_image *rgba = img->len>0 ? sd_rgba_image_create(img->w, img->h) : sd_rgba_image_create(1, 1);
    uint16_t x = 0;
    uint16_t y = 0;
    int i = 0;
    if (img->w == 0 || img->h == 0) {
        // XXX CREDITS.BK has a bunch of 0 width sprites, for some unknown reason
        return rgba;
    }
    while(i < img->len) {
        // read a word
        uint16_t c = (uint8_t)img->data[i] + ((uint8_t)img->data[i+1] << 8);
        char op = c % 4;
        uint16_t data = c / 4;
        i += 2; // we read 2 bytes
        switch(op) {
            case 0:
                x = data;
                break;
            case 2:
                y = data;
                break;
            case 1:
                while(data > 0) {
                    uint8_t b = img->data[i];
                    int pos = ((y * img->w) + x) * 4;
                    if(remapping > -1) {
                        rgba->data[pos+0] = (uint8_t)pal->data[(uint8_t)pal->remaps[remapping][b]][0];
                        rgba->data[pos+1] = (uint8_t)pal->data[(uint8_t)pal->remaps[remapping][b]][1];
                        rgba->data[pos+2] = (uint8_t)pal->data[(uint8_t)pal->remaps[remapping][b]][2];
                    } else {
                        rgba->data[pos+0] = (uint8_t)pal->data[b][0];
                        rgba->data[pos+1] = (uint8_t)pal->data[b][1];
                        rgba->data[pos+2] = (uint8_t)pal->data[b][2];
                    }
                    rgba->data[pos+3] = 255; // fully opaque
                    i++; // we read 1 byte
                    x++;
                    data--;
                }
                x = 0;
                break;
            case 3:
                assert(i == img->len);
                break;
        }
    }
    return rgba;
}

