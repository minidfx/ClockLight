/**
* @Author: Burgy Benjamin <minidfx>
* @Date:   2016-02-16T19:20:15+01:00
* @Email:  benjamin.burgy@gmail.com
* @Last modified by:   minidfx
* @Last modified time: 2016-03-22T22:03:19+01:00
*/

#include <pebble.h>
#include "layers.h"

static Window *window;

static void window_load(Window *window)
{
    init_window_layer(window);

    draw_line();
    draw_battery_line();
    draw_time();
    draw_date();
    draw_week_day();

    // Register services
    tick_timer_service_subscribe(MINUTE_UNIT, handle_minute);
    battery_state_service_subscribe(handle_battery);

    // Get a tm structure
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    update_time(tick_time);
    handle_battery(battery_state_service_peek());
}

static void window_unload(Window *window)
{
    destroy_application_layers();

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
