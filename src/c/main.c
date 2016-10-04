#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_toptime_layer;
static TextLayer *s_bottime_layer;
static TextLayer *s_date_layer;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
Layer *window_layer;
static BitmapLayer *s_background_layer, *s_bt_icon_layer;
static GBitmap *s_background_bitmap, *s_bt_icon_bitmap;

void strupper(char *sptr) {
  while (*sptr) {
    if ((*sptr >= 'a' ) && (*sptr <= 'z')) *sptr -= 32;
    sptr++;
  }
}

static void update_time(GRect final_unobstructed_screen_area) {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  // Check the available window size
  GRect bounds = layer_get_bounds(window_layer);
  
  // Write the current hours and minutes into a buffer
  static char s_buffer_top[8];
  static char s_buffer_bot[8];
  if (final_unobstructed_screen_area.size.h < bounds.size.h) { // QuickView active
    strftime(s_buffer_top, sizeof(s_buffer_top), clock_is_24h_style() ?
             "%H:%M" : "%I:%M", tick_time);
  }
  else { // normal view
    strftime(s_buffer_top, sizeof(s_buffer_top), clock_is_24h_style() ?
             "%H" : "%I", tick_time);
    strftime(s_buffer_bot, sizeof(s_buffer_bot), "%M", tick_time);
  }
  static char s_buffer_date[20];
  strftime(s_buffer_date, sizeof(s_buffer_date), "%A, %d", tick_time);
  
  strupper(s_buffer_date);
  
  // Display this time on the TextLayer
  text_layer_set_text(s_toptime_layer, s_buffer_top);
  text_layer_set_text(s_bottime_layer, s_buffer_bot);
  text_layer_set_text(s_date_layer, s_buffer_date);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  GRect bounds = layer_get_unobstructed_bounds(window_layer);
  update_time(bounds);
}

static void prv_unobstructed_will_change(GRect final_unobstructed_screen_area,void *context) {
  update_time(final_unobstructed_screen_area);
}

static void bluetooth_callback(bool connected) {
  // Show icon if disconnected
  layer_set_hidden(bitmap_layer_get_layer(s_bt_icon_layer), connected);

  if(!connected) {
    // Issue a vibrating alert
    vibes_double_pulse();
  }
}

static void main_window_load(Window *window) {
  // Get information about the Window
  window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create GBitmap
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);

  // Create BitmapLayer to display the GBitmap
  s_background_layer = bitmap_layer_create(bounds);

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  
  // Create the TextLayer with specific bounds
  s_toptime_layer = text_layer_create(
      GRect(0, 18, bounds.size.w , 50));
  s_bottime_layer = text_layer_create(
      GRect(0, 106, bounds.size.w, 50));
  s_date_layer = text_layer_create(
      GRect(0, 74, bounds.size.w, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_toptime_layer, GColorClear);
  text_layer_set_text_color(s_toptime_layer, GColorWhite);
  text_layer_set_text(s_toptime_layer, "12");
  text_layer_set_background_color(s_bottime_layer, GColorClear);
  text_layer_set_text_color(s_bottime_layer, GColorWhite);
  text_layer_set_text(s_bottime_layer, "34");
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_text(s_date_layer, "WEDNESDAY, 42");

  // Apply to TextLayer
  text_layer_set_font(s_toptime_layer, fonts_get_system_font(FONT_KEY_LECO_38_BOLD_NUMBERS));  text_layer_set_text_alignment(s_toptime_layer, GTextAlignmentCenter);
  text_layer_set_font(s_bottime_layer, fonts_get_system_font(FONT_KEY_LECO_38_BOLD_NUMBERS));  text_layer_set_text_alignment(s_bottime_layer, GTextAlignmentCenter);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_toptime_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_bottime_layer));
  
  // Create the Bluetooth icon GBitmap
  s_bt_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_ICON);

  // Create the BitmapLayer to display the GBitmap
  s_bt_icon_layer = bitmap_layer_create(GRect(0, 0, 30, 30));
  bitmap_layer_set_bitmap(s_bt_icon_layer, s_bt_icon_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bt_icon_layer));
  // Show the correct state of the BT connection from the start
  bluetooth_callback(connection_service_peek_pebble_app_connection());
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_toptime_layer);
  text_layer_destroy(s_bottime_layer);
  text_layer_destroy(s_date_layer);
  
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  
  // destroy bluetooth icon
  gbitmap_destroy(s_bt_icon_bitmap);
  bitmap_layer_destroy(s_bt_icon_layer);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_set_background_color(s_main_window, GColorBlack);

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_unobstructed_bounds(window_layer);
  update_time(bounds);

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Subscribe for Quickview changes
  UnobstructedAreaHandlers handler = {
    .will_change = prv_unobstructed_will_change
  };
  unobstructed_area_service_subscribe(handler, NULL);
  
  // Register for Bluetooth connection updates
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}