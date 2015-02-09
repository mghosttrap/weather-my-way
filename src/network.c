#include <pebble.h>
#include "network.h"
#include "battery_layer.h"
#include "weather_layer.h"
#include "debug_layer.h"
#include "main.h"
#include "persist.h"

const  int MAX_RETRY = 2;
static int retry_count = 0;

/**
 * Process the event which contains the current weather data
 */
bool processCurrentWeather( DictionaryIterator* received, WeatherData *weather )
{
    Tuple* tuple = dict_read_first( received );
    // while we have tuples and regognize keys
    while ( tuple )
    {
        switch ( tuple->key )
        {
            case KEY_TEMPERATURE:
            {
                weather->temperature = tuple->value->int32;
                break;
            }
            case KEY_CONDITION:
            {
                weather->condition   = tuple->value->int32;
                break;
            }
            case KEY_SUNRISE:
            {
                weather->sunrise     = tuple->value->int32;
                break;
            }
            case KEY_SUNSET:
            {
                weather->sunset      = tuple->value->int32;
                break;
            }
            case KEY_PUB_DATE:
            {
                strncpy(weather->pub_date, tuple->value->cstring, 6);
                break;
            }
            case KEY_LOCALE:
            {
                strncpy(weather->locale, tuple->value->cstring, 255);
                break;
            }
            case KEY_TZOFFSET:
            {
                weather->tzoffset    = tuple->value->int32;
                break;
            }
            default:
            {
                return false;
            }
        } // switch
        tuple = dict_read_next( received );
    } // while
    
    weather->error       = WEATHER_E_OK;
    weather->updated     = time(NULL);
    
    if (weather->debug)
    {
        debug_enable_display();
        debug_update_weather(weather);
    }
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather temp:%i cond:%i pd:%s tzos:%i loc:%s",
            weather->temperature, weather->condition, weather->pub_date,
            weather->tzoffset, weather->locale);
    return true;
}

/**
 * Process the event which contains the hourly weather data
 */
bool processHourlyWeather( DictionaryIterator* received, WeatherData *weather )
{
    Tuple* tuple = dict_read_first( received );
    while ( tuple )
    {
        switch ( tuple->key )
        {
            case KEY_H1_TEMP:
            {
                weather->h1_temp = tuple->value->int32;
                break;
            }
            case KEY_H1_COND:
            {
                weather->h1_cond = tuple->value->int32;
                break;
            }
            case KEY_H1_TIME:
            {
                weather->h1_time = tuple->value->int32;
                break;
            }
            case KEY_H1_POP:
            {
                weather->h1_pop  = tuple->value->int32;
                break;
            }
            case KEY_H2_TEMP:
            {
                weather->h2_temp = tuple->value->int32;
                break;
            }
            case KEY_H2_COND:
            {
                weather->h2_cond = tuple->value->int32;
                break;
            }
            case KEY_H2_TIME:
            {
                weather->h2_time = tuple->value->int32;
                break;
            }
            case KEY_H2_POP:
            {
                weather->h2_pop  = tuple->value->int32;
                break;
            }
            default:
            {
                return false;
            }
        } // switch
        tuple = dict_read_next( received );
    } // while
    
    weather->hourly_enabled = true;
    weather->hourly_updated = time(NULL);
    return true;
}

/**
 * Process the event(s) which contain configuration settings
 */
