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

****  This is a development version targeting the 2.8" ILI9341 LCD display with an XPT2046 touch overlay ****
  
Modifed to include configuration portal on-demand when the WeMos reset button is pressed twice.
    Modified by DK Fowler ... 20-Nov-2016
    
    This method works well on Wemos boards which have a single reset button on board. It avoids using a pin for launching the configuration portal.

    How It Works
    When the ESP8266 loses power all data in RAM is lost but when it is reset the contents of a small region of RAM is preserved. So when the device 
    starts up it checks this region of ram for a flag to see if it has been recently reset. If so it launches a configuration portal, if not it sets 
    the reset flag. After running for a while this flag is cleared so that it will only launch the configuration portal in response to closely spaced resets.

    Settings
    There are two values to be set in the sketch.

    DRD_TIMEOUT - Number of seconds to wait for the second reset. Set to 10 in the example.
    DRD_ADDRESS - The address in RTC RAM to store the flag. This memory must not be used for other purposes in the same sketch. Set to 0 in the example.

    This example, contributed by DataCute needs the Double Reset Detector library from https://github.com/datacute/DoubleResetDetector .

Modified by DK Fowler ... 22-Nov-2016
    Added code to allow configuration of additional parameters, including Weather Underground parameters (API key, language, city/PWS,
    and country), and UTC offset for adjusting time from GMT.
    Note:  This version works with the exception of the time-setting routines.  The TimeClient class is instantiated with the UTC offset, which
    is an issue when the UTC offset is obtained within the SETUP function (vs. globally; the local scope causes issues when the time is obtained
    later on update).  Rather than resolve this, I've moved to another version that uses a different method and library to get the date/time,
    but more importantly, also adjusts for DST.

Modified by DK Fowler ... 30-Nov-2016
    Added a number of customizations made by Neptune, as per the latest example on Github for the WeatherStation.  This hopefully addresses
    two issues from the last version:  the global scope for the TimeClient class (no longer used), and the automatic adjustment for DST.
    Following are comments from this release made by Neptune (though not all of these changes have been added here).
 * Customizations by Neptune (NeptuneEng on Twitter, Neptune2 on Github)
 *  
 *  Added Wifi Splash screen and credit to Squix78
 *  Modified progress bar to a thicker and symmetrical shape
 *  Replaced TimeClient with built-in lwip sntp client (no need for external ntp client library)
 *  Added Daylight Saving Time Auto adjuster with DST rules using simpleDSTadjust library
 *  https://github.com/neptune2/simpleDSTadjust
 *  Added Setting examples for Boston, Zurich and Sydney
 *  Selectable NTP servers for each locale
 *  DST rules and timezone settings customizable for each locale
 *  See https://www.timeanddate.com/time/change/ for DST rules
 *  Added AM/PM or 24-hour option for each locale
 *  Changed to 7-segment Clock font from http://www.keshikan.net/fonts-e.html   (***Note: font not implemented for the ILI9341 TFT display here...DKF)
 *  Added Forecast screen for days 4-6 (requires 1.1.3 or later version of esp8266_Weather_Station library)
 *  Added support for DHT22, DHT21 and DHT11 Indoor Temperature and Humidity Sensors
 *  Fixed bug preventing display.flipScreenVertically() from working
 *  Slight adjustment to overlay

Modified by DK Fowler ... 01-Dec-2016
    This version implements timers (tickers) for setting when updates are required for the current
    weather, forecast, and astronomy information.  This allows setting different frequencies for 
    the updates, as the forecast data only needs updating every couple hours, and the astronomy
    data only a couple of times each day.  This helps to cut down on the number of calls to the
    API for WeatherUnderground, which is restricted for free use.

    Also implemented a timer for scrolling through the various mid-panel displays, alowing for 
    cycling through 9-day forecast information (vs. the original 3-days).

    Finally, implemented a timer for scrolling through various bottom-panel displays, allowing for
    display of current observation details and detailed current forecast text (future) along with
    astronomy data.

Modified by DK Fowler ... 04-Dec-2016
    Added significant additional information and tweaks to the display.  Added routines to display
    panels for the forecast text for the next 3 forecast periods (36 hours or so).  Added feels-like
    temperature and weather station to the display.  Display of the forecast text required implementing
    a word-wrap function to allow cleanly breaking the text on the display panel.

Modified by DK Fowler ... 08-Dec-2016
    Added custom parameter to the configuration screen for WiFiManager to allow designating city locale for
    setting time zone and DST rules.  Added custom labels for the new input fields on the form, including
    a new header file (TimeZone.h) that displays several lines of input help for this field.  Note that
    the only current legitimate options for this field are for the US time-zones, though this can easily
    be extended for international locales/rules.  Also cleaned up a couple of other display bugs, including
    overwrites on the text for the progress bar for .BMP downloads.

    Finally, corrected counts for the downloaded icons (currrent 19 specified for the weather icons, large
    and small; 23 for the moon-phase icons.)
 	
Modified by DK Fowler ... 12/15-Dec-2016
	Corrected errors with DST rules for Hawaii (DST not currently implemented), and Arizona (most of state 
	does not currently implement DST).  Corrected minor display overwrite issue with "am/pm".
	
Modified by DK Fowler ... 16-Dec-2016
	Added Japan/Tokyo time-zone.

Modified by DK Fowler ... 19-22-Dec-2016
  Added logic for enhancements to request active weather alerts.  This includes several new functions added
  to the Weather Underground library to obtain this information.

  Corrected several bugs from prior release, which included improperly setting the METRIC flag used for retrieving
  Weather Underground data.  This included adding a new function to the Weather Underground client library
  to allow initialization of this setting after instantiation of the client (after retrieving the settings from
  either the SPIFFS config file or the config web-page.  Added functions to the WU client library to allow 
  retrieving forecast details based on this setting (previously, only non-metric forecast details were returned
  regardless).

  Corrected a minor display formatting issue with sunrise/sunset, moonrise/moonset in the WU client library
  which previously returned blanks for the hours/minutes when these were "0".

  Added additional functionality to the display word-wrap routine that allows breaking long text displays either
  at a blank or ellipses (...); these were frequently returned in the weather alerts data just added.  This
  function also now returns the number of display panels for the display string processed (useful for determining 
  additional processing that may need to be performed as a result of multiple display panels).

  Added probability of precipitation (PoP) to both WU client library and to the display of the forecast panels.

Modified by DK Fowler ... 02-Jan-2017
  Temporarily corrected minor bug with missing icon for "chancetstorms".  Added code to check for this condition and
  replace the icon with the one for "tstorms" as a temporary workaround until the correct icon is loaded.

Modified by DK Fowler ... 05-Jan-2017
  Minor tweak to the last observation date/time to prevent it from being partially truncated at bottom of the screen.

Modified by DK Fowler ... 08-Jan-2017
  Removed loading of icons from squix78's website, and instead now depend on the SPIFFS data upload tool to load icons
  into SPIFFS.  As part of this effort, created missing icons (both large and mini format) for "chancetstorms" and
  included these.  Added icons for the current conditions display for nighttime, to replace those showing a sun with
  the moon.  Added routine to calculate whether it is currently nighttime for this determination.
  Also added splash screen to credit Weather Underground and main developers / contributors.

  Also added some additional error checking and retry logic around setting date/time.  This was to handle sporadic failures
  to update date/time, which ultimately would result in replacing the default daytime icon with one for night on occasion.

  As part of the restructuring of how weather icons are loaded, note the change in the location of placement into individual
  subdirectories in SPIFFS.  This was done for better organization.  Also note that there is apparently a length restriction
  on the total pathname to SPIFFS files (somewhere around 31 characters).  If this is exceeded, the file will not be found.

Modified by DK Fowler ... 28-Jan-2017
  Corrected a number of bugs in the display of weather icons.  The original Weather Underground library code returned the same
  icon values for several states (e.g., "sleet" and "snow").  Modified WU client and associated function in this code to uniquely
  return values for each weather state.  This allows unambiguous display of the correct weather icon (both for the current
  conditions and for the 10-day forecast mini-icons).  Also added additional error checking so that if calls to Weather
  Underground fail to return valid data, the update flag is set to retry immediately rather than wait for the next update cycle.
  (This was to deal with occasional situations where the API call fails, and invalid results were displayed until the next update
  cycle.)  Finally, corrected another minor overlay bug with the display of the "Feels like" temperature.

Modified by DK Fowler ... 01-Feb-2017
  Added WiFi signal strength indicator to upper-right corner of display.  The calculation of % strength is from squix78's original
  code provided for the OLED .96" display, though the display routine was rewritten to mock that of a cell-phone signal strength
  indicator.  Constants can easily be modified in the beginning of the display routine to change number of bars, x/y position of
  the display, height / width of the bars, and colors of both the active signal and background bars.

  Also corrected minor bug that occurred occasionally when the number of lines in the forecast text exceeded the display frame.  This
  occurred because a header line indicating the current 12-hour period was first displayed in the frame, effectively reducing the
  remaining line capacity by one.  Corrected by adding an optional parameter to the function call to wrap the text that allows
  passing the number of header lines already printed.

Modified by DK Fowler ... 17-Mar-2017
  Changed the way that weather alert notifications are displayed.  Modified routine so that the indicator in the upper-left panel is
  now shown as inverse (black text on colored background).  Note that this isn't supported directly by the graphics library except for
  the default font, so I have coded it "manually" to work for other fonts.  
  
  Also corrected minor issues with the prototype declarations(drawTime was the original routine from squix78's code; changed to drawDateTime).
  This had caused some compile errors in earlier versions of the IDE.  Corrected rare timing/display bug that showed up when more than one
  alert was active; this resulted in the last page of the first alert(s) that were truncated to have the last page only flashed on the 
  display as a delay wasn't holding it for the proper display time.  Corrected rare display bug for long, wrapped text (such as detailed
  forecast and weather alerts) that occasionally caused the display line to be too long to fit the display and wrap.  This should now be
  detected and the line-break correctly calculated.  Finally, corrected bug in the way exceptionally-long alert descriptions were clipped
  not enough room on the display to show the full description).  This is (hopefully) now detected and the line is correctly broken/wrapped.

Modified by DK Fowler ... 22-Mar-2017
  Made further refinements to the stringWordWrap function to allow passing an optional parameter to indicate the alignment of the passed 
  string segment, which still defaults to LEFT.  This allows breaking header lines for display of the weather alerts but using a CENTER
  alignment.  This change necessitated changing the display approach from using tft.print's to ui.displayString's.

  Also added indicator ("+") to the last active weather alert indicator in the upper-left alert panel if the number of active alerts exceeds
  5.  This is due to the fact that the panel size is restricted in size with room only for 5 alert indicators.

  Finally added new function to the Weather Underground library getForecastIconText, which returns the text of the icon to be displayed
  rather than the previous short-hand single character.  This avoids having to translate this back via the original getMeteoconIcon function
  in order to determine which icon .BMP to display.  This was a workaround to avoid having to change the function by the same name in the
  Weather Underground library, which would break older code examples.  This function previously returned ambiguous values for several weather
  states, resulting in the incorrect icon being display (e.g., "sleet" icon for both sleet and snow).

Modified by DK Fowler ... 08-Apr-2017
  Further refinements made to stringWordWrap function to correct a bug where a delay for the last display was not occurring, causing the last
  display segment to just briefly flash on the screen.  Added an additional optional parameter to the function to indicate that the message
  was truncated (such as with long weather alert texts).  The JSON parser only provides 512-bytes maximum for the parsed text.  Moved the display
  of the truncation flag to the stringWordWrap function to simplify the determination of a final delay in this instance.
  
  Replaced the displayBMP routine in the tab GfxUi.cpp with a modified routine to support changes that were made in the Adafruit GFX and ILI9341
  display libraries to support start/end transaction.  This was apparently done during a series of updates to support the ESP32 and the library
  updates caused the BMP display routines to garble the icon displays.
  
  Also, restored the original WebResource routine call to allow checking for and downloading icons that may be missing from squix's site.  (
  These are now normally loaded externally using the ESP8266 Sketch Data Upload tool.)

Modified by DK Fowler ... 12-Apr-2017
  Removed include for TimeLib library, as there are no new dependencies on this library.  The code now uses the original core time library 
  provided with the ESP8266 board support.

