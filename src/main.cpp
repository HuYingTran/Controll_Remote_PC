#include <Arduino.h>
#include <HTTPUpdate.h>
#include <WiFi.h>
uint32_t updateCounter = 0;
String version = "1.1";
String key = "b4e16e07-a351-4594-b79e-7c5d961a60bd";

const int BUTTON_A1 = 18;
const int RELAY_A1 = 4;
bool relay_states = false;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR buttonPush() {
  portENTER_CRITICAL_ISR(&mux);//Phần quan trọng cần được bảo vệ khỏi mọi truy cập đồng thời để thay đổi nó
  Serial.println("Button Pushed!!!");
  relay_states = ! relay_states;
  digitalWrite(RELAY_A1, relay_states);
  Serial.print("relay_states:");
  Serial.println(relay_states);
  portEXIT_CRITICAL_ISR(&mux);//Cho phép tiếp tục chạy các task khác
}

void update_FOTA();
void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_A1, INPUT_PULLUP);
  // cài đặt ngắt vào chân button, kiểu ngắt là falling (xung xuống), hàm gọi khi có sự kiện ngắt là button push
  attachInterrupt(digitalPinToInterrupt( BUTTON_A1), buttonPush, FALLING);
  // chọn led là đầu ra
  pinMode(RELAY_A1, OUTPUT);
  //ghi giá trị ban đầu
  digitalWrite(RELAY_A1, relay_states);
  
  WiFi.begin("Cindy", "cindy@2001");
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}
 
void loop()
{
  Serial.print("Ver: ");
  Serial.println(version);
  delay(500);
  if (WiFi.status() == WL_CONNECTED)
  {
    updateCounter++;
    if (updateCounter > 10)
    {
      updateCounter = 0;
      Serial.println("Check update");
      update_FOTA();
    }
  }
}
 
String getChipId()
{
  String ChipIdHex = String((uint32_t)(ESP.getEfuseMac() >> 32), HEX);
  ChipIdHex += String((uint32_t)ESP.getEfuseMac(), HEX);
  return ChipIdHex;
}
 
void update_FOTA()
{
  String url = "http://otadrive.com/deviceapi/update?";
  url += "k=" + key;
  url += "&v=" + version;
  url += "&s=" + getChipId(); // định danh thiết bị trên Cloud
 
  WiFiClient client;
  httpUpdate.update(client, url, version);
}
