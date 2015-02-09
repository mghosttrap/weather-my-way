#ifndef WEATHER_ICON_MAPS_H
#define WEATHER_ICON_MAPS_H
/**
 * This file contains fast reference maps, which enable very fast and simple
 * conversion between the raw weather code value and the icon used to represent
 * it in this app.
 */

// Fast lookup table for the Yahoo Weather nighttime conditions
const int S_yahoo_nightconditions_map[] =
{
    W_ICON_WIND,                // tornado
    W_ICON_WIND,                // tropical storm
    W_ICON_WIND,                // hurricane
    W_ICON_THUNDER,             // severe thunderstorms
    W_ICON_THUNDER,             // thunderstorms
    W_ICON_RAIN_SNOW,           // mixed rain and snow
    W_ICON_RAIN_SLEET,          // mixed rain and sleet
    W_ICON_SNOW_SLEET,          // mixed snow and sleet
    W_ICON_RAIN_SLEET,          // freezing drizzle
    W_ICON_DRIZZLE,             // drizzle
    W_ICON_RAIN_SLEET,          // freezing rain
    W_ICON_RAIN,                // showers
    W_ICON_RAIN,                // showers
    W_ICON_SNOW,                // snow flurries
    W_ICON_SNOW,                // light snow showers
    W_ICON_HEAVY_SNOW,          // blowing snow
    W_ICON_SNOW,                // snow
    W_ICON_SLEET,               // hail
    W_ICON_SLEET,               // sleet
    W_ICON_FOG,                 // dust
    W_ICON_FOG,                 // foggy
    W_ICON_FOG,                 // haze
    W_ICON_FOG,                 // smoky
    W_ICON_WIND,                // blustery
    W_ICON_WIND,                // windy
    W_ICON_COLD,                // cold
    W_ICON_CLOUDY,              // cloudy
    W_ICON_CLOUDY,              // mostly cloudy (night)
    W_ICON_MOSTLY_CLOUDY_DAY,   // mostly cloudy (day)
    W_ICON_PARTLY_CLOUDY_NIGHT, // partly cloudy (night)
    W_ICON_PARTLY_CLOUDY_DAY,   // partly cloudy (day)
    W_ICON_CLEAR_NIGHT,         // clear (night)
    W_ICON_CLEAR_DAY,           // sunny
    W_ICON_FAIR_NIGHT,          // fair (night)
    W_ICON_FAIR_DAY,            // fair (day)
    W_ICON_RAIN_SLEET,          // mixed rain and hail
    W_ICON_HOT,                 // hot
    W_ICON_THUNDER_SUN,         // isolated thunderstorms
    W_ICON_THUNDER,             // scattered thunderstorms
    W_ICON_THUNDER,             // scattered thunderstorms
    W_ICON_RAIN,                // scattered showers
    W_ICON_HEAVY_SNOW,          // heavy snow
    W_ICON_SNOW,                // scattered snow showers
    W_ICON_HEAVY_SNOW,          // Heavy snow
    W_ICON_PARTLY_CLOUDY_NIGHT, // partly cloudy
    W_ICON_RAIN,                // thundershowers
    W_ICON_SNOW,                // snow showers
    W_ICON_THUNDER              // isolated thundershowers
    //3200 not available
};

