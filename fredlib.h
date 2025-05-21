#pragma once

#include <Adafruit_SHT4x.h>
#include <WiFININA.h>
#include <string>

#include "fredconstants.h"

/*
fredlib library
An arduino library to simplify and abstract away the details of having to deal with:
- Arduino Nano IoT 33 connection to send post https requests;
- Adafruit SHT 4x sensors in normal conditions;
- Developing an application that sends measurements periodically to a server.
*/

/*
A struct to represent a measurement
*/
typedef struct {
    float relative_humidity;
    float temperature;
} fred_measurement_t;

/*
Waits for and established connection with serial usb port
*/
void fred_utility_serial_setup_and_wait_for_usb_connection_blocking();

/*
Tries to connect iteratively to the wifi until successful
*/
void fred_wifi_block_until_successful_connection();

/*
Parameters:
- sensor: the pointer to the object that represents the sensor
*/
void fred_sht_sensor_wait_for_i2c_then_start_and_setup_or_reset_blocking(Adafruit_SHT4x* sensor);

/*
Parameters:
- sensor: the sensor object to make the measurements
- result: pointer to the struct that will hold the result
*/
bool fred_sht_make_measurement(Adafruit_SHT4x* sensor, fred_measurement_t* result);

/*
Debug function to print a measurement's data
*/
void fred_sht_print_measurement(fred_measurement_t measurement);

/*
With a delay of two minutes from attempt to attempt, iteratively tries to establish a connection with the server until successful
*/
void fred_client_connect_blocking(WiFiSSLClient* client);

/*
Flushes the data and disconnects
*/
void fred_client_disconnect(WiFiSSLClient* client);

/*
Sends a string to the server and returns true if successful, else false
*/
bool fred_client_send_string(const std::string& message, WiFiSSLClient* client);

/*
Sends an https request using the provided client with the data from the given measurement
*/
bool fred_client_send_https_post_request(WiFiSSLClient* client, fred_measurement_t* measurement);