Modified by DK Fowler ... 11-May-2017
  Added support for a touch-screen (specifically, the XPT2046 as implemented using SPI for the ILI9341, such as the 2.4 or 2.8" resistive-touch
  display.  Wiring and code borrowed from Daniel Eichorn's Plane-Spotter project (thanks Dani!).  
  The wiring and pin assignments for the WeMos D1 Mini:
  
      WeMos D1 Mini         ILI9341
      =============         =======
      D4                    T_IRQ
      D6                    T_DO
      D7                    T_DIN
      D3                    T_CS
      D5                    T_CLK
      D6                    SDO
      D8                    LED
      D5                    SCK
      D7                    SDI
      D2                    D/C
      RST                   RESET
      D1                    CS
      GND                   GND
      3V3                   VCC

Modified by DK Fowler ... 22-Jan-2018
  Reversed the display of the high/low temperatures on the forecast panels.  This was to follow a more logical approach, as the low temperature is generally for the
  following "nighttime", whereas the high is for the day's "daytime" high.  Also added a display line for the current day's high/low temp beneath the "feels like" temp
  for the current day's display so that it is always displayed.  Modified display of weather station name so that if it contains the string "pws:", don't display it
  twice (previously, "PWS:" was automatically added to the beginning of the display name).

  Adding the current day's high/low required tweaking the positioning of some of the existing on-screen elements, including those for "erasing" current screen regions.

  Continued development of the touch-screen enhancements, including detection of specific regions that are touched.  Added routine to toggle display of the WiFi strength
  indicator, based on touching the upper-right region of the screen.

Modified by DK Fowler ... 23-Jan-2018
  Modified the display of the initial configuration message written to the LCD display if a config file is not found on SPIFFS; previously, the message was too long
  for the display width.
  Added brief (500ms) delay following TFT initialization in an attempt to minimize the occasional "white screen" when power is applied.  The theory here is that the
  TFT initialization must complete before any attempts to write to the screen, otherwise, a white screen situation results.
  Continued development of the touch-screen features...
  Added routine to check for a touch-screen calibration file at startup; if it exists, then the contents are read and used to map the display to the touch-screen coordinates.
  Otherwise, a routine is initiated to get the upper-left and lower-right extents of the touch-screen, and the calibration information is saved.
  Added touch-screen function to allow display of detailed forecast information based on touching the appropriate mid-panel region for a given displayed icon.  This routine will
  hold the display until the screen is touched again before refreshing.

Modified by DK Fowler ... 26-Jan-2018
  Added parameters to the web configuration screen to allow custom settings for the mid- and bottom-panel delays.  These can be easily customized to your own preferences
  (settings from 1-999 secs) to allow more time to read the panels as desired.  The defaults are still specified in the settings.h tab.

Modified by DK Fowler ... 27-Jan-2018
  Moved block of code that creates the custom parameters for use in the web configuration portal to immediately before the start of the portal.  This allows any already
  stored values to be used as the defaults for the configuration portal (as opposed to the hard-coded default values in setting.h that were used previously).  Caution
  should be used in changing these values, as extensive validation of the input parameters are not done prior to storing, so it may be possible to inadverdently corrupt
  the stored config values (e.g., deleting the WU API key, or setting it to be invalid).

Modified by DK Fowler ... 29-Jan-2018
  Added touch-detect regions for the date/time region, the current conditions region, and the bottom panel.  A long touch (>3 secs) on the date/time region now initiates
  the web-configuration routine.  A touch on the current conditions will display the detailed forecast day/night screen for today (just as touching the date in the
  forecast panel will do).  A long touch (>3 secs) on the bottom panel will display detailed statistics on the ESP.  (This is a minor re-write of the original code
  from Daniel Eichorn's drawAbout() routine, adjusted for lack of the miniGrafx library.  Thanks again Dani!)

Modified by DK Fowler ... 16-Feb-2018
  Added logic to the display WiFi quality routine to display "0%" if the WiFi is currently disconnected.

Modified by DK Fowler ... 28-Feb-2018
  Added flag to allow deferring updates to the alert data if the alerts are in the process of being displayed; this is to avoid potential conflicts in updating
  the alert data as it is being displayed.

  Also, added SSID and local IP address for the station to the "ESP statistics" page when invoked via the touch-screen.

Modified by DK Fowler ... 01-Mar-2018
  Added auto-restart functionality to allow automatic reboots for the weather station based on a set schedule (number of seconds, default set to every 14 days).  This
  interval can now be set on the web configuration page, along with the time-of-day to reboot once this interval expires.  This feature was added to allow periodically
  clearing heap fragmentation that occurs during normal operation.  Also added a threshold for free heap (set to 5kb by default).  If the free heap drops below this
  threshold, the station will restart.

Modified by DK Fowler ... 02-Mar-2018
  Added check to disconnect the WiFi if currently connected prior to initiating the web configuration portal as requested via touch.  Previously, if the WiFi was
  currently connected, storing of the WiFi credentials was bypassed, essentially never allowing changes to previously stored credentials.
  
Modified by DK Fowler ... 05-Mar-2018
  Changed hard delays in display of alert message panels to instead call a new delay routine that includes a call to check if the display is touched.  This was to 
  allow touch interrupts during the display of the alert messages where they previously were disabled during the panel delays.  Moved the check for a screen touch
  and logic for which display routine to execute as a result depending on the screen region to a separate function vs. the mainline loop.

Modified by DK Fowler ... 09-11-Mar-2018
  Added logic to hold the current color and cursor position at the start of the new delay routine, such that these can be restored following the delay.  This is
  due to the fact that a requested display screen initiated by touch could change these attributes, and these must be restored for subsequent display "pages", such
  as during the display of multiple alert message panels.  Added some minor tweaks to the display of the alert truncation flag such that this does not overwrite
  the last characters of the alert message.

  Broke out the updating of the current conditions data from the display of this data.  This allowed minimizing the API calls to WU while executing touch-initiated
  displays, such that data updates (API calls) are made only when necessary.  This is accomplished through timing the "stall time" while a touch-initiated display
  appears; if the stall time is greater than the typical update cycle, than the data is refreshed upon return.  Otherwise, the screen panels alone are refreshed.

  Changed the new delay routine to check for screen touches as well to exit the delay loop (vs. time alone); this allows for a faster refresh of the screen panels
  following touch-initiated displays.

  Moved the display of the weather station from the time-display routine to the display routine for the current conditions panel (a more logical location which allows for
  better refreshes.

Modified by DK Fowler ... 14-Mar-2018
  Removed storing of current text color from touch-initiated "drawAbout" routine, which provides details on the ESP and last reset condition.  This corrected a minor bug
  that caused the incorrect text color to be restored during the delayAndCheckTouch routine, resulting in the remaining alert message text to be displayed in white, if
  initiated via touch during the display of alert messages in the bottom panel.

  "Wrapped" the WiFi.disconnect added in the web configuration portal logic (see 2-Mar-18 comments above) with WiFi.persistent(false)/(true) statements.  This was to
  prevent stored WiFi credentials from getting wiped when WiFi was disconnected, even if these are not changed.  This could have manifested previously if the web
  config portal was initiated, but essentially cancelled via booting the ESP without entering credentials again.

Modified by DK Fowler ... 12->19-Dec-2018  
  This is a major re-write in order to change the data source from Weather Underground to Open Weather Map.
  This was necessitated due to IBM (now owner of WU) removing no-cost access to the WU data.
  
  Unfortunately the data provided by Open Weather Map is a MUCH-reduced dataset of what was provided by 
  WU.  It does not include text-based forecasts, weather warnings/alerts, and much of the detailed info
  that was provided and displayed in prior releases of this code.
  
  This version utilizes the standard weather station library written by Daniel Eichorn (and Marcel at ThingPulse).
  Note that this is a rushed-conversion to beat the end-of-year deadline at which time IBM will no longer provide free
  access to WU data, and has therefore not been thoroughly tested.

Modified by DK Fowler ... 31-Dec-2018
  Added additional code around the checks for invalid data returned from OWM.  Previously, the code continued to rapidly retry until valid data was returned,
  which resulted in a continuous loop condition if the data was never returned.  Now, if invalid data is detected, a retry will occur only after waiting 10 seconds,
  and after 10 retry attempts, the ESP will restart.  This should allow manual initiation of the configuration portal for instance, if the source of the error
  is an invalid configuration.  Messages are also displayed on the screen and serial port to indicate the retry attempts.
  
*/

#include <FS.h>
#include <Arduino.h>

#include <Adafruit_GFX.h>         // Core graphics library
#include <Adafruit_ILI9341.h>     // Hardware-specific library
#include <XPT2046_Touchscreen.h>  // For touch-screen
#include <SPI.h>
#include <math.h>                 // Core math functions

// Additional UI functions

#include "GfxUi.h"

// Fonts created by http://oleddisplay.squix.ch/
#include "ArialRoundedMTBold_14.h"
#include "ArialRoundedMTBold_36.h"
#include "Droid_Sans_10.h"
#include "Droid_Sans_Bold_10.h"
#include "Droid_Sans_12.h"
//#include "DSEG7Classic-BoldFont.h"

// Download helper
#include "WebResource.h"

#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

// Helps with connecting to internet
#include <WiFiManager.h>

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

// check settings.h for adapting to your needs
#include <JsonListener.h>
#include <OpenWeatherMapCurrent.h>
#include <OpenWeatherMapForecast.h>
#include <Astronomy.h>
#include <SunMoonCalc.h>

#include "settings.h"
//#include <WundergroundClient.h>
#include "time.h"
//#include <TimeLib.h>

//#include <DoubleResetDetector.h>  //https://github.com/datacute/DoubleResetDetector
#include <simpleDSTadjust.h>
#include "TimeZone.h"

/*********************************************************
 * Important: see settings.h to configure your settings!!!
 * *******************************************************/

ADC_MODE(ADC_VCC);

//DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);

// Debug for panel updates
int lastPanelMillis = 0;

//flag for saving data
bool shouldSaveConfig = false;

// Define structure for storing time information
struct tm * timeinfo;
time_t dstOffset = 0;
simpleDSTadjust dstAdjusted(StartRule, EndRule);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
GfxUi ui = GfxUi(&tft);

XPT2046_Touchscreen ts(TOUCH_CS);  // Param 2 - NULL - No interrupts
//XPT2046_Touchscreen ts(TOUCH_CS, 255);  // Param 2 - 255 - No interrupts
//XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);  // Param 2 - Touch IRQ Pin - interrupt enabled polling

// Calibration coordinates for touchscreen
uint16_t  minCoordX, maxCoordX, minCoordY, maxCoordY, touchX, touchY;
// Timing for how long screen is touched, in millis...
uint32_t   touchTimeMS, startTouchTime;

// Flag for displaying WiFi strength signal
bool displayWiFiSignalFlag;

// Touch region
byte touchRegion;

// flag changed in the ticker function every 1 minute
bool readyForDHTUpdate = false;

// For holding/resetting current text color
//    Added 09-Mar-2018, DKF
uint16_t currTextColor;

WebResource webResource;

// Indicates whether ESP has WiFi credentials saved from previous session, or double reset detected
bool initialConfig = false;

// Set to false, if you prefere imperial/inches, Fahrenheit
//WundergroundClient wunderground(isMetric);

OpenWeatherMapCurrentData currentWeather;
OpenWeatherMapForecastData forecasts[MAX_FORECASTS];
Astronomy::MoonData moonData;

//** New astronomy variables
//String moonAgeImage = "";
uint8_t moonAge = 0;

// Initialize the temperature/ humidity sensor
//DHT dht(DHTPIN, DHTTYPE);
float humidity = 0.0;
float temperature = 0.0;

// Counters for error-retry attempts for data...added 31-Dec-2018, DKF
byte invalidCurrCondCnt = 0;
byte invalidAstroCnt = 0;
byte invalidForecastsCnt = 0;

// Instantiate tickers for updating current conditions, forecast, and astronomy info
volatile int forecastTimer  = 0;      // Part of workaround for ticker timer limit
volatile int astronomyTimer = 0;
volatile uint16_t restartTimer = 0;   // Added 01-Mar-2018, DKF
int stallForecastTimer = 0;           // Added 09-Mar-2018, DKF, to hold the current forecast timer when a touch display request is made
int stallAstronomyTimer = 0;          // Added 09-Mar-2018, DKF, " "
Ticker ticker;
Ticker forecastTicker;
Ticker astronomyTicker;
Ticker midPanelTicker;
Ticker botPanelTicker;
Ticker restartTicker;           // Added 01-Mar-2018, DKF


//declaring prototypes
void configModeCallback (WiFiManager *myWiFiManager);
void saveConfigCallback ();
void downloadCallback(String filename, int16_t bytesDownloaded, int16_t bytesTotal);
ProgressCallback _downloadCallback = downloadCallback;
void readSPIFFSConfig();      // Moved to a separate function vs. in-line, 28-Jan-2018, DKF
void startWebConfigPortal();  // Moved to a separate function vs. in-line, 28-Jan-2018, DKF
void downloadResources();
void updateData();
void refreshCurrentConditions();
void setDSTRules(String TZCity);
void updateForecastData();
void updateAstronomyData();
void updateMidPanel();
void updateBotPanel();
void drawProgress(uint8_t percentage, String text);
void drawWeatherStationAttributions();
void drawDateTime();
bool checkIfNight(time_t currentTime);
const char* getMeteoconIcon(String iconText);
const char* getMiniMeteoconIcon(String iconText);
String getTime(time_t *timestamp);
void drawCurrentWeather();
void drawCurrentObservations();
void drawForecast();
void drawForecast2();
void drawForecast3();
void drawForecast4();
void drawForecast5();
void drawForecastDetail(uint16_t x, uint16_t y, uint8_t dayIndex);
String getMeteoconIconDKF(String iconText);
void drawAstronomy();
void drawForecastText(int forecastTextPeriod);
void drawSeparator(uint16_t y);
void setReadyForWeatherUpdate();
void setReadyForForecastUpdate();
void setReadyForAstronomyUpdate();
void setReadyForMidPanelUpdate();
void setReadyForBotPanelUpdate();
void setReadyForRestart();            // Added 01-Mar-2018, DKF
byte stringWordWrap(String displayString, byte headerLines=0, TextAlignment txtAlign=LEFT, bool truncFlag=false, int yBase=245, int maxPanelLines=6);
void drawWiFiQuality();
int8_t getWifiQuality();
byte checkTouch(uint16_t touchX, uint16_t touchY, uint32_t startTouchTime);
void checkTouchCalibration();
bool checkScreenTouch();
void delayAndCheckTouch(uint32_t delayMillis);
void saveCalibration();
void manualInitCalibration();
void touchDisplayDayForecast(uint8_t TSdayIndex);
void diableTimers();
void restartUpdates(uint32_t displayMillis);
char* windCompassDir(float windDeg);
float hPaToInHg (float pressHPa);
float windChill (float currTemp, float currWindSpeed);
float heatIndex (float currTemp, float currHum);


long lastDownloadUpdate = millis();

void setup() {
  Serial.begin(115200);

  // The LED pin needs to set HIGH
  // Use this pin to save energy
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  readSPIFFSConfig();     // Moved to separate function, 28-Jan-2018, DKF
  
  tft.begin();    // display
  delay(500);     // to see if this helps some of the initial "white screen" displays at power-up...added 23-Jan-2018, DKF
  ts.begin();     // touchscreen

  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&ArialRoundedMTBold_14);
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  currTextColor = ILI9341_ORANGE;
  ui.setTextAlignment(CENTER);

  configPortalHostname = "ESP" + String(ESP.getChipId(), HEX);
  Serial.print("Hostname:  "); Serial.println(configPortalHostname);
  
  if (WiFi.SSID()==""){
    Serial.println("No stored access-point credentials; initiating configuration portal.");   
    tft.fillScreen(ILI9341_BLACK);
    ui.drawString(120, 56, "No stored access-point");             // Tweaked message display, previously was too long for display width...DKF, 23-Jan-2018
    ui.drawString(120, 74, "credentials...");
    ui.drawString(120, 92, "Initiating configuration portal.");
    delay(2000);
    initialConfig = true;
  } // else                                                          // Changed to else to ensure both conditions/messages don't fire...DKF, 28-Feb-2018
//  if (drd.detectDoubleReset()) {
//    Serial.println("Double-reset detected...");
//    tft.fillScreen(ILI9341_BLACK);
//    ui.drawString(120, 56, "Double-reset detected...");
//    ui.drawString(120, 70, "Initiating configuration portal.");
//    delay(1000);
//    initialConfig = true;
//  }
  if (initialConfig) {
    startWebConfigPortal();     // Moved to separate function, 28-Jan-2018, DKF
  }

//  Test whether touch-screen calibration has been done, and if not, execute and store the results
  checkTouchCalibration();
  
  drawWeatherStationAttributions();   // Display attributions including WU logo at startup...

  WiFi.mode(WIFI_STA);        // Force to station mode because if device was switched off while in access point mode it will start up next time in access point mode.
  WiFi.hostname(WiFiHost);    // Added 19-Dec-2018 to resolve DHCP issues after connecting?

  unsigned long startedAt = millis();
  Serial.print("After waiting ");
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&ArialRoundedMTBold_14);
  tft.setTextColor(ILI9341_ORANGE);
  currTextColor = ILI9341_ORANGE;
  
  ui.drawString(120, 160, "Connecting to WiFi");

  int connRes = WiFi.waitForConnectResult();
  float waited = (millis()- startedAt);
  Serial.print(waited/1000);
  Serial.print(" secs in setup() connection result is ");
  Serial.println(connRes);
  WiFi.printDiag(Serial);
  if (WiFi.status()!=WL_CONNECTED){
    Serial.println("Failed to connect, finishing setup anyway.");
  } else {
    Serial.print("Connected...local ip: ");
    Serial.println(WiFi.localIP());
    Serial.print("SSID:  "); Serial.println(WiFi.SSID());
    tft.fillScreen(ILI9341_BLACK);
    ui.drawString(120, 160, "Connected...");
    ui.drawString(120, 178, "SSID " + (String(WiFi.SSID()) ));
    delay(1000);
  }
  
  // OTA Setup
  OTAHostName = OTAHostName + String(ESP.getChipId(), HEX);      // Changed to be unique from config portal hostname, 28-Jan-2018, DKF
  ArduinoOTA.setHostname((const char *)OTAHostName.c_str());
  ArduinoOTA.begin();

  // download images from the net. If images already exist don't download
  downloadResources();

//  Debug alerts...
//  strcpy(TZ_CITY, "Mountain");
//  strcpy(WUNDERGROUND_CITY,"pws:KNDELLEN3");  // for testing weather alerts processing
//  strcpy(WUNDERGROUND_CITY,"pws:KIDGRACE9");  // for testing weather alerts processing
//  strcpy(WUNDERGROUND_COUNTRY,"NO");
//  strcpy(WUNDERGROUND_CITY,"Svene");          // for testing weather alerts processing
  
  // set the start/end rules for Daylight Savings Time based on the city selected
  setDSTRules(TZ_CITY);
  
  // load the weather information
  updateData();               // Current observations (time, current conditions)
  refreshCurrentConditions(); // Draws/refreshes current conditions region; added 09-Mar-2018 as a separate routine from data updates
  updateForecastData();       // 5-day forecast data
  updateAstronomyData();          // Moon age, illumination, etc.
  //updateAlertsData();         // Active weather alerts

  //  Convert the screen panel delays from the stored configuration to integer values to attach timers...
  //    Added 26-Jan-2018, DKF
  if (String(MID_PANEL_DELAY).toInt() > 0 && String(MID_PANEL_DELAY).toInt() <= 999) {      // If the converted value is between 1-999 secs, use it, otherwise
    UPDATE_MID_PANEL_INTERVAL_SECS = String(MID_PANEL_DELAY).toInt();                       // use the default value from the settings.h initialization
  }
  if (String(BOT_PANEL_DELAY).toInt() > 0 && String(BOT_PANEL_DELAY).toInt() <= 999) {      // If the converted value is between 1-999 secs, use it, otherwise
    UPDATE_BOT_PANEL_INTERVAL_SECS = String(BOT_PANEL_DELAY).toInt();                       // use the default value from the settings.h initialization
  }

  //  Convert the restart time from the stored configuration to integer values to check against current time...
  //    Added 01-Mar-2018, DKF
  String restartHrStr = String(RESTART_TIME[0]);
  restartHrStr += String(RESTART_TIME[1]);
  String restartMinStr = String(RESTART_TIME[3]);
  restartMinStr += String(RESTART_TIME[4]);
  if (restartHrStr.toInt() > 0 && restartHrStr.toInt() <= 23) {                             // If the converted hour value is between 0-23, use it, otherwise
    restartHr = restartHrStr.toInt();                                                       // use the default value from the setting.h initialization
  }
  if (restartMinStr.toInt() > 0 && restartMinStr.toInt() <= 59) {                           // If the converted minute value is between 0-59, use it, otherwise
    restartMin = restartMinStr.toInt();                                                     // use the default value from the setting.h initialization
  }

  //  Convert the restart interval in seconds from the stored configuration to an integer value...
  //    Added 01-Mar-2018
  if (String(RESTART_INTERVAL).toInt() >= 0 && String(RESTART_INTERVAL).toInt() <= 4294967295 ) { // If the converted restart interval is between 0-4294967295, use it,
    RESTART_INTERVAL_SECS = String(RESTART_INTERVAL).toInt();                                     // otherwise, use the default value from the setting.h initialization
  }
  
//  Debug
//  Serial.print("***Converted int mid-panel delay:  "); Serial.println(UPDATE_MID_PANEL_INTERVAL_SECS);
//  Serial.print("***Converted int bot-panel delay:  "); Serial.println(UPDATE_BOT_PANEL_INTERVAL_SECS);
//  delay(5000);
  
  ticker.attach(UPDATE_INTERVAL_SECS, setReadyForWeatherUpdate);                    // Now for current observations update only
  midPanelTicker.attach(UPDATE_MID_PANEL_INTERVAL_SECS, setReadyForMidPanelUpdate); // To trigger updating mid-panel on screen
  botPanelTicker.attach(UPDATE_BOT_PANEL_INTERVAL_SECS, setReadyForBotPanelUpdate); // To trigger updating bottom-panel on screen

//  Note the triggering times (60 secs) for each of the following tickers.  This is a workaround for the
//  maximum timer limitation for the ticker function (approximately 71 minutes).
  forecastTicker.attach(60, setReadyForForecastUpdate);           // For controlling 10-day forecast update
  astronomyTicker.attach(60, setReadyForAstronomyUpdate);         // For controlling astronomy data update
  restartTicker.attach(60, setReadyForRestart);                   // For controlling automatic restart to clear heap fragmentation; 01-Mar-2018, DKF
  
}

long lastDrew = 0;
void loop() {
  // Handle OTA update requests
  ArduinoOTA.handle();

  // Call the double reset detector loop method every so often,
  // so that it can recognise when the timeout expires.
  // You can also call drd.stop() when you wish to no longer
  // consider the next reset as a double reset.
  // drd.loop();

  // Check if we should update the clock
//  if (millis() - lastDrew > 30000 && wunderground.getSeconds() == "00") {
  if (millis() - lastDrew > (UPDATE_CLOCK_SECS * 1000)) {
    drawDateTime();
    lastDrew = millis();
  }

  // Check if we should update weather information
//  if (millis() - lastDownloadUpdate > 1000 * UPDATE_INTERVAL_SECS) {
  if (readyForWeatherUpdate) {
      updateData();
      refreshCurrentConditions();       // Added 09-Mar-2018, as a separate routine from data updates
//      lastDownloadUpdate = millis();
  }
  if (readyForForecastUpdate) {
    updateForecastData();
  }

  if (readyForAstronomyUpdate) {
    updateAstronomyData();
  }

  //if (readyForAlertsUpdate) {
  //  updateAlertsData();
  //}

  if (readyForMidPanelUpdate) {
    updateMidPanel();
  }

  if (readyForBotPanelUpdate) {
    updateBotPanel();
  }

  if (readyForRestart) {            // Added to allow automatic restart to clear heap fragmentation; 01-Mar-2018, DKF
    Serial.println("Automatic restart to clear heap fragmentation triggered...");

    // Display message on TFT
    tft.fillScreen(ILI9341_BLACK);
    ui.setTextAlignment(CENTER);
    tft.setFont(&Droid_Sans_12);
    tft.setTextColor(ILI9341_WHITE);
    currTextColor = ILI9341_WHITE;

    ui.drawString(120, 160, "Auto-restart time");
    delay(2000);            // Pause to show restart message
    
    ESP.restart();          // Modified by DK Fowler ... 20-Dec-2018, from ESP.reset in order to do a "cleaner" restart
    delay(5000);
  }

  //  Check free heap...if below set threshold, restart to clear; 01-Mar-2018, DKF
  if ( ESP.getFreeHeap() <= heapThreshold) {
    Serial.println("Automatic restart to free heap triggered...");

    // Display message on TFT
    tft.fillScreen(ILI9341_BLACK);
    ui.setTextAlignment(CENTER);
    tft.setFont(&Droid_Sans_12);
    tft.setTextColor(ILI9341_WHITE);
    currTextColor = ILI9341_WHITE;
    
    ui.drawString(120, 142, "Auto-restart to");
    ui.drawString(120, 160, "free memory");
    delay(2000);            // Pause to show restart message
    
    ESP.restart();        // Modified by DK Fowler ... 20-Dec-2018, from ESP.reset in order to do a "cleaner" restart
    delay(5000);
    
  }

  checkScreenTouch(); 
  
  delay(20);          // Ensure WiFi is serviced

}

// Called if WiFi has not been configured yet
void configModeCallback (WiFiManager *myWiFiManager) {
  ui.setTextAlignment(CENTER);
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&ArialRoundedMTBold_14);
  tft.setTextColor(ILI9341_ORANGE);
  currTextColor = ILI9341_ORANGE;
  
  ui.drawString(120, 28, "Wifi Manager");
  ui.drawString(120, 42, "Please connect to AP");
  tft.setTextColor(ILI9341_WHITE);
  currTextColor = ILI9341_WHITE;
  
  ui.drawString(120, 56, myWiFiManager->getConfigPortalSSID());
  // ui.drawString(120, 56, WiFi.configPortalHostname());
  tft.setTextColor(ILI9341_ORANGE);
  currTextColor = ILI9341_ORANGE;
  
  ui.drawString(120, 70, "To setup Wifi Configuration");
  tft.setFont(&Droid_Sans_12);
  tft.setTextColor(ILI9341_CYAN);
  currTextColor = ILI9341_CYAN;
  
  ui.drawString(120, 98,  "If configuration page does not");
  ui.drawString(120, 116, "appear automatically, connect to");
  ui.drawString(120, 134, "address 192.168.4.1 in browser.");
}

