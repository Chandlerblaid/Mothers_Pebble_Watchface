#include <pebble.h>

// #define PURPLE #551A8B

static Window *window;
static TextLayer *time_layer, *text_layer1, *to_layer, *from_layer, *date_layer;
static BitmapLayer *background_layer;
static GBitmap *background_bitmap;
static GFont time_font, date_font;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
//   text_layer_set_text(text_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
//   text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
//   text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void update_time() {
  // Get a tm struct
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  // Write the current hours and mins into a buffer
  static char buffer[8];
  strftime(buffer, sizeof(buffer), clock_is_24h_style() ?
          "%H:%M" : "%I:%M", tick_time);
  
  // Copy date into buffer from tm structure
  static char date_buffer[16];
  strftime(date_buffer, sizeof(date_buffer), "%a %d %b", tick_time);
  
  // Display this time on the time TextLayer
  text_layer_set_text(time_layer, buffer);
  
  // Show the date on the date TextLayer
  text_layer_set_text(date_layer, date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
 update_time();
}

static void window_load(Window *window) {
  // Get info about the window
  Layer *window_layer = window_get_root_layer(window);
  
  GRect bounds = layer_get_bounds(window_layer);
  GRect time_layer_bounds = GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 40);
  GRect text_layer1_bounds = GRect((bounds.size.w+1)/4, PBL_IF_ROUND_ELSE(18, 12), bounds.size.w/2, 30);
  GRect to_layer_bounds = GRect((bounds.size.w+1)/4, (21*(bounds.size.h+1))/32, bounds.size.w/2, 15);
  GRect from_layer_bounds = GRect((bounds.size.w+1)/4, ((3*(bounds.size.h+1))/4), bounds.size.w/2, 15);
  GRect date_layer_bounds = GRect(0, PBL_IF_ROUND_ELSE(93, 87), bounds.size.w, 20);
  
  // Create TimeLayer with bounds
  time_layer = text_layer_create(time_layer_bounds);
  
  // Create DateLayer with bounds
  date_layer = text_layer_create(date_layer_bounds);
  
  // Create TextLayer1 with bounds
  text_layer1 = text_layer_create(text_layer1_bounds);
  
  // Create To and from layer with bounds
  to_layer = text_layer_create(to_layer_bounds);
  from_layer = text_layer_create(from_layer_bounds);
  
  // Create Time and Date Fonts
  time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PERFECT_DOS_42));
  date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PERFECT_DOS_18));
  
  // Create Bitmap
  background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_PURPLE_HEART);
  GRect bb = gbitmap_get_bounds(background_bitmap);
  GRect purple_heart_bounds = GRect((((bounds.size.w+1)/2)+5) - ((bb.size.w+1)/2), PBL_IF_ROUND_ELSE(-6, 0), bounds.size.w+35, PBL_IF_ROUND_ELSE(bounds.size.h+6, bounds.size.h));
  
  // Create Bitmap Layer to hold the Bitmap
  background_layer = bitmap_layer_create(purple_heart_bounds);
  
  // Customize time layout
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorBlack);
  text_layer_set_text(time_layer, "00:00");
  text_layer_set_font(time_layer, time_font);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  
  // Customize date layout
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorBlack);
  text_layer_set_text(date_layer, "Nov 6, 1994");
  text_layer_set_font(date_layer, date_font);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  
  // Customize to layer layout
  text_layer_set_background_color(to_layer, GColorClear);
  text_layer_set_text_color(to_layer, GColorWhite);
  text_layer_set_text(to_layer, "To: Mother");
  text_layer_set_text_alignment(to_layer, GTextAlignmentCenter);
  
  // Customize from layer layout
  text_layer_set_background_color(from_layer, GColorClear);
  text_layer_set_text_color(from_layer, GColorWhite);
  text_layer_set_text(from_layer, "From: Blaid");
  text_layer_set_text_alignment(from_layer, GTextAlignmentCenter);  
  
  // Customize text layout 1
  text_layer_set_background_color(text_layer1, GColorClear);
  text_layer_set_text_color(text_layer1, GColorWhite);
  text_layer_set_text(text_layer1, "Happy \nMother's Day!");
  text_layer_set_text_alignment(text_layer1, GTextAlignmentCenter);
    
  // Throw the bitmap on the bitmap layer and it as a child of the main window
  bitmap_layer_set_bitmap(background_layer, background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(background_layer));
  
  // Add to and from layers as a child of the main window layer
  layer_add_child(window_layer, text_layer_get_layer(to_layer));
  layer_add_child(window_layer, text_layer_get_layer(from_layer));
  
  // Add text layer 1 as a child of the main window layer
  layer_add_child(window_layer, text_layer_get_layer(text_layer1));
  
  // Add date layer as a child to the window's root layer
  layer_add_child(window_layer, text_layer_get_layer(date_layer));
  
  // Add time layer as a child to the window's root layer
  layer_add_child(window_layer, text_layer_get_layer(time_layer));
}

static void window_unload(Window *window) {
  // Destroy the time layer
  text_layer_destroy(time_layer);
  
  // Destroy the date layer
  text_layer_destroy(date_layer);
  
  // Destroy text layer 1
  text_layer_destroy(text_layer1);
  
  // Destroy to and from layers
  text_layer_destroy(to_layer);
  text_layer_destroy(from_layer);
  
  // Destroy/unload the custom font
  fonts_unload_custom_font(time_font);
  fonts_unload_custom_font(date_font);
  
  // Destroy the bitmap
  gbitmap_destroy(background_bitmap);
  
  // Destroy the bitmap layer
  bitmap_layer_destroy(background_layer);
}

static void init(void) {
  // Create main Window element and assign it to the static pointer
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  
  // Set handlers to manage the elements inside the window
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
  // Set window background color
  window_set_background_color(window, GColorImperialPurple);

  // Show the window with animation
  const bool animated = true;
  window_stack_push(window, animated);
  
  // Make sure the time is displayed from the start
  update_time();
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit(void) {
  // Destroy the window
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
