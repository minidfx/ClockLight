#include <pebble.h>

static Window *window;
static TextLayer *time_layer_ptr;
static TextLayer *date_layer_ptr;
static GPath *line_path_ptr;
static Layer *path_layer;

static const GPathInfo LINE_PATH_INFO = {
  .num_points = 2,
  .points = (GPoint []) {{10, 101}, {130, 101}}
};

static void update_time(struct tm *tick_time) {
  static char s_time_buffer[8];
  static char s_date_buffer[12];

  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  strftime(s_date_buffer, sizeof(s_date_buffer), "%B %d", tick_time);

  text_layer_set_text(time_layer_ptr, s_time_buffer);
  text_layer_set_text(date_layer_ptr, s_date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
}

static void draw_line_callback(Layer *layer, GContext *context) {
  line_path_ptr = gpath_create(&LINE_PATH_INFO);

  // Fill the path:
  graphics_context_set_fill_color(context, GColorWhite);
  gpath_draw_filled(context, line_path_ptr);

  // Stroke the path:
  graphics_context_set_stroke_color(context, GColorBlack);
  gpath_draw_outline(context, line_path_ptr);
}

static void display_clock(Layer *window_layer, GRect bounds) {
  // Create the TextLayer with specific bounds
  time_layer_ptr = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(60, 95), bounds.size.w, 42));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(time_layer_ptr, GColorClear);
  text_layer_set_text_color(time_layer_ptr, GColorBlack);
  text_layer_set_font(time_layer_ptr, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(time_layer_ptr, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(time_layer_ptr));
}

static void display_date(Layer *window_layer, GRect bounds) {
  // Create the TextLayer with specific bounds
  date_layer_ptr = text_layer_create(GRect(18, PBL_IF_ROUND_ELSE(60, 70), bounds.size.w, 30));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(date_layer_ptr, GColorClear);
  text_layer_set_text_color(date_layer_ptr, GColorBlack);
  text_layer_set_font(date_layer_ptr, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(date_layer_ptr, GTextAlignmentLeft);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(date_layer_ptr));
}

static void window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  display_clock(window_layer, bounds);
  display_date(window_layer, bounds);

  path_layer = layer_create(bounds);
  layer_set_update_proc(path_layer, draw_line_callback);
  layer_add_child(window_layer, path_layer);

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  update_time(tick_time);
}

static void window_unload(Window *window) {
  text_layer_destroy(time_layer_ptr);
  text_layer_destroy(date_layer_ptr);

  gpath_destroy(line_path_ptr);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
