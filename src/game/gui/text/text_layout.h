#ifndef TEXT_LAYOUT_H
#define TEXT_LAYOUT_H

#include "game/gui/text/enums.h"
#include "resources/fonts.h"
#include "utils/str.h"
#include "utils/vector.h"
#include "video/surface.h"

typedef struct text_layout_item {
    uint16_t x; // Relative X coordinate withing the bounding box
    uint16_t y; // Relative Y coordinate withing the bounding box
    const surface *glyph;
} text_layout_item;

typedef struct text_layout {
    vector items;
    uint16_t w;
    uint16_t h;
} text_layout;

text_layout *text_layout_create(uint16_t w, uint16_t h);
void text_layout_free(text_layout **layout);
size_t find_next_line_end(const str *buf, const font *font, text_row_direction direction, size_t start_index,
                          uint8_t letter_spacing, uint16_t max_width);
void text_layout_compute(text_layout *layout, const str *buf, const font *font, text_vertical_align vertical_align,
                         text_horizontal_align horizontal_align, text_margin margin, text_row_direction direction,
                         uint8_t line_spacing, uint8_t letter_spacing, uint8_t max_lines);

#endif // TEXT_LAYOUT_H
