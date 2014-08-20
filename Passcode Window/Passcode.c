/*
    Developed by: Patrick Balestra
    Twitter: @BalestraPatrick
*/

#include <pebble.h>
#include "Home.h"

static Window *window;

static TextLayer *number_1;
static TextLayer *number_2;
static TextLayer *number_3;
static TextLayer *number_4;
static TextLayer *number_5;
static TextLayer *number_6;
static TextLayer *number_7;
static TextLayer *number_8;
static TextLayer *number_9;
static TextLayer *number_0;
static TextLayer *status_text_layer;

static BitmapLayer *first_layer;
static BitmapLayer *second_layer;
static BitmapLayer *third_layer;
static BitmapLayer *fourth_layer;
static BitmapLayer *line_layer;

static GBitmap *circle_icon;
static GBitmap *circle_filled_icon;
static GBitmap *line;

static int cursor_position;

static int selected_number;
static char wrote_passcode[5];
static char saved_passcode[5];

static int wrote_passcode_int = 0;

// Functions

static void update_bar() {
    if (selected_number == 0)   {
        text_layer_set_background_color(number_0, GColorBlack);
        text_layer_set_text_color(number_0, GColorWhite);

        text_layer_set_background_color(number_1, GColorWhite);
        text_layer_set_text_color(number_1, GColorBlack);
    } else if (selected_number == 1) {
        text_layer_set_background_color(number_1, GColorBlack);
        text_layer_set_text_color(number_1, GColorWhite);

        text_layer_set_background_color(number_0, GColorWhite);
        text_layer_set_text_color(number_0, GColorBlack);
        text_layer_set_background_color(number_2, GColorWhite);
        text_layer_set_text_color(number_2, GColorBlack);
    } else if (selected_number == 2) {
        text_layer_set_background_color(number_2, GColorBlack);
        text_layer_set_text_color(number_2, GColorWhite);

        text_layer_set_background_color(number_1, GColorWhite);
        text_layer_set_text_color(number_1, GColorBlack);
        text_layer_set_background_color(number_3, GColorWhite);
        text_layer_set_text_color(number_3, GColorBlack);
    } else if (selected_number == 3) {
        text_layer_set_background_color(number_3, GColorBlack);
        text_layer_set_text_color(number_3, GColorWhite);

        text_layer_set_background_color(number_2, GColorWhite);
        text_layer_set_text_color(number_2, GColorBlack);
        text_layer_set_background_color(number_4, GColorWhite);
        text_layer_set_text_color(number_4, GColorBlack);
    } else if (selected_number == 4) {
        text_layer_set_background_color(number_4, GColorBlack);
        text_layer_set_text_color(number_4, GColorWhite);

        text_layer_set_background_color(number_3, GColorWhite);
        text_layer_set_text_color(number_3, GColorBlack);
        text_layer_set_background_color(number_5, GColorWhite);
        text_layer_set_text_color(number_5, GColorBlack);
    } else if (selected_number == 5) {
        text_layer_set_background_color(number_5, GColorBlack);
        text_layer_set_text_color(number_5, GColorWhite);

        text_layer_set_background_color(number_4, GColorWhite);
        text_layer_set_text_color(number_4, GColorBlack);
        text_layer_set_background_color(number_6, GColorWhite);
        text_layer_set_text_color(number_6, GColorBlack);
    } else if (selected_number == 6) {
        text_layer_set_background_color(number_6, GColorBlack);
        text_layer_set_text_color(number_6, GColorWhite);

        text_layer_set_background_color(number_5, GColorWhite);
        text_layer_set_text_color(number_5, GColorBlack);
        text_layer_set_background_color(number_7, GColorWhite);
        text_layer_set_text_color(number_7, GColorBlack);
    } else if (selected_number == 7) {
        text_layer_set_background_color(number_7, GColorBlack);
        text_layer_set_text_color(number_7, GColorWhite);

        text_layer_set_background_color(number_6, GColorWhite);
        text_layer_set_text_color(number_6, GColorBlack);
        text_layer_set_background_color(number_8, GColorWhite);
        text_layer_set_text_color(number_8, GColorBlack);
    } else if (selected_number == 8) {
        text_layer_set_background_color(number_8, GColorBlack);
        text_layer_set_text_color(number_8, GColorWhite);

        text_layer_set_background_color(number_7, GColorWhite);
        text_layer_set_text_color(number_7, GColorBlack);
        text_layer_set_background_color(number_9, GColorWhite);
        text_layer_set_text_color(number_9, GColorBlack);
    } else if (selected_number == 9) {
        text_layer_set_background_color(number_9, GColorBlack);
        text_layer_set_text_color(number_9, GColorWhite);

        text_layer_set_background_color(number_8, GColorWhite);
        text_layer_set_text_color(number_8, GColorBlack);
    }
}

