#include "reader.h"
#include "settings.h"
#include <pebble.h>

#define RESOURCE_ID_BOOK RESOURCE_ID_DATA_BOOK

static char *s_text = NULL;
static size_t s_text_len = 0;

void reader_init(void) {
  // Load the raw resource (book.txt) into memory
  ResHandle rh = resource_get_handle(RESOURCE_ID_BOOK);
  s_text_len = (size_t)resource_size(rh);
  s_text = malloc(s_text_len + 1);
  if(s_text) {
    resource_load(rh, s_text, s_text_len);
    s_text[s_text_len] = '\0';
  } else {
    s_text_len = 0;
  }
}

void reader_deinit(void) {
  if(s_text) {
    free(s_text);
    s_text = NULL;
  }
  s_text_len = 0;
}

const char* reader_get_text(void) {
  return s_text ? s_text : "";
}

size_t reader_get_text_len(void) {
  return s_text_len;
}
