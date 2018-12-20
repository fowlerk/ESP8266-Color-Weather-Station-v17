/**The MIT License (MIT)
Copyright (c) 2015 by Daniel Eichhorn
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
See more at http://blog.squix.ch
*/

// Tft library connectors for ILI9341 display, XPT2046 touch-screen
#define TFT_DC D2
#define TFT_CS D1
#define TOUCH_CS D3
#define TOUCH_IRQ  D4
#define LED_PIN D8

#include "DHT.h"
#include <simpleDSTadjust.h>

// DHT Settings
// Uncomment whatever type you're using!

// #define DHTPIN D2 // NodeMCU
//#define DHTPIN D8 // Wemos D1R2 Mini (DHT not currently implemented here)

//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321 (DHT not currently implemented here)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#if DHTTYPE == DHT22
#define DHTTEXT "DHT22"
#elif DHTTYPE == DHT21
#define DHTTEXT "DHT21"
#elif DHTTYPE == DHT11
#define DHTTEXT "DHT11"
#endif
char FormattedTemperature[10];
char FormattedHumidity[10];

// Double-reset detector settings (for on-demand configuration portal)...

// Number of seconds after reset during which a 
// subseqent reset will be considered a double reset.
#define DRD_TIMEOUT 10

// RTC Memory Address for the DoubleResetDetector to use
#define DRD_ADDRESS 0

// Settings for the name of the OTA host and config portal password...
// HOSTNAME for OTA update
// #define HOSTNAME "ESP8266-OTA-"
String OTAHostName = "ESP8266-OTA-";            // Changed to be unique from config portal hostname, 28-Jan-2018, DKF
String configPortalHostname;                    // Moved from main source to settings, 28-Jan-2018, DKF
const char* configPortalPassword = "portal-pass";

const char* WiFiHost = "ESPWeather";            // Host name for station

int activeAlerts;                   // for number of active weather alerts
bool isNight = false;               // for flagging if it is nighttime
bool timeIsSet = false;             // flag to indicate if time has been set

uint8_t midPanelIndex = 0;          // index used to indicate which forecast panel is currently active; used for touch-enhancements, added 22-Jan-18, DKF

// Uncomment for 24 Hour style clock
//#define STYLE_24HR

// *** The following are default values for the configuration...
//      DST rules for US Eastern Time Zone (New York, Boston, Louisville)
int UTC_OFFSET = -5;
dstRule StartRule = {"EDT", Second, Sun, Mar, 2, 3600}; // Eastern Daylight time = UTC/GMT -4 hours;
dstRule EndRule = {"EST", First, Sun, Nov, 1, 0};       // Eastern Standard time = UTC/GMT -5 hour;

#define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"

