/** 
 * JavaScript code for querying current weather conditions for the Weather-My-Way pebble watchface app
 * @file js/pebble-js-app.js
 */

/* 
 * Source code documentation is compatible with JSDoc3
 * https://github.com/jsdoc3/jsdoc
 */

var SERVICE_OPEN_WEATHER  = "open";
var SERVICE_YAHOO_WEATHER = "yahoo";
var EXTERNAL_DEBUG_URL    = '';
var CONFIGURATION_URL     = 'http://jaredbiehler.github.io/weather-my-way/config/';

/**
 * The global configuration.
 */
var Global = {
  externalDebug:     false, // POST logs to external server - dangerous! lat lon recorded
  wuApiKey:          null, // register for a free api key!
  hourlyIndex1:      2, // 3 Hours from now 
  hourlyIndex2:      8, // 9 hours from now
  updateInProgress:  false,
  updateWaitTimeout: 1 * 60 * 1000, // one minute in ms
  lastUpdateAttempt: new Date(),
  maxRetry:          3,
  retryWait:         500, // ms
  config: {
    debugEnabled:   false,
    batteryEnabled: true,
    weatherService: SERVICE_YAHOO_WEATHER,
    weatherScale:   'F',
    weatherLatitude: 41.739841,
    weatherLongitude: -93.620774
  },
};

/** Allow console messages to be turned on / off */
(function(){
    var original = console.log;
    var logMessage = function (message) {
        if (Global.config.debugEnabled)
        {
          original.apply(console, arguments);
        }  
    };
    console.log  = logMessage;
    console.warn = logMessage;
})();

/**
 * Post an XmlHttpRequest with data to the the supplied url
 *
 * @param url Valid url which the data will be posted to
 * @param data Data which will be posted
 */
var post = function(url, data)
{
    try
    {
        var req = new XMLHttpRequest();
        req.open('POST', url, true);
        req.send(data);
    }
    catch(ex)
    {
        console.warn('POST failed: ' + ex.message);
    }
};

/**
 * Post the supplied debug messages to the external debug url
 */
var postDebugMessage = function (data)
{
    if (!Global.externalDebug || EXTERNAL_DEBUG_URL === null) {
        return;
    }
    try {
        post(EXTERNAL_DEBUG_URL, 'data='+JSON.stringify(data));
    } catch (ex) {
        console.warn('Post Debug Message failed:'+ex.message);
    }
};


/**
 * The pebble acknowledgement that the sent message was received and handled
 */
var ack  = function ()
{
    console.log("Pebble ACK sendAppMessage");
};

/**
 * The pebble acknowledgement that an error occured in the processing of the message
 */
var nack = function (data, retry)
{
    retry = typeof retry !== 'undefined' ? retry : 0;
    retry++;
    if (retry >= Global.maxRetry) {
        console.warn("Pebble NACK sendAppMessage max exceeded");
        return;
    }
    console.warn("Pebble NACK sendAppMessage retryCount:"+retry+" data:"+JSON.stringify(data));
    if (data)
    {
        setTimeout(function(){
               Pebble.sendAppMessage(data, ack, function(e){ nack(data, retry); });
           }, Global.retryWait + Math.floor(Math.random() * Global.retryWait));
    }
};

/**
 * Create, request and handle the JSON formatted response 
 *
 * @param url       The complete url we will use for the request
 * @param callback  The callback which will be executed at completion
 */
var getJson = function(url, callback)
{
    try {
        var req = new XMLHttpRequest();
        req.open('GET', url, true);
        req.onload = function(e) {
            if (req.readyState == 4) {
                if(req.status == 200) {
                    try {
                        //console.log(req.responseText);
                        var response = JSON.parse(req.responseText);
                        callback(null, response);
                    } catch (ex) {
                        callback(ex.message);
                    }
                } else {
                    callback("Error request status not 200, status: "+req.status);
                }
            }
        };
        req.send(null);
    } catch(ex) {
        callback("Unable to GET JSON: "+ex.message);
    }
};

/**
 * Given options, make the weather data request through the connected device
 *
 * @param options Options which specify url and parameters for the request
 */
