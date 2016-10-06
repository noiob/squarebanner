#pragma once
#include <pebble.h>

#define SETTINGS_KEY 1

// A structure containing our settings
typedef struct ClaySettings {
  GColor BackgroundColor;
  GColor ForegroundColor;
  bool DisconnectIcon;
  bool DisconnectVibrate;
} __attribute__((__packed__)) ClaySettings; 

static void prv_default_settings();
static void prv_load_settings();
static void prv_save_settings();
void strupper(char *sptr);
static void update_time(GRect final_unobstructed_screen_area);
static void tick_handler(struct tm *tick_time, TimeUnits units_changed);
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context);
static void prv_unobstructed_will_change(GRect final_unobstructed_screen_area,void *context);
static void bluetooth_callback(bool connected);
static void main_window_load(Window *window);
static void main_window_unload(Window *window);
static void init();
static void deinit();