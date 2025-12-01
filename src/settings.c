#include "settings.h"
#include <pebble.h>

#define PERSIST_KEY_THEME 1
#define PERSIST_KEY_FONT 2
#define PERSIST_KEY_OFFSET 3

static Theme s_theme = THEME_LIGHT;
static FontSize s_font = FONT_MEDIUM;

void settings_init(void) {
  if(persist_exists(PERSIST_KEY_THEME)) {
    s_theme = persist_read_int(PERSIST_KEY_THEME);
  } else {
    s_theme = THEME_LIGHT;
  }
  if(persist_exists(PERSIST_KEY_FONT)) {
    s_font = persist_read_int(PERSIST_KEY_FONT);
  } else {
    s_font = FONT_MEDIUM;
  }
}

void settings_deinit(void) {
  // nothing for now
}

Theme settings_get_theme(void) {
  return s_theme;
}

FontSize settings_get_font_size(void) {
  return s_font;
}

void settings_set_theme(Theme t) {
  s_theme = t;
  persist_write_int(PERSIST_KEY_THEME, (int)t);
}

void settings_set_font_size(FontSize f) {
  s_font = f;
  persist_write_int(PERSIST_KEY_FONT, (int)f);
}

void settings_save_position(int32_t y_offset) {
  persist_write_int(PERSIST_KEY_OFFSET, (int)y_offset);
}

int32_t settings_load_position(void) {
  if(persist_exists(PERSIST_KEY_OFFSET)) {
    return persist_read_int(PERSIST_KEY_OFFSET);
  }
  return 0;
}
