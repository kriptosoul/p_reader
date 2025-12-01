#include <pebble.h>
#include "ui.h"
#include "reader.h"
#include "settings.h"

static Window *s_window;
static ScrollLayer *s_scroll_layer;
static TextLayer *s_text_layer;
static Layer *s_window_layer;

static GFont s_font;

static void apply_theme(void) {
  if(settings_get_theme() == THEME_DARK) {
    window_set_background_color(s_window, GColorBlack);
    text_layer_set_text_color(s_text_layer, GColorWhite);
  } else {
    window_set_background_color(s_window, GColorWhite);
    text_layer_set_text_color(s_text_layer, GColorBlack);
  }
}

static GFont font_for_size(FontSize f) {
  switch(f) {
    case FONT_SMALL: return fonts_get_system_font(FONT_KEY_GOTHIC_14);
    case FONT_MEDIUM: return fonts_get_system_font(FONT_KEY_GOTHIC_18);
    case FONT_LARGE: return fonts_get_system_font(FONT_KEY_GOTHIC_24);
    default: return fonts_get_system_font(FONT_KEY_GOTHIC_18);
  }
}

static void draw_text_layer_size(void) {
  // Update font & layer layout
  s_font = font_for_size(settings_get_font_size());
  text_layer_set_font(s_text_layer, s_font);
  // adjust content size to text size
  GSize max = layer_get_bounds(window_get_root_layer(s_window)).size;
  GRect content_bounds = GRect(0, 0, max.w, 4000); // large height; ScrollLayer will adjust
  layer_set_frame(text_layer_get_layer(s_text_layer), content_bounds);
  scroll_layer_set_content_size(s_scroll_layer, GSize(content_bounds.size.w, content_bounds.size.h));
}

static void window_load(Window *window) {
  s_window = window;
  s_window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(s_window_layer);

  s_scroll_layer = scroll_layer_create(bounds);
  s_text_layer = text_layer_create(GRect(0,0,bounds.size.w, bounds.size.h));
  text_layer_set_overflow_mode(s_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_text(s_text_layer, reader_get_text());
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentLeft);

  scroll_layer_add_child(s_scroll_layer, text_layer_get_layer(s_text_layer));
  layer_add_child(s_window_layer, scroll_layer_get_layer(s_scroll_layer));

  apply_theme();
  draw_text_layer_size();

  // restore saved offset
  int32_t offset = settings_load_position();
  scroll_layer_set_content_offset(s_scroll_layer, GPoint(0, offset), GCornerNone);
}

static void window_unload(Window *window) {
  // save offset
  GPoint pt = scroll_layer_get_content_offset(s_scroll_layer);
  settings_save_position(pt.y);

  text_layer_destroy(s_text_layer);
  scroll_layer_destroy(s_scroll_layer);
}

// Buttons: Up/Down scroll by page, Select opens quick settings menu
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  GRect bounds = layer_get_bounds(window_get_root_layer(s_window));
  GPoint cur = scroll_layer_get_content_offset(s_scroll_layer);
  int32_t newy = cur.y - (bounds.size.h - 20);
  if(newy < 0) newy = 0;
  scroll_layer_set_content_offset(s_scroll_layer, GPoint(0, newy), true);
}
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  GRect bounds = layer_get_bounds(window_get_root_layer(s_window));
  GPoint cur = scroll_layer_get_content_offset(s_scroll_layer);
  int32_t newy = cur.y + (bounds.size.h - 20);
  scroll_layer_set_content_offset(s_scroll_layer, GPoint(0, newy), true);
}

// Simple settings menu using single click of Select cycles font, long select toggles theme
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  FontSize f = settings_get_font_size();
  f = (f + 1) % 3;
  settings_set_font_size(f);
  draw_text_layer_size();
}

static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  Theme t = settings_get_theme();
  t = (t == THEME_LIGHT) ? THEME_DARK : THEME_LIGHT;
  settings_set_theme(t);
  apply_theme();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, NULL);
}

void ui_init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){ .load = window_load, .unload = window_unload });
  window_set_click_config_provider(s_window, click_config_provider);
  window_stack_push(s_window, true);
}

void ui_deinit(void) {
  window_destroy(s_window);
}