// callback called during download of files. Updates progress bar
void downloadCallback(String filename, int16_t bytesDownloaded, int16_t bytesTotal) {
  Serial.println(String(bytesDownloaded) + " / " + String(bytesTotal));

  int percentage = 100 * bytesDownloaded / bytesTotal;
  if (percentage == 0) {
    ui.drawString(120, 160, filename);
  }
  if (percentage % 5 == 0) {
    ui.setTextAlignment(CENTER);
    ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
    currTextColor = ILI9341_ORANGE;

    //ui.drawString(120, 160, String(percentage) + "%");
    ui.drawProgressBar(10, 165, 240 - 20, 15, percentage, ILI9341_WHITE, ILI9341_BLUE);
  }

}

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

// Download the bitmaps
void downloadResources() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&ArialRoundedMTBold_14);
  char id[5];
  for (int i = 0; i < 19; i++) {
    sprintf(id, "%02d", i);
    tft.fillRect(0, 120, 240, 45, ILI9341_BLACK);
    webResource.downloadFile("http://www.squix.org/blog/wunderground/" + wundergroundIcons[i] + ".bmp", "/Condition/" + wundergroundIcons[i] + ".bmp", _downloadCallback);
  }
  for (int i = 0; i < 19; i++) {
    sprintf(id, "%02d", i);
    tft.fillRect(0, 120, 240, 45, ILI9341_BLACK);
    webResource.downloadFile("http://www.squix.org/blog/wunderground/mini/" + wundergroundIcons[i] + ".bmp", "/Mini/" + wundergroundIcons[i] + ".bmp", _downloadCallback);
  }
  for (int i = 0; i < 23; i++) {
    tft.fillRect(0, 120, 240, 45, ILI9341_BLACK);
    webResource.downloadFile("http://www.squix.org/blog/moonphase_L" + String(i) + ".bmp", "/Moon Phases/moonphase_L" + String(i) + ".bmp", _downloadCallback);
  }
}

// Update the internet based information and update screen
//    Modified by DK Fowler ... 09-Mar-2018, to break data updates from screen refresh
void updateData() {
  tft.setFont(&ArialRoundedMTBold_14);
  drawProgress(33, "Updating time...");
  delay(500);
  Serial.printf("UTC Offset:  %d\n", UTC_OFFSET);

  configTime(UTC_OFFSET * 3600, 0, NTP_SERVERS);
  while(!time(nullptr)) {
    Serial.print("#");
    delay(100);
  }

  drawProgress(66, "Updating conditions...");

  // calculate for time calculation how much the dst class adds.
  //    Modified by DK Fowler ... 12-Dec-2018
  //  Note that the original dstOffset calculated here was an hour off, considering the returned value from the OpenWeatherMap API for sunrise/set.  Not sure whether this
  //  is due to the returned value NOT considering DST, so no need to correct for it, or if the calculation for considering the DST correction should only be applied
  //  during DST (one hour different from standard time).  For now, the UTC offset is the correct value to use for adjusting to the current time, but this may
  //  need to be reconsidered once returned values from the API can be observed DURING DST.  Unfortunately, the OWM API documentation is not specific on this point.
  // dstOffset = UTC_OFFSET * 3600 + dstAdjusted.time(nullptr) - time(nullptr);
  dstOffset = UTC_OFFSET * 3600;
  Serial.printf("Time difference for DST: %d\n", dstOffset);
  Serial.printf("DST adjusted time:  %d\n", dstAdjusted.time(nullptr));
  Serial.printf("Time:  %d\n", time(nullptr));

  OpenWeatherMapCurrent *currentWeatherClient = new OpenWeatherMapCurrent();
  currentWeatherClient->setMetric(isMetric);
  currentWeatherClient->setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  currentWeatherClient->updateCurrentById(&currentWeather, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);
  delete currentWeatherClient;
  currentWeatherClient = nullptr;
  
  //**wunderground.updateConditions(WUNDERGROUND_API_KEY, WUNDERGROUND_LANGUAGE, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY);
  drawProgress(100, "Done...");
  delay(1000);
}

// Refresh the current conditions screen region
//    Modified by DK Fowler ... 09-Mar-2018, to break data updates from screen refresh
void refreshCurrentConditions() {

//  tft.fillScreen(ILI9341_BLACK);              // This seems to sometimes leave the screen in an "all-bright" condition??
  drawDateTime();
//  tft.drawRect(0, 62, 240, 86, ILI9341_WHITE);  // Test area for wipe...
  tft.fillRect(0, 62, 240, 86, ILI9341_BLACK);  // Wipe prior data in the "current weather" panel
  drawCurrentWeather();

// Weather Station
//    Modified 11-Mar-2018, DK Fowler, to move drawing of weather station to this routine from the time-update routine
  ui.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  currTextColor = ILI9341_GREEN;

  ui.setTextAlignment(RIGHT);
  tft.setFont(&Droid_Sans_10);
//** Create display string for personal weather station.  If the station contains "pws" in the name, don't display it
// twice, otherwise, add "PWS:" to the station name.
//    Modified by DK Fowler ... 22-Jan-2018
  String temp;
  //if ( ((String(WUNDERGROUND_CITY)).indexOf("pws")) != -1) {
  //  temp = String(WUNDERGROUND_CITY);
  //} else {
  //  temp = "PWS: " + String(WUNDERGROUND_CITY);
  //}
  //ui.drawString(220, 75, temp);
  ui.drawString(220, 75, DISPLAYED_CITY_NAME);

    String weatherIcon = getMeteoconIcon(currentWeather.icon);

//** Check to see if we have valid current conditions.  If not, set the flag to update the data again.
//  if ( (getMeteoconIcon(wunderground.getTodayIcon())) == "unknown" ) {
//  if ( (wunderground.getTodayIconText() == "unknown" ) || (wunderground.getTodayIconText().length() == 0) ) {
//     Serial.println("Invalid current conditions...attempting to update...");
//     readyForWeatherUpdate = true;
//  } else {
//     readyForWeatherUpdate = false;
//  }

  // Check for icon set to "unknown" or returned length of 0...if so, assume bad data.
  //    Added by DK Fowler ... 31-Dec-2018
  if ( (getMeteoconIcon(currentWeather.icon) == "unknown" ) || (strlen(getMeteoconIcon(currentWeather.icon)) == 0) ) {
    if (invalidCurrCondCnt > 10) {
      Serial.println("Invalid current conditions retry count exceeded...restarting...");
      drawProgress(100, "Retries exceeded");
      delay(1000);
      drawProgress(100, "Restarting...");
      delay(2000);
      ESP.restart();
      delay(2000);
    } else {
      invalidCurrCondCnt++;
      Serial.println("Invalid current conditions...attempting to update...");
      drawProgress(100, "Invalid conditions"); 
      readyForWeatherUpdate = true;
      delayAndCheckTouch(10000);     // delay 10 seconds before continuing
    }
  } else {
     readyForWeatherUpdate = false;
     invalidCurrCondCnt = 0;
  }

//** Check to see if we have valid astronomy data.  If not, set the flag to update these values.
//  if ( (wunderground.getSunriseTime() == "") && 
//       (wunderground.getSunsetTime() == "") ) {
//     Serial.println("Invalid astronomy data...attempting to update...");
//     readyForAstronomyUpdate = true;
//  }

  //  Check to see if we have valid astronomy data.  If not, set the flag to update these values.
  //    Added by DK Fowler ... 31-Dec-2018
  if ( ( (moonData.phase < 0) || (moonData.phase > 8) ) || ( (moonData.illumination < 0.0) || (moonData.illumination > 100.0) ) ) {
    if (invalidAstroCnt > 10) {
      Serial.println("Invalid astronomy data retry count exceeded...restarting...");
      drawProgress(100, "Retries exceeded");
      delay(1000);
      drawProgress(100, "Restarting...");
      delay(2000);
      ESP.restart();
      delay(2000);
    } else {
      invalidAstroCnt++;
      Serial.println("Invalid astronomy data...attempting to update...");
      drawProgress(100, "Invalid astro data"); 
      readyForAstronomyUpdate = true;
      delayAndCheckTouch(10000);     // delay 10 seconds before continuing
    }
  } else {
     readyForAstronomyUpdate = false;
     invalidAstroCnt = 0;
  }

  readyForMidPanelUpdate = true;                // Update mid-panel since progress bar overwrites this.
}

void updateMidPanel() {
  midPanelNumber++;
  if (midPanelNumber > maxMidPanels) midPanelNumber = 1;
  switch (midPanelNumber) {
    case 1:
      drawForecast();
      break;
    case 2:
      drawForecast2();
      break;
    case 3:
      drawForecast3();
      break;
    case 4:
      drawForecast4();
      break;
    case 5:
      drawForecast5();
      break;
  }
  readyForMidPanelUpdate = false;
}

void updateBotPanel() {
  // Reset the flag for updating the bottom panel; this is done here externally from the ticker routine because this flag is manipulated manually to force bottom panel
  // updates when there are no alerts (to skip alert panel processing), as well as following the display of the final alert panel when they do exist.
  readyForBotPanelUpdate = false;      
  botPanelNumber++;
  if (botPanelNumber > maxBotPanels) botPanelNumber = 1;
  Serial.print("updateBotPanel:  time(ms):  "); Serial.println(millis());
  Serial.print("*** Bottom panel update: "); Serial.print(millis()-lastPanelMillis); Serial.print(" ms; panel number: "); Serial.println(botPanelNumber);
  switch (botPanelNumber) {
    case 1:
      drawAstronomy();
      break;
    case 2:
      drawCurrentObservations();
      break;
    //case 3:
      //activeAlerts = wunderground.getActiveAlertsCnt();
      //if (activeAlerts != 0) {
        // Set flag to ensure alerts are not updated until after all alert information is displayed, if alerts ticker should fire during
        // the display process....
        //    Added by DK Fowler ... 28-Feb-2018
       // alertsDisplayInProgress = true;
       // drawAlerts();
       // drawAlertsPanels();
       // skipAlertUpdate = false;
        //  Reset flag to resume normal alerts updating...
        //    Added by DK Fowler ... 28-Feb-2018
       // alertsDisplayInProgress = false;
       //} else {
       // skipAlertUpdate = true;         // If no alerts, do an immediate panel update to move to next panel
       // readyForBotPanelUpdate = true;
      //}
      //break;
    //case 4:
      //drawForecastText(0);
      //break;
    //case 5:
      //drawForecastText(1);
      //break;
    //case 6:
      //drawForecastText(2);
      //break;
  }
  //if (skipAlertUpdate) {
  //  skipAlertUpdate = false;        // reset for next display cycle
  //}
  lastPanelMillis = millis();
}

void updateForecastData() {
  drawProgress(50, "Updating forecasts...");

  OpenWeatherMapForecast *forecastClient = new OpenWeatherMapForecast();
  forecastClient->setMetric(isMetric);
  forecastClient->setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  //uint8_t allowedHours[] = {12, 0};
  uint8_t allowedHours[] = {15, 9, 0};
  forecastClient->setAllowedHours(allowedHours, sizeof(allowedHours));
  forecastClient->updateForecastsById(forecasts, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID, MAX_FORECASTS);
  delete forecastClient;
  forecastClient = nullptr;

  //wunderground.updateForecast(WUNDERGROUND_API_KEY, WUNDERGROUND_LANGUAGE, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY);
  drawProgress(100, "Done...");
  delay(1000);
  readyForMidPanelUpdate = true;       // Update mid-panel since progress bar overwrites this.

//** Check to see if we have valid forecast data.  If not, set the flag to update again.
  //if ( (wunderground.getForecastTitle(0).substring(0, 3)) == "") {    // check first returned DoW to see if we have a value
  //   Serial.println("Invalid forecast data...attempting to update...");
  //   readyForForecastUpdate = true;
  //} else {
     readyForForecastUpdate = false;    
  //}

  // Check first icon in forecast data to see if it is "unknown" or the returned length is 0; if so, assume bad data.
  //    Added by DK Fowler ... 31-Dec-2018
  if ( (getMeteoconIcon(forecasts[0].icon) == "unknown" ) || (strlen(getMeteoconIcon(forecasts[0].icon)) == 0) ) {
    if (invalidForecastsCnt > 10) {
      Serial.println("Invalid forecasts retry count exceeded...restarting...");
      drawProgress(100, "Retries exceeded");
      delay(1000);
      drawProgress(100, "Restarting...");
      delay(2000);
      ESP.restart();
      delay(2000);
    } else {
      invalidForecastsCnt++;
      Serial.println("Invalid forecasts data...attempting to update...");
      drawProgress(100, "Invalid forecasts"); 
      readyForForecastUpdate = true;
      delayAndCheckTouch(10000);     // delay 10 seconds before continuing
    }
  } else {
     readyForForecastUpdate = false;
     invalidForecastsCnt = 0;
  }

}

void updateAstronomyData() {
  drawProgress(50, "Updating astronomy...");

  Astronomy *astronomy = new Astronomy();
  moonData = astronomy->calculateMoonData(time(nullptr));
  float lunarMonth = 29.53;
  moonAge = moonData.phase <= 4 ? lunarMonth * moonData.illumination / 2 : lunarMonth - moonData.illumination * lunarMonth / 2;

  //moonAgeImage = String((char) (65 + ((uint8_t) ((26 * moonAge / 30) % 26))));
  delete astronomy;
  astronomy = nullptr;

  // Testing...
  //SunMoonCalc(time(nullptr), 38.2718404, -85.5763783);
  //Serial.print("####Moon rise:  "); Serial.println(Moon.rise);
  //Serial.print("####Moon set:   "); Serial.println(Moon.set);
  
  //**wunderground.updateAstronomy(WUNDERGROUND_API_KEY, WUNDERGROUND_LANGUAGE, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY);
  drawProgress(100, "Done...");
  delay(1000);
  readyForMidPanelUpdate = true;       // Update mid-panel since progress bar overwrites this.
  readyForBotPanelUpdate = true;
  readyForAstronomyUpdate = false;
}

// Progress bar helper
void drawProgress(uint8_t percentage, String text) {
  tft.setFont(&ArialRoundedMTBold_14); 
  ui.setTextAlignment(CENTER);
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  currTextColor = ILI9341_ORANGE;

  tft.fillRect(0, 156, 240, 52, ILI9341_BLACK);
  ui.drawString(120, 180, text);
  ui.drawProgressBar(10, 185, 240 - 20, 15, percentage, ILI9341_WHITE, ILI9341_BLUE);
}

// Called every 1 minute
void updateDHT() {
  //  Commented out reading of temp/humidity as it is current not used in this sketch
  //humidity = dht.readHumidity();
  //temperature = dht.readTemperature(!isMetric);
  readyForDHTUpdate = false;
}

void setDSTRules(String TZ_CITY) {
  if (TZ_CITY == "Boston") {
        UTC_OFFSET = -5;
        StartRule = (dstRule) {"EDT", Second, Sun, Mar, 2, 3600}; // Eastern Daylight time = UTC/GMT -4 hours
        EndRule = (dstRule) {"EST", First, Sun, Nov, 1, 0};       // Eastern Standard time = UTC/GMT -5 hours
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Louisville") {
        UTC_OFFSET = -5;
        StartRule = (dstRule) {"EDT", Second, Sun, Mar, 2, 3600}; // Eastern Daylight time = UTC/GMT -4 hours
        EndRule = (dstRule) {"EST", First, Sun, Nov, 1, 0};       // Eastern Standard time = UTC/GMT -5 hours
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Chicago") {
        UTC_OFFSET = -6;
        StartRule = (dstRule) {"CDT", Second, Sun, Mar, 2, 3600}; // Central Daylight time = UTC/GMT -5 hours
        EndRule = (dstRule) {"CST", First, Sun, Nov, 1, 0};       // Central Standard time = UTC/GMT -6 hours
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Mountain") {
        UTC_OFFSET = -7;
        StartRule = (dstRule) {"MDT", Second, Sun, Mar, 2, 3600}; // Mountain Daylight time = UTC/GMT -6 hours
        EndRule = (dstRule) {"MST", First, Sun, Nov, 1, 0};       // Mountain Standard time = UTC/GMT -7 hours
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Arizona-DST") {		// Same as Mountain
        UTC_OFFSET = -7;
        StartRule = (dstRule) {"MDT", Second, Sun, Mar, 2, 3600}; // Mountain Daylight time = UTC/GMT -6 hours
        EndRule = (dstRule) {"MST", First, Sun, Nov, 1, 0};		    // Mountain Standard time = UTC/GMT -7 hours
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Arizona-noDST") {		// Most of Arizona doesn't currently implement daylight saving time
        UTC_OFFSET = -7;
        StartRule = (dstRule) {"MST", First, Sun, Nov, 1, 0}; 	  // Mountain Standard time = UTC/GMT -7 hours
        EndRule = (dstRule) {"MST", First, Sun, Nov, 1, 0};
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Pacific") {
        UTC_OFFSET = -8;
        StartRule = (dstRule) {"PDT", Second, Sun, Mar, 2, 3600}; // Pacific Daylight time = UTC/GMT -7 hours
        EndRule = (dstRule) {"PST", First, Sun, Nov, 1, 0};       // Pacific Standard time = UTC/GMT -8 hours
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Alaska") {
        UTC_OFFSET = -9;
        StartRule = (dstRule) {"ADT", Second, Sun, Mar, 2, 3600}; // Alaskan Daylight time = UTC/GMT -8 hours
        EndRule = (dstRule) {"AST", First, Sun, Nov, 1, 0};       // Alaskan Standard time = UTC/GMT -9 hours
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Hawaii") {								                // Hawaii doesn't currently implement daylight saving time
        UTC_OFFSET = -10;
        StartRule = (dstRule) {"HST", First, Sun, Nov, 1, 0}; 	  // Hawaiian Standard time = UTC/GMT -10 hours
        EndRule = (dstRule) {"HST", First, Sun, Nov, 1, 0};
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Zurich") {
        UTC_OFFSET = +1;
        StartRule = (dstRule) {"CEST", Last, Sun, Mar, 2, 3600};  // Central European Summer Time = UTC/GMT +2 hours
        EndRule = (dstRule) {"CET", Last, Sun, Oct, 2, 0};        // Central European Time = UTC/GMT +1 hour
        #define NTP_SERVERS "0.ch.pool.ntp.org", "1.ch.pool.ntp.org", "2.ch.pool.ntp.org"
        isMetric = true;
  } else if (TZ_CITY == "Tokyo") {								                // Japan doesn't implement daylight saving time
		    UTC_OFFSET = +9;
		    StartRule = (dstRule) {"JST", First, Sun, Nov, 1, 0}; 	  // Japan Standard time = UTC/GMT +9 hours
		    EndRule = (dstRule) {"JST", First, Sun, Nov, 1, 0}; 	    // NO DST
		    #define NTP_SERVERS "3.jp.pool.ntp.org", "0.jp.pool.ntp.org", "2.jp.pool.ntp.org"
        isMetric = true;
  } else if (TZ_CITY == "Sydney") {
        UTC_OFFSET = +10;
        StartRule = (dstRule) {"AEDT", First, Sun, Oct, 2, 3600}; // Australia Eastern Daylight time = UTC/GMT +11 hours
        EndRule = (dstRule) {"AEST", First, Sun, Apr, 2, 0};      // Australia Eastern Standard time = UTC/GMT +10 hours
        #define NTP_SERVERS "0.au.pool.ntp.org", "1.au.pool.ntp.org", "2.au.pool.ntp.org"
        isMetric = true;
  } else {						// No DST support
        UTC_OFFSET = 0;
        StartRule = (dstRule) {"GMT", First, Sun, Nov, 1, 0};     // GMT default
        EndRule = (dstRule) {"GMT", First, Sun, Nov, 1, 0};
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  }
  simpleDSTadjust dstAdjusted(StartRule, EndRule);
  //wunderground.initMetric(isMetric);      // (re-)initialize the metric settings for the Weather Underground library
}

