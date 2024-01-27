#include <HTTPUpdate.h>
#include <Arduino_JSON.h>

#include "IRModule.h"
#include "GateModule.h"
#include "VoneMqttClient.h"
#include "CarDetectorModule.h"

int irPin = A0, irRedLEDPin = 21, irGreenLEDPin = 47;
int inGateServoPin = 39, inGateButtonPin = 48;
int outGateServoPin = 38, outGateButtonPin = 40;
int triggerPin = A1, echo1Pin = A2, echo2Pin = A3, detectedPin = A4;
int parkingLotId = 1;

IRModule irModule;
GateModule inGate, outGate;
VMqttClient client;
CarDetectorModule carDetector(triggerPin, echo1Pin, echo2Pin);

void setup(){
  Serial.begin(115200); 
  client.setup();
  irModule.setup(irPin, irRedLEDPin, irGreenLEDPin); 
  inGate.setup(inGateServoPin,inGateButtonPin, GATE_IN);
  outGate.setup(outGateServoPin,outGateButtonPin, GATE_OUT);
  carDetector.setup(detectedPin, parkingLotId);
  pinMode(12, OUTPUT);
}

void loop(){
  digitalWrite(12, LOW);
  client.loop();
  inGate.monitor(client, false);
  outGate.monitor(client, false);
  bool isNear = irModule.monitor(false);
  carDetector.monitor(client, isNear);

  delay(500);
}