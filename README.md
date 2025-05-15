# Arduino Environmental Monitoring Project

## Overview

This project implements an environmental monitoring system using Arduino Nano 33 IoT microcontrollers. Two sensors are deployed on separate home balconies to collect temperature and relative humidity data. This data is periodically transmitted to a custom-built web server, where it's stored and visualized on an interactive graph, accessible via a personal website.

This endeavor was born from a desire to create a practical application spanning multiple facets of programming, including embedded systems, full-stack web development, and server administration.

## Features

* **Remote Sensing**: Utilizes Arduino Nano 33 IoT devices connected to temperature and humidity sensors.
* **Data Transmission**: Sensor readings are sent over HTTPS to a custom server API.
* **Web Visualization**: Data is displayed on an interactive chart on my personal website.
* **Robust Microcontroller Code**: The Arduino C++ code is designed for reliability, including automatic reconnection capabilities if the network connection is lost.
* **Full-Stack Solution**: The project encompasses embedded C++ programming for the Arduinos, a Python (Flask) backend, and a JavaScript-powered frontend.

## Technology Stack

This project leverages a diverse set of technologies:

### Microcontroller (Arduino)

* **Language**: C++
    * Handles periodic sensor readings (temperature and humidity).
    * Manages network connectivity (Wi-Fi) and secure data transmission.
    * Engineered for robustness, including self-documenting efforts due to sparse documentation for some open-source libraries used.
* **Hardware**: Arduino Nano 33 IoT, SHT4x temperature/humidity sensors.

### Web Server & Application

* **Backend**: Python with Flask framework.
    * Provides an API endpoint to receive data from the Arduinos.
    * Communicates with a MySQL database (using a custom mini-library "fredbconn" for connection pooling) to store and retrieve measurements.
    * Served by Waitress behind an Nginx reverse proxy.
* **Frontend**:
    * **JavaScript**: Fetches data from the backend and dynamically renders an interactive chart.
    * **HTML & CSS**: Structure and style the webpage displaying the sensor data.

### Server Infrastructure & Automation

* **Operating System**: Linux (on a Virtual Private Server - VPS).
* **Web Server**: Nginx (acting as a reverse proxy).
* **SSL/TLS**: Certbot with Let's Encrypt™ for automated SSL certificate management, ensuring secure HTTPS connections.
* **Automation**: Bash scripting for:
    * Setting up systemd services.
    * Cron jobs for tasks like restarting the web application on server reboot and managing old log files.

## Project Structure

This repository contains the C++ source code for the Arduino microcontrollers. The code is organized into several files:

* `nanosensor.ino`: The main program for the Arduino.
* `fredlib.h` / `fredlib.cpp`: A custom library abstracting sensor interactions, Wi-Fi connectivity, and HTTPS communication.
* `fredsetup.h` / `fredsetup.cpp`: Helper library for various setup routines on the Arduino.
* `fredtest.h` / `fredtest.cpp`: Contains test functions for different components of the Arduino code.
* `fredconstants.h`: Declares constants kept in constants.c.
