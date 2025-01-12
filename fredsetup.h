#pragma once

#include "WiFiSSLClient.h"
#include <Adafruit_SHT4x.h>

/*
fredsetup library
An abstract micro-library to easily set up an Arduino Nano IoT 33 for standard to router connection, using an Adafruit SHT4x sensor.
*/

/*
Types of arduino setup
*/
typedef enum {
    fred_SETUP_USB,
    fred_SETUP_USB_WIFI_SENSOR,
    fred_SETUP_USB_WIFI,
    fred_SETUP_USB_SENSOR,
    fred_SETUP_CLIENT,
    fred_SETUP_SENSOR_CLIENT
} fred_setup_type_t;

/*
Parameters:
- setupType the type of setup
- sensor the sensor in case of startup of sensor
*/
void fred_quick_setup(fred_setup_type_t setupType, Adafruit_SHT4x* sensor, WiFiSSLClient* client);