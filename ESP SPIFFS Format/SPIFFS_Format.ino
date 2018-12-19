/*
 * Example sketch to format a SPIFFS file system on the ESP8266.
 * 
 * Written by DK Fowler ... 08-Jan-2017
 * 
 */

#include "FS.h"

void setup() {
  Serial.begin(115200);

  delay(2000);
  Serial.println("Opening SPIFFS...");
  // always use this to "mount" the filesystem
  bool result = SPIFFS.begin();
  //  Serial.println("SPIFFS opened: " + result);

  // Format the file system and return result
  Serial.println("Beginning format of SPIFFS...please be patient.");
  result = SPIFFS.format();
  
  if (!result) {
    Serial.println("Error formatting SPIFFS file system:  " + result);
  } else {
    Serial.println("SPIFFS file system successfully formatted.");
  }
  SPIFFS.end();
}

void loop() {
  // nothing to do for now, this is just a simple test

}

