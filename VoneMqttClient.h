#pragma once

#include <VOneMqttClient.h>


// Define device id
const char* IN_SERVO_ID = "9fb28488-d203-4380-b9b3-5292a07a20f7";
const char* OUT_SERVO_ID = "bde39f5c-4524-4530-ba29-fe765a397832";
const char* LED_STATUS_ID = "aa568c30-b0a7-4a60-873d-f3e6f7377ea5";
const char* CUSTOMER_COUNTER_ID = "30c46c66-5565-4588-be36-1642af9f360f";

class VMqttClient{
  private:
    VOneMqttClient voneClient;

  public:
    VMqttClient(){}

    void setup_wifi() {
      delay(10);
      // We start by connecting to a WiFi network
      Serial.println();
      Serial.print("Connecting to ");
      Serial.println(WIFI_SSID);

      WiFi.mode(WIFI_STA);
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }

      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }

    void setup() {
      setup_wifi();
      voneClient.setup();
    }

    void loop(){
      if (!voneClient.connected()) {
        voneClient.reconnect();
        voneClient.publishDeviceStatusEvent(LED_STATUS_ID, true);
        voneClient.publishDeviceStatusEvent(OUT_SERVO_ID, true);
        voneClient.publishDeviceStatusEvent(IN_SERVO_ID, true);
        voneClient.publishDeviceStatusEvent(CUSTOMER_COUNTER_ID, true);
      }
      voneClient.loop();
    }

    void publish(const char* deviceId, JSONVar &payload){
      voneClient.publishTelemetryData(deviceId, payload);
    }

};


