/**
* @Author: Burgy Benjamin <minidfx>
* @Date:   2016-02-16T19:19:55+01:00
* @Email:  benjamin.burgy@gmail.com
* @Last modified by:   minidfx
* @Last modified time: 2016-03-22T22:03:27+01:00
*/

#include <pebble.h>
#include "layers.h"

static TextLayer *ptr_time_layer;
static TextLayer *ptr_date_layer;
static TextLayer *ptr_week_day_layer;

static Layer *ptr_line_layer;
static Layer *ptr_battery_layer;
static Layer *ptr_window_layer;

static GRect window_bounds;

static uint8_t charge_percent = 0;
static uint8_t textPaddingLeft = 15;
static uint8_t battery_line_width = 4;

void handle_minute(struct tm *tick_time, TimeUnits units_changed)
{
    update_time(tick_time);
}

void update_time(struct tm *tick_time)
{
    static char s_time_buffer[8];
    static char s_date_buffer[12];
    static char s_week_day_buffer[15];

    strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
    strftime(s_date_buffer, sizeof(s_date_buffer), "%B %d", tick_time);
    strftime(s_week_day_buffer, sizeof(s_week_day_buffer), "%A", tick_time);

    text_layer_set_text(ptr_time_layer, s_time_buffer);
    text_layer_set_text(ptr_date_layer, s_date_buffer);
    text_layer_set_text(ptr_week_day_layer, s_week_day_buffer);
}

void handle_battery(BatteryChargeState charge_state)
{
    update_battery_line(charge_state.charge_percent);
}

void init_window_layer(Window *window)
{
    // Get information about the Window
    ptr_window_layer = window_get_root_layer(window);
    window_bounds = layer_get_bounds(ptr_window_layer);
}

void draw_line_callback(Layer *layer, GContext *context)
{
    GPoint start = GPoint(0, 0);
    GPoint end = GPoint(122, 0);

    graphics_draw_line(context, start, end);
}

void draw_line()
{
    ptr_line_layer = layer_create(GRect(0, 110, 122, 1));
    layer_set_update_proc(ptr_line_layer, draw_line_callback);
    layer_add_child(ptr_window_layer, ptr_line_layer);
}

void draw_battery_line_callback(Layer *layer, GContext *context)
{
    uint8_t lineHeight = charge_percent * window_bounds.size.h / 100;
    GRect rect_bounds = GRect(0, window_bounds.size.h - lineHeight, battery_line_width, window_bounds.size.h);

    // Draw a rectangle
    graphics_draw_rect(context, rect_bounds);

    // Fill rectangle
    graphics_fill_rect(context, rect_bounds, 0, GCornersAll);
}

void draw_battery_line()
{
    ptr_battery_layer = layer_create(GRect(window_bounds.size.w - battery_line_width, 0, battery_line_width, window_bounds.size.h));
    layer_set_update_proc(ptr_battery_layer, draw_battery_line_callback);
    layer_add_child(ptr_window_layer, ptr_battery_layer);
}

void update_battery_line(uint8_t percent)
{
    charge_percent = percent;
    layer_mark_dirty(ptr_battery_layer);
}

void draw_time()
{
    ptr_time_layer = text_layer_create(GRect(textPaddingLeft, PBL_IF_ROUND_ELSE(70, 105), window_bounds.size.w - textPaddingLeft, 42));

    // Improve the layout to be more like a watchface
    text_layer_set_background_color(ptr_time_layer, GColorClear);
    text_layer_set_text_color(ptr_time_layer, GColorBlack);
    text_layer_set_font(ptr_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    text_layer_set_text_alignment(ptr_time_layer, GTextAlignmentLeft);

    // Add it as a child layer to the Window's root layer
    layer_add_child(ptr_window_layer, text_layer_get_layer(ptr_time_layer));
}

void draw_date()
{
    ptr_date_layer = text_layer_create(GRect(textPaddingLeft, PBL_IF_ROUND_ELSE(70, 80), window_bounds.size.w - textPaddingLeft, 30));

    // Improve the layout to be more like a watchface
    text_layer_set_background_color(ptr_date_layer, GColorClear);
    text_layer_set_text_color(ptr_date_layer, GColorBlack);
    text_layer_set_font(ptr_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_text_alignment(ptr_date_layer, GTextAlignmentLeft);

    // Add it as a child layer to the Window's root layer
    layer_add_child(ptr_window_layer, text_layer_get_layer(ptr_date_layer));
}

void draw_week_day()
{
    ptr_week_day_layer = text_layer_create(GRect(textPaddingLeft, PBL_IF_ROUND_ELSE(50, 60), window_bounds.size.w - textPaddingLeft, 30));

    // Improve the layout to be more like a watchface
    text_layer_set_background_color(ptr_week_day_layer, GColorClear);
    text_layer_set_text_color(ptr_week_day_layer, GColorBlack);
    text_layer_set_font(ptr_week_day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_text_alignment(ptr_week_day_layer, GTextAlignmentLeft);

    // Add it as a child layer to the Window's root layer
    layer_add_child(ptr_window_layer, text_layer_get_layer(ptr_week_day_layer));
}

void destroy_application_layers()
{
    text_layer_destroy(ptr_time_layer);
    text_layer_destroy(ptr_date_layer);
    text_layer_destroy(ptr_week_day_layer);

    layer_destroy(ptr_line_layer);
    layer_destroy(ptr_battery_layer);
    layer_destroy(ptr_window_layer);
}
