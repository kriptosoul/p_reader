#pragma once
#include <pebble.h>

typedef enum {
  THEME_LIGHT = 0,
  THEME_DARK = 1
} Theme;

typedef enum {
  FONT_SMALL = 0,
  FONT_MEDIUM = 1,
  FONT_LARGE = 2
} FontSize;

void settings_init(void);
void settings_deinit(void);
Theme settings_get_theme(void);
FontSize settings_get_font_size(void);
void settings_set_theme(Theme t);
void settings_set_font_size(FontSize f);
void settings_save_position(int32_t y_offset);
int32_t settings_load_position(void);
