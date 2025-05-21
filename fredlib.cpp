#include <WiFiSSLClient.h>
#include <WiFiNINA.h>
#include <string>
#include <ArduinoJson.h>

#include "fredlib.h"

void fred_utility_serial_setup_and_wait_for_usb_connection_blocking() {
    while(!Serial) {
        Serial.begin(USBBaudRate);
        delay(1000);
    }
}

bool fred_wifi_is_connected() {
    return WiFi.status() == WL_CONNECTED;
}

void fred_wifi_block_until_successful_connection() {
    if(fred_wifi_is_connected()) return;
    WiFi.setHostname(WifiHostname);
    WiFi.setTimeout(60 * 1000);
    while(WiFi.status() != WL_CONNECTED) WiFi.begin(WifiSSID, WifiPassword);
}

void fred_sht_sensor_wait_for_i2c_then_start_and_setup_or_reset_blocking(Adafruit_SHT4x* sensor) {
    while(!sensor->begin()) delay(1000);
    sensor->setHeater(SHT4X_NO_HEATER);
    sensor->setPrecision(SHT4X_HIGH_PRECISION);
}

bool fred_sht_make_measurement(Adafruit_SHT4x* sensor, fred_measurement_t* result) {
    sensors_event_t humidityEvent;
    sensors_event_t tempEvent;
    bool successful = sensor->getEvent(&humidityEvent, &tempEvent);
    if(!successful) return false;
    result->relative_humidity = humidityEvent.relative_humidity;
    result->temperature = tempEvent.temperature;
    return true;
}

void fred_sht_print_measurement(fred_measurement_t measurement) {
    Serial.print("Temperature: "); Serial.println(measurement.temperature);
    Serial.print("Relative humidity: "); Serial.println(measurement.relative_humidity);
}

void fred_client_connect_blocking(WiFiSSLClient* client) {
    if(client->connected()) return;
    fred_wifi_block_until_successful_connection();

    /*
    Things to consider about the following line of code:

    - Cleanup of the sockets and connections is managed internally if the connection is re-tried

    - The connect call is blocking and waits 10 seconds. The blocking behaviour and 10 second wait
    time for the connection to established is managed internally by the Arduino libraries
    */
    fred_client_disconnect(client);
    while(!client->connect(ServerDomain, ServerPort));
    client->setRetry(false);
}

void fred_client_disconnect(WiFiSSLClient* client) {
    client->flush();
    client->stop();
}

bool fred_client_send_string(const std::string& message, WiFiSSLClient* client) {
    size_t checksum = client->print(message.c_str());
    return checksum == message.length();
}

bool fred_client_send_https_post_request(WiFiSSLClient* client, fred_measurement_t* measurement) {
    JsonDocument jsonDocument;

    String temperatureString = String(measurement->temperature, 1);
    String relative_humidityString = String(measurement->relative_humidity, 0);

    jsonDocument["temperature"] = temperatureString;
    jsonDocument["relative-humidity"] = relative_humidityString;

    String jsonPayload;
    serializeJson(jsonDocument, jsonPayload);

    client->println("POST " + String(ServerEndpoint) + " HTTP/1.1");
    client->println("Host: " + String(ServerDomain));
    client->println("Content-Type: application/json");
    client->println("Arduino-Name: " + String(ArduinoName));
    client->println("Arduino-Password: " + String(ArduinoPassword));
    client->println("Connection: close");
    client->println("Content-Length: " + String(jsonPayload.length()));
    client->println();
    client->println(jsonPayload);
    
    if(client->getWriteError()) {
        client->clearWriteError();
        return false;
    }

    return true;
}