#include <pebble.h>
#include "Home.h"
#include "Passcode.h"

static void init(void) {
    
    // For debug purpose we immediately save a passcode. You would do that only we the user wants to have a passcode
    persist_write_string(1, "1234");
    
    if (persist_exists(1)) {
        ask_passcode();
    } else {
        unlock_app();
    }
    
    // When the user doesn't want a passcode anymore, delete its key
    //persist_delete(1);
}

static void deinit(void) {
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}