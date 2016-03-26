/**
* @Author: Burgy Benjamin <minidfx>
* @Date:   2016-02-16T19:20:15+01:00
* @Email:  benjamin.burgy@gmail.com
* @Last modified by:   minidfx
* @Last modified time: 2016-03-25T10:01:17+01:00
*/

#include <pebble.h>
#include "layers.h"

static Window *window;

static void window_load(Window *window)
{
    init_window_layer(window);

    // Set the watch language
    const char *language = i18n_get_system_locale();
    setlocale(LC_ALL, language);

    draw_line();
    draw_battery_line();
    draw_time();
    draw_date();
    draw_week_day();
    draw_bluetooth();

    // Register services
    tick_timer_service_subscribe(MINUTE_UNIT, handle_minute);
    battery_state_service_subscribe(handle_battery);
    connection_service_subscribe((ConnectionHandlers)
    {
        .pebble_app_connection_handler = handle_app_connection_handler,
        .pebblekit_connection_handler = handle_kit_connection_handler
    });

    // Get a tm structure
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    update_datetime(tick_time);
    handle_battery(battery_state_service_peek());
    handle_app_connection_handler(connection_service_peek_pebble_app_connection());
    handle_kit_connection_handler(connection_service_peek_pebblekit_connection());
}

static void window_unload(Window *window)
{
    tick_timer_service_unsubscribe();
    battery_state_service_unsubscribe();

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

    load_resources();

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
