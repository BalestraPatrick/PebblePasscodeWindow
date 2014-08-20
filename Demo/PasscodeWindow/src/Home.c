#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

static void window_load(Window *window) {
    text_layer = text_layer_create(GRect(5, 20, 134, 120));
    text_layer_set_text(text_layer, "You just successfully unlocked your awesome app.");
    text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
    text_layer_destroy(text_layer);
    window_destroy(window);
}

void unlock_app() {
	window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload
    });
    const bool animated = true;
    window_stack_push(window, animated);
}