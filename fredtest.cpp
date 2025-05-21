#include "Adafruit_SHT4x.h"
#include <WiFiSSLClient.h>
#include "fredlib.h"
#include "fredsetup.h"
#include "fredtest.h"

#include <ArduinoJson.h>
#include <WiFiNINA.h>


static void fred_test_led() {
    pinMode(LED_BUILTIN, OUTPUT);

    while(true) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
    }
}

static void fred_test_usb_wifi() {
    fred_quick_setup(fred_SETUP_USB_WIFI, NULL, NULL);

    Serial.print("Current RSSI: ");
    Serial.println(WiFi.RSSI());

    while(true) delay(10 * 1000);
}

static void fred_test_usb_sensor() {
    Adafruit_SHT4x sensor;
    fred_quick_setup(fred_SETUP_USB_SENSOR, &sensor, NULL);
    
    fred_measurement_t measurement;

    if(!fred_sht_make_measurement(&sensor, &measurement)) {
        Serial.println("Failed to take measurement");
        while(true) delay(10 * 1000);
    }

    fred_sht_print_measurement(measurement);

    while(true) delay(10 * 1000);
}

static void fred_test_led_client() {
    pinMode(LED_BUILTIN, OUTPUT);

    for(int i = 0; i < 15; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
    }

    WiFiSSLClient client;
    fred_quick_setup(fred_SETUP_CLIENT, NULL, &client);

    fred_client_send_string("Hello, world", &client);

    while(true) delay(10 * 1000);
}

static void fred_test_periodic_read_from_sensor_and_send_to_server() {
    Adafruit_SHT4x sensor;
    WiFiSSLClient client;

    const int minutesBetweenEachMeasurement = 1;

    fred_quick_setup(fred_SETUP_SENSOR_CLIENT, &sensor, &client);

    fred_measurement_t measurement;

    while(true) {
        fred_client_connect_blocking(&client);
        fred_sht_make_measurement(&sensor, &measurement);
        fred_client_send_https_post_request(&client, &measurement);
        fred_client_disconnect(&client);
        delay(minutesBetweenEachMeasurement * 1000 * 60);
    }
}

static void fred_test_json() {

    JsonDocument jsonDocument;

    Adafruit_SHT4x sensor;

    fred_quick_setup(fred_SETUP_USB_SENSOR, &sensor, NULL);

    fred_measurement_t measurement;
    fred_sht_make_measurement(&sensor, &measurement);

    String temperatureString = String(measurement.temperature, 1);
    String relative_humidityString = String(measurement.relative_humidity, 0);

    jsonDocument["temperature"] = temperatureString;
    jsonDocument["relative-humidity"] = relative_humidityString;

    String jsonPayload;
    serializeJson(jsonDocument, jsonPayload);

    size_t jsonSize = measureJson(jsonDocument);

    fred_quick_setup(fred_SETUP_USB, NULL, NULL);

    Serial.print("Json size: "); Serial.println(jsonSize);
    Serial.println("Json string:");
    Serial.println(jsonPayload);

    while(true) delay(10 * 1000);
}

static void fred_test_sensor_client_https() {
    Adafruit_SHT4x sensor;
    WiFiSSLClient client;

    fred_quick_setup(fred_SETUP_SENSOR_CLIENT, &sensor, &client);

    fred_measurement_t measurement;

    fred_sht_make_measurement(&sensor, &measurement);

    if(!fred_client_send_https_post_request(&client, &measurement)) Serial.println("Error writing");

    while(true) delay(10 * 1000);
}

void fred_quick_test(fred_test_type_t testType) {
    switch(testType) {
    case fred_TEST_LED:
        fred_test_led();
        break;
    case fred_TEST_USB_WIFI:
        fred_test_usb_wifi();
        break;
    case fred_TEST_USB_SENSOR:
        fred_test_usb_sensor();
        break;
    case fred_TEST_LED_CLIENT:
        fred_test_led_client();
        break;
    case fred_TEST_PERIODIC_READ_FROM_SENSOR_AND_SEND_TO_SERVER:
        fred_test_periodic_read_from_sensor_and_send_to_server();
        break;
    case fred_TEST_JSON:
        fred_test_json();
        break;
    case fred_TEST_SENSOR_CLIENT_HTTPS:
        fred_test_sensor_client_https();
        break;
    }
}