static void successfully_unlocked() {
    unlock_app();
    window_stack_remove(window, false);
}

static void wrong_passcode() {
    cursor_position = 0;
    snprintf(wrote_passcode, 5, "0");
    bitmap_layer_set_bitmap(first_layer, circle_icon);
    bitmap_layer_set_bitmap(second_layer, circle_icon);
    bitmap_layer_set_bitmap(third_layer, circle_icon);
    bitmap_layer_set_bitmap(fourth_layer, circle_icon);

    text_layer_set_text(status_text_layer, "Wrong passcode.\nTry again.");

}

static void check_passcode() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Checking inserted passcode %s with saved passcode %s", wrote_passcode, saved_passcode);
    if (strcmp(wrote_passcode, saved_passcode) == 0) {
        successfully_unlocked();
    } else {
        wrong_passcode();
    }
}

static void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (selected_number >= 0 && selected_number < 9) {
        selected_number++;
        update_bar();
    }
}

static void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (selected_number > 0 && selected_number <= 9) {
        selected_number--;
        update_bar();
    }
    
}

static void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    // Select number
    if (cursor_position == 0) {
        wrote_passcode_int += selected_number*1000;
        cursor_position++;
        bitmap_layer_set_bitmap(first_layer, circle_filled_icon);
    } else if (cursor_position == 1) {
        wrote_passcode_int += selected_number*100;
        cursor_position++;
        bitmap_layer_set_bitmap(second_layer, circle_filled_icon);
    } else if (cursor_position == 2) {
        wrote_passcode_int += selected_number*10;
        cursor_position++;
        bitmap_layer_set_bitmap(third_layer, circle_filled_icon);
    } else if (cursor_position == 3) {
        wrote_passcode_int += selected_number;
        bitmap_layer_set_bitmap(fourth_layer, circle_filled_icon);
        
        if (wrote_passcode_int < 10) {
            snprintf(wrote_passcode, 5, "000%d", wrote_passcode_int);
        } else if (wrote_passcode_int < 100) {
            snprintf(wrote_passcode, 5, "00%d", wrote_passcode_int);
        } else if (wrote_passcode_int < 1000) {
            snprintf(wrote_passcode, 5, "0%d", wrote_passcode_int);
        } else {
            snprintf(wrote_passcode, 5, "%d", wrote_passcode_int);
        }
        
        check_passcode();
    }
}

static void config_provider(Window *window) {

    window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
}