bool processConfigSettings( DictionaryIterator* received, WeatherData *weather )
{
    Tuple* tuple = dict_read_first( received );
    while ( tuple )
    {
        switch ( tuple->key )
        {
            case KEY_SERVICE:
            {
                char* service = strcmp( tuple->value->cstring, SERVICE_OPEN_WEATHER ) == 0 ? SERVICE_OPEN_WEATHER : SERVICE_YAHOO_WEATHER;
                strncpy(weather->service, service, 6);
                break;
            }
            case KEY_DEBUG:
            {
                weather->debug   = (bool)tuple->value->int32;
                break;
            }
            case KEY_SCALE:
            {
                char* scale = strcmp( tuple->value->cstring, SCALE_CELSIUS ) == 0 ? SCALE_CELSIUS : SCALE_FAHRENHEIT;
                strncpy(weather->scale, scale, 2);
                break;
            }
            case KEY_BATTERY:
            {
                weather->battery = (bool)tuple->value->int32;
                break;
            }
            case KEY_HOURLY_ENABLED:
            {
                weather->hourly_enabled = (bool)tuple->value->int32;
                weather->hourly_updated = time(NULL);
                break;
            }
            default:
            {
                return false;
            }
        } // switch
        tuple = dict_read_next( received );
    } // while
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Configuration serv:%s scale:%s debug:%i batt:%i",
            weather->service, weather->scale, weather->debug, weather->battery);
    
    if (weather->battery)
    {
        battery_enable_display();
    }
    else
    {
        battery_disable_display();
    }
    
    if (weather->debug)
    {
        debug_enable_display();
        debug_update_weather(weather);
    }
    else
    {
        debug_disable_display();
    }
    
    store_persisted_values(weather);
    return true;
}

/**
 * Handle an incoming message
 */
static void appmsg_in_received( DictionaryIterator *received, void *context )
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "In received.");
    
    WeatherData *weather = (WeatherData*) context;
    
    // Process the current weather
    bool handled = processCurrentWeather( received, context );
    // try to process hourly weather (wunder provides it all in one message)
    handled = ( processHourlyWeather( received, context ) ) ? true : handled;
    // if the message wasn't handled, process updated configuration settings
    handled = (handled) ? handled : processConfigSettings( received, context );

    // if the message wasn't handled, handle any other message
    if ( !handled )
    {
        // process program control/flow events
        Tuple* tuple = dict_read_first( received );
        while ( tuple )
        {
            switch ( tuple->key )
            {
                case KEY_ERROR:
                {
                    weather->error   = WEATHER_E_NETWORK;
                    APP_LOG(APP_LOG_LEVEL_DEBUG, "Error: %s", tuple->value->cstring);
                    break;
                }
                case KEY_JS_READY:
                {
                    weather->js_ready = true;
                    weather->error    = WEATHER_E_OK;
                    APP_LOG(APP_LOG_LEVEL_DEBUG, "Javascript is ready");
                    debug_update_message("JS ready");
                    initial_jsready_callback();
                    break;
                }
                default:
                {
                    weather->error = WEATHER_E_PHONE;
                    // Unknown key
                    APP_LOG(APP_LOG_LEVEL_DEBUG, "appmsg_in_received: unknown key: %u",
                            (unsigned int)tuple->key );
                }
            } // switch
            tuple = dict_read_next( received );
        } // while
    } // if
    
    weather_layer_update(weather);
    // Success! reset the retry count...
    retry_count = 0;
}

/**
 * Translate the error code into a human readable value
 */
static char *translate_error(AppMessageResult result)
{
    switch (result)
    {
    case APP_MSG_OK:
        return "APP_MSG_OK";
    case APP_MSG_SEND_TIMEOUT:
        return "APP_MSG_SEND_TIMEOUT";
    case APP_MSG_SEND_REJECTED:
        return "APP_MSG_SEND_REJECTED";
    case APP_MSG_NOT_CONNECTED:
        return "APP_MSG_NOT_CONNECTED";
    case APP_MSG_APP_NOT_RUNNING:
        return "APP_MSG_APP_NOT_RUNNING";
    case APP_MSG_INVALID_ARGS:
        return "APP_MSG_INVALID_ARGS";
    case APP_MSG_BUSY:
        return "APP_MSG_BUSY";
    case APP_MSG_BUFFER_OVERFLOW:
        return "APP_MSG_BUFFER_OVERFLOW";
    case APP_MSG_ALREADY_RELEASED:
        return "APP_MSG_ALREADY_RELEASED";
    case APP_MSG_CALLBACK_ALREADY_REGISTERED:
        return "APP_MSG_CALLBACK_ALREADY_REGISTERED";
    case APP_MSG_CALLBACK_NOT_REGISTERED:
        return "APP_MSG_CALLBACK_NOT_REGISTERED";
    case APP_MSG_OUT_OF_MEMORY:
        return "APP_MSG_OUT_OF_MEMORY";
    case APP_MSG_CLOSED:
        return "APP_MSG_CLOSED";
    case APP_MSG_INTERNAL_ERROR:
        return "APP_MSG_INTERNAL_ERROR";
    default:
        return "UNKNOWN ERROR";
    }
}