// Fast lookup table for the Yahoo Weather daytime conditions
const int S_yahoo_dayconditions_map[] =
{
    W_ICON_WIND,                // tornado
    W_ICON_WIND,                // tropical storm
    W_ICON_WIND,                // hurricane
    W_ICON_THUNDER,             // severe thunderstorms
    W_ICON_THUNDER,             // thunderstorms
    W_ICON_RAIN_SNOW,           // mixed rain and snow
    W_ICON_RAIN_SLEET,          // mixed rain and sleet
    W_ICON_SNOW_SLEET,          // mixed snow and sleet
    W_ICON_RAIN_SLEET,          // freezing drizzle
    W_ICON_DRIZZLE,             // drizzle
    W_ICON_RAIN_SLEET,          // freezing rain
    W_ICON_RAIN,                // showers
    W_ICON_RAIN,                // showers
    W_ICON_SNOW,                // snow flurries
    W_ICON_SNOW,                // light snow showers
    W_ICON_HEAVY_SNOW,          // blowing snow
    W_ICON_SNOW,                // snow
    W_ICON_SLEET,               // hail
    W_ICON_SLEET,               // sleet
    W_ICON_FOG,                 // dust
    W_ICON_FOG,                 // foggy
    W_ICON_FOG,                 // haze
    W_ICON_FOG,                 // smoky
    W_ICON_WIND,                // blustery
    W_ICON_WIND,                // windy
    W_ICON_COLD,                // cold
    W_ICON_CLOUDY,              // cloudy
    W_ICON_CLOUDY,              // mostly cloudy (night)
    W_ICON_MOSTLY_CLOUDY_DAY,   // mostly cloudy (day)
    W_ICON_PARTLY_CLOUDY_NIGHT, // partly cloudy (night)
    W_ICON_PARTLY_CLOUDY_DAY,   // partly cloudy (day)
    W_ICON_CLEAR_NIGHT,         // clear (night)
    W_ICON_CLEAR_DAY,           // sunny
    W_ICON_FAIR_NIGHT,          // fair (night)
    W_ICON_FAIR_DAY,            // fair (day)
    W_ICON_RAIN_SLEET,          // mixed rain and hail
    W_ICON_HOT,                 // hot
    W_ICON_THUNDER_SUN,         // isolated thunderstorms
    W_ICON_THUNDER,             // scattered thunderstorms
    W_ICON_THUNDER,             // scattered thunderstorms
    W_ICON_RAIN_SUN,            // scattered showers
    W_ICON_HEAVY_SNOW,          // heavy snow
    W_ICON_SNOW,                // scattered snow showers
    W_ICON_HEAVY_SNOW,          // Heavy snow
    W_ICON_PARTLY_CLOUDY_DAY,   // partly cloudy
    W_ICON_RAIN,                // thundershowers
    W_ICON_SNOW,                // snow showers
    W_ICON_THUNDER_SUN          // isolated thundershowers
    //3200 not available
};

// Fast lookup table for the Weather Underground nighttime forecast
const int S_wunder_nightforecast_icon_map[] =
{
    W_ICON_NOT_AVAILABLE, // index 0 (not defined by weather underground)
    W_ICON_CLEAR_NIGHT, // Clear
    W_ICON_FAIR_NIGHT,  // Partly Cloudy
    W_ICON_PARTLY_CLOUDY_NIGHT,   // Mostly Cloudy
    W_ICON_CLOUDY,      // Cloudy
    W_ICON_FOG,         // Hazy
    W_ICON_FOG,         // Foggy
    W_ICON_HOT,         // Very Hot
    W_ICON_COLD,        // Very Cold
    W_ICON_SNOW,        // Blowing Snow
    W_ICON_DRIZZLE,     // Chance of Showers
    W_ICON_RAIN,        // Showers
    W_ICON_RAIN,        // Chance of Rain
    W_ICON_RAIN,        // Rain
    W_ICON_THUNDER,     // Chance of a Thunderstorm
    W_ICON_THUNDER,     // Thunderstorm
    W_ICON_SLEET,       // Flurries
    W_ICON_NOT_AVAILABLE,// OMITTED
    W_ICON_SNOW_SLEET,  // Chance of Snow Showers
    W_ICON_SNOW_SLEET,  // Snow Showers
    W_ICON_SNOW,        // Chance of Snow
    W_ICON_SNOW,        // Snow
    W_ICON_SNOW_SLEET,  // Chace of Ice Pellets
    W_ICON_SNOW_SLEET,  // Ice Pellets
    W_ICON_SNOW         // Blizzard
};