void drawDateTime() {
  // Setup adjust Library rules
  //simpleDSTadjust dstAdjusted(StartRule, EndRule);

  char *dstAbbrev;
  char time_str[11];

  // Added following logic to ensure time is set before using the time structure for further processing.
  //    Added by DK Fowler ... 10-Jan-2017
  // Check to ensure the time has been set (not zero); if not, try up to 5 times to get it before proceeding and
  // set a flag to indicate success for later use.
  byte timeAttempt = 0;
  time_t now = 0;           // Initialize at 0; otherwise, the variable occasionally has random values
  while ((now == 0) && (timeAttempt < 5) ) { 
    if (timeAttempt > 0) delay(1000);   // delay before retries 2-5
    timeAttempt++;
    Serial.println("Requesting time update...attempt " + String(timeAttempt));
    now = dstAdjusted.time(&dstAbbrev);
    Serial.print("DST adjusted time now:  "); Serial.println(now);
  }
  if (now == 0) {
    Serial.println("...time not set after multiple attempts...");
    timeIsSet = false;
  } else {
    timeIsSet = true;
  }
  
  timeinfo = localtime(&now);

//  tft.drawRect(40, 0, 200, 28, ILI9341_WHITE);
  tft.fillRect(40, 0, 200, 28, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  currTextColor = ILI9341_WHITE;
  
  tft.setFont(&ArialRoundedMTBold_14);

  String date;
// Added check to present an indicator if date/time has not been set yet.
  if (timeIsSet) {
    date = ctime(&now);
    date = date.substring(0,11) + String(1900+timeinfo->tm_year);
  } else {
    date = "--- --- -- ----";
  }
  ui.drawString(120, 20, date);

//  tft.setFont(&DSEG7_Classic_Bold_21);
  tft.setFont(&ArialRoundedMTBold_36);

// Added check to present an indicator if time has not been set yet.
  if (timeIsSet) {
    #ifdef STYLE_24HR
      sprintf(time_str, "%02d:%02d:%02d\n",timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
      // tft.drawRect(40, 26, 200, 36, ILI9341_WHITE);
      tft.fillRect(40, 26, 200, 36, ILI9341_BLACK);
      ui.drawString(120, 56, time_str);
      drawSeparator(65);
    #else
      int hour = (timeinfo->tm_hour+11)%12+1;  // take care of noon and midnight
      sprintf(time_str, "%2d:%02d:%02d\n", hour, timeinfo->tm_min, timeinfo->tm_sec);
      // tft.drawRect(40, 26, 200, 36, ILI9341_WHITE);
      tft.fillRect(40, 26, 200, 36, ILI9341_BLACK);
      ui.drawString(120, 56, time_str);
      drawSeparator(65);
    #endif
  } else {
      sprintf(time_str, "--:--:--\n");
      // tft.drawRect(40, 26, 200, 36, ILI9341_WHITE);
      tft.fillRect(40, 26, 200, 36, ILI9341_BLACK);
      ui.drawString(120, 56, time_str);
      drawSeparator(65);
  }    

  ui.setTextAlignment(LEFT);
  tft.setFont(&ArialRoundedMTBold_14);
#ifdef STYLE_24HR
  sprintf(time_str, "%s", dstAbbrev);
  ui.drawString(202, 40, time_str);
#else
  sprintf(time_str, "%s", dstAbbrev);
  ui.drawString(202, 40, time_str);
  sprintf(time_str, "%s", timeinfo->tm_hour>=12?"pm":"am");
  ui.drawString(202, 56, time_str);
#endif

// Check if it is nighttime...skip check if time has not been set yet.
if (timeIsSet) {
  isNight = checkIfNight(now);
}

// WiFi Signal
drawWiFiQuality();

}

// routine to check whether it is currently nighttime
bool checkIfNight(time_t currentTime) {
//  This routine checks to see if the (current) passed time is within a window considered as "nighttime"; this
//  is assumed to be between 30 minutes after sunset to 30 minutes before sunrise.  It is used to determine
//  if a more suitable icon should be used to display for certain current conditions.

  time_t sunriseTime = currentWeather.sunrise;
  String sunrise = getTime(&sunriseTime);
  Serial.printf("Sunrise time unadjusted, UTC:  %d\n", sunriseTime);
  Serial.print("..."); Serial.println(sunrise);

  sunriseTime = currentWeather.sunrise + dstOffset;;
  sunrise = getTime(&sunriseTime);
  Serial.printf("Sunrise time DST adjusted:  %d\n", sunriseTime);
  Serial.print("..."); Serial.println(sunrise);

  time_t sunsetTime = currentWeather.sunset;
  String sunset = getTime(&sunsetTime);
  Serial.printf("Sunset time unadjusted, UTC:  %d\n", sunsetTime);
  Serial.print("..."); Serial.println(sunset);

  sunsetTime = currentWeather.sunset + dstOffset;;
  sunset = getTime(&sunsetTime);
  Serial.printf("Sunset time DST adjusted:  %d\n", sunsetTime);
  Serial.print("..."); Serial.println(sunset);

  
  //time_t sunsetTime = currentWeather.sunset + dstOffset;
  //Serial.printf("Sunrise time, UTC:  %d\n", sunriseTime);
  //Serial.printf("Sunset time,  UTC:  %d\n", sunsetTime);
  //String sunset  = getTime(&sunsetTime);

  timeinfo = gmtime(&currentTime);
  Serial.print("Now:  "); Serial.print(timeinfo->tm_hour); Serial.print(":"); Serial.println(timeinfo->tm_min);
  Serial.print("Sunrise: "); Serial.println(sunrise);
  Serial.print("Sunset:  "); Serial.println(sunset);
  
  int sunriseMin = sunrise.substring(0,2).toInt() * 60 + sunrise.substring(3,5).toInt() ;
  int sunsetMin  = sunset.substring(0,2).toInt() * 60 + sunset.substring(3,5).toInt();

  Serial.print("*** Sunrise mins:  " + String(sunriseMin)); Serial.println("  Sunset mins:  " + String(sunsetMin));
  if ( (sunriseMin == 0) && (sunsetMin == 0) ) Serial.println("No values for sunrise/sunset...assuming daytime.");

  int nowMin = timeinfo->tm_hour * 60 + timeinfo->tm_min;

  Serial.println("*** Now mins:  " + String(nowMin));

  if ( ((nowMin >= sunriseMin-30) && (nowMin <= sunsetMin+30)) || 
       ((sunriseMin == 0) && (sunsetMin == 0)) ) {
    Serial.println("Daytime!");
    return(false);
  } else {
    Serial.println("Nighttime!");
    return(true);
  }
}
  
// Helper function, should be part of the weather station library and should disappear soon
const char* getMeteoconIcon(String iconText) {
  if (iconText == "01d" || iconText == "01n") return "sunny";
  if (iconText == "02d" || iconText == "02n") return "partlysunny";
  if (iconText == "03d" || iconText == "03n") return "partlycloudy";
  if (iconText == "04d" || iconText == "04n") return "mostlycloudy";
  if (iconText == "09d" || iconText == "09n") return "rain";
  if (iconText == "10d" || iconText == "10n") return "rain";
  if (iconText == "11d" || iconText == "11n") return "tstorms";
  if (iconText == "13d" || iconText == "13n") return "snow";
  if (iconText == "50d" || iconText == "50n") return "fog";
  return "unknown";
}

const char* getMiniMeteoconIcon(String iconText) {
  if (iconText == "01d" || iconText == "01n") return "minisunny";
  if (iconText == "02d" || iconText == "02n") return "minipartlysunny";
  if (iconText == "03d" || iconText == "03n") return "minipartlycloudy";
  if (iconText == "04d" || iconText == "04n") return "minimostlycloudy";
  if (iconText == "09d" || iconText == "09n") return "minirain";
  if (iconText == "10d" || iconText == "10n") return "minirain";
  if (iconText == "11d" || iconText == "11n") return "minitstorms";
  if (iconText == "13d" || iconText == "13n") return "minisleet";
  if (iconText == "50d" || iconText == "50n") return "minifog";
  return "miniunknown";
}

// draws current weather information
void drawCurrentWeather() {
  // Weather Icon
//  String weatherIcon = getMeteoconIcon(wunderground.getTodayIcon());
//  String weatherIcon = wunderground.getTodayIconText();

  String weatherIcon = getMeteoconIcon(currentWeather.icon);
  Serial.println("*** Current weather icon:  " + weatherIcon);
  
  // If nighttime, replace the icon with one more appropriate for the current time
  if (isNight) Serial.println("Nighttime passed to drawCurrentWeather...");
  if ((isNight) && ((weatherIcon == "clear" ) ||
                   ( weatherIcon == "sunny" ) ||
                   ( weatherIcon == "mostlycloudy" ) ||
                   ( weatherIcon == "mostlysunny" ) ||
                   ( weatherIcon == "partlysunny"  ) ||
                   ( weatherIcon == "partlycloudy" ))) {
     weatherIcon = "nt_" + weatherIcon ;
     Serial.println("*** Weather icon replaced:  " + weatherIcon);
  }

  ui.drawBmp("/Condition/" + weatherIcon + ".bmp", 0, 55);
  
  // Weather Text
  //tft.setFont(&ArialRoundedMTBold_14);    // font cause overwrite of icon on long weather descriptions
  tft.setFont(&Droid_Sans_12);
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  currTextColor = ILI9341_ORANGE;
  
  ui.setTextAlignment(RIGHT);
//DKF
  //ui.drawString(220, 88, wunderground.getWeatherText());
  ui.drawString(220, 88, currentWeather.description);

  // Temperature display
  tft.setFont(&ArialRoundedMTBold_36);
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  currTextColor = ILI9341_ORANGE;
  
  ui.setTextAlignment(RIGHT);
  String degreeSign = "F";
  if (isMetric) {
    degreeSign = "C";
  }
  // String temp = wunderground.getCurrentTemp() + degreeSign;
  String temp = String(currentWeather.temp, 1) + degreeSign;
  ui.drawString(220, 120, temp);
  drawSeparator(135);

  // Feels-like temperature display
  float currWindChillTemp = windChill(currentWeather.temp, currentWeather.windSpeed);
  float currHeatIndexTemp = heatIndex(currentWeather.temp, currentWeather.humidity);

  //  To determine whether to use the wind chill or the heat index as the value to display, 
  //  compare the current temperature vs. the returned values for wind chill and heat index.
  //  If the wind chill is less than the current temp, use it.  If the heat index is greater
  //  than the current temp, use it instead.
  //  Note that the wind chill calculation is only valid for temps at or below 50degF and winds 
  //  greater than 3mph.  The heat index estimation is only valid for temps at or above 80degF 
  //  and humidity at or above 40%.

  float feelsLikeTemp;
  if (currWindChillTemp < currentWeather.temp) {
    feelsLikeTemp = currWindChillTemp;
  } else if (currentWeather.temp < currHeatIndexTemp) {
    feelsLikeTemp = currHeatIndexTemp;
  } else {
    feelsLikeTemp = currentWeather.temp;
  }
  //Serial.print("###Current wind chill:  "); Serial.println(windChillF, 2);

  ui.setTextAlignment(RIGHT);
  tft.setFont(&Droid_Sans_10);
  temp = "Feels like: " + String(feelsLikeTemp,1);
  ui.drawString(220, 132, temp);
 
}

// Draw current observations panel.
void drawCurrentObservations () {
  // In order to make it easy to adjust the position of this
  // display, the following x/y coordinates set the baseline
  // position for the remaining display lines.
  int x = 0;
  int y = 243;
  // The following control the current line/column/
  // font height and column width.  Adjust as desired
  // based on font used and column appearance.
  int fontHeight = 12;
  int fontWidth = 5;      // used to set position of dynamic information after labels
  int maxLabelWidth = 9;  // used as basedline for width of static lables
  int columnWidth = 120;
  float lineNumber = 1;
  int columnNumber = 1;
  
  tft.fillRect(0, 233, 240, 87, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  currTextColor = ILI9341_WHITE;
  
  tft.setFont(&Droid_Sans_10);
  String temp = "Current Observations";
  ui.drawString(120, y, temp);
  lineNumber = lineNumber + 1.5;    // add blank line after heading

  ui.setTextAlignment(LEFT);
  temp = "Humid:  ";
  ui.drawString(x+(columnNumber-1), y+(fontHeight*(lineNumber-1)), temp);
  //**temp = wunderground.getHumidity();
  temp = String(currentWeather.humidity) + "%";
  ui.drawString(x+(columnNumber-1)+(maxLabelWidth*fontWidth), y+(fontHeight*(lineNumber-1)), temp);
  lineNumber++;

  String windString;
  //**if (wunderground.getWindSpeed() == "0.0") {
  //  windString = "Calm";
  //} else {
  //  windString = wunderground.getWindDir() + " " + wunderground.getWindSpeed(); 
  //}

  if (currentWeather.windSpeed == 0) {
    windString = "Calm";
  } else {
    windString = String(currentWeather.windSpeed,0);      //** Need to add wind direction (windDeg) by converting windDeg to N,S, etc.
  }
  
  //  Add units...14-Dec-2018, DKF
  if ( isMetric && (windString != "Calm") ) {
    windString = String(windCompassDir(currentWeather.windDeg)) + " " + windString + "m/s";
  } else if ( windString != "Calm") {
    windString = String(windCompassDir(currentWeather.windDeg)) + " " + windString + "mph";
  }
  
  temp = "Wind:   ";
  ui.drawString(x+(columnNumber-1), y+(fontHeight*(lineNumber-1)), temp);
  temp = windString;
  ui.drawString(x+(columnNumber-1)+(maxLabelWidth*fontWidth), y+(fontHeight*(lineNumber-1)), temp);
  lineNumber++;

  //**temp = "Precip: ";                        //** Need to see if there is a close equivalent
  //ui.drawString(x+(columnNumber-1), y+(fontHeight*(lineNumber-1)), temp);
  //temp = wunderground.getPrecipitationToday();
  //ui.drawString(x+(columnNumber-1)+(maxLabelWidth*fontWidth), y+(fontHeight*(lineNumber-1)), temp);
  //lineNumber++;

  temp = "Moon:   ";
  ui.drawString(x+(columnNumber-1), y+(fontHeight*(lineNumber-1)), temp);
  //**ui.drawString(120, 250, MOON_PHASES[moonData.phase]);   // Need to find a place for this
  //temp = wunderground.getMoonPhase();
  temp = MOON_PHASES[moonData.phase];
  ui.drawString(x+(columnNumber-1)+(maxLabelWidth*fontWidth), y+(fontHeight*(lineNumber-1)), temp);
  lineNumber++;

// Column 2...

  columnNumber++;
  lineNumber = 2.5;     // reset for second column

  temp = "Press: ";
  ui.drawString(x+((columnNumber-1)*columnWidth), y+(fontHeight*(lineNumber-1)), temp);
  //temp = wunderground.getPressure();
  if (isMetric) {
    temp = String(currentWeather.pressure) + "hPa";     //** Need to convert to in Hg
  } else {
    temp = String(hPaToInHg(currentWeather.pressure)) + "in";
  }
  ui.drawString(x+((columnNumber-1)*columnWidth)+(maxLabelWidth*fontWidth), y+(fontHeight*(lineNumber-1)), temp);
  lineNumber++;

  //temp = "UV:    ";       //** Need an equivalent or replacement
  //ui.drawString(x+((columnNumber-1)*columnWidth), y+(fontHeight*(lineNumber-1)), temp);
  //temp = wunderground.getUV();
  //ui.drawString(x+((columnNumber-1)*columnWidth)+(maxLabelWidth*fontWidth), y+(fontHeight*(lineNumber-1)), temp);
  //lineNumber++;

  //temp = wunderground.getObservationTime();
  time_t time = currentWeather.observationTime + dstOffset;
  struct tm * timeinfo = localtime(&time);
  //ui.drawString(120, y - 15, WDAY_NAMES[timeinfo->tm_wday] + " " + String(timeinfo->tm_hour) + ":00");
  Serial.print("Observation time:  "); Serial.println(asctime(timeinfo));

  char tempChar[50];
  //temp = String(sprintf(tempChar, "Observation at: %s", asctime (timeinfo) ));
  sprintf(tempChar, "Observed: %s", asctime(timeinfo) );
  Serial.print("...obsv text:  "); Serial.println(tempChar);
  //temp = WDAY_NAMES[timeinfo->tm_wday] + " " + String(timeinfo->tm_hour) + ":00"      //** Redo to translate month, day to native language
  ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  currTextColor = ILI9341_CYAN;
  
  tft.setFont(&Droid_Sans_10);
  ui.setTextAlignment(CENTER);
  //temp = tempChar.toString(22);
  ui.drawString(120, 312, tempChar);

}

// draws the three forecast columns
void drawForecast() {
//  tft.drawRect(0, 165, 240, 50, ILI9341_WHITE);  // May leave this for future; adds a nice effect 
  //tft.drawRect(0, 150, 240, 82, ILI9341_WHITE);  // Frames the entire forecast area
  tft.fillRect(0, 150, 240, 82, ILI9341_BLACK);    // Clear prior forecast panel

  // Today's high/low forecast...draw it here to ensure forecast data has been gathered first.
  //    Added by DK Fowler ... 22-Jan-2018
  ui.setTextAlignment(RIGHT);
  tft.setFont(&Droid_Sans_10);
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  currTextColor = ILI9341_WHITE;
  
  uint8_t dayIndex = 0;
  //**ui.drawString(220, 144, "High: " + wunderground.getForecastHighTemp(dayIndex) + "  Low: " + wunderground.getForecastLowTemp(dayIndex));

  drawForecastDetail(10, 165, 0);
  drawForecastDetail(95, 165, 1);
  drawForecastDetail(178, 165, 2);

  midPanelIndex = 1;    // index to indicate which forecast panel is currently displayed, used by touch-enhancements; added 22-Jan-18, DKF

  drawSeparator(165 + 65 + 10);
}

void drawForecast2() {
//  tft.drawRect(0, 165, 240, 50, ILI9341_WHITE);  // May leave this for future; adds a nice effect 
  //tft.drawRect(0, 150, 240, 82, ILI9341_WHITE);
  tft.fillRect(0, 150, 240, 82, ILI9341_BLACK);   // Clear prior forecast panel

  drawForecastDetail(10, 165, 3);
  drawForecastDetail(95, 165, 4);
  drawForecastDetail(178, 165, 5);

  midPanelIndex = 2;    // index to indicate which forecast panel is currently displayed, used by touch-enhancements; added 22-Jan-18, DKF
  
  drawSeparator(165 + 65 + 10);
}

void drawForecast3() {
//  tft.drawRect(0, 165, 240, 50, ILI9341_WHITE);  // May leave this for future; adds a nice effect
  //tft.drawRect(0, 150, 240, 82, ILI9341_WHITE);
  tft.fillRect(0, 150, 240, 82, ILI9341_BLACK);   // Clear prior forecast panel

  drawForecastDetail(10, 165, 6);
  drawForecastDetail(95, 165, 7);
  drawForecastDetail(178, 165, 8);
  
  midPanelIndex = 3;    // index to indicate which forecast panel is currently displayed, used by touch-enhancements; added 22-Jan-18, DKF
   
  drawSeparator(165 + 65 + 10);
}

void drawForecast4() {
//  tft.drawRect(0, 165, 240, 50, ILI9341_WHITE);  // May leave this for future; adds a nice effect
  //tft.drawRect(0, 150, 240, 82, ILI9341_WHITE);
  tft.fillRect(0, 150, 240, 82, ILI9341_BLACK);   // Clear prior forecast panel

  drawForecastDetail(10, 165, 9);
  drawForecastDetail(95, 165, 10);
  drawForecastDetail(178, 165, 11);
  
  midPanelIndex = 4;    // index to indicate which forecast panel is currently displayed, used by touch-enhancements; added 22-Jan-18, DKF
   
  drawSeparator(165 + 65 + 10);
}

void drawForecast5() {
//  tft.drawRect(0, 165, 240, 50, ILI9341_WHITE);  // May leave this for future; adds a nice effect
  //tft.drawRect(0, 150, 240, 82, ILI9341_WHITE);
  tft.fillRect(0, 150, 240, 82, ILI9341_BLACK);   // Clear prior forecast panel

  drawForecastDetail(10, 165, 12);
  drawForecastDetail(95, 165, 13);
  drawForecastDetail(178, 165, 14);
  
  midPanelIndex = 5;    // index to indicate which forecast panel is currently displayed, used by touch-enhancements; added 22-Jan-18, DKF
   
  drawSeparator(165 + 65 + 10);
}

// helper for the forecast columns
void drawForecastDetail(uint16_t x, uint16_t y, uint8_t dayIndex) {
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  currTextColor = ILI9341_ORANGE;
  
//  tft.setFont(&ArialRoundedMTBold_14);
  tft.setFont(&Droid_Sans_12);
  ui.setTextAlignment(CENTER);
  time_t time = forecasts[dayIndex].observationTime + dstOffset;
  struct tm * timeinfo = localtime (&time);
  ui.drawString(x + 25, y, WDAY_NAMES[timeinfo->tm_wday] + " " + ( isMetric ? (String(timeinfo->tm_mday) + "/" + String(timeinfo->tm_mon+1) ) :
                                                                               (String(timeinfo->tm_mon+1) + "/" + String(timeinfo->tm_mday) ) ) ); 
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  currTextColor = ILI9341_WHITE;
  ui.drawString(x + 25, y + 14, ( timeinfo->tm_hour < 10 ? ("0" + String(timeinfo->tm_hour) ) :
                                                           (String(timeinfo->tm_hour) ) ) + ":00 " + String(forecasts[dayIndex].temp, 0) + (isMetric ? "°C" : "°F"));

  //gfx.drawPalettedBitmapFromPgm(x, y + 15, getMiniMeteoconIconFromProgmem(forecasts[dayIndex].icon));
  String weatherIcon = getMeteoconIcon(forecasts[dayIndex].icon);
  Serial.print("*** Forecast icon[day]:  "); Serial.print(weatherIcon); Serial.print("["); Serial.print(dayIndex); Serial.println("] ***");
  //if (weatherIcon == "chancetstorms") weatherIcon = "tstorms";      // Temporary fix for missing "chancetstorms" mini .bmp
  ui.drawBmp("/Mini/" + weatherIcon + ".bmp", x, y + 15);

  ui.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
  currTextColor = ILI9341_BLUE;
  //ui.drawString(x + 25, y + 60, String(forecasts[dayIndex].rain, 1) + (isMetric ? "mm" : "in"));
  float rainInches = forecasts[dayIndex].rain / 25.4;
  //if (forecasts[dayIndex].rain > 0.00) {   // Only print rain if forecast is > 0
    ui.drawString(x + 25, y + 62, (isMetric ? (String(forecasts[dayIndex].rain, 1) + "mm") : 
                                               (String(rainInches, 2) + "in") ) );     // ...apparent bug in squix code as 3h rain is always returned in mm???
  //}
  
  //String day = wunderground.getForecastTitle(dayIndex).substring(0, 3);
  //day.toUpperCase();
  //String forecastDate = wunderground.getForecastMonth(dayIndex/2) + "/" + 
  //                      wunderground.getForecastDay(dayIndex/2);
  //String dayDate = day + "-" + forecastDate;
  //ui.drawString(x + 25, y, dayDate);

//  Reversed the display of the high/low to be more logical...see comments at beginning of code for further details.
//    Modified by DK Fowler ... 22-Jan-2018
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  currTextColor = ILI9341_WHITE;
  
  //ui.drawString(x + 25, y + 14, wunderground.getForecastHighTemp(dayIndex) + "|" + wunderground.getForecastLowTemp(dayIndex));
  
//  String weatherIcon = getMeteoconIcon(wunderground.getForecastIcon(dayIndex));
//**  String weatherIcon = wunderground.getForecastIconText(dayIndex);
//**  Serial.print("*** Forecast icon[day]:  "); Serial.print(weatherIcon); Serial.print("["); Serial.print(dayIndex); Serial.println("] ***");
  //if (weatherIcon == "chancetstorms") weatherIcon = "tstorms";      // Temporary fix for missing "chancetstorms" mini .bmp
//**  ui.drawBmp("/Mini/" + weatherIcon + ".bmp", x, y + 15);
  // overlay probability of precipitation
  //String PoP = wunderground.getPoP(dayIndex) + "%";
  //ui.setTextAlignment(RIGHT);
  //ui.drawString(x + 55, y + 65, PoP);

}

// draw moonphase and sunrise/set and moonrise/set, age, and illumination
void drawAstronomy() {
  tft.fillRect(0, 233, 240, 87, ILI9341_BLACK);

  int moonAgeImage = 24 * moonAge / 30.0;
  Serial.print("Moon age:  "); Serial.println(moonAge);
  Serial.print("Moon age image:  "); Serial.println(moonAgeImage);
  ui.drawBmp("/Moon Phases/moonphase_L" + String(moonAgeImage) + ".bmp", 120 - 30, 250);
      
  tft.setFont(&ArialRoundedMTBold_14);  
  ui.setTextAlignment(LEFT);
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  currTextColor = ILI9341_ORANGE;
  
  ui.drawString(20, 270, SUN_MOON_TEXT[0]);
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  currTextColor = ILI9341_WHITE;
  
  ui.setTextAlignment(RIGHT);
  time_t time = currentWeather.sunrise + dstOffset;
  ui.drawString(50, 285, getTime(&time));
  time = currentWeather.sunset + dstOffset;
  ui.drawString(50, 300, getTime(&time));

  ui.setTextAlignment(RIGHT);
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  currTextColor = ILI9341_ORANGE;
  
  ui.drawString(233, 270, SUN_MOON_TEXT[3]);
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  currTextColor = ILI9341_WHITE;

  //**ui.drawString(120, 250, MOON_PHASES[moonData.phase]);   // Need to find a place for this
  
  ui.drawString(200, 285, SUN_MOON_TEXT[4] + ":");
  ui.drawString(239, 285, String(moonAge) + "d");
  ui.drawString(200, 300, SUN_MOON_TEXT[5] + ":");
  ui.drawString(239, 300, String(moonData.illumination * 100, 0) + "%");
  
}


// display current forecast text
void drawForecastText(int forecastTextPeriod) {
  tft.fillRect(0, 231, 240, 87, ILI9341_BLACK);
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  currTextColor = ILI9341_WHITE;

  tft.setFont(&Droid_Sans_10);  
  ui.setTextAlignment(LEFT);
  tft.setTextWrap(true);
  String temp;

  tft.setCursor(0,243);
//  temp = wunderground.getForecastTitle(forecastTextPeriod) + ":";             // Current period
//  tft.println(temp);
//  temp = wunderground.getForecastText(forecastTextPeriod);
  temp = "Void";      // Temp until replaced
  stringWordWrap(temp, 1);

}
  
// Helper function, should be part of the weather station library and should disappear soon
// Modified to include separate unambiguous icons for several states that previously returned
// the same values; this allows displaying different images for these states.  Originally, 
// several (such as sleet and snow) displayed the same icons.
//    Modified by DK Fowler ... 28-Jan-2017
String getMeteoconIconDKF(String iconText) {
  if (iconText == "F") return "chanceflurries";
  if (iconText == "Q") return "chancerain";
  if (iconText == "W") return "chancesleet";
  if (iconText == "V") return "chancesnow";
  if (iconText == "S") return "chancetstorms";
  if (iconText == "B") return "clear";
  if (iconText == "Y") return "cloudy";
  if (iconText == "I") return "flurries";       // formerly "F"
  if (iconText == "M") return "fog";
  if (iconText == "E") return "hazy";
  if (iconText == "L") return "mostlycloudy";   // formerly "Y"
  if (iconText == "H") return "mostlysunny";
  if (iconText == "H") return "partlycloudy";
  if (iconText == "J") return "partlysunny";
  if (iconText == "T") return "sleet";          // formerly "W"
  if (iconText == "R") return "rain";
  if (iconText == "N") return "snow";           // formerly "W"
  if (iconText == "B") return "sunny";
  if (iconText == "0") return "tstorms";

  return "unknown";
}

// if you want separators, uncomment the tft-line
void drawSeparator(uint16_t y) {
   //tft.drawFastHLine(10, y, 240 - 2 * 10, 0x4228);
}

// display the Open Weather Map logo and other attributions at startup
void drawWeatherStationAttributions() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&Droid_Sans_12);
  ui.setTextAlignment(CENTER);
  tft.setTextColor(ILI9341_CYAN);
  currTextColor = ILI9341_CYAN;
  
  ui.drawString(120, 75,  "Powered by");

  //ui.drawBmp("/Other/wundergroundLogo_4c.bmp", 20, 95);
  ui.drawBmp("/Other/OpenWeatherMap.bmp", 2, 95);

  ui.drawString(120, 240, "Credits: squix78, fowlerk, tzapu,");
  ui.drawString(120, 256, "neptune, datacute, Adafruit, others");
  ui.drawString(120, 272, "Icons:  Ashley Jager,");
  ui.drawString(120, 288, "manifestinteractive");

  tft.setTextColor(ILI9341_CYAN);
  currTextColor = ILI9341_CYAN;
  
  delay(5000);

  //  Temporary code to hold attribution display to allow code uploading during development

  //bool isTouched = ts.touched();
  //while (!isTouched) {
  //  delay(10);
  //  isTouched = ts.touched();
  //}

}

void setReadyForWeatherUpdate() {
  Serial.println("Setting readyForWeatherUpdate to true");
  readyForWeatherUpdate = true;
}

void setReadyForForecastUpdate() {
/*
    This routine is triggered every 60 secs to increment and check to see
    if the 10-day forecast interval has been reached, triggering an update
    of the forecast information. 
*/
  forecastTimer++;
//  Serial.print("***Checking 10-day forecast update timer, timer="); Serial.println(forecastTimer);
  if (forecastTimer >= (UPDATE_INTERVAL_FORECAST_SECS / 60)) {
    Serial.println("Setting readyForForecastUpdate to true");
    readyForForecastUpdate = true;
    forecastTimer = 0;
  }
}

void setReadyForAstronomyUpdate() {
/*
    This routine is triggered every 60 secs to increment and check to see
    if the astronomy data update interval has been reached, triggering an update
    of this information. 
*/
  astronomyTimer++;
//  Serial.print("***Checking astronomy update timer, timer="); Serial.println(astronomyTimer);
  if (astronomyTimer >= (UPDATE_INTERVAL_ASTRO_SECS / 60)) {
    Serial.println("Setting readyForAstronomyUpdate to true");
    readyForAstronomyUpdate = true;
    astronomyTimer = 0;
  }
}

void setReadyForMidPanelUpdate() {
  Serial.println("Setting readyForMidPanelUpdate to true");
  readyForMidPanelUpdate = true;
}

void setReadyForBotPanelUpdate() {
  Serial.println("Setting readyForBotPanelUpdate to true");
  readyForBotPanelUpdate = true;
}

void setReadyForRestart() {
/*
    This routine is triggered every 60 secs to increment and check to see
    if an automatic restart is requested.  This is to clear heap fragmentation.
      Added 01-Mar-2018 ... DK Fowler
*/
  restartTimer++;
//  Serial.print("***Checking restart timer, timer="); Serial.println(restartTimer);
  if (restartTimer >= (RESTART_INTERVAL_SECS / 60)) {
    // If the time has been set and the current time is past the restart time, then set the flag to restart
    if ( timeIsSet && ( (timeinfo->tm_hour == restartHr) && (timeinfo->tm_min >= restartMin) ) ) {
      Serial.println("Setting readyForRestart to true");
      readyForRestart = true;
      restartTimer = 0;
    }
  }  
}

// Routine to perform word-wrap display of text string
byte stringWordWrap(String displayString, byte headerLines, TextAlignment txtAlign, bool truncFlag, int yBase, int maxPanelLines) {
  // The approach in this routine is as follows:
  //    1) Estimate the possible number of chars that can be displayed per line with the
  //       current font.  This is done by first calculating the width in pixels of a 10-character
  //       sample string using the current font.
  //    2) Divide the maximum pixels per line by the average pixel width/char.
  //    3) Now begin scanning the display string starting at the maximum character position per line,
  //       moving backwards from there looking for the first space or ellipse sequence.  If either is found,
  //       then this is an acceptable place to break the line, so display it.
  //    4) Otherwise, keep moving backwards until either is found; break there and display the
  //       segment.
  //    5) Rinse and repeat starting with the character following the break, through the end of
  //       the display string.
  //    6) Of course, need to handle null strings and lines with no break-points (break anyway).
  //    7) The routine keeps track of the number of lines in order to display multiple panels if
  //       required.  It will delay between panels using the default timing used for the bottom
  //       panel.
  // 
  //    The routine will return the count of panels displayed (which can be/is used to determine if
  //    additional panel delays are required.  An optional parameter can be passed to indicate a
  //    number of header lines already printed in the frame to word-wrap.
  //
  //    Added additional optional parameter to set the text alignment to other than the default LEFT.
  //      Added by DK Fowler ... 22-Mar-2017
  //
  //    Modified initial sanitation of the source string to loop until all instances of multiple spaces
  //    are removed.
  //      Modified by DK Fowler ... 22-Mar-2017
  //
  //    Modified routine to added additional optional parameter used to indicate if the passed message string
  //    had been truncated.  This will cause a truncation flag to be displayed following the end of the text.
  //    This is primarily used for the display of weather alerts, which can be truncated due to the fact that
  //    the streaming JSON parser only allocated 512-bytes for each message maximum.  The truncation flag
  //    is now displayed in this routine (vs. in the calling weather-alert routine as it was originally).  This
  //    allows correct delaying at the end of the final display panel.
  //      Modified by DK Fowler ... 08-Apr-2017
  //
  //    Modified routine to allow passing optional parameters to specify the starting vertical display position (yBase)
  //    and maximum panel lines to display for a panel (maxPanelLines).  These were previously hard-coded based on the
  //    original bottom panel position.  They were added as parameters to allow more flexibility in using the routine
  //    to display string-wrapped panels in other screeen regions (such as the forecast panels added for the touch
  //    screen functionality).
  //      Modified by DK Fowler ... 23-Jan-2018
   
  char sampleString[] = "aaaaaaaaaa";  // Adjust sample string as appropriate based on type of typical data
  char testString[50];
  int16_t x1, y1;
  uint16_t w, h, x, y;
  int breakLocation, spaceBreakLocation, ellipseBreakLocation;
  // int xBase, yBase = 245,           // Removed yBase 23-Jan-2018, DKF; now passed as an optional parameter
  // int xBase, xPos = 0, yPos = 245;  // Removed initialization here for yPos, 23-Jan-2018; now initialized based on passed optional parameter yBase.
  int xBase, xPos = 0, yPos;           // Added 03/22/17 DKF to allow more options for initial positioning of text
  // byte maxPanelLines = 6;           // Default maximum panel lines; adjust for font-size changes; removed 23-Jan-2018, DKF; now passed as an optional parameter
  byte lineCnt = 0;                    // Used to track number of lines displayed for this panel
  bool resetTickerFlag = false;
  bool lastPanelDelay = false;         // Added 04/08/17 DKF to indicate a delay is required after display of last display panel
  byte panelCnt = 0;

  // Initialize the starting position yPos based on the passed optional parameter yBase.
  //    Added by DK Fowler ... 23-Jan-2018
  yPos = yBase;
  
  // debug
  long      startExecutionMillis = 0,
            endExecutionMillis = 0,
            executionMillis = 0;

  startExecutionMillis = millis();
  
//  Serial.println("*** stringWordWrap called ***");
  if (displayString == "") return 0;     // Just return and do nothing if the passed display line is null.

// Modified following 22-Mar-2017 to loop until all multiple spaces are removed from the sources string
  while (displayString.indexOf("  ") != -1 ) {
    displayString.replace("  "," ");     // Replace all instances of multiple spaces with one space.
  }
  
  x = 0; y = 20;                       // Sample coordinates used to calculate the max chars per line.
  tft.getTextBounds(sampleString, x, y, &x1, &y1, &w, &h);
  float pixelsChar = w/10;            // 10 characters in the sample string.
//  Serial.print("Sample string width (pixels):  "); Serial.println(w);
//  Serial.print("Average number of pixels/char (w):  "); Serial.println(pixelsChar,3);

  int charsPerLine = (240 / pixelsChar) - 1;
  int estBreak = charsPerLine;
  int beginLineIndex = 0;

  if (headerLines !=0) {
    lineCnt = headerLines;                         // Added 1-Feb-2017 DKF to handle overflow on text forecast with header line
    yPos = yPos + (headerLines * (h + 6));         // Added 22-Mar-2017 DKF to allow specifying optional different alignments
  } else {
    yPos = yPos + 1;
  }
// Serial.print("*** stringWordWrap...alignment passed = "); Serial.println(txtAlign);
  switch (txtAlign) {                               // Added 22-Mar-2017 DKF to allow specifying optional different alignment than left-justified
    case LEFT:
      ui.setTextAlignment(LEFT);
      xBase = 0;
      break;
    case RIGHT:
      ui.setTextAlignment(RIGHT);
      xBase = ILI9341_TFTWIDTH;
      break;
    case CENTER:
      ui.setTextAlignment(CENTER);
      xBase = ILI9341_TFTWIDTH / 2;
      break;
  }    

  int displayStringLen = displayString.length();
//  Serial.println("***Beginning word wrap of passed string***");
//  Serial.print("Length of passed string:  "); Serial.println(displayStringLen);
//  Serial.print("Calc chars / line:  "); Serial.println(charsPerLine);
  while (estBreak < displayStringLen) {
    // Search backwards from the estimated line break for the first space or "..." sequence
//    Serial.print("Beginning line index:  "); Serial.println(beginLineIndex);
//    Serial.print("Estimated break location:  "); Serial.println(estBreak);
    spaceBreakLocation = (displayString.substring(beginLineIndex,estBreak)).lastIndexOf(" ",estBreak);
    ellipseBreakLocation = (displayString.substring(beginLineIndex,estBreak)).lastIndexOf("...",estBreak)+2;
    // set the break location at the greater of where a space is found or an ellipse sequence
    if (spaceBreakLocation >= ellipseBreakLocation) {
      breakLocation = spaceBreakLocation;
    } else {
      breakLocation = ellipseBreakLocation;
    }
//    Serial.print("Found break location at character:  "); Serial.println(breakLocation);
    // If no space is found in the line segment, force a break at the max chars / line position.
    if (breakLocation == -1) {
      breakLocation = estBreak;
    } else {
      breakLocation = breakLocation + beginLineIndex;
    }
    // Added following logic to test the line to be displayed to make sure it will fit on the display
    // without wrapping.  This was to correct certain rare situations where the combination of certain
    // "wide" characters (such as "W") would cause the line to be too long to fit.
    //    Added by DK Fowler ... 17-Mar-2017
    displayString.substring(beginLineIndex,breakLocation).toCharArray(testString, (breakLocation-beginLineIndex));
    tft.getTextBounds(testString, x, y, &x1, &y1, &w, &h);
    if (w >= 239) { // max display width; may add logic to get this from the driver in the future vs. hard-coding
      estBreak--;   // decrement the line break estimation and try again...
    } else {
//      tft.println(displayString.substring(beginLineIndex,breakLocation));
      ui.drawString(xBase, yPos, displayString.substring(beginLineIndex,breakLocation) );
      lineCnt++;
      yPos = yPos + h + 3;
      if (lineCnt >= maxPanelLines) {
        panelCnt++;
        lineCnt = 0;
        yPos = yBase + 1;
        // temporarily detach the bottom panel ticker since we're going to control display
        // of the variable number of message panels in this routine
        botPanelTicker.detach();
        // debug...
        Serial.println("*** stringWordWrap delaying for panel-break view... ***");
        delayAndCheckTouch(UPDATE_BOT_PANEL_INTERVAL_SECS * 1000);
        // delay(UPDATE_BOT_PANEL_INTERVAL_SECS * 1000);
        Serial.print("stringWordWrap panel-break delay:  time(ms):  "); Serial.println(millis());        
        resetTickerFlag = true;            // flag that the ticker needs to be reattached
        // debug
        // ui.drawString(10, 160, "xBase = " + String(xBase) );
        // ui.drawString(10, 174, "yPos  = " + String(yPos) );
        // delay(2000);
        
        tft.setCursor(xBase,yPos);         // reset cursor to top of panel
        tft.fillRect(0, 233, 240, 87, ILI9341_BLACK);   // clear panel for next display
      }
//    Serial.print("["); Serial.print(displayString.substring(beginLineIndex,breakLocation)); Serial.println("]");
      beginLineIndex = breakLocation + 1;
      estBreak = breakLocation + charsPerLine; 
    }  
  }
  if (estBreak >= displayStringLen) {
    //  Display the last string segment
    //    Modified 11-Mar-2018...DK Fowler
    //  Modified the following code to add the truncation indicator to the last display string segment, where applicable.  This can occur 
    //  since the JSON parser only returns the first 512 characters.  This change allows for a "cleaner" display string, as previously, 
    //  it was likely that the last character(s) would be overwritten by the truncation indicator.
    String lastStringSeg = displayString.substring(beginLineIndex);
    if (truncFlag) {
      lastStringSeg = lastStringSeg + "<...>";
    }
    ui.drawString(xBase, yPos, lastStringSeg);
    //lastStringSeg.toCharArray(testString, (lastStringSeg.length()));
    //tft.getTextBounds(testString, xPos, yPos, &x1, &y1, &w, &h);
    //xPos = xPos + w;
    //tft.setCursor(xPos, yPos);
    lastPanelDelay = true;
    panelCnt++;               // Added 5-Mar-2018, DKF, to correct bug in returning correct panel count
  }  
//    Serial.print("["); Serial.print(displayString.substring(beginLineIndex)); Serial.println("]");
  if (resetTickerFlag) {
  // First, check to see if a last delay is required for the display of the remaining panel.  This situation could arise rarely
  // when the last string segment to be displayed is at the beginning of a new panel.
  //    Added 04/08/17...DK Fowler
    if (lastPanelDelay) {
      // debug...
      Serial.println("*** stringWordWrap delaying for last panel-break view... ***");
      delayAndCheckTouch(UPDATE_BOT_PANEL_INTERVAL_SECS * 1000);
      // delay(UPDATE_BOT_PANEL_INTERVAL_SECS * 1000);
      Serial.print("stringWordWrap panel-break delay:  time(ms):  "); Serial.println(millis());        
    }
  // reattach ticker for bottom panel update
    botPanelTicker.attach(UPDATE_BOT_PANEL_INTERVAL_SECS, setReadyForBotPanelUpdate); // To trigger updating bottom-panel on screen
    // force a bottom panel update to get back in sync with the ticker
    readyForBotPanelUpdate = true;
    Serial.print("stringWordWrap reset bot ticker:  time(ms):  "); Serial.println(millis());
  }

// debug
endExecutionMillis = millis();
executionMillis = endExecutionMillis - startExecutionMillis;
Serial.print("*** stringWordWrap execution time:  "); Serial.println(executionMillis);

return panelCnt;
} 

void drawWiFiQuality() { 
  const byte numBars   = 5;     // set the number of total bars to display
  const byte barWidth  = 3;     // set bar width, height in pixels
  const byte barHeight = 20;    // should be multiple of numBars, or to indicate zero value
  const byte barSpace = 1;      // set number of pixels between bars
  const byte barXPosBase = 215; // set the baseline X-pos for drawing the bars
  const byte barYPosBase = 20;  // set the baseline Y-pos for drawing the bars
  const uint16_t barColor     = ILI9341_YELLOW;
  const uint16_t barBackColor = ILI9341_DARKGREY;
  
  int8_t quality = getWifiQuality();
  //  Check to see if WiFi is disconnected; if so, then automatically set the quality to 0.
  //    Added by DK Fowler ... 16-Feb-2018
  if (WiFi.status()!=WL_CONNECTED){
    quality = 0;
  }

  if (displayWiFiSignalFlag) {
    tft.setFont(&Droid_Sans_10);
    ui.setTextAlignment(RIGHT);
    tft.setTextColor(ILI9341_YELLOW);
    currTextColor = ILI9341_YELLOW;
    
    ui.drawString(220, 10,  String(quality) + "%");

    for (int8_t i = 0; i < numBars; i++) {                      // current bar loop
      byte barSpacer = i * barSpace;
      byte tempBarHeight = (barHeight / numBars) * (i + 1);
      for (int8_t j = 0; j < tempBarHeight; j++) {              // draw bar height loop
          for (byte ii = 0; ii < barWidth; ii++) {              // draw bar width loop
            byte nextBarThreshold = (i + 1) * (100 / numBars);
            byte currentBarThreshold = i * (100 / numBars);
            byte currentBarIncrements = (barHeight / numBars) * (i + 1);
            float rangePerBar = (100 / numBars);
            float currentBarStrength;
            if ( ( quality > currentBarThreshold ) && ( quality < nextBarThreshold) ) {
              currentBarStrength = ( (quality - currentBarThreshold) / rangePerBar) * currentBarIncrements;
            } else if ( quality >= nextBarThreshold ) {
              currentBarStrength = currentBarIncrements;
            } else {
              currentBarStrength = 0;
            }
            if (j < currentBarStrength) {
              tft.drawPixel((barXPosBase + barSpacer + ii) + (barWidth * i), barYPosBase - j, barColor);
            } else {
              tft.drawPixel((barXPosBase + barSpacer + ii) + (barWidth * i), barYPosBase - j, barBackColor);
            }
          }  
      }
    }
  } else {
  // Draw target for touch...
  // tft.drawRect(190, 0, 50, 28, ILI9341_WHITE);
    tft.fillRect(190, 0, 50, 28, ILI9341_BLACK);
  }  

}
    
// converts the dBm to a range between 0 and 100%
int8_t getWifiQuality() {
  int32_t dbm = WiFi.RSSI();
  if(dbm <= -100) {
      return 0;
  } else if(dbm >= -50) {
      return 100;
  } else {
      return 2 * (dbm + 100);
  }
}

void calibrateTouch() {

  uint16_t upperLeftCoordX, upperLeftCoordY, lowerRightCoordX, lowerRightCoordY;
  uint16_t LCDCoordX, LCDCoordY;

  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&ArialRoundedMTBold_14);
  ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  currTextColor = ILI9341_CYAN;
  
  ui.setTextAlignment(CENTER);


  // draw upper, lower target
  tft.drawFastVLine(0, 0, 20, ILI9341_WHITE);
  tft.drawFastHLine(0, 0, 20, ILI9341_WHITE);
  
  ui.drawString(120, 160, "Touch upper left corner");

  //  Check to see if screen has been touched

  boolean isTouched = ts.touched();
  while (!isTouched) {
    delay(10);
    isTouched = ts.touched();
  }
  // Give further instructions
  tft.fillRect(0, 160, 240, 200, ILI9341_BLACK);
  ui.drawString(120, 160, "Now, move towards corner");
  ui.drawString(120, 176, "Watch min coordinates until");
  ui.drawString(120, 192, "they no longer decrease");

  minCoordX = 10000;
  minCoordY = 10000;
  maxCoordX = 0;
  maxCoordY = 0;
  long lastTouchMillis = millis();
  // Screen touch detected...now start gathering coordinates
  while (isTouched || ((millis() - lastTouchMillis) < 5000) ) {
    TS_Point pt = ts.getPoint();
    Serial.println("Upper left coordinates:");
    Serial.print("x = ");
    Serial.print(pt.x);
    Serial.print(", y = ");
    Serial.println(pt.y);

    tft.setCursor(80, 40);
    tft.print("Current pos:");
    tft.setCursor(10, 60);
    tft.print("X = ");
    tft.fillRect(40, 46, 60, 16, ILI9341_BLACK);
    tft.setCursor(50, 60);
    tft.print(pt.x);
    tft.setCursor(130, 60);
    tft.print("Y = ");
    tft.fillRect(160, 46, 60, 16, ILI9341_BLACK);
    tft.setCursor(170, 60);
    tft.print(pt.y);
    
//    tft.fillRect(0, 21, 240, 319, ILI9341_BLACK);
//    ui.setTextAlignment(CENTER);
//    ui.drawString(20, 80, String("Current pos:  X=" + String(pt.x) + " Y=" + String(pt.y)));
    upperLeftCoordX = pt.x;
    upperLeftCoordY = pt.y;
    Serial.print("Upper left coords (X,Y):  "); Serial.print(upperLeftCoordX); Serial.print(", "); Serial.println(upperLeftCoordY);
    if (upperLeftCoordX < minCoordX) minCoordX = upperLeftCoordX;
    if (upperLeftCoordY < minCoordY) minCoordY = upperLeftCoordY;
    if (upperLeftCoordX > maxCoordX) maxCoordX = upperLeftCoordX;
    if (upperLeftCoordY > maxCoordY) maxCoordY = upperLeftCoordY;


    tft.setCursor(75, 100);
    tft.print("Minimum pos:");
    tft.setCursor(10, 120);
    tft.print("X = ");
    tft.fillRect(40, 106, 60, 16, ILI9341_BLACK);
    tft.setCursor(50, 120);
    tft.print(minCoordX);
    tft.setCursor(130, 120);
    tft.print("Y = ");
    tft.fillRect(160, 106, 60, 16, ILI9341_BLACK);
    tft.setCursor(170, 120);
    tft.print(minCoordY);

//    ui.drawString(20, 96, String("Minimum pos:  X=" + String(minCoordX) + " Y=" + String(minCoordY)));
    yield();

    isTouched = ts.touched();
    if (isTouched) lastTouchMillis = millis();

  }

  tft.fillScreen(ILI9341_BLACK);
  ui.drawString(120, 160, "OK...Now touch lower");
  ui.drawString(120, 176, "right corner");

  // draw lower target
  tft.drawFastVLine(239, 300, 20, ILI9341_WHITE);
  tft.drawFastHLine(220, 319, 20, ILI9341_WHITE);

  //  Check to see if screen has been touched

  isTouched = ts.touched();
  while (!isTouched) {
    delay(10);
    isTouched = ts.touched();
  }
  // Give further instructions
  tft.fillRect(0, 160, 240, 50, ILI9341_BLACK);
  ui.drawString(120, 160, "Now, move towards corner");
  ui.drawString(120, 176, "Watch max coordinates until");
  ui.drawString(120, 192, "they no longer increase");

  lastTouchMillis = millis();
  // Screen touch detected...now start gathering coordinates
  while (isTouched || ((millis() - lastTouchMillis) < 5000) ) {
    TS_Point pt = ts.getPoint();
    Serial.println("Lower right coordinates:");
    Serial.print("x = ");
    Serial.print(pt.x);
    Serial.print(", y = ");
    Serial.println(pt.y);

    tft.setCursor(80, 40);
    tft.print("Current pos:");
    tft.setCursor(10, 60);
    tft.print("X = ");
    tft.fillRect(40, 46, 60, 16, ILI9341_BLACK);
    tft.setCursor(50, 60);
    tft.print(pt.x);
    tft.setCursor(130, 60);
    tft.print("Y = ");
    tft.fillRect(160, 46, 60, 16, ILI9341_BLACK);
    tft.setCursor(170, 60);
    tft.print(pt.y);
    
    lowerRightCoordX = pt.x;
    lowerRightCoordY = pt.y;
    Serial.print("Lower right coords (X,Y):  "); Serial.print(lowerRightCoordX); Serial.print(", "); Serial.println(lowerRightCoordY);
    if (lowerRightCoordX > maxCoordX) maxCoordX = lowerRightCoordX;
    if (lowerRightCoordY > maxCoordY) maxCoordY = lowerRightCoordY;
    if (lowerRightCoordX < minCoordX) minCoordX = lowerRightCoordX;
    if (lowerRightCoordY < minCoordY) minCoordY = lowerRightCoordY;


    tft.setCursor(75, 100);
    tft.print("Maximum pos:");
    tft.setCursor(10, 120);
    tft.print("X = ");
    tft.fillRect(40, 106, 60, 16, ILI9341_BLACK);
    tft.setCursor(50, 120);
    tft.print(maxCoordX);
    tft.setCursor(130, 120);
    tft.print("Y = ");
    tft.fillRect(160, 106, 60, 16, ILI9341_BLACK);
    tft.setCursor(170, 120);
    tft.print(maxCoordY);

    yield();

    isTouched = ts.touched();
    if (isTouched) lastTouchMillis = millis();

  }

  Serial.print("Minimum coordinates:  ");
  Serial.print("X="); Serial.print(minCoordX); Serial.print(" Y="); Serial.println(minCoordY);

  Serial.print("Maximum coordinates:  ");
  Serial.print("X="); Serial.print(maxCoordX); Serial.print(" Y="); Serial.println(maxCoordY);

  tft.fillScreen(ILI9341_BLACK);
  ui.drawString(120, 160, "Calibration set!");

  delay(5000);

  // Give instruction to continue to map screen to touch
  tft.fillScreen(ILI9341_BLACK);
  ui.drawString(120, 160, "Touch screen to continue");

  //  Check to see if screen has been touched

  isTouched = ts.touched();
  while (!isTouched) {
    delay(10);
    isTouched = ts.touched();
  }

// Now try mapping the touch coordinates to the screen coordinates
  lastTouchMillis = millis();
  while (isTouched || ((millis() - lastTouchMillis) < 5000) ) {
    Serial.print("Last touched millis:  "); Serial.println((millis() - lastTouchMillis));
    TS_Point pt = ts.getPoint();
    tft.setCursor(80, 40);
    tft.print("Current pos:");
    tft.setCursor(10, 60);
    tft.print("X = ");
    tft.fillRect(40, 46, 60, 16, ILI9341_BLACK);
    tft.setCursor(50, 60);
    tft.print(pt.x);
    tft.setCursor(130, 60);
    tft.print("Y = ");
    tft.fillRect(160, 46, 60, 16, ILI9341_BLACK);
    tft.setCursor(170, 60);
    tft.print(pt.y);

    LCDCoordX = map(pt.y,maxCoordY,minCoordY,0,239);
    constrain(LCDCoordX,0,239);
    LCDCoordY = map(pt.x,minCoordX,maxCoordX,0,319);
    constrain(LCDCoordY,0,319);
    
    tft.setCursor(75, 100);
    tft.print("Screen pos:");
    tft.setCursor(10, 120);
    tft.print("X = ");
    tft.fillRect(40, 106, 60, 16, ILI9341_BLACK);
    tft.setCursor(50, 120);
    tft.print(LCDCoordX);
    tft.setCursor(130, 120);
    tft.print("Y = ");
    tft.fillRect(160, 106, 60, 16, ILI9341_BLACK);
    tft.setCursor(170, 120);
    tft.print(LCDCoordY);

    yield();

    isTouched = ts.touched();
    if (isTouched) lastTouchMillis = millis();

  }  

  tft.fillScreen(ILI9341_BLACK);
  ui.drawString(120, 160, "Done!");

  delay(5000);

}

