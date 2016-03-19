/**
* @Author: Burgy Benjamin <minidfx>
* @Date:   2016-02-16T19:20:15+01:00
* @Email:  benjamin.burgy@gmail.com
* @Last modified by:   minidfx
* @Last modified time: 2016-03-19T13:10:45+01:00
*/

#include <pebble.h>
#include "application_layers.c"
#include "calendar.c"

static Window *window;

static void update_time(struct tm *tick_time)
{
  static char s_time_buffer[8];
  static char s_date_buffer[12];

  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  strftime(s_date_buffer, sizeof(s_date_buffer), "%B %d", tick_time);

  text_layer_set_text(time_layer_ptr, s_time_buffer);
  text_layer_set_text(date_layer_ptr, s_date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  update_time(tick_time);
}

static void window_load(Window *window)
{
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  display_clock(window_layer, bounds);
  display_date(window_layer, bounds);
  display_next_pin(window_layer, bounds);

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

static void window_unload(Window *window)
{
  destroy_application_layers();
}

static void init(void)
{
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers)
  {
    .load = window_load,
    .unload = window_unload,
  });

  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void)
{
  window_destroy(window);
}

int main(void)
{
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();

  deinit();
}
