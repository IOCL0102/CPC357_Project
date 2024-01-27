#pragma once

#include <Arduino_JSON.h>
#include "NewPing.h"
#include "VOneMqttClient.h"

#define LIGHT_UP_DISTANCE 3

class CarDetectorModule{
  private:
    int triggerPin, echo1Pin, echo2Pin, detectedPin;
    NewPing frontSonar, backSonar;
    bool isCarDetected;
    int prevFrontDistance, prevBackDistance;
    int parkingLotId;

  public:
    CarDetectorModule(int triggerPin, int echo1Pin, int echo2Pin): 
    frontSonar(triggerPin, echo1Pin), backSonar(triggerPin, echo2Pin) {isCarDetected = false; prevFrontDistance=0; prevBackDistance=0;}

    void setup(int detectedPin, int parkingLotId){
      this->detectedPin = detectedPin;
      this->parkingLotId = parkingLotId;
      pinMode(detectedPin, OUTPUT); 
    }

    void monitor(VMqttClient &client, bool isNear, bool verbose = true){
      JSONVar payload;

      if(!isNear){
        // Send data to server if the car leaves
        if(isCarDetected){
          payload["status"] = 0;
          client.publish(LED_STATUS_ID, payload);
        }
        isCarDetected = false;
        digitalWrite(detectedPin, LOW);
        return;
      }

      int frontAvg= 0, backAvg = 0;
      int avgCount = 10;
      for(int i = 0 ; i < avgCount; i++){
          frontAvg += frontSonar.ping_cm();
          backAvg += backSonar.ping_cm();
      }


      int distanceFront = (frontAvg/avgCount) * 0.4 + prevFrontDistance * 0.6;
      int distanceBack = (backAvg/avgCount)* 0.4 + prevBackDistance * 0.6;
      prevFrontDistance = distanceFront;
      prevBackDistance = distanceBack;

      if(verbose){
        Serial.print("Distance Front: ");
        Serial.print(distanceFront);
        Serial.print(" Distance back: ");
        Serial.println(distanceBack);
      }

      if(distanceFront <= LIGHT_UP_DISTANCE && distanceBack <= LIGHT_UP_DISTANCE && distanceFront != 0 && distanceBack != 0 && !isCarDetected){
        // Send data to server
        payload["status"] = 1;
        
        client.publish(LED_STATUS_ID, payload);
        digitalWrite(detectedPin, HIGH);
        isCarDetected = true;
        Serial.println("..................................................CAR DETECTED !");
      }
    }

};