var fetchWeather = function(options)
{
    //console.log('URL: ' + options.url);
    getJson(options.url, function(err, response) {
        try {
            if (err) {
                throw err;
            }

            var weather = options.parse(response);
            console.log('Weather Data: ' + JSON.stringify(weather));

            Pebble.sendAppMessage(weather, ack, function(e){
                                  nack(weather);
                                  });
            postDebugMessage(weather);

        } catch (ex) {
            console.warn("Could not find weather data in response: " + ex.message);
            var error = { "error": "HTTP Error" };
            Pebble.sendAppMessage(error, ack, nack);
            postDebugMessage(error);
        }
        Global.updateInProgress = false;
    });
};

/**
 * Fetch weather data from Yahoo
 *
 * @param latitude  Latitude portion of the GPS location we want data on
 * @param longitude Longitude portion of the GPS location we want data on
 */
var fetchYahooWeather = function(latitude, longitude)
{
    var subselect, neighbor, query, multi, options = {};
    
    subselect   = 'SELECT woeid FROM geo.placefinder WHERE text="'+latitude+','+longitude+'" AND gflags="R"';
    neighbor    = 'SELECT * FROM geo.placefinder WHERE text="'+latitude+','+longitude+'" AND gflags="R";';
    query       = 'SELECT * FROM weather.forecast WHERE woeid IN ('+subselect+') AND u="'+Global.config.weatherScale.toLowerCase()+'";';
    multi       = "SELECT * FROM yql.query.multi WHERE queries='"+query+" "+neighbor+"'";
    options.url = "https://query.yahooapis.com/v1/public/yql?format=json&q="+encodeURIComponent(multi)+"&nocache="+new Date().getTime();
    
    options.parse = function(response) {
        var sunrise, sunset, pubdate, locale;
        sunrise = response.query.results.results[0].channel.astronomy.sunrise;
        sunset  = response.query.results.results[0].channel.astronomy.sunset;
        pubdate = new Date(Date.parse(response.query.results.results[0].channel.item.pubDate));
        locale  = response.query.results.results[1].Result.neighborhood;
        if (locale === null) {
            locale = response.query.results.results[1].Result.city;
        }
        if (locale === null) {
            locale = 'unknown';
        }
        
        return {
            condition:   parseInt(response.query.results.results[0].channel.item.condition.code),
            temperature: parseInt(response.query.results.results[0].channel.item.condition.temp),
            sunrise:     Date.parse(new Date().toDateString()+" "+sunrise) / 1000,
            sunset:      Date.parse(new Date().toDateString()+" "+sunset) / 1000,
            locale:      locale,
            pubdate:     pubdate.getHours()+':'+('0'+pubdate.getMinutes()).slice(-2),
            tzoffset:    new Date().getTimezoneOffset() * 60
        };
    };
    
    fetchWeather(options); 
};

/**
 * Fetch weather data from Open Weather Map
 *
 * @param latitude  Latitude portion of the GPS location we want data on
 * @param longitude Longitude portion of the GPS location we want data on
 */
var fetchOpenWeather = function(latitude, longitude)
{
    var options = {};
    options.url = "http://api.openweathermap.org/data/2.5/weather?lat=" + latitude + "&lon=" + longitude + "&cnt=1";
    
    options.parse = function(response) {
        var temperature, sunrise, sunset, condition, pubdate;
        
        var tempResult = response.main.temp;
        if (Global.config.weatherScale === 'C') {
            // Convert temperature to Celsius
            temperature = Math.round(tempResult - 273.15);
        }
        else {
            // Otherwise, convert temperature to Fahrenheit
            temperature = Math.round(((tempResult - 273.15) * 1.8) + 32);
        }
        
        condition = response.weather[0].id;
        sunrise   = response.sys.sunrise;
        sunset    = response.sys.sunset;
        pubdate   = new Date(response.dt*1000);
        
        return {
            condition:   condition,
            temperature: temperature,
            sunrise:     sunrise,
            sunset:      sunset,
            locale:      response.name,
            pubdate:     pubdate.getHours()+':'+('0'+pubdate.getMinutes()).slice(-2),
            tzoffset:    new Date().getTimezoneOffset() * 60
        };
    };
    fetchWeather(options);
};