// Fast lookup table for the Weather Underground daytime forecast
const int S_wunder_dayforecast_icon_map[] =
{
    W_ICON_NOT_AVAILABLE, // index 0 (not defined by weather underground)
    W_ICON_CLEAR_DAY,   // Clear
    W_ICON_FAIR_DAY,    // Partly Cloudy
    W_ICON_PARTLY_CLOUDY_DAY,   // Mostly Cloudy
    W_ICON_CLOUDY,      // Cloudy
    W_ICON_FOG,         // Hazy
    W_ICON_FOG,         // Foggy
    W_ICON_HOT,         // Very Hot
    W_ICON_COLD,        // Very Cold
    W_ICON_SNOW,        // Blowing Snow
    W_ICON_RAIN_SUN,    // Chance of Showers
    W_ICON_RAIN,        // Showers
    W_ICON_RAIN_SUN,    // Chance of Rain
    W_ICON_RAIN,        // Rain
    W_ICON_THUNDER_SUN, // Chance of a Thunderstorm
    W_ICON_THUNDER,     // Thunderstorm
    W_ICON_SLEET,       // Flurries
    W_ICON_NOT_AVAILABLE,// OMITTED
    W_ICON_SNOW_SLEET,  // Chance of Snow Showers
    W_ICON_SNOW_SLEET,  // Snow Showers
    W_ICON_SNOW,        // Chance of Snow
    W_ICON_SNOW,        // Snow
    W_ICON_SNOW_SLEET,  // Chace of Ice Pellets
    W_ICON_SNOW_SLEET,  // Ice Pellets
    W_ICON_SNOW         // Blizzard
};


// Fast lookup map for current day time conditions from weather underground
const int S_wunder_dayconditions_map[] =
{
    W_ICON_DRIZZLE, // Drizzle
    W_ICON_RAIN, // Rain
    W_ICON_SNOW, // Snow
    W_ICON_SNOW, // Snow Grains
    W_ICON_SLEET, // Ice Crystals
    W_ICON_SLEET, // Ice Pellets
    W_ICON_SLEET, // Hail
    W_ICON_DRIZZLE, // Mist
    W_ICON_FOG, // Fog
    W_ICON_FOG, // Fog Patches
    W_ICON_FOG, // Smoke
    W_ICON_FOG, // Volcanic Ash
    W_ICON_FOG, // Widespread Dust
    W_ICON_FOG, // Sand
    W_ICON_FOG, // Haze
    W_ICON_DRIZZLE, // Spray
    W_ICON_FOG, // Dust Whirls
    W_ICON_FOG, // Sandstorm
    W_ICON_SNOW, // Low Drifting Snow
    W_ICON_FOG, // Low Drifting Widespread Dust
    W_ICON_FOG, // Low Drifting Sand
    W_ICON_HEAVY_SNOW, // Blowing Snow
    W_ICON_FOG, // Blowing Widespread Dust
    W_ICON_WIND, // Blowing Sand
    W_ICON_DRIZZLE, // Rain Mist
    W_ICON_RAIN, // Rain Showers
    W_ICON_SNOW, // Snow Showers
    W_ICON_SNOW, // Snow Blowing Snow Mist
    W_ICON_SLEET, // Ice Pellet Showers
    W_ICON_RAIN_SLEET, // Hail Showers
    W_ICON_RAIN_SLEET, // Small Hail Showers
    W_ICON_THUNDER, // Thunderstorm
    W_ICON_THUNDER, // Thunderstorms and Rain
    W_ICON_THUNDER, // Thunderstorms and Snow
    W_ICON_THUNDER, // Thunderstorms and Ice Pellets
    W_ICON_THUNDER, // Thunderstorms with Hail
    W_ICON_THUNDER, // Thunderstorms with Small Hail
    W_ICON_DRIZZLE, // Freezing Drizzle
    W_ICON_SLEET, // Freezing Rain
    W_ICON_SLEET, // Freezing Fog
    W_ICON_FOG, // Patches of Fog
    W_ICON_FOG, // Shallow Fog
    W_ICON_FOG, // Partial Fog
    W_ICON_CLOUDY, // Overcast
    W_ICON_CLEAR_DAY, // Clear
    W_ICON_FAIR_DAY, // Partly Cloudy
    W_ICON_CLOUDY, // Mostly Cloudy
    W_ICON_FAIR_DAY, // Scattered Clouds
    W_ICON_RAIN_SLEET, // Small Hail
    W_ICON_THUNDER, // Squalls
    W_ICON_WIND, // Funnel Cloud
    W_ICON_NOT_AVAILABLE, // Unknown Precipitation
    W_ICON_NOT_AVAILABLE // Unknown
};

