#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static BitmapLayer *image_layer;
static GBitmap *image;

static void update_time() {
   // Get a tm structure
   time_t temp = time(NULL);
   struct tm *tick_time = localtime(&temp);
   
   // Create a long-lived buffer
   static char buffer[] ="00:00";
   
   // Write the current hours and minutes into the buffer
   if (clock_is_24h_style()==true) {
      // Use 24 hour format
      strftime(buffer, sizeof("00:00"), "%H:%M",tick_time);
   }
   else {
      // Use 12 hour format
      strftime(buffer, sizeof("00:00"), "%I:%M",tick_time);
   }
   
   // Display this time on the TextLayer
   text_layer_set_text(s_time_layer,  buffer);
}

static void main_window_load(Window *window){
   // Creat time TextLayer
   s_time_layer = text_layer_create(GRect (0,115,144,110));
   text_layer_set_background_color(s_time_layer,GColorClear);
   text_layer_set_text_color(s_time_layer, GColorBlack);
   text_layer_set_text(s_time_layer, "00:00");
   
  // Improve the layout to be more like a watchface
   text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
   //text_layer_set_text_alignment(s_time_layer,GTextAlignmentCenter);
   text_layer_set_text_alignment(s_time_layer,GTextAlignmentCenter);
  
  // Add it as a child layer to the Window's root layer
   layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
   
   // Make sure the time is displayer from the start
   update_time();
   //------------------- create image layer
   Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
   image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SCAMPI_BIKE);
   // The bitmap layer holds the image for display
   image_layer = bitmap_layer_create(bounds);
   bitmap_layer_set_bitmap(image_layer, image);
   bitmap_layer_set_alignment(image_layer, GAlignTop);
   layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
   
}
  
static void main_window_unload(Window *window){
   // Destroy Textlayer
    text_layer_destroy(s_time_layer);
   // Destory image layer
   gbitmap_destroy(image);
   bitmap_layer_destroy(image_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
   update_time();   
}



static void init() {
     // Create main Window element and assign to pointer
    s_main_window = window_create();
  
    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_main_window, (WindowHandlers) {
         .load = main_window_load,
         .unload = main_window_unload
      });
     
    // Show the Window on the watch, with animated = ture
    window_stack_push(s_main_window, true);
   
   // Register with TickTimeService
   tick_timer_service_subscribe(MINUTE_UNIT,tick_handler);
}

static void deinit(){
   // Destory Window
  window_destroy(s_main_window);
}

int main(void) {
   init();
   app_event_loop();
   deinit();
}
