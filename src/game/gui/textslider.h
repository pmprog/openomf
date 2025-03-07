#ifndef TEXTSLIDER_H
#define TEXTSLIDER_H

#include "game/gui/component.h"
#include "game/gui/text_render.h"

typedef void (*textslider_slide_cb)(component *c, void *userdata, int pos);

component *textslider_create(const text_settings *tconf, const char *text, const char *help, unsigned int positions,
                             int has_off, textslider_slide_cb cb, void *userdata);
component *textslider_create_bind(const text_settings *tconf, const char *text, const char *help,
                                  unsigned int positions, int has_off, textslider_slide_cb cb, void *userdata,
                                  int *bind);
// by default, the "next" and "previous" item selection noises are panned; call this to disable that effect.
void textslider_disable_panning(component *c);

#endif // TEXTSLIDER_H
