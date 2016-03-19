/**
* @Author: Burgy Benjamin <minidfx>
* @Date:   2016-02-16T19:20:15+01:00
* @Email:  benjamin.burgy@gmail.com
* @Last modified by:   minidfx
* @Last modified time: 2016-03-19T14:48:53+01:00
*/

#include <pebble.h>
#include "application_layers.c"

static Window *window;

static void update_time(struct tm *tick_time)
{
  static char s_time_buffer[8];
  static char s_date_buffer[12];
  static char s_week_day_buffer[15];

  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  strftime(s_date_buffer, sizeof(s_date_buffer), "%B %d", tick_time);
  strftime(s_week_day_buffer, sizeof(s_week_day_buffer), "%A", tick_time);

  text_layer_set_text(time_layer_ptr, s_time_buffer);
  text_layer_set_text(date_layer_ptr, s_date_buffer);
  text_layer_set_text(week_day_layer_ptr, s_week_day_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  update_time(tick_time);
}

static void handle_battery(BatteryChargeState charge_state)
{
  static char battery_text[] = "...";

  if (charge_state.is_charging)
  {
    snprintf(battery_text, sizeof(battery_text), "...");
  }
  else
  {
    snprintf(battery_text, sizeof(battery_text), "%d%%", charge_state.charge_percent);
  }

  text_layer_set_text(battery_layer_ptr, battery_text);
}

static void window_load(Window *window)
{
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  display_clock(window_layer, bounds);
  display_date(window_layer, bounds);
  display_week_day(window_layer, bounds);
  display_battery(window_layer, bounds);

  path_layer = layer_create(bounds);
  layer_set_update_proc(path_layer, draw_line_callback);
  layer_add_child(window_layer, path_layer);

  // Register services
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_state_service_subscribe(handle_battery);

  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  update_time(tick_time);
  handle_battery(battery_state_service_peek());
}

static void window_unload(Window *window)
{
  text_layer_destroy(time_layer_ptr);
  text_layer_destroy(next_pin_ptr);
  text_layer_destroy(battery_layer_ptr);
  text_layer_destroy(week_day_layer_ptr);
  layer_destroy(path_layer);
  gpath_destroy(line_path_ptr);

  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
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