/**
 * Handle the callback indicating a message has been dropped
 */
static void appmsg_in_dropped( AppMessageResult reason, void *context )
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "In dropped: %s", translate_error(reason));
}

/**
 * Handle the callback indicating a message was successfully sent
 */
static void appmsg_out_sent( DictionaryIterator *sent, void *context )
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Out sent.");
}

/**
 * Handle the callback indicating an outgoing message failed to send
 */
static void appmsg_out_failed( DictionaryIterator *failed, AppMessageResult reason,
                              void *context )
{
    WeatherData *weather_data = (WeatherData*) context;
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Out failed: %s", translate_error(reason));
    
    retry_count++;
    
    switch (reason)
    {
        case APP_MSG_NOT_CONNECTED:
        {
            weather_data->error = WEATHER_E_DISCONNECTED;
            request_weather(weather_data);
            break;
        }
        case APP_MSG_SEND_REJECTED:
        case APP_MSG_SEND_TIMEOUT:
        default:
        {
            weather_data->error = WEATHER_E_PHONE;
            request_weather(weather_data);
            break;
        }
    }
}

/**
 * Initialize the messaging callbacks
 */
void init_network( WeatherData *weather_data )
{
    int max_in  = 1200; //app_message_inbox_size_maximum();
    int max_out = 500; //app_message_outbox_size_maximum();
    
    app_message_register_inbox_received(appmsg_in_received);
    app_message_register_inbox_dropped(appmsg_in_dropped);
    app_message_register_outbox_sent(appmsg_out_sent);
    app_message_register_outbox_failed(appmsg_out_failed);
    app_message_set_context(weather_data);
    app_message_open(max_in, max_out);
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "AppMessage max_IN: %i, max_OUT: %i", max_in, max_out);
    
    weather_data->error    = WEATHER_E_OK;
    weather_data->updated  = 0;
    weather_data->js_ready = false;
    
    weather_data->hourly_updated = 0;
    weather_data->hourly_enabled = false;
    
    retry_count = 0;
}

/**
 * Clean things up
 */
void close_network()
{
    app_message_deregister_callbacks();
}

/**
 * Send a request to the JavaScript engine to request the weather data
 *
 * \return True on success, false otherwise
 */
bool request_weather( WeatherData *weather_data )
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Request weather, retry: %i", retry_count);
    
    if (retry_count > MAX_RETRY)
    {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Too many retries");
        retry_count = 0;
        return false;
    }
    
    if (!bluetooth_connection_service_peek())
    {
        weather_data->error = WEATHER_E_DISCONNECTED;
        return false;
    }
    DictionaryIterator *iter = NULL;
    AppMessageResult result = app_message_outbox_begin(&iter);
    
    if (iter == NULL || result != APP_MSG_OK)
    {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Null iter");
        return false;
    }
    
    dict_write_cstring(iter, KEY_SERVICE, weather_data->service);
    dict_write_cstring(iter, KEY_SCALE, weather_data->scale);
    dict_write_uint8(iter, KEY_DEBUG, (uint8_t)weather_data->debug);
    dict_write_uint8(iter, KEY_BATTERY, (uint8_t)weather_data->battery);
    
    dict_write_end(iter);
    
    result = app_message_outbox_send();
    return result == APP_MSG_OK;
}
