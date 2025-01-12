#pragma once

typedef enum {
    fred_TEST_JSON,
    fred_TEST_LED,
    fred_TEST_USB_WIFI,
    fred_TEST_USB_SENSOR,
    fred_TEST_LED_CLIENT,
    fred_TEST_PERIODIC_READ_FROM_SENSOR_AND_SEND_TO_SERVER,
    fred_TEST_SENSOR_CLIENT_HTTPS
} fred_test_type_t;

void fred_quick_test(fred_test_type_t testType);