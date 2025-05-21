#include <WiFiNINA.h>

#include "fredlib.h"
#include "fredtest.h"
#include "fredsetup.h"
#include "fredconstants.h"

void setup() {
    Adafruit_SHT4x sensor;
    WiFiSSLClient client;

    fred_quick_setup(fred_SETUP_SENSOR_CLIENT, &sensor, &client);

    fred_measurement_t measurement;

    while(true) {
        fred_client_connect_blocking(&client);
        fred_sht_make_measurement(&sensor, &measurement);
        fred_client_send_https_post_request(&client, &measurement);
        delay(10 * 1000);
        fred_client_disconnect(&client);
        delay(minutesBetweenEachMeasurement * 1000 * 60);
    }
}

void loop() {

}