// Adjust according to your language
const String WDAY_NAMES[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const String MONTH_NAMES[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const String SUN_MOON_TEXT[] = {"Sun", "Rise", "Set", "Moon", "Age", "Illum"};
const String MOON_PHASES[] = {"New Moon", "Waxing Crescent", "First Quarter", "Waxing Gibbous",
                              "Full Moon", "Waning Gibbous", "Third quarter", "Waning Crescent"};

// Weather API Settings (default)

// OpenWeatherMap Settings
//** values in metric or imperial system?
boolean isMetric = false;
const uint8_t MAX_FORECASTS = 15;

//const boolean IS_METRIC = false;

char OPEN_WEATHER_MAP_APP_ID[33] = "OWM App ID";
//char WUNDERGROUND_LANGUAGE[4] = "EN";
/*
Arabic -> ar, Bulgarian -> bg, Catalan -> ca, Czech -> cz, German -> de, Greek -> el,
English -> en, Persian (Farsi) -> fa, Finnish -> fi, French -> fr, Galician -> gl,
Croatian -> hr, Hungarian -> hu, Italian -> it, Japanese -> ja, Korean -> kr,
Latvian -> la, Lithuanian -> lt, Macedonian -> mk, Dutch -> nl, Polish -> pl,
Portuguese -> pt, Romanian -> ro, Russian -> ru, Swedish -> se, Slovak -> sk,
Slovenian -> sl, Spanish -> es, Turkish -> tr, Ukrainian -> ua, Vietnamese -> vi,
Chinese Simplified -> zh_cn, Chinese Traditional -> zh_tw.
*/
//String OPEN_WEATHER_MAP_LANGUAGE = "en";
char OPEN_WEATHER_MAP_LANGUAGE[4] = "en";
//char WUNDERGROUND_COUNTRY[4] = "US";
//char WUNDERGROUND_CITY[20] = "KY/40223";
/*
Go to https://openweathermap.org/find?q= and search for a location. Go through the
result set and select the entry closest to the actual location you want to display 
data for. It'll be a URL like https://openweathermap.org/city/2657896. The number
at the end is what you assign to the constant below.
 */
//String OPEN_WEATHER_MAP_LOCATION_ID = "4299276";
char OPEN_WEATHER_MAP_LOCATION_ID[10] = "4299276";          // Louisville, KY, US
//String DISPLAYED_CITY_NAME = "Louisville";
char DISPLAYED_CITY_NAME[30] = "Louisville";
char TZ_CITY[30] = "Louisville";
char MID_PANEL_DELAY[3] = "6";                          // Added 26-Jan-2018, DKF
char BOT_PANEL_DELAY[3] = "10";
char RESTART_TIME[6] = "02:00";                         // Default auto-restart time; 01-Mar-2018, DKF
char RESTART_INTERVAL[11] = "1209600";                  // Default restart interval in secs (14 days); 01-Mar-2018, DKF

uint8_t restartHr = 2;                                  // Default auto-restart hr/min; 01-Mar-2018, DKF
uint8_t restartMin = 0;
uint8_t heapThreshold = 5120;                           // Free heap limit, used to trigger restart when heap gets low; 01-Mar-2018, DKF

//Thingspeak Settings
//const String THINGSPEAK_CHANNEL_ID = "XXXXX";
//const String THINGSPEAK_API_READ_KEY = "XXXXXXXXXXXXXXXX";

//  Various time constants used for updating
const int UPDATE_INTERVAL_SECS = 10 * 60;                // Update current observations every 10 minutes
const int UPDATE_INTERVAL_FORECAST_SECS = 1 * 60 * 60;   // Update forecast every hour
//const int UPDATE_INTERVAL_FORECAST_SECS = 2 * 60 * 60;   // Update forecast every 2 hours
//const int UPDATE_INTERVAL_ASTRO_SECS = 2 * 60;           // Update astronomy data every 2 minutes (testing)
const int UPDATE_INTERVAL_ASTRO_SECS = 12 * 60 * 60;     // Update astronomy data 2 times daily
const int UPDATE_INTERVAL_ALERTS_SECS = 5 * 60;          // Update alerts data every 5 minutes
uint8_t UPDATE_MID_PANEL_INTERVAL_SECS = 6;              // Update mid-panel every 6 seconds; changed from const to uint8 to allow changes via web-config; 26-Jan-2018, DKF
uint8_t UPDATE_BOT_PANEL_INTERVAL_SECS = 10;             // Update bottom-panel every 10 seconds; changed from const to uint8 to allow changes via web-config; 26-Jan-2018, DKF
//const int UPDATE_CLOCK_SECS = 15;                        // Update clock display every 15 seconds
const int UPDATE_CLOCK_SECS = 1;                         // Update clock display every second
uint32_t RESTART_INTERVAL_SECS = 14 * 24 * 60 * 60;      // Automatic restart frequency (set to every 2 weeks (14 days x 24 hrs x 60 mins x 60 secs) ); 01-Mar-2018, DKF
// uint32_t RESTART_INTERVAL_SECS = 2 * 60;              // Automatic restart frequency test (2 mins); 01-Mar-2018, DKF

// flags changed in the ticker functions based on frequency above
bool readyForWeatherUpdate = false;
bool readyForForecastUpdate = false;
bool readyForAstronomyUpdate = false;
bool readyForAlertsUpdate = false;
bool readyForMidPanelUpdate = false;
bool readyForBotPanelUpdate = false;
bool readyForRestart = false;                             // Added to allow automatic restart to clear heap fragmentation...DKF, 01-Mar-2018

// For controlling rotating mid-panel
int midPanelNumber = 0;
int maxMidPanels = 5;

// For controlling rotating bottom-panel
int botPanelNumber = 0;
//int maxBotPanels = 6;
int maxBotPanels = 2;
bool skipAlertUpdate;     // flag to trigger skipping panel wait if no alerts are present

// List, so that the downloader knows what to fetch
String wundergroundIcons [] = {"chanceflurries","chancerain","chancesleet","chancesnow","clear","cloudy","flurries","fog","hazy","mostlycloudy","mostlysunny","partlycloudy","partlysunny","rain","sleet","snow","sunny","tstorms","unknown"};

/***************************
 * End Settings
 **************************/