static void window_load(Window *window) {

    persist_read_string(1, saved_passcode, 50);

    number_0 = text_layer_create(GRect(124, 0, 20, 15));
    text_layer_set_background_color(number_0, GColorBlack);
    text_layer_set_text_color(number_0, GColorWhite);
    text_layer_set_font(number_0, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text(number_0, "0");
    text_layer_set_text_alignment(number_0, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(number_0));
    
    number_1 = text_layer_create(GRect(124, 15, 20, 15));
    text_layer_set_background_color(number_1, GColorWhite);
    text_layer_set_text_color(number_1, GColorBlack);
    text_layer_set_font(number_1, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text(number_1, "1");
    text_layer_set_text_alignment(number_1, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(number_1));

    number_2 = text_layer_create(GRect(124, 30, 20, 15));
    text_layer_set_background_color(number_2, GColorWhite);
    text_layer_set_text_color(number_2, GColorBlack);
    text_layer_set_font(number_2, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text(number_2, "2");
    text_layer_set_text_alignment(number_2, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(number_2));

    number_3 = text_layer_create(GRect(124, 45, 20, 15));
    text_layer_set_background_color(number_3, GColorWhite);
    text_layer_set_text_color(number_3, GColorBlack);
    text_layer_set_font(number_3, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text(number_3, "3");
    text_layer_set_text_alignment(number_3, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(number_3));

    number_4 = text_layer_create(GRect(124, 60, 20, 15));
    text_layer_set_background_color(number_4, GColorWhite);
    text_layer_set_text_color(number_4, GColorBlack);
    text_layer_set_font(number_4, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text(number_4, "4");
    text_layer_set_text_alignment(number_4, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(number_4));

    number_5 = text_layer_create(GRect(124, 75, 20, 15));
    text_layer_set_background_color(number_5, GColorWhite);
    text_layer_set_text_color(number_5, GColorBlack);
    text_layer_set_font(number_5, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text(number_5, "5");
    text_layer_set_text_alignment(number_5, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(number_5));

    number_6 = text_layer_create(GRect(124, 90, 20, 15));
    text_layer_set_background_color(number_6, GColorWhite);
    text_layer_set_text_color(number_6, GColorBlack);
    text_layer_set_font(number_6, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text(number_6, "6");
    text_layer_set_text_alignment(number_6, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(number_6));

    number_7 = text_layer_create(GRect(124, 105, 20, 15));
    text_layer_set_background_color(number_7, GColorWhite);
    text_layer_set_text_color(number_7, GColorBlack);
    text_layer_set_font(number_7, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text(number_7, "7");
    text_layer_set_text_alignment(number_7, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(number_7));

    number_8 = text_layer_create(GRect(124, 120, 20, 15));
    text_layer_set_background_color(number_8, GColorWhite);
    text_layer_set_text_color(number_8, GColorBlack);
    text_layer_set_font(number_8, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text(number_8, "8");
    text_layer_set_text_alignment(number_8, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(number_8));

    number_9 = text_layer_create(GRect(124, 135, 20, 17));
    text_layer_set_background_color(number_9, GColorWhite);
    text_layer_set_text_color(number_9, GColorBlack);
    text_layer_set_font(number_9, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text(number_9, "9");
    text_layer_set_text_alignment(number_9, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(number_9));

    status_text_layer = text_layer_create(GRect(0, 80, 124, 40));
    text_layer_set_text_color(status_text_layer, GColorBlack);
    text_layer_set_font(status_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(status_text_layer, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(status_text_layer));

    selected_number = 0;
    cursor_position = 0;
  
    window_set_click_config_provider(window, (ClickConfigProvider) config_provider);

    circle_icon = gbitmap_create_with_resource(RESOURCE_ID_CIRCLE);
    circle_filled_icon = gbitmap_create_with_resource(RESOURCE_ID_CIRCLE_FILLED);
    
    first_layer = bitmap_layer_create(GRect(7, 60, 20, 20));
    bitmap_layer_set_alignment(first_layer, GAlignCenter);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(first_layer));
    
    bitmap_layer_set_bitmap(first_layer, circle_icon);
    
    second_layer = bitmap_layer_create(GRect(37, 60, 20, 20));
    bitmap_layer_set_alignment(second_layer, GAlignCenter);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(second_layer));
    bitmap_layer_set_bitmap(second_layer, circle_icon);
    
    third_layer = bitmap_layer_create(GRect(67, 60, 20, 20));
    bitmap_layer_set_alignment(third_layer, GAlignCenter);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(third_layer));
    bitmap_layer_set_bitmap(third_layer, circle_icon);
    
    fourth_layer = bitmap_layer_create(GRect(97, 60, 20, 20));
    bitmap_layer_set_alignment(first_layer, GAlignCenter);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(fourth_layer));
    bitmap_layer_set_bitmap(fourth_layer, circle_icon);

    line = gbitmap_create_with_resource(RESOURCE_ID_LINE);

    line_layer = bitmap_layer_create(GRect(122, 0, 2, 168));
    bitmap_layer_set_alignment(line_layer, GAlignCenter);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(line_layer));
    bitmap_layer_set_bitmap(line_layer, line);    
    
}

static void window_unload(Window *window) {
    text_layer_destroy(number_1);
    text_layer_destroy(number_2);
    text_layer_destroy(number_3);
    text_layer_destroy(number_4);
    text_layer_destroy(number_5);
    text_layer_destroy(number_6);
    text_layer_destroy(number_7);
    text_layer_destroy(number_8);
    text_layer_destroy(number_9);
    text_layer_destroy(number_0);
    text_layer_destroy(status_text_layer);
    bitmap_layer_destroy(first_layer);
    bitmap_layer_destroy(second_layer);
    bitmap_layer_destroy(third_layer);
    bitmap_layer_destroy(fourth_layer);
    bitmap_layer_destroy(line_layer);
    gbitmap_destroy(circle_icon);
    gbitmap_destroy(circle_filled_icon);
    gbitmap_destroy(line);
    window_destroy(window);
}

void ask_passcode() {
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    const bool animated = true;
    window_stack_push(window, animated);
}
