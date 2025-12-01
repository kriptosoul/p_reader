#pragma once
#include <pebble.h>

void reader_init(void);
void reader_deinit(void);
const char* reader_get_text(void);
size_t reader_get_text_len(void);