/**
 * Fetch weather data from Weather Underground
 *
 * @param latitude  Latitude portion of the GPS location we want data on
 * @param longitude Longitude portion of the GPS location we want data on
 */
var fetchWunderWeather = function(latitude, longitude)
{
    var options = {};
    options.url = 'http://api.wunderground.com/api/'+Global.wuApiKey+'/hourly/q/'+latitude+','+longitude+'.json';
    
    options.parse = function(response) {
        
        var h1 = response.hourly_forecast[Global.hourlyIndex1],
        h2 = response.hourly_forecast[Global.hourlyIndex2];
        
        return {
            h1_temp: Global.config.weatherScale === 'C' ? parseInt(h1.temp.metric) : parseInt(h1.temp.english),
            h1_cond: parseInt(h1.fctcode),
            h1_time: parseInt(h1.FCTTIME.epoch),
            h1_pop:  parseInt(h1.pop),
            h2_temp: Global.config.weatherScale === 'C' ? parseInt(h2.temp.metric) : parseInt(h2.temp.english),
            h2_cond: parseInt(h2.fctcode),
            h2_time: parseInt(h2.FCTTIME.epoch),
            h2_pop:  parseInt(h2.pop)
        };
    };
    fetchWeather(options);
};

/**
 * Given an object, serialize it into a key=value string format
 *
 * @param obj Object which we want to serialize
 */
var serialize = function(obj) {
    var str = [];
    for(var p in obj)
    {
        if (obj.hasOwnProperty(p)) {
            str.push(encodeURIComponent(p) + "=" + encodeURIComponent(obj[p]));
        }
    }
    return str.join("&");
};

/**
 * Fetches weather data from the selected weather source for the specified location
 *
 * @param latitude  Latitude portion of the GPS location we want data on
 * @param longitude Longitude portion of the GPS location we want data on

 */
var queryWeatherConditions = function(latitude, longitude)
{
    if (Global.config.weatherService === SERVICE_OPEN_WEATHER)
    {
        fetchOpenWeather(latitude, longitude);
    }
    else
    {
        fetchYahooWeather(latitude, longitude);
    }
    // Leverage WeatherUnderground if we have a key
    if (Global.wuApiKey !== null)
    {
        fetchWunderWeather(latitude, longitude);
    }
    else
    {
        var data = {hourly_enabled: 0};
        console.log("Hourly disabled, no WU ApiKey");
        Pebble.sendAppMessage(data, ack, function(ev){ nack(data); });
    }
};

/**
 * Called when the location is successfully aquired
 *
 * @param pos position data
 */
var locationSuccess = function (pos) {
    var coordinates = pos.coords;
    console.log("Got coordinates: " + JSON.stringify(coordinates));
    queryWeatherConditions( pos.latitude, pos.longitude );
};

/** 
 * Called when the location is not successfully aquired
 *
 * @param err Error information
 */
var locationError = function (err) {
    var message = 'Location error (' + err.code + '): ' + err.message;
    console.warn(message);
    Pebble.sendAppMessage({ "error": "Loc unavailable" }, ack, nack);
    postDebugMessage({"error": message});
    Global.updateInProgress = false;
};

/** 
 * Called whenever the weather data is out of date, fetches the current
 * location we want weather data for 
 */
var updateWeather = function ()
{
    var nextUpdateTime = Global.lastUpdateAttempt.getTime() + Global.updateWaitTimeout;
    if (Global.updateInProgress && new Date().getTime() < nextUpdateTime)
    {
        console.log("Update already started in the last "+(Global.updateWaitTimeout/60000)+" minutes");
        return;
    }
    Global.updateInProgress  = true;
    Global.lastUpdateAttempt = new Date();
    
    var locationOptions = { "enableHighAccuracy": false, "timeout": 15000, "maximumAge": 60000 };
    navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
};

/* - - - - - - - - - - - - - - - - - - - - - - - - */

/** Setup Pebble Event Listeners */

/**
 * Handle the event when the Pebble indicates it is ready
 */