// Fast lookup map for current night time conditions from weather underground
const int S_wunder_nightconditions_map[] =
{
    W_ICON_DRIZZLE, // Drizzle
    W_ICON_RAIN, // Rain
    W_ICON_SNOW, // Snow
    W_ICON_SNOW, // Snow Grains
    W_ICON_SLEET, // Ice Crystals
    W_ICON_SLEET, // Ice Pellets
    W_ICON_SLEET, // Hail
    W_ICON_DRIZZLE, // Mist
    W_ICON_FOG, // Fog
    W_ICON_FOG, // Fog Patches
    W_ICON_FOG, // Smoke
    W_ICON_FOG, // Volcanic Ash
    W_ICON_FOG, // Widespread Dust
    W_ICON_FOG, // Sand
    W_ICON_FOG, // Haze
    W_ICON_DRIZZLE, // Spray
    W_ICON_FOG, // Dust Whirls
    W_ICON_FOG, // Sandstorm
    W_ICON_SNOW, // Low Drifting Snow
    W_ICON_FOG, // Low Drifting Widespread Dust
    W_ICON_FOG, // Low Drifting Sand
    W_ICON_HEAVY_SNOW, // Blowing Snow
    W_ICON_FOG, // Blowing Widespread Dust
    W_ICON_WIND, // Blowing Sand
    W_ICON_DRIZZLE, // Rain Mist
    W_ICON_RAIN, // Rain Showers
    W_ICON_SNOW, // Snow Showers
    W_ICON_SNOW, // Snow Blowing Snow Mist
    W_ICON_SLEET, // Ice Pellet Showers
    W_ICON_RAIN_SLEET, // Hail Showers
    W_ICON_RAIN_SLEET, // Small Hail Showers
    W_ICON_THUNDER, // Thunderstorm
    W_ICON_THUNDER, // Thunderstorms and Rain
    W_ICON_THUNDER, // Thunderstorms and Snow
    W_ICON_THUNDER, // Thunderstorms and Ice Pellets
    W_ICON_THUNDER, // Thunderstorms with Hail
    W_ICON_THUNDER, // Thunderstorms with Small Hail
    W_ICON_DRIZZLE, // Freezing Drizzle
    W_ICON_SLEET, // Freezing Rain
    W_ICON_SLEET, // Freezing Fog
    W_ICON_FOG, // Patches of Fog
    W_ICON_FOG, // Shallow Fog
    W_ICON_FOG, // Partial Fog
    W_ICON_CLOUDY, // Overcast
    W_ICON_CLEAR_NIGHT, // Clear
    W_ICON_FAIR_NIGHT, // Partly Cloudy
    W_ICON_CLOUDY, // Mostly Cloudy
    W_ICON_FAIR_NIGHT, // Scattered Clouds
    W_ICON_RAIN_SLEET, // Small Hail
    W_ICON_THUNDER, // Squalls
    W_ICON_WIND, // Funnel Cloud
    W_ICON_NOT_AVAILABLE, // Unknown Precipitation
    W_ICON_NOT_AVAILABLE // Unknown
};

#endif

