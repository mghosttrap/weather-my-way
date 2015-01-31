
Weather My Way (Pebble SDK 2.0)
=================================

![pebble banner](https://raw.githubusercontent.com/jaredbiehler/weather-my-way/master/screenshots/pebble-banner.png)

This is my attempt at learning about the Pebble SDK by adjusting an app for my own needs. The original Futura Weather watchface was the cleanest and best looking design I had seen but I prefer YAHOO! Weather data. So, I added the option to switch between both, and added hourly data from Weather Underground. 

Give it a shot and let me know what you think: [download pbw here](https://github.com/jaredbiehler/weather-my-way/releases/)

![pebble screen1](https://raw.githubusercontent.com/jaredbiehler/weather-my-way/master/screenshots/pebble-screenshot1.png)&nbsp;
![pebble screen2](https://raw.githubusercontent.com/jaredbiehler/weather-my-way/master/screenshots/pebble-screenshot2.png)&nbsp;
![pebble screen3](https://raw.githubusercontent.com/jaredbiehler/weather-my-way/master/screenshots/pebble-screenshot3.png)

## Requirements

The hourly data for this app comes from the [Weather Underground API](http://www.wunderground.com/weather/api/). Unfortunately, their API is not free. However, Weather Underground does provide a free developer API key (500 hits / day) which more than suffices for this app. Don't share your API key, as once the quota has been reached the key will stop working. 

Might also need a [Google Geocoding API](https://developers.google.com/maps/documentation/geocoding/index) key to convert an address to the lat/long of a home location. The code which might need this is currently unimplemented.

## Configuration 

(When it is release worthy) Try it yourself here.

 Query string variables: 
```
s=[yahoo|open]  // weather service
u=[F|C]         // weather scale
d=[true|false]  // debug enabled
b=[on|off]      // battery enabled
a=[apikey]      // Weather Underground API Key 
```

![config screen](https://raw.githubusercontent.com/jaredbiehler/weather-my-way/master/screenshots/weather-my-way-config.png)

## Work in Progress
 - Fast restart - Save last weather report (location, time, values) so it doesn't need to query if the watch face is switched out and back.
 - Changes to reduce battery utilization on the connected device
  - Adding ability to specify a USPS zip code or lat/long (a home location)
  - Changes to the location monitoring to alleviate the need to constantly poke the JavaScript engine - Although location updates appear faster than previous implementation
 - Add the ability to use the current location to monitor active alerts from the National Weather Service (US)
 - Added monitoring of bluetooth events, notification on disconnect

## Progress
 - Hourly weather! 
 - Configurable weather provider (YAHOO!, Open Weather map)
  - More granularity in weather condition expression (via YAHOO!)
 - Configurable minimal battery display
 - Configurable weather scale (°F / °C)
 - Configurable debug mode ((L)ast updated, (P)ublish Date, Neighborhood)
 - JQuery Mobile configuration screen [here](http://jaredbiehler.github.io/weather-my-way/config/)
 - Config settings are persisted 
 - No longer subscribed to second ticks, minute & day for better battery performance
 - Tested on iOS 7.1 & Android 4.3
 - Improved network link monitoring (limited retries on both Pebble and JS sides)
 - many rewrites, separation of concerns


### Based on work by:
 - "Futura Weather 2" by Niknam - https://github.com/Niknam/futura-weather-sdk2.0
 - "WeatherWatch" by Katharine - https://github.com/Katharine/WeatherWatch
 - "Roboto Weather" by Martin Rosinski - http://www.mypebblefaces.com/apps/3601/3408/