var OnPebbleReady = function(e)
{
    console.log("Starting ...");
    var data = { "js_ready": true };
    Pebble.sendAppMessage(data, ack, function(e){ nack(data); });
    var initialInstall = localStorage.getItem('initialInstall');
    if (initialInstall === null && Global.wuApiKey === null)
    {
        localStorage.setItem('initialInstall', false);
        Pebble.showSimpleNotificationOnPebble("API Key Needed", "This watchface requires a free API key from Weather Underground. Please visit Settings in the Pebble App to find out more!");
    }
};

Pebble.addEventListener("ready", OnPebbleReady);

/**
 * Handle the appmessage event. Triggers checks to see if updated weather should be pulled
 */
var OnAppMessage = function(data)
{
    console.log("Got a message - Starting weather request ... " + JSON.stringify(data));
    try {
        Global.config.weatherService = data.payload.service === SERVICE_OPEN_WEATHER ? SERVICE_OPEN_WEATHER : SERVICE_YAHOO_WEATHER;
        Global.config.debugEnabled   = data.payload.debug   === 1;
        Global.config.batteryEnabled = data.payload.battery === 1;
        Global.config.weatherScale   = data.payload.scale   === 'C' ? 'C' : 'F';
        Global.wuApiKey              = localStorage.getItem('wuApiKey');
        updateWeather();
    } catch (ex) {
        console.warn("Could not retrieve data sent from Pebble: "+ex.message);
    }
};

Pebble.addEventListener("appmessage", OnAppMessage);

/**
 * This is the reason for the Global.config variable - I couldn't think of a way (short of asking Pebble)
 * for the latest config settings. So I persist them in a rather ugly Global variable. 
 */
var OnShowConfiguration = function (e) {
    var options = {
        's': Global.config.weatherService,
        'd': Global.config.debugEnabled,
        'u': Global.config.weatherScale,
        'b': Global.config.batteryEnabled ? 'on' : 'off',
        'a': Global.wuApiKey
    };
    var url = CONFIGURATION_URL+'?'+serialize(options);
    console.log('Configuration requested using url: '+url);
    Pebble.openURL(url);
};

Pebble.addEventListener("showConfiguration", OnShowConfiguration);

/**
 * Handle when the Webview containing the settings page has been closed
 */
var OnWebviewClosed = function(e) {
    /*
     * Android Hack - for whatever reason this event is always firing on Android with a message of 'CANCELLED'
     */
    if (e.response && e.response !== 'CANCELLED') {
        try {
            var settings = JSON.parse(decodeURIComponent(e.response));
            
            // Android 'cancel' sends a blank object
            if (Object.keys(settings).length <= 0) {
                return;
            }
            
            console.log("Settings received: "+JSON.stringify(settings));
            
            var refreshNeeded = (settings.service  !== Global.config.weatherService ||
                                 settings.scale    !== Global.config.weatherScale   ||
                                 settings.wuApiKey !== Global.wuApiKey);
            
            Global.config.weatherService = settings.service === SERVICE_OPEN_WEATHER ? SERVICE_OPEN_WEATHER : SERVICE_YAHOO_WEATHER;
            Global.config.weatherScale   = settings.scale   === 'C' ? 'C' : 'F';
            Global.config.debugEnabled   = settings.debug   === 'true';
            Global.config.batteryEnabled = settings.battery === 'on';
            Global.wuApiKey              = settings.wuApiKey;
            
            if (Global.wuApiKey !== null) {
                localStorage.setItem('wuApiKey', Global.wuApiKey);
            } else {
                localStorage.removeItem('wuApiKey');
            }
            
            var config = {
                service: Global.config.weatherService,
                scale:   Global.config.weatherScale,
                debug:   Global.config.debugEnabled   ? 1 : 0,
                battery: Global.config.batteryEnabled ? 1 : 0
            };
            
            Pebble.sendAppMessage(config, ack, function(ev){ nack(config); });
            
            if (refreshNeeded) {
                updateWeather();
            }
        } catch(ex) {
            console.warn("Unable to parse response from configuration:"+ex.message);
        }
    }
};

Pebble.addEventListener("webviewclosed", OnWebviewClosed);
