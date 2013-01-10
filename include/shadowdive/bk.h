#ifndef _SD_BK_H
#define _SD_BK_H

#include <stdint.h>

typedef struct sd_palette_t sd_palette;
typedef struct sd_bk_anim_t sd_bk_anim;
typedef struct sd_vga_image_t sd_vga_image;

typedef struct sd_bk_file_t {
    uint32_t file_id;
    uint8_t unknown_a;

    sd_bk_anim *anims[50];

    sd_vga_image *background;

    uint8_t num_palettes;
    sd_palette **palettes;

    char footer[30];
} sd_bk_file;

/**
  * Creates BK file structure
  */
sd_bk_file* sd_bk_create();

/**
  * Sets background image
  */
void sd_bk_set_background(sd_bk_file *bk, sd_vga_image *img);

/**
  * Load .BK file. Returns 0 on error.
  */
int sd_bk_load(sd_bk_file *bk, const char *filename);

/**
  * Saves BK file structure. Returns 1 on success, 0 on error.
  */
int sd_bk_save(sd_bk_file *bk, const char* filename);

/**
  * Deletes bk_file structure from memory.
  */
void sd_bk_delete(sd_bk_file *bk);

#endif // _SD_BK_H
