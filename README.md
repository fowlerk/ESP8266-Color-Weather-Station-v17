#  Enhanced ESP8266 Color Weather Station w/ Touchscreen -- (Version v17, Open Weather Map) (based on original by Daniel Eichorn (squix78) )

This project is a series of enhancements made to the original color weather station for the ESP8266 created by Daniel Eichorn/ThingPulse, with contributions by several others.  Though the original display layout came from squix78, I've made substantial changes to add additional information.

This version of the project was written specifically for the WeMos Mini D1 with the 2.8" or 3.2" color LCD display based on the ILI9341 and XPT2046 touchscreen controller that squix78 used.  These can be had very inexpensively from sources such as AliExpress (<$10, shipped to the US, or slightly more for the 3.2" version).  You may also want to pick up a cheap stylus for the screen if it doesn't come with one.

This edition (v17) is a substantial re-write of my prior releases to move from Weather Underground's data source to Open Weather Map.  As such, it now utilizes the standard weather station library written and provided by ThingPulse.  However, it differs substantially from their version, which squix78 rewrote to use his MiniGrafx library to provide smooth-scrolling animations of the display.  Due to memory compromises, his version does not include the "photo-realistic" moon and weather icons, which I actually prefer.  So, I've elected not to use that library in favor of the better graphics, with the compromise of some minor screen-flashing on updates / page changes.  I've retained as many of the prior enhancements (see v15 for details) I made, including such things as a web-based configuration for basic setup / screen-timings, etc.  I also elected to make some tweaks to the display from ThingPulse's version (though I think I've retained all the information (and more!) that is provided with their version.

*** Important notes on this re-write!  To set expectations, the data provided by Open Weather Map is SUBSTANTILLY LESS than what was provided by Weather Underground.  First, the number of weather stations, though impressive world-wide, is much less than WU.  I found maybe 10% of the number in my area from the personal weather stations that WU provided.  The OWM data does not include such basic information as text-based forecasts, weather warnings/alerts, moon-rise/set times, daily high/low temperatures, 24-hour precipitation, etc., etc.  I find this more than frustrating, and will likely move to a different data source, but wanted to get something up quickly before the year-end deadline when IBM will expire free access to the WU data.

Now some good news for those that have looked at ThingPulse's version.  I've added a few additional things in this initial conversion.  You'll notice that I've expanded the number of forecast screens from ThingPulse's 10 (2 per day for the 5-day forecast) to 3 per day, for a total of 15.  I did this because the 2-per day with the absence of other critical weather data (high/low temp, probability of precip, 24-hour precip, etc.), I couldn't even get a good read of the high/low temperatures expected for a given day, or even precipitation forecast (the only precip forecase provided is a 3-hr estimate for the prior 3 hours!)  Next, I'd previously added a "feels-like" temp that WU provided on the current-day conditions; this is no longer provided by OWM, so I added some routines to calculate this (at least within a 1-2 degree estimate based on the methods provided by NOAA and used by Canada/UK).  I've done this for both wind-chill and heat-index, depending on the closest conditions; this may not match precisely with other data sources, but should be close.  Next, I noticed an apparent bug in ThingPulse's display of the forecast 3-hr precipitation for Imperial units.  Though their code correctly changes the units displayed from mm to inches, the actual value appears to always be returned in mm from OWM.  I've added calcuations to convert this appropriately.  Next, the barometric pressure was displayed in hectopascals (hPa), even for those using non-metric displays.  I'm not sure about other areas, but here in the US we are accustomed to seeing this display in in/Hg units.  So I've added a conversion to display pressure this way if units are set to non-metric.  Lastly, for wind direction, at least in the US, we're accustomed to seeing "compass direction" vs. degrees.  So, for non-metric users, I convert the degrees returned to a compass direction.

Sadly, missing from this early release is the text-based forecast screens from my earlier versions, and most importantly (to me), the weather alerts that I found so useful.  (I have some thoughts on this for a future release, so I'm not giving up hope on this permanently!)

Since some of the important data from v11-15 is no longer available, some of the touch-screen functionality has been removed at least for now...see the next section of touch-screen functionality.


##  Overview of Changes from v15

Changes from the prior release regarding touch-screen functions:
* a touch in the upper-rightmost corner (next to the date display) will toggle the WiFi strength indicator on/off (still available*)
* a touch to the current conditions (weather icon / temperature) region will bring up the day / night forecast for today (removed*)
* a touch to any of the forecast icons for the 9-day forecast will bring up the day / night forecast for that date (removed*; now 3 forecast displays per day for 5 days included in the scrolling mid-panel)
* a long touch (>4-5 secs) to the time display region will enter configuration mode (previously triggered via a double-reset of the ESP) (still available*)
* a long touch (>4-5 secs) to the forecast region (date / weather icon / temperature) will initiate the screen calibration routine again (still available*)
* a long touch (>4-5 secs) to the bottom panel region will bring up a detailed ESP status screen, showing heap, WiFi connection info, last reset error condition, etc. (adapted from Daniel Eichorn...thanks Dani!) (still available*)

All of these touch displays return to the normal "rotating" screens by touching the screen again.

## Wiring

(*** Same as prior versions and matches ThingPulse's wiring as of 19-Dec-2018 ***)

| ILI9341   | NodeMCU | WeMos D1 Mini | ESP8266 Pin
| --------- |:---------:|:---------------:|:-------------:
| T_IRQ     | D4    | D4  | GPIO2
| T_DO      | D6    | D6  | GPIO12
| T_DIN     | D7    | D7  | GPIO13
| T_CS      | D3    | D3  | GPIO0
| T_CLK     | D5    | D5  | GPIO14
| SDO/MISO  | D6    | D6  | GPIO12
| LED       | D8    | D8  | GPIO15
| SCK       | D5    | D5  | GPIO14
| SDI/MOSI  | D7    | D7  | GPIO13
| DC/RS     | D2    | D2  | GPIO4
| RESET     | RST   | RST | RST
| CS        | D1    | D1  | GPIO5
| GND       | GND   | GND | GND
| VCC       | 3V3   | 3V3 | 3V3

**Note that I have not tested the NodeMCU pin references, but from what I understand these are the same as for the WeMos D1 mini.  Verify and test these at your own risk!**  Also, note that some of these require more than one connection to the same pin.  Though this can be accomplished a number of ways, I highly recommend get some of the WeMos prototyping boards (if you are using this ESP variant) and soldering the connections directly to this board, then slipping it over the pin headers on the WeMos itself.  This also allows a good location for soldering the capacitor if you elect to use it.


##  Contributed Libraries (and links)

(*** Note there are a few changes from v15 ***)


Library Referenced | Description | Link
------------------ | ----------- | ----
**<FS.h>** | Part of ESP8266 core, SPIFFS filesystem support | 
**<Adafruit_GFX.h>** | Adafruit's core graphics library | <https://github.com/adafruit/Adafruit-GFX-Library>
**<Adafruit_ILI9341.h>** | Adafruit's ILI9341 LCD hardware-specific library | <https://github.com/adafruit/Adafruit_ILI9341>
**<SPI.h>** | Part of ESP8266 core, used for DHT temperature readings if used
**<math.h>** | Part of Arduino core, advanced math functions used for calculating wind-chill/heat-index
**<XPT2046_Touchscreen.h>** | XPT2046 library for touch-screen controller by Paul Stoffregen | <https://github.com/PaulStoffregen/XPT2046_Touchscreen>
**<WiFiManager.h>** | WiFi Manager library by Tzapu | <https://github.com/tzapu/WiFiManager>
**<ArduinoJson.h>** | JSON library by Bblanchon | <https://github.com/bblanchon/ArduinoJson>
**<JsonListener.h>** | JSON streaming parser by Squix78 | <https://github.com/squix78/json-streaming-parser>
**<OpenWeatherMapCurrent.h>** | Weather Underground client library by Squix78 - current conditions | <https://github.com/squix78/esp8266-weather-station>
**<OpenWeatherMapForecast.h>** | Weather Underground client library by Squix78 - 5-day forecast | <https://github.com/squix78/esp8266-weather-station>
**<Astronomy.h>** | Astronomy client library by Squix78 - sunrise/sest, moon illumination, age, phase calcuations | <https://github.com/squix78/esp8266-weather-station>
**<SunMoonCalc.h>** | (not currently used - maybe for future moonrise/set calcs?)
**<WundergroundClient.h>** | (*** No longer used ***) | <https://github.com/squix78/esp8266-weather-station>
**<simpleDSTadjust.h>**	| Simple daylight-savings time adjustment by Neptune | <https://github.com/neptune2/simpleDSTadjust>
**"TimeZone.h"** | Time zone conversion library by JChristensen | <https://github.com/JChristensen/Timezone>
**"DHT.h"** | Adafruit's DHT sensor library | <https://github.com/adafruit/DHT-sensor-library>
**<Adafruit_Sensor.h>** | Adafruit's Unified Sensor library | <https://github.com/adafruit/Adafruit_Sensor>

**Note 1:**  This release now utilizes ThingPulse's (squix78's) ESP8266 Weather Station library, and no longer requires inclusion of my customized version (see v15 for details)

##  Quick Start

(*** Modified 12/19/18 ***)
1.  Install the libraries that are required as noted above.
1.  Wire the WeMos (or equivalent) as noted.  For best stability, add a 1000uF capacitor between VCC and GND near the WeMos pins, and solder the J1 jumper on the TFT board to bypass the regulator if powering the display from the WeMos.
1.  Go to <https://openweathermap.org/api> and sign up for a free Open Weather Map API key.  Note this for later use in configuration.
1.  For best accuracy, Open Weather Map weather stations are referenced by an ID (7-digits in my case); there are several ways to find the closest station/ID for your locale, but I recommend downloading the bulk-listing from here: <http://bulk.openweathermap.org/sample/>.  Decompress it, then open it in your favorite text editor and search for your city name.  You're likely to find several; each listing includes it's lat/lon location coordinates.  You can copy these and paste them into the search bar for Google Maps to plot the exact location for each; choose one that is closest to your location.  Alternately, if you get your lat/lon location from Google Maps, you can search the list for one that appears closest to those coordinates.  This may take a bit of time, but it's a one-time setup.  Note this for later configuration (noted as Open Weather Map location ID).
1.  Install the ESP8266 Sketch Data Upload tool from here:  https://github.com/esp8266/arduino-esp8266fs-plugin
1.  Download and unzip this repository to your system.
1.  Ensure your board is set correctly with the appropriate flash size (4M, 3M SPIFFS) in the IDE.
1.  Compile, download, and run the SPIFFS format tool located in the "/ESP SPIFFS Format" folder.  This will give you a clean SPIFFS file system area.  Be patient...it takes a few minutes to format the filesystem.  This step may not be necessary if you reload the /data folder as described below, as it appears that the file system is first formatted by the Data Upload tool. (Note that if you have used a prior version of my code and DON'T format the SPIFFS, you'll have problems as the format for the stored configuration file has changed; this will likely cause the ESP to crash!)  I've also made a number of changes to some of the icons for better screen positioning, so don't skip this step.
1.  Compile and download the Weather Station v17 code to the WeMos.
1.  Run the ESP8266 Data Upload tool located in the Tools menu of the IDE.  You should receive information in the IDE indicating the copying of the icon folders located in the "/data" folder to the ESP8266.
1.  On first run, the station will run a display calibration routine to map the touchscreen coordinates to the display coordinates.  Follow the on-screen instructions by touching the screen targets as indicated in the upper-left and lower-right corners of the screen.  The last step in the calibration allows you to see how closely the coordinates are mapped by displaying the screen coordinates as you touch various locations on the screen.  This will time out and continue startup if the screen is not touched for 5 seconds.
1.  The initial configuration is done via a configuration portal or webpage. The first time you bring up the app, if it cannot find previously stored settings, it will bring up an access point and configuration portal. Look for an access point that begins with "ESP..." and connect to it. The access point is password protected, with a default password as "portal-pass".  It is a captive portal, and in most cases a browser will open automatically connecting to the portal after connection. If it doesn't, then type the address "192.168.4.1" in a browser to bring it up once connected to the AP. It should be fairly self-explanatory at that point; enter your WiFi AP/password credentials, Open Weather Map App ID (API key), Open Weather Map city (location ID), location description (for display), timezone city from the list, etc., and click "Save". The ESP should restart and try to connect using the new configuration. There's other features of the configuration portal, such as scanning for all available access points and allowing you to just select one, but again, it's pretty self-explanatory once you see the configuration page.  (If you've done this before with a prior release, you'll need to reconfigure and save the settings, as the SPIFFS format will erase the configuration file stored previously.  
1.  If it doesn't connect, then you can at any time bring up the configuration portal again manually by touching the time-display region of the screen and holding for 4-5 seconds. You should also see information on the LCD display when it comes up in config mode.
1.  The initial defaults for the configuration portal are located on the "Settings.h" tab as well. If you look through this, you'll find parameters for the OWM App ID (API key), the timezone city, city/location ID, etc. These are currently set for my location and preferences, but you can change them here if you want also.

## Final Thoughts

Don't get me started on IBM's move to squeeze more money out of what began as an open-source weather source.  Yes, they have finally decided to provide access (limited, not yet fully described) to data for those that provide information from their own personal weather stations, but I find their whole approach to this offensive.  I hope all owners of PWSs elect to drop their data from Weather Underground and move elsewhere.  If you're in the mood for a good read, take a look as some of the comments in their blog.

I've done some preliminary investigation into APIs provided by NOAA/NWS, and they are very robust.  These include everything and more that were provided previously under Weather Undergound, though of course their data is limited to the US.  As I get time over the upcoming months, I plan to look at possibly extending ThingPulse's modules with additonal ones to pull some of NOAA's data instead.  I'm particularly interested in getting access again to the weather alerts information for my area.

Finally, please note that this is a rushed conversion in order to provide a fall-back from my v15 release.  The code needs a lot of clean-up, and I certainly have not had time to debug it thoroughly.  If you find problems, submit an issue and I'll take a look when I get a chance (likely not before the New Year!)