// Routine to check which region of the screen was touched
byte checkTouch (uint16_t touchX, uint16_t touchY, uint32_t startTouchTime) {

  Serial.print("Start touch time MS:  "); Serial.println(startTouchTime);
  
  uint16_t LCDCoordX, LCDCoordY;
  uint32_t lastTouchTime;
  byte region;

  Serial.print("checkTouch:  touchX = "); Serial.print(touchX); Serial.print(", touchY = "); Serial.println(touchY);
  
  // first map the touch screen coordinates to the LCD coordinates
  LCDCoordX = map(touchY,maxCoordY,minCoordY,0,239);
  constrain(LCDCoordX,0,239);
  LCDCoordY = map(touchX,minCoordX,maxCoordX,0,319);
  constrain(LCDCoordY,0,319);

  Serial.print("checkTouch:  LCDCoordX = "); Serial.print(LCDCoordX); Serial.print(", LCDCoordY = "); Serial.println(LCDCoordY);
    
  bool isTouched = ts.touched();
  while (isTouched) {
    delay(10);        // to ensure time for WiFi operations
    isTouched = ts.touched();
  }
  lastTouchTime = millis();
  touchTimeMS = lastTouchTime - startTouchTime;
  
  if ((LCDCoordX >=190) && ((LCDCoordY <=25) && (LCDCoordY >= 0)) )   {
    region = 1;     // WiFi indicator panel
  } else if (((LCDCoordX >=0) && (LCDCoordX <79)) && ((LCDCoordY <=230) && (LCDCoordY >= 155)) ) {
    region = 2;     // Forecast panel 1
  } else if (((LCDCoordX >=80) && (LCDCoordX <159)) && ((LCDCoordY <=230) && (LCDCoordY >= 155)) ) {
    region = 3;     // Forecast panel 2
  } else if (((LCDCoordX >=160) && (LCDCoordX <=240)) && ((LCDCoordY <=230) && (LCDCoordY >= 155)) ) {
    region = 4;     // Forecast panel 3
  } else if (((LCDCoordX >=0) && (LCDCoordX <=240)) && ((LCDCoordY <=62) && (LCDCoordY >= 26)) ) {
    region = 5;     // Date / time panel
  } else if (((LCDCoordX >=0) && (LCDCoordX <=240)) && ((LCDCoordY <=144) && (LCDCoordY >= 65)) ) {
    region = 6;     // Current conditions / temperature panel
  } else if (((LCDCoordX >=0) && (LCDCoordX <=240)) && (LCDCoordY >= 232) ) {
    region = 7;     // Bottom panel
  } else {
    region = 0;
  }

  return region;

}

