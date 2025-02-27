#ifndef TEXT_ENUMS_H
#define TEXT_ENUMS_H

#include <stdint.h>

typedef enum text_vertical_align
{
    ALIGN_TEXT_TOP = 0,
    ALIGN_TEXT_MIDDLE,
    ALIGN_TEXT_BOTTOM
} text_vertical_align;

typedef enum text_horizontal_align
{
    ALIGN_TEXT_LEFT = 0,
    ALIGN_TEXT_CENTER,
    ALIGN_TEXT_RIGHT
} text_horizontal_align;

typedef struct text_margin {
    uint8_t left;
    uint8_t right;
    uint8_t top;
    uint8_t bottom;
} text_margin;

typedef enum text_row_direction
{
    TEXT_ROW_HORIZONTAL = 0,
    TEXT_ROW_VERTICAL,
} text_row_direction;

typedef enum glyph_shadow
{
    GLYPH_SHADOW_NONE = 0,
    GLYPH_SHADOW_TOP = 0x1,
    GLYPH_SHADOW_BOTTOM = 0x2,
    GLYPH_SHADOW_LEFT = 0x4,
    GLYPH_SHADOW_RIGHT = 0x8,
    GLYPH_SHADOW_ALL = 0xF
} glyph_shadow;

#endif // TEXT_ENUMS_H
