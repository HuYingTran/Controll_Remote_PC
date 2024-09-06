#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

const int RELAY_A1 = 2;
const int BUTON_A1 = 0;

bool relay_a1_states = false;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR buttonPush() {
  portENTER_CRITICAL_ISR(&mux);
  Serial.println("Button Pushed!!!");
  digitalWrite(RELAY_A1, ~relay_a1_states);
  portEXIT_CRITICAL_ISR(&mux);//Cho phép tiếp tục chạy các task khác
}
void setup() {
  Serial.begin(9600);
  pinMode(BUTON_A1, INPUT_PULLUP);
  pinMode(RELAY_A1, OUTPUT);
  digitalWrite(RELAY_A1, relay_a1_states);

  attachInterrupt(digitalPinToInterrupt(BUTON_A1), buttonPush, FALLING);
}
void loop() {
  // do nothing
}