void checkTouchCalibration() {
/*
 *    This routine will check for the existence of the touch-screen calibration file, and if found, will load the
 *    screen calibration coordinate mapping.  If the file does not exist, it will initiate the screen calibration 
 *    routine and store the resulting coordinates.
 *        Written by DK Fowler ... 23-Jan-2018
 */
    
if (SPIFFS.exists("/tscal.json")) {
  //file exists, reading and loading
  Serial.println("reading TS calibration file");
  File calFile = SPIFFS.open("/tscal.json", "r");
  if (calFile) {
    Serial.println("opened TS calibration file");
    size_t size = calFile.size();
    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);

    calFile.readBytes(buf.get(), size);
    DynamicJsonBuffer jsonTSBuffer;
    JsonObject& TSjson = jsonTSBuffer.parseObject(buf.get());
    TSjson.printTo(Serial);
    if (TSjson.success()) {
      Serial.println("\nparsed TS calibration json");

      minCoordX = TSjson["TSminCoordX"];
      maxCoordX = TSjson["TSmaxCoordX"];
      minCoordY = TSjson["TSminCoordY"];
      maxCoordY = TSjson["TSmaxCoordY"];
      Serial.println("...finished reading calibration...");
      
    // end read          
    } else {
      Serial.println("failed to load calibration json information");
    }
  }   // failed to open calibration file
} else {
  Serial.println("/tscal.json does not exist.");
  //  Initiate the touch-screen calibration routine...
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&ArialRoundedMTBold_14);
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  currTextColor = ILI9341_ORANGE;
  
  ui.setTextAlignment(CENTER);
  ui.drawString(120, 56, "No stored touch-screen");
  ui.drawString(120, 74, "calibration info...");
  ui.drawString(120, 92, "Initiating calibration routine.");
  delay(2000);

  calibrateTouch();
  saveCalibration();
       
}   // end of check if SPIFFS calibration file exists

}

