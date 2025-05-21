#include "WiFiSSLClient.h"
#include "Adafruit_SHT4x.h"

#include "fredsetup.h"
#include "fredlib.h"

static void fred_setup_usb() {
    fred_utility_serial_setup_and_wait_for_usb_connection_blocking();
}

static void fred_setup_wifi() {
    fred_wifi_block_until_successful_connection();
}

static void fred_setup_sensor(Adafruit_SHT4x* sensor) {
    fred_sht_sensor_wait_for_i2c_then_start_and_setup_or_reset_blocking(sensor);
}

static void fred_setup_client(WiFiSSLClient* client) {
    fred_client_connect_blocking(client);
}

static void fred_setup_sensor_client(Adafruit_SHT4x* sensor, WiFiSSLClient* client) {
    fred_setup_sensor(sensor);
    fred_setup_client(client);
}

void fred_quick_setup(fred_setup_type_t setupType, Adafruit_SHT4x* sensor, WiFiSSLClient* client) {
    switch(setupType) {
        case fred_SETUP_USB_WIFI_SENSOR:
            fred_setup_usb();
            fred_setup_wifi();            
            fred_setup_sensor(sensor);
            break;
        case fred_SETUP_USB_WIFI:
            fred_setup_usb();
            fred_setup_wifi();
            break;
        case fred_SETUP_USB_SENSOR:
            fred_setup_usb();
            fred_setup_sensor(sensor);
            break;
        case fred_SETUP_CLIENT:
            fred_setup_client(client);
            break;
        case fred_SETUP_SENSOR_CLIENT:
            fred_setup_sensor_client(sensor, client);
            break;
        case fred_SETUP_USB:
            fred_setup_usb();
            break;
    }
}