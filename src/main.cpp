#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoHA.h>
#include <secret.h>

#define LED_PIN         2
#define BROKER_ADDR     IPAddress(192,168,1,222)

// #define WIFI_SSID       ""
// #define WIFI_PASSWORD   ""


WiFiClient client;
HADevice device;
HAMqtt mqtt(client, device);

HANumber number("myNumber");
HASensor valve("myValve");

void onNumberCommand(HANumeric number, HANumber* sender)
{
    if (!number.isSet()) {
        // the reset command was send by Home Assistant
    } else {
        // you can do whatever you want with the number as follows:
        int8_t numberInt8 = number.toInt8();
        int16_t numberInt16 = number.toInt16();
        int32_t numberInt32 = number.toInt32();
        uint8_t numberUInt8 = number.toUInt8();
        uint16_t numberUInt16 = number.toUInt16();
        uint32_t numberUInt32 = number.toUInt32();
        float numberFloat = number.toFloat();
    }

    sender->setState(number); // report the selected option back to the HA panel
}


void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    // Unique ID must be set!
    byte mac[6];
    WiFi.macAddress(mac);
    device.setUniqueId(mac, sizeof(mac));

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // connect to wifi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500); // waiting for the connection
    }
    Serial.println();
    Serial.println("Connected to the network");

    // set device's details (optional)
    device.setName("AirQENS");
    device.setSoftwareVersion("1.0.0");

    number.onCommand(onNumberCommand);

    // Optional configuration
    number.setIcon("mdi:home");
    number.setName("My number");

        valve.setIcon("mdi:home");
    valve.setName("Water valve");



    mqtt.begin(BROKER_ADDR, MQTT_USER, MQTT_PASS);
}

void loop() {
    mqtt.loop();
    valve.setValue("10");
    delay(1000);
    // You can also change the state at runtime as shown below.
    // This kind of logic can be used if you want to control your switch using a button connected to the device.
    // led.setState(true); // use any state you want
}