void saveCalibration()  {

/*
 *  This routine will save the mapped touch-screen coordinates to a calibration file in SPIFFS.
 *      Written by DK Fowler ... 31-Jan-2018
 */

  //  Now save coordinates for screen mapping...
  Serial.println("saving TS calibration");
  DynamicJsonBuffer jsonTSBuffer;
  JsonObject& TSjson = jsonTSBuffer.createObject();
  TSjson["TSminCoordX"] = minCoordX;
  TSjson["TSmaxCoordX"] = maxCoordX;
  TSjson["TSminCoordY"] = minCoordY;
  TSjson["TSmaxCoordY"] = maxCoordY;

  File calFile = SPIFFS.open("/tscal.json", "w");
  if (!calFile) {
    Serial.println("failed to open TS calibration file for writing");
  }

  TSjson.printTo(Serial);
  TSjson.printTo(calFile);
  Serial.println("finished saving TS calibration file, closing");
  calFile.close();
  //end save
 
}

void manualInitCalibration() {

/*   This routine will initiate the calibration routine to map the touch-screen coordinates to the physical screen.  This is a wrapper function for when
 *   the calibration routine is manually initiated by the user by touching the screen in the appropriate location.
 *      Written by DK Fowler ... 31-Jan-2018
 */

// Save the current time to allow checking for data updates prior to resuming.
//    Added by DK Fowler ... 09-Mar-2018
uint32_t manualInitCalStartTime = millis();

// detach all tickers while we run the calibration...
disableTimers();

// First, clear screen and display a brief message...
tft.fillScreen(ILI9341_BLACK);
tft.setFont(&ArialRoundedMTBold_14);
ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
currTextColor = ILI9341_CYAN;

ui.setTextAlignment(CENTER);
ui.drawString(120, 160, "Initiating screen calibration");
ui.drawString(120, 176, "Follow on-screen instructions");
delay(2000);

// Run the calibration routine and save the results...
calibrateTouch();
saveCalibration();

uint32_t manualInitCalEndTime = millis();
uint32_t manualInitCalExecTimeMillis = manualInitCalEndTime - manualInitCalStartTime;

// Re-enable screen update timers and update the screen
restartUpdates(manualInitCalExecTimeMillis);     // re-enable tickers and update screen to resume

}

void touchDisplayDayForecast(uint8_t TSdayIndex) {
/*
 * This routine will display a detailed screen showing both the day and night forecast details for the forecast day selected by touching the appropriate
 * screen icon.  It will display the detailed screen for 30 seconds (time as specified in the #define below) before returning to the typical rotating
 * screen display.
 *    Written by DK Fowler ... 24-Jan-2018
 *    
 */

//  Hold the start time in order to allow decision on whether data needs to be updated on exit
//    Added by DK Fowler ... 09-Mar-2018

uint32_t touchDisplayStartTime = millis();

// detach all tickers while we display the detailed forecast...
disableTimers();

// First, clear screen
tft.fillScreen(ILI9341_BLACK);

// Daytime forecast...
//**String weatherIcon = wunderground.getForecastIconText(TSdayIndex);
//**ui.drawBmp("/Condition/" + weatherIcon + ".bmp", 0, 0);
//**Serial.print("*** TS Forecast icon[day]:  "); Serial.print(weatherIcon); Serial.print("["); Serial.print(TSdayIndex); Serial.println("] ***");

// Forecast date text
tft.setFont(&Droid_Sans_12);
ui.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
//currTextColor = ILI9341_YELLOW;

ui.setTextAlignment(RIGHT);
//String forecastDate = wunderground.getForecastMonth(TSdayIndex/2) + "/" + wunderground.getForecastDay(TSdayIndex/2);
String forecastDate = "01/01";  // Temp until replaced
//String forecastTextPeriod = wunderground.getForecastTitle(TSdayIndex);
String forecastTextPeriod = "Void"; // Temp until replaced
String dayDate = forecastTextPeriod + "-" + forecastDate;
ui.drawString(230, 16, dayDate);

// Forecast high temp
ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
//currTextColor = ILI9341_WHITE;

//ui.drawString(230, 33, "High: " + wunderground.getForecastHighTemp(TSdayIndex));
ui.drawString(230, 33, "High: 999");   // Temp until replaced
  
// Probability of precipitation
//String PoP = wunderground.getPoP(TSdayIndex) + "%";
String PoP = "0%";      // Temp until replaced
PoP = "Precip:  " + PoP;
ui.setTextAlignment(RIGHT);
tft.setFont(&Droid_Sans_12);
ui.drawString(230, 50, PoP);

// Weather Text
//  ui.drawString(220, 34, wunderground.getForecastText(TSdayIndex));
//String temp = wunderground.getForecastText(TSdayIndex);
String temp = "Void";   // Temp until replaced
tft.setFont(&Droid_Sans_10);  
stringWordWrap(temp, 0, LEFT, false, 88, 6);

// Draw divider
tft.drawFastHLine(80, 171, 80, ILI9341_CYAN);

// Nighttime forecast...
//**weatherIcon = wunderground.getForecastIconText(TSdayIndex+1);

// If the icon name contains any of the following, leave it as is since these have unique nighttime icons.  Otherwise,
// we need to strip off the leading "nt_" indicating night for most icons...as these icons are common to day/night.
//**if ( !(( weatherIcon == "nt_clear" ) ||
//**       ( weatherIcon == "nt_sunny" ) ||
//**       ( weatherIcon == "nt_mostlycloudy" ) ||
//**       ( weatherIcon == "nt_mostlysunny" ) ||
//**       ( weatherIcon == "nt_partlysunny"  ) ||
//**       ( weatherIcon == "nt_partlycloudy" ) ) ) {
//**   weatherIcon = weatherIcon.substring(3) ;
//**   Serial.println("*** TS night weather icon replaced:  " + weatherIcon);
//**}

//**ui.drawBmp("/Condition/" + weatherIcon + ".bmp", 0, 161);
//**Serial.print("*** TS Forecast night icon[day]:  "); Serial.print(weatherIcon); Serial.print("["); Serial.print(TSdayIndex+1); Serial.println("] ***");
  
// Forecast date text
ui.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
//currTextColor = ILI9341_YELLOW;

tft.setFont(&Droid_Sans_12);
ui.setTextAlignment(RIGHT);
//forecastDate = wunderground.getForecastMonth(TSdayIndex/2) + "/" + wunderground.getForecastDay(TSdayIndex/2);
forecastDate = "01/01";   // Temp until replaced
//forecastTextPeriod = wunderground.getForecastTitle(TSdayIndex+1);
forecastTextPeriod = "Void";    // Temp until replaced
dayDate = forecastTextPeriod + "-" + forecastDate;
ui.drawString(230, 188, dayDate);

// Forecast low temp
ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
//currTextColor = ILI9341_WHITE;

//ui.drawString(230, 205, "Low: " + wunderground.getForecastLowTemp(TSdayIndex));
ui.drawString(230, 205, "Low: 0");   // Temp until replaced
  
// Overlay probability of precipitation
//PoP = wunderground.getPoP(TSdayIndex+1) + "%";
PoP = "0%";    // Temp until replaced
PoP = "Precip:  " + PoP;
tft.setFont(&Droid_Sans_12);
ui.setTextAlignment(RIGHT);
ui.drawString(230, 222, PoP);

// Weather Text
//temp = wunderground.getForecastText(TSdayIndex+1);
temp = "Void";    // Temp until replaced
tft.setFont(&Droid_Sans_10);  
stringWordWrap(temp, 0, LEFT, false, 254, 6);

//  Wait until the screen has been touched

bool isTouched = ts.touched();
while (!isTouched) {
  delay(10);
  isTouched = ts.touched();
}

uint32_t touchDisplayEndTime = millis();
uint32_t screenDisplayTimeMillis = touchDisplayEndTime - touchDisplayStartTime;

restartUpdates(screenDisplayTimeMillis);     // re-enable tickers and update screen to resume

}

void disableTimers()  {

/*
 *  This routine will disable all update timers in order to prevent further screen updates until
 *  re-enabled.
 *      Written by DK Fowler ... 29-Jan-2018
*/

stallForecastTimer = forecastTimer;         // Added 09-Mar-2018, DKF, to hold the current value of these timers for checking for updates after resuming
stallAstronomyTimer = astronomyTimer;       // Added 09-Mar-2018, DKF, " "

ticker.detach();            // Now for current observations update only
midPanelTicker.detach();    // To trigger updating mid-panel on screen
botPanelTicker.detach();    // To trigger updating bottom-panel on screen
forecastTicker.detach();    // For controlling 10-day forecast update
astronomyTicker.detach();   // For controlling astronomy data update

}

void restartUpdates(uint32_t displayMillis) {

/*
 *  This routine will re-enable all update timers and reset the screen update flags in order to trigger
 *  a screen refresh.
 *      Written by DK Fowler ... 29-Jan-2018
 *  
*/

// Clear screen before resuming
tft.fillScreen(ILI9341_BLACK);

// all done, reattach the appropriate tickers to continue, and reset the update flags to refresh the screen
ticker.attach(UPDATE_INTERVAL_SECS, setReadyForWeatherUpdate);                    // Now for current observations update only
midPanelTicker.attach(UPDATE_MID_PANEL_INTERVAL_SECS, setReadyForMidPanelUpdate); // To trigger updating mid-panel on screen
botPanelTicker.attach(UPDATE_BOT_PANEL_INTERVAL_SECS, setReadyForBotPanelUpdate); // To trigger updating bottom-panel on screen
forecastTicker.attach(60, setReadyForForecastUpdate);                             // For controlling 10-day forecast update
astronomyTicker.attach(60, setReadyForAstronomyUpdate);                           // For controlling astronomy data update

// Check the display time to see how long we've been paused displaying the "touched" screen prior to resuming.  If longer than the ticker timer, than refresh the data
// in addition to refreshing the screen region(s).  Otherwise, just refresh the screen region.  This logic should help to avoid unnecessary API calls to WU.
//    Added by DK Fowler ... 09-Mar-2018
if (displayMillis >= (UPDATE_INTERVAL_SECS * 1000) ) {
  updateData();                                                                   // Update current conditions immediately on return prior to screen refresh
  readyForWeatherUpdate = false;
}
refreshCurrentConditions();
uint32_t remainForecastTimerMillis = (UPDATE_INTERVAL_FORECAST_SECS - (stallForecastTimer*60) ) * 1000;
if (displayMillis >= remainForecastTimerMillis) {
  readyForForecastUpdate = true;
}
uint32_t remainAstronomyTimerMillis = (UPDATE_INTERVAL_ASTRO_SECS - (stallAstronomyTimer*60) ) * 1000;
if (displayMillis >= remainAstronomyTimerMillis) {  
  readyForAstronomyUpdate = true;
}
updateMidPanel();
readyForBotPanelUpdate = true;

}


bool checkScreenTouch() {
/*
 * This routine will check if the screen has been touched, and execute various display routines depending on which region is touched.
 *    Modified by DK Fowler ... 5-Mar-2018, to move to a separate function from mainline routine
 */

  //  Check to see if screen has been touched
  boolean isTouched = ts.touched();
  TS_Point pt = ts.getPoint();
  if (isTouched) {
    startTouchTime = millis();    // get the start time for the touch to pass to the checkTouch routine to calculate total touch-time
    // just for testing...
    Serial.print("Screen touch detected:  "); Serial.print("x = "); Serial.print(pt.x); Serial.print(", y = "); Serial.println(pt.y);
    touchX = pt.x;
    touchY = pt.y;

    touchRegion = 0;
    touchRegion = checkTouch(touchX, touchY, startTouchTime);
    Serial.print("Touch region calculated as "); Serial.println(touchRegion);

    uint8_t dayIndex;
    String day, forecastDate, dayDate;
    
    switch (touchRegion) {

      case 0:   // no match for the region touched
        break;

      case 1:   // touch region for WiFi signal strength display
        if (displayWiFiSignalFlag) {
          displayWiFiSignalFlag = false;
        } else {
          displayWiFiSignalFlag = true; 
        }
        drawWiFiQuality();
        break;

      case 2:   // touch region for forecast panel 1
        //dayIndex = (((midPanelIndex - 1) * 3) + (1 - 1)) * 2;
        if (touchTimeMS > 3000) {     // only execute if display is touched in the region for > 3 secs.
          manualInitCalibration();
        } //else {
          //touchDisplayDayForecast(dayIndex);
        //}       
        break;

      case 3:   // touch region for forecast panel 2
        //dayIndex = (((midPanelIndex - 1) * 3) + (2 - 1)) * 2;
        if (touchTimeMS > 3000) {     // only execute if display is touched in the region for > 3 secs.
          manualInitCalibration();
        } //else {
          //touchDisplayDayForecast(dayIndex);
        //}             
        break;

      case 4:   // touch region for forecast panel 3
        //dayIndex = (((midPanelIndex - 1) * 3) + (3 - 1)) * 2;
        if (touchTimeMS > 3000) {     // only execute if display is touched in the region for > 3 secs.
          manualInitCalibration();
        } //else {
          //touchDisplayDayForecast(dayIndex);
        //}       
        break;

      case 5:   // touch region for date/time
        if (touchTimeMS > 3000) {     // only execute if display is touched in the region for > 3 secs.
          tft.fillRect(0, 150, 240, 82, ILI9341_BLACK);    // Clear prior forecast panel
          tft.setFont(&Droid_Sans_12);
          ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
          currTextColor = ILI9341_CYAN;
          
          ui.setTextAlignment(CENTER);
          // ui.drawString(120, 190, "date/time touch, millis: " + String(touchTimeMS));
          startWebConfigPortal();
        }  
        break;

      case 6:   // touch region for current conditions
        dayIndex = 0;
        //touchDisplayDayForecast(dayIndex);
        break;

      case 7:   // touch region for bottom panel
        if (touchTimeMS > 3000) {     // only execute if display is touched in the region for > 3 secs.
          tft.fillRect(0, 150, 240, 82, ILI9341_BLACK);    // Clear prior forecast panel
          tft.setFont(&Droid_Sans_12);
          ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
          currTextColor = ILI9341_CYAN;
          
          ui.setTextAlignment(CENTER);
          drawAbout();
        }  
        break;

      default:
        break;
    }
    return true;    
  } else {
    return false;
  }

}

void delayAndCheckTouch(uint32_t delayMillis) {
/*
 * This routine will provide a delay loop for the specified milliseconds, while checking each iteration for a screen touch.  Any detected touch will
 * initiate an appropriate display screen depending on the region touched.
 *    Written by DK Fowler ... 5-Mar-2018
 *    
 */

  uint16_t calledTextColor = currTextColor; // Added 09-Mar-2018, DKF, to save the text color when the routine was called for later restoration
  int16_t holdCursorX, holdCursorY;        // Added 09-Mar-2018, DKF, to save current cursor position for later restoration

  holdCursorX = tft.getCursorX();
  holdCursorY = tft.getCursorY();

  // debug
  // ui.drawString(10, 100, "X = " + String(holdCursorX) );
  // ui.drawString(10, 116, "Y = " + String(holdCursorY) );
  // delay(2000);
  
  bool wasTouched = false;                // Added 09-Mar-2018, DKF, to get whether display was touched during delay loop
  uint32_t startDelayMillis = millis();   // get the start of execution time

  while ( (millis() < (startDelayMillis + delayMillis) ) && !wasTouched ) {       // If screen is touched, immediately exit delay loop afterwards
    wasTouched = checkScreenTouch();
    delay(20);
  }

  // Reset the text color to what it was before the routine was called
  //    Added by DK Fowler ... 09-Mar-2018
  ui.setTextColor(calledTextColor);
  // Restore cursor to its original position
  tft.setCursor(holdCursorX, holdCursorY);
  // Reset the text alignment left...(most common; may want to get current alignment when routine is called and restore it here for future)
  ui.setTextAlignment(LEFT);

}


void readSPIFFSConfig() {
/*
 * This routine will read the stored configuration from SPIFFS if it exists.
 *    Modified by DK Fowler ... 28-Jan-2018, to move this to its own function vs. in-line
 */
 
//  SPIFFS.begin();
//  Uncomment if you want to update all internet resources
//  Serial.println("*** Formatting SPIFFS, please wait... ***");
//  SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    // Uncomment following lines to delete the config.json file...
    //    if (SPIFFS.remove("/config.json")) {
    //       Serial.println("***Successfully deleted config.json file...***");
    //     } else {
    //       Serial.println("***...error deleting config.json file...***");
    //     }
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          //strcpy(WUNDERGROUND_API_KEY, json["WUNDERGROUND_API_KEY"]);
          strcpy(OPEN_WEATHER_MAP_APP_ID, json["OPEN_WEATHER_MAP_APP_ID"]);
          //strcpy(WUNDERGROUND_LANGUAGE, json["WUNDERGROUND_LANGUAGE"]);
          strcpy(OPEN_WEATHER_MAP_LANGUAGE, json["OPEN_WEATHER_MAP_LANGUAGE"]);
          //strcpy(WUNDERGROUND_COUNTRY, json["WUNDERGROUND_COUNTRY"]);
          //strcpy(WUNDERGROUND_CITY, json["WUNDERGROUND_CITY"]);
          strcpy(OPEN_WEATHER_MAP_LOCATION_ID, json["OPEN_WEATHER_MAP_LOCATION_ID"]);
          strcpy(DISPLAYED_CITY_NAME, json["DISPLAYED_CITY_NAME"]);
          strcpy(TZ_CITY, json["TZ_CITY"]);
          strcpy(MID_PANEL_DELAY, json["MID_PANEL_DELAY"]);               // Added 26-Jan-2018, DKF
          strcpy(BOT_PANEL_DELAY, json["BOT_PANEL_DELAY"]);               // Added 26-Jan-2018, DKF
          strcpy(RESTART_TIME, json["RESTART_TIME"]);                     // Added 01-Mar-2018, DKF
          strcpy(RESTART_INTERVAL, json["RESTART_INTERVAL"]);             // Added 01-Mar-2018, DKF
          Serial.println("...finished...");
        } else {
          Serial.println("failed to load json config");
        }
      }
    } else {
      Serial.println("/config.json does not exist.");
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read
}

