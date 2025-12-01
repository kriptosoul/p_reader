#include <pebble.h>
#include "reader.h"
#include "ui.h"
#include "settings.h"

static Window *s_main_window;

static void init(void) {
  settings_init();
  reader_init();
  ui_init();
}

static void deinit(void) {
  ui_deinit();
  reader_deinit();
  settings_deinit();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
