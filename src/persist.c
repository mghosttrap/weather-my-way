#include "network.h"
#include "debug_layer.h"
#include "battery_layer.h"
#include "config.h"
#include "persist.h"

/**
 * Must happen after layers are created! 
 *
 * It seems odd to have this directly update the layers... Instead it should post a 
 * message or something to force the appropriate view controller to redraw it's views
 */ 
void load_persisted_values(WeatherData *weather_data) 
{
  // Debug
  weather_data->debug = persist_exists(KEY_DEBUG_MODE) ? persist_read_bool(KEY_DEBUG_MODE) : DEFAULT_DEBUG_MODE; 

  if (weather_data->debug) {
    debug_enable_display();
    debug_update_message("Initializing...");
  } else {
    debug_disable_display();
  }

  // Battery
  weather_data->battery = persist_exists(KEY_DISPLAY_BATTERY) ? persist_read_bool(KEY_DISPLAY_BATTERY) : DEFAULT_DISPLAY_BATTERY; 

  if (weather_data->battery) {
    battery_enable_display();
  } else {
    battery_disable_display();
  }
  
  // Weather Service
  if (persist_exists(KEY_WEATHER_SERVICE)) {
    persist_read_string(KEY_WEATHER_SERVICE, weather_data->service, sizeof(weather_data->service));
  } else {
    strcpy(weather_data->service, DEFAULT_WEATHER_SERVICE);
  }

  // Weather Scale
  if (persist_exists(KEY_WEATHER_SCALE)) {
    persist_read_string(KEY_WEATHER_SCALE, weather_data->scale, sizeof(weather_data->scale));
  } else {
    strcpy(weather_data->scale, DEFAULT_WEATHER_SCALE);
  }

  APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistLoad:  d:%d b:%d s:%s u:%s", 
      weather_data->debug, weather_data->battery, weather_data->service, weather_data->scale);
}

/**
 * Persist weather data and app control values
 *
 * TODO: Add some weather data to this. It would enable the watchface to quickly restore 
 * the last known state when the user bounces from this face to another and back.
 */
void store_persisted_values(WeatherData *weather_data) 
{
  persist_write_bool(KEY_DEBUG_MODE, weather_data->debug);
  persist_write_bool(KEY_DISPLAY_BATTERY, weather_data->battery);
  persist_write_string(KEY_WEATHER_SERVICE, weather_data->service);
  persist_write_string(KEY_WEATHER_SCALE, weather_data->scale);


  APP_LOG(APP_LOG_LEVEL_DEBUG, "PersistStore:  d:%d b:%d s:%s u:%s", 
      weather_data->debug, weather_data->battery, weather_data->service, weather_data->scale);
}

/**
 * Load persisted weather data values. The app can then decide if it is current enough 
 * to defer a weather data request.
 *
 * \return true on success, false otherwise
 */
bool load_weather_values( WeatherData* weather_data )
{
    return false;
}

/**
 * Store weather data values to allow the app to quickly restore
 *
 * \return true on success, false otherwise
 */
bool store_weather_values( WeatherData * weather_data )
{
    // Store off the current time,
    // current conditions,
    // hourly forecast
    return false;
}