void startWebConfigPortal() {
/*  
 *   This routine will start the web configuration portal and save the results as appropriate.  Before returning, it will perform  
 *   an ESP reset to ensure multiple requests to start the portal while running are successful.
 *      Modified by DK Fowler ... 28-Jan-2018, to move this routine to a separate function vs. in-line
 */

    Serial.println("Starting configuration portal.");

    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    //set config save notify callback
    wifiManager.setSaveConfigCallback(saveConfigCallback);

// Uncomment for testing wifi manager
//  wifiManager.resetSettings();
    wifiManager.setAPCallback(configModeCallback);

    // The extra parameters to be configured (can be either global or just in the setup)
    // After connecting, parameter.getValue() will get you the configured value
    // id/name placeholder/prompt default length
    //WiFiManagerParameter custom_WUNDERGROUND_API_KEY("WUkey", "WU API key", WUNDERGROUND_API_KEY, 18, "<p>Weather Underground API Key</p");
    WiFiManagerParameter custom_OPEN_WEATHER_MAP_APP_ID("OWMkey", "OWM App ID", OPEN_WEATHER_MAP_APP_ID, 33, "<p>OpenWeatherMap App ID</p");
    //WiFiManagerParameter custom_WUNDERGROUND_LANGUAGE("WUlanguage", "WU language", WUNDERGROUND_LANGUAGE, 4, "<p>Weather Underground Language</p");
    WiFiManagerParameter custom_OPEN_WEATHER_MAP_LANGUAGE("OWMlanguage", "OWM language", OPEN_WEATHER_MAP_LANGUAGE, 4, "<p>OpenWeatherMap Language</p");
    //WiFiManagerParameter custom_WUNDERGROUND_COUNTRY("WUcountry", "WU country", WUNDERGROUND_COUNTRY, 4, "<p>Weather Underground Country</p");
    //WiFiManagerParameter custom_WUNDERGROUND_CITY("WUcity", "WU city/PWS", WUNDERGROUND_CITY, 20, "<p>Weather Underground City/PWS</p");
    WiFiManagerParameter custom_OPEN_WEATHER_MAP_LOCATION_ID("OWMLocID", "OWM location ID", OPEN_WEATHER_MAP_LOCATION_ID, 10, "<p>OpenWeatherMap Location ID</p");
    WiFiManagerParameter custom_DISPLAYED_CITY_NAME("DispCityName", "Displayed City Name", DISPLAYED_CITY_NAME, 30, "<p>Displayed City Name</p");
    WiFiManagerParameter custom_TZ_CITY("TZCity", "TZ City", TZ_CITY, 30, TimeZoneConfig);
    //  Added custom panel delay setting...26-Jan-2018, DKF
    WiFiManagerParameter custom_MID_PANEL_DELAY("midPanelDelay", "Mid Panel Delay", MID_PANEL_DELAY, 3, "<p>Middle Panel Delay</p");
    WiFiManagerParameter custom_BOT_PANEL_DELAY("botPanelDelay", "Bot Panel Delay", BOT_PANEL_DELAY, 3, "<p>Bottom Panel Delay</p");
    //  Added custom auto restart time...01-Mar-2018, DKF
    WiFiManagerParameter custom_RESTART_TIME("restartTime", "Restart Time", RESTART_TIME, 6, "<p>Auto Restart Time (e.g., 21:00)</p");
    WiFiManagerParameter custom_RESTART_INTERVAL("restartInterval", "Restart Interval", RESTART_INTERVAL, 11, "<p>Auto Restart Interval (secs.)</p");

    //add all your parameters here
    //wifiManager.addParameter(&custom_WUNDERGROUND_API_KEY);
    wifiManager.addParameter(&custom_OPEN_WEATHER_MAP_APP_ID);
    //wifiManager.addParameter(&custom_WUNDERGROUND_LANGUAGE);
    wifiManager.addParameter(&custom_OPEN_WEATHER_MAP_LANGUAGE);
    //wifiManager.addParameter(&custom_WUNDERGROUND_COUNTRY);
    //wifiManager.addParameter(&custom_WUNDERGROUND_CITY);
    wifiManager.addParameter(&custom_OPEN_WEATHER_MAP_LOCATION_ID);
    wifiManager.addParameter(&custom_DISPLAYED_CITY_NAME);
    wifiManager.addParameter(&custom_TZ_CITY);
    wifiManager.addParameter(&custom_MID_PANEL_DELAY);          // Added 26-Jan-2018, DKF
    wifiManager.addParameter(&custom_BOT_PANEL_DELAY);          // Added 26-Jan-2018, DKF
    wifiManager.addParameter(&custom_RESTART_TIME);             // Added 01-Mar-2018, DKF
    wifiManager.addParameter(&custom_RESTART_INTERVAL);         // Added 01-Mar-2018, DKF

    // Check to see if we're currently connected to WiFi, and if so, disconnect.  This is necessary to change AP's, or the WiFi credentials will not
    // be saved if already connected.
    //    Added 02-Mar-2018, DKF
    if (WiFi.status() == WL_CONNECTED) {
      WiFi.persistent(false);                                   // Added 14-Mar-2018, DKF; this will supposedly prevent stored credentials from being wiped
      WiFi.disconnect();                                        // "          "        "
      WiFi.persistent(true);                                    // "          "        "

    }
    
    //it starts an access point 
    //and goes into a blocking loop awaiting configuration
    
    // Serial.print("Portal: "); Serial.print(configPortalHostname); Serial.print("  pass:  "); Serial.println(configPortalPassword);
    if (!wifiManager.startConfigPortal(configPortalHostname.c_str(), configPortalPassword)) {
      Serial.println("Not connected to WiFi but continuing anyway.");
    } else {
      //if you get here you have connected to the WiFi
      Serial.println("Connected to WiFi.");

        //read updated parameters
      //strcpy(WUNDERGROUND_API_KEY, custom_WUNDERGROUND_API_KEY.getValue());
      strcpy(OPEN_WEATHER_MAP_APP_ID, custom_OPEN_WEATHER_MAP_APP_ID.getValue());
      //strcpy(WUNDERGROUND_LANGUAGE, custom_WUNDERGROUND_LANGUAGE.getValue());
      strcpy(OPEN_WEATHER_MAP_LANGUAGE, custom_OPEN_WEATHER_MAP_LANGUAGE.getValue());
      //strcpy(WUNDERGROUND_COUNTRY, custom_WUNDERGROUND_COUNTRY.getValue());
      //strcpy(WUNDERGROUND_CITY, custom_WUNDERGROUND_CITY.getValue());
      strcpy(OPEN_WEATHER_MAP_LOCATION_ID, custom_OPEN_WEATHER_MAP_LOCATION_ID.getValue());
      strcpy(DISPLAYED_CITY_NAME, custom_DISPLAYED_CITY_NAME.getValue());
      strcpy(TZ_CITY, custom_TZ_CITY.getValue());
      strcpy(MID_PANEL_DELAY, custom_MID_PANEL_DELAY.getValue());   // Added 26-Jan-2018, DKF
      strcpy(BOT_PANEL_DELAY, custom_BOT_PANEL_DELAY.getValue());   // Added 26-Jan-2018, DKF
      strcpy(RESTART_TIME, custom_RESTART_TIME.getValue());         // Added 01-Mar-2018, DKF
      strcpy(RESTART_INTERVAL, custom_RESTART_INTERVAL.getValue()); // Added 01-Mar-2018, DKF

      // Debug
//      Serial.print("***Retrieved mid-panel delay:  "); Serial.println(MID_PANEL_DELAY);
//      Serial.print("***Retrieved bot-panel delay:  "); Serial.println(BOT_PANEL_DELAY);
//      delay(5000);
     
  //save the custom parameters to FS
      if (shouldSaveConfig) {
        Serial.println("saving config");
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.createObject();
        //json["WUNDERGROUND_API_KEY"] = WUNDERGROUND_API_KEY;
        json["OPEN_WEATHER_MAP_APP_ID"] = OPEN_WEATHER_MAP_APP_ID;
        //json["WUNDERGROUND_LANGUAGE"] = WUNDERGROUND_LANGUAGE;
        json["OPEN_WEATHER_MAP_LANGUAGE"] = OPEN_WEATHER_MAP_LANGUAGE;
        //json["WUNDERGROUND_COUNTRY"] = WUNDERGROUND_COUNTRY;
        //json["WUNDERGROUND_CITY"] = WUNDERGROUND_CITY;
        json["OPEN_WEATHER_MAP_LOCATION_ID"] = OPEN_WEATHER_MAP_LOCATION_ID;
        json["DISPLAYED_CITY_NAME"] = DISPLAYED_CITY_NAME;
        json["TZ_CITY"] = TZ_CITY;
        json["MID_PANEL_DELAY"] = MID_PANEL_DELAY;                  // Added 26-Jan-2018, DKF
        json["BOT_PANEL_DELAY"] = BOT_PANEL_DELAY;                  // Added 26-Jan-2018, DKF
        json["RESTART_TIME"] = RESTART_TIME;                        // Added 01-Mar-2018, DKF
        json["RESTART_INTERVAL"] = RESTART_INTERVAL;                // Added 01-Mar-2018, DKF

        Serial.print("TZ City:  "); Serial.println(TZ_CITY);

        File configFile = SPIFFS.open("/config.json", "w");
        if (!configFile) {
          Serial.println("failed to open config file for writing");
        }

        json.printTo(Serial);
        json.printTo(configFile);
        Serial.println("finished saving configuration file, closing");
        configFile.close();
      //end save

      }

    }
    //  Modified by DK Fowler ... 20-Dec-2018, from ESP.reset in order to do a "cleaner" restart
    ESP.restart(); // This is a bit crude. For some unknown reason webserver can only be started once per boot up 
                 // so resetting the device allows to go back into config mode again when it reboots.
    delay(5000);
    
}

void drawAbout() {
/*
  
  This routine will display a detailed screen concerning the current ESP state.
    Written by Daniel Eichorn, adapted for use without the minigrafix lib by DK Fowler ... 29-Jan-2018

*/

// Save the current time for later checking if data updates are required prior to resuming...
//    Added 09-Mar-2018, DK Fowler
  uint32_t drawAboutStartTime = millis();
  
// Define some variables to ease alignment
  uint16_t xColBase, yLineBase, yFontInc;

// First, clear screen
  tft.fillScreen(ILI9341_BLACK);

// Disable timers used for triggering screen updates temporarily...
  disableTimers();
  
  tft.setFont(&ArialRoundedMTBold_14);
  ui.setTextAlignment(CENTER);
  ui.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  //currTextColor = ILI9341_YELLOW;
  
  ui.drawString(120, 40, "ESP Statistics");

  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  //currTextColor = ILI9341_WHITE;
  
  tft.setFont(&Droid_Sans_12);
  ui.setTextAlignment(LEFT);
  xColBase  = 0;
  yLineBase = 64;
  yFontInc  = 18;
//  drawLabelValue(7, "Heap Mem:", String(ESP.getFreeHeap() / 1024)+"kb");
  ui.drawString(xColBase, yLineBase, "Heap Mem: " + String(ESP.getFreeHeap() / 1024) + "kb");
  yLineBase = yLineBase + yFontInc;
//  drawLabelValue(8, "Flash Mem:", String(ESP.getFlashChipRealSize() / 1024 / 1024) + "MB");
  ui.drawString(xColBase, yLineBase, "Flash Mem: " + String(ESP.getFlashChipRealSize() / 1024 / 1024) + "MB");
  yLineBase = yLineBase + yFontInc;
  ui.drawString(xColBase, yLineBase, "WiFi SSID:  " + String(WiFi.SSID()));
  yLineBase = yLineBase + yFontInc;
  String ip = WiFi.localIP().toString();
  ui.drawString(xColBase, yLineBase, "Local IP:  " + ip);
  yLineBase = yLineBase + yFontInc;
//  drawLabelValue(9, "WiFi Strength:", String(WiFi.RSSI()) + "dB");
  ui.drawString(xColBase, yLineBase, "WiFi Strength: " + String(WiFi.RSSI()) + "dB");
  yLineBase = yLineBase + yFontInc;
//  drawLabelValue(10, "Chip ID:", String(ESP.getChipId()));
  ui.drawString(xColBase, yLineBase, "Chip ID: " + String(ESP.getChipId()));
  yLineBase = yLineBase + yFontInc;
//  drawLabelValue(11, "VCC: ", String(ESP.getVcc() / 1024.0) +"V");
  ui.drawString(xColBase, yLineBase, "VCC: " + String(ESP.getVcc() / 1024.0) +"V");
  yLineBase = yLineBase + yFontInc;
//  drawLabelValue(12, "CPU Freq.: ", String(ESP.getCpuFreqMHz()) + "MHz");
  ui.drawString(xColBase, yLineBase, "CPU Freq.: " + String(ESP.getCpuFreqMHz()) + "MHz");
  char time_str[15];
  const uint32_t millis_in_day = 1000 * 60 * 60 * 24;
  const uint32_t millis_in_hour = 1000 * 60 * 60;
  const uint32_t millis_in_minute = 1000 * 60;
  uint8_t days = millis() / (millis_in_day);
  uint8_t hours = (millis() - (days * millis_in_day)) / millis_in_hour;
  uint8_t minutes = (millis() - (days * millis_in_day) - (hours * millis_in_hour)) / millis_in_minute;
  sprintf(time_str, "%2dd%2dh%2dm", days, hours, minutes);
  yLineBase = yLineBase + yFontInc;
//  drawLabelValue(13, "Uptime: ", time_str);
  ui.drawString(xColBase, yLineBase, "Uptime: " + String(time_str));
  yLineBase = yLineBase + yFontInc;

  ui.setTextAlignment(LEFT);
  ui.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  //currTextColor = ILI9341_YELLOW;
  
  ui.drawString(15, 230, "Last Reset:  ");
  xColBase = 15;
  yLineBase = 245;
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  //currTextColor = ILI9341_WHITE;
  
  String temp = String(ESP.getResetInfo());
  //ui.drawString(xColBase, yLineBase, ESP.getResetInfo());
//  ui.drawStringMaxWidth(15, 265, 240 - 2 * 15, ESP.getResetInfo());
  tft.setFont(&Droid_Sans_10);
  stringWordWrap(temp, 0, LEFT, false, 245, 6);

//  Wait until the screen has been touched

  bool isTouched = ts.touched();
  while (!isTouched) {
    delay(10);
    isTouched = ts.touched();
  }

//  Get the end time to check if data updates are required prior to resuming
//    Added 09-Mar-2018, DK Fowler
  uint32_t drawAboutEndTime = millis();
  uint32_t drawAboutDisplayTimeMillis = drawAboutEndTime - drawAboutStartTime;
  
//  Re-enable screen updates to resume "normal" operation
  restartUpdates(drawAboutDisplayTimeMillis);

}

String getTime(time_t *timestamp) {
  struct tm *timeStruct = gmtime(timestamp);
  
  char buf[6];
  sprintf(buf, "%02d:%02d", timeStruct->tm_hour, timeStruct->tm_min);
  return String(buf);
}

char* windCompassDir(float windDeg) {
  /*
   * This routine will convert the passed wind direction in degrees to "compass" direction (e.g., N, NNE, etc.)
   *    Written by DK Fowler ... 14-Dec-2018
   *    
   */

   char* windDirChar[] = {"N","NNE","NE","ENE","E","ESE","SE","SSE","S","SSW","SW","WSW","W","WNW","NW","NNW","N"};
   byte index = round(windDeg / 22.5);
   Serial.print("Wind dir, deg:  "); Serial.println(windDeg, 2);
   Serial.print("Wind dir, compass:  "); Serial.println(windDirChar[index]);
   return windDirChar[index];
}

float hPaToInHg (float pressHPa) {
  /*
   * This routine will convert barometric pressure represented as hPa (hectopascals) to inches of mercury (in Hg).
   * The OpenWeatherMap pressures are returned as hPa even with units set to Imperial, which isn't the most common
   * representation in the US (and perhaps elsewhere).
   *    Written by DK Fowler ... 15-Dec-2018
   *    
   */

  double pressConvCon = 0.02952998751 ;
  Serial.print("Press hPa:  "); Serial.print(pressHPa,0); Serial.print("\tPress inHg:  "); Serial.println((pressHPa * pressConvCon),2);
  return (pressHPa * pressConvCon);
}

float windChill (float currTemp, float currWindSpeed) {
  /*
   * This routine will calculate the estimated wind chill based on the passed temperature and wind speed.  If the flag isMetric is set, it
   * calculates the result based on degrees Celsius, using wind speed in 
   *    Written by DK Fowler ... 16-Dec-2018
   *    
    From Wikipedia (UK/Canadian/US model):   
    The equivalent formula in US customary units is:
    Twc = 35.74 + 0.6215Ta - (35.75 * v pow(+0.16)) + (0.4275Ta * v pow(+0.16) )

    where Twc is the wind chill index, based on the Fahrenheit scale; Ta is the air temperature in degrees Fahrenheit, and v is the wind speed in miles per hour.

    The standard Wind Chill formula for Environment Canada is:
    Twc = 13.12 + 0.6215Ta - (11.37 * v pow(+0.16)) + (0.3965Ta * v pow(+0.16) )

    where Twc is the wind chill index, based on the Celsius temperature scale; Ta is the air temperature in degrees Celsius; and v is the wind speed at 10 m (33 ft)
    standard anemometer height, in kilometres per hour.

    Note:  Windchill temperature is defined only for temperatures at or below 10 °C (50 °F) and wind speeds above 4.8 kilometres per hour (3.0 mph).

   */

  float windChillTemp;
  if (isMetric) {
    // Wind chill only applies to temperatures (Celsius) <= 10 and wind > 4.8kph...
    if ( (currTemp > 10) || ((currWindSpeed*3.6) <= 4.8) ) return currTemp;
   // convert wind speed in m/s to kph (1 m/s = 3.6 kph)
    windChillTemp = 13.12 + (0.6215 * currTemp) - (11.37 * ( pow((currWindSpeed*3.6), 0.16))) + ( (0.3965 * currTemp) * (pow((currWindSpeed*3.6), 0.16) ) );
  } else {
    // Wind chill only applies to temperatures (Fahrenheit) <= 50 and wind > 3.0mph...
    if ( (currTemp > 50) || (currWindSpeed <= 3.0) ) return currTemp; 
    windChillTemp = 35.74 + (0.6215 * currTemp) - (35.75 * ( pow(currWindSpeed, 0.16))) + ( (0.4275 * currTemp) * (pow(currWindSpeed, 0.16) ) );
  }
  return windChillTemp;
   
}

float heatIndex (float currTemp, float currHum) {
  /*
   * This routine will calculate the heat index based on the passed temperature and humidity values.
   *    Written by DK Fowler ... 16-Dec-2018
   *    

   From Wikpedia courtesy of NOAA:
   The formula below approximates the heat index in degrees Fahrenheit, to within ±1.3 °F (0.7 °C). It is the result of a multivariate fit 
   (temperature equal to or greater than 80 °F (27 °C) and relative humidity equal to or greater than 40%) to a model of the human body.[2][10] 
   This equation reproduces the above NOAA National Weather Service table (except the values at 90 °F (32 °C) & 45%/70% relative humidity vary 
   unrounded by less than ±1, respectively).   
   
   Heat Index = -42.379 + 2.04901523T + 10.14333127R - 0.22475541TR - 6.83783 x 10-3T2 - 5.481717 x 10-2R2 + 1.22874 x 10-3T2R + 8.5282 x 10-4TR2 - 1.99 x 10-6T2R2

    T - air temperature (F)
    R - relative humidity (percentage)
       
   */

  float tempF, heatIndexTempF, heatIndexTempC;
  
  if (isMetric) {
    // Convert temp C to F for calculating the index...
    tempF = currTemp * 9/5 + 32;
  } else {
    tempF = currTemp;
  }  
  // This heat index estimation only applies to temperatures >= 80F and humidity >= 40%...
  if ( (tempF < 80.0) || ((currHum < 40.0) ) ) return currTemp;
  heatIndexTempF = -42.379 + (2.04901523 * tempF) + (10.14333127 * currHum) - (0.22475541 * tempF * currHum) - (6.83783 * pow(10,-3) * pow(tempF,2)) - 
                   (5.481717 * pow(10,-2) * pow(currHum,2)) + (1.22874 * pow(10,-3) * pow(tempF,2) * currHum) + (8.5282 * pow(10,-4) * tempF * pow(currHum,2)) - 
                   (1.99 * pow(10,-6) * pow(tempF,2) * pow(currHum,2) ) ;
  // If metric, convert the heat index in F back to C before returning...
  if (isMetric) {
    heatIndexTempC = (heatIndexTempF - 32) * 5/9;
    return heatIndexTempC;
  } else {
    return heatIndexTempF;
  }
  
}
