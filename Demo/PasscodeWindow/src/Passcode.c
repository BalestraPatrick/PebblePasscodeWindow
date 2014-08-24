/*
 Developed by: Patrick Balestra
 Twitter: @BalestraPatrick

 Enhancements by Edwin Finch
 www.edwinfinch.com
 */

#include <pebble.h>
#include "Passcode.h"
	
#define CIRCLE_HEIGHT 55
#define CIRCLE_RADIUS 10
	
Window *window;

TextLayer *number_1, *number_2, *number_3, *number_4, *number_5, *number_6, *number_7, *number_8, *number_9;
TextLayer *number_0, *status_text_layer;

BitmapLayer *line_layer;

Layer *circle_layer;

InverterLayer *highlighter, *theme;

GBitmap *circle_icon, *circle_filled_icon, *line;

int cursor_position;

int selected_number;
char wrote_passcode[5];
char saved_passcode[5];

int wrote_passcode_int = 0;
int circles_filled = 0;
bool theme_public = 0;

// Functions

TextLayer* number_layer_init(GRect location, char *index) {
    TextLayer *layer = text_layer_create(location);
    text_layer_set_background_color(layer, GColorWhite);
    text_layer_set_text_color(layer, GColorBlack);
    text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text(layer, index);
    text_layer_set_text_alignment(layer, GTextAlignmentCenter);
    return layer;
}

void update_bar() {
    inverter_layer_destroy(highlighter);
    int height = 15 * selected_number;
    highlighter = inverter_layer_create(GRect(124, height, 20, 15));
    layer_add_child(window_get_root_layer(window), inverter_layer_get_layer(highlighter));
}

void successfully_unlocked() {
    window_stack_pop(true);
}

void wrong_passcode() {
    cursor_position = 0;
    snprintf(wrote_passcode, 5, "0");
    
	circles_filled = -1;
	layer_mark_dirty(circle_layer);
	
    text_layer_set_text(status_text_layer, "Wrong passcode.\nTry again.");
}

void check_passcode() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Checking inserted passcode %s with saved passcode %s", wrote_passcode, saved_passcode);
    if (strcmp(wrote_passcode, saved_passcode) == 0) {
        successfully_unlocked();
    } else {
        wrong_passcode();
    }
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (selected_number >= 0 && selected_number < 9) {
        selected_number++;
        update_bar();
    }
}

void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (selected_number > 0 && selected_number <= 9) {
        selected_number--;
        update_bar();
    }
    
}

void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    // Select number
    switch(cursor_position){
        case 0:
            wrote_passcode_int += selected_number*1000;
            cursor_position++;
            break;
        case 1:
            wrote_passcode_int += selected_number*100;
            cursor_position++;
            break;
        case 2:
            wrote_passcode_int += selected_number*10;
            cursor_position++;
            break;
        case 3:
            wrote_passcode_int += selected_number;
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
            break;
    }
	circles_filled++;
	layer_mark_dirty(circle_layer);
}

void config_provider(Window *window) {
    window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
}

void circle_proc(Layer *layer, GContext *ctx){
	graphics_context_set_stroke_color(ctx, GColorBlack);
	graphics_context_set_fill_color(ctx, GColorBlack);
	int i;
	for(i = 0; i < 4; i++){
		int fix = 17+(30*i);
		graphics_draw_circle(ctx, GPoint(fix, CIRCLE_HEIGHT), CIRCLE_RADIUS);
	}
	int k;
	for(k = 0; k < circles_filled; k++){
		int fix = 17+(30*k);
		graphics_fill_circle(ctx, GPoint(fix, CIRCLE_HEIGHT), CIRCLE_RADIUS);
	}
}

void window_load(Window *window) {
    persist_read_string(1, saved_passcode, 50);
    
    Layer *window_layer = window_get_root_layer(window);
    
    number_0 = number_layer_init(GRect(124, 0, 20, 15), "0");
    layer_add_child(window_layer, text_layer_get_layer(number_0));
    
    number_1 = number_layer_init(GRect(124, 15, 20, 15), "1");
    layer_add_child(window_layer, text_layer_get_layer(number_1));
    
    number_2 = number_layer_init(GRect(124, 30, 20, 15), "2");
    layer_add_child(window_layer, text_layer_get_layer(number_2));
    
    number_3 = number_layer_init(GRect(124, 45, 20, 15), "3");
    layer_add_child(window_layer, text_layer_get_layer(number_3));
    
    number_4 = number_layer_init(GRect(124, 60, 20, 15), "4");
    layer_add_child(window_layer, text_layer_get_layer(number_4));
    
    number_5 = number_layer_init(GRect(124, 75, 20, 15), "5");
    layer_add_child(window_layer, text_layer_get_layer(number_5));
    
    number_6 = number_layer_init(GRect(124, 90, 20, 15), "6");
    layer_add_child(window_layer, text_layer_get_layer(number_6));
    
    number_7 = number_layer_init(GRect(124, 105, 20, 15), "7");
    layer_add_child(window_layer, text_layer_get_layer(number_7));
    
    number_8 = number_layer_init(GRect(124, 120, 20, 15), "8");
    layer_add_child(window_layer, text_layer_get_layer(number_8));
    
    number_9 = number_layer_init(GRect(124, 135, 20, 17), "9");
    layer_add_child(window_layer, text_layer_get_layer(number_9));
    
    status_text_layer = text_layer_create(GRect(0, 90, 124, 40));
    text_layer_set_text_color(status_text_layer, GColorBlack);
    text_layer_set_font(status_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(status_text_layer, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(status_text_layer));
    
    selected_number = 0;
    cursor_position = 0;
	
	circle_layer = layer_create(GRect(0, 20, 144, 100));
	layer_set_update_proc(circle_layer, circle_proc);
	layer_add_child(window_layer, circle_layer);
    
    update_bar();
    
    window_set_click_config_provider(window, (ClickConfigProvider) config_provider); 
    
    line = gbitmap_create_with_resource(RESOURCE_ID_LINE);
    
    line_layer = bitmap_layer_create(GRect(122, 0, 2, 168));
    bitmap_layer_set_alignment(line_layer, GAlignCenter);
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(line_layer));
    
    bitmap_layer_set_bitmap(line_layer, line);
	
	theme = inverter_layer_create(GRect(0, 0, 144, 168));
	layer_add_child(window_layer, inverter_layer_get_layer(theme));
	layer_set_hidden(inverter_layer_get_layer(theme), theme_public);
}

void window_unload(Window *window) {
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
    bitmap_layer_destroy(line_layer);
	layer_destroy(circle_layer);
    gbitmap_destroy(line);
    inverter_layer_destroy(highlighter);
}

void passcode_settings(bool themeIsLight, bool use_test_num){
	theme_public = themeIsLight;
	if(use_test_num){
		persist_write_string(1, "1234");
	}
}

void ask_passcode() {
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    window_stack_push(window, true);
}

void deinit(){
	window_destroy(window);
}

int main(){
	passcode_settings(0, 1);
	ask_passcode();
	app_event_loop();
	deinit();
}