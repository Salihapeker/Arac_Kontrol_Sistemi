#include <mcp2515.h>
#include <SPI.h>
#include <virtuabotixRTC.h>

#define SENSOR_PIN 7 

volatile int pulseSayac = 0;  
unsigned long lastTime = 0;
float tekerlekCevresi = 1.8;
float hiz = 0.0;
int sensor = 7;
int degisken ; 
int saniye = 0 ;
int state; 

// RTC DS1302 bağlantı pinleri (CLK, DAT, RST)
virtuabotixRTC myRTC(4,5,6);

MCP2515 mcp2515(10);  
struct can_frame canMsg;

void setup() {

  pinMode(sensor, INPUT);
  state =digitalRead(sensor);

  Serial.begin(9600);
  SPI.begin();

  if (mcp2515.reset() != MCP2515::ERROR_OK) {
    Serial.println("CAN Modülü reset edilemedi!");
  }
  if (mcp2515.setBitrate(CAN_250KBPS, MCP_16MHZ) != MCP2515::ERROR_OK) {
    Serial.println("CAN Modülü hız ayarı başarısız!");
  }
  if (mcp2515.setNormalMode() != MCP2515::ERROR_OK) {
    Serial.println("CAN Modülü normal moda geçemedi!");
  }

  Serial.println("Gonderici baslatildi...");

  myRTC.setDS1302Time(18, 10, 10, 7, 26, 3, 2024); 
  myRTC.updateTime(); // RTC zamanını güncelle
  
}

void loop() {

  degisken = state;
  state =digitalRead(sensor);
      if(degisken != state){
          pulseSayac++;
          Serial.println(state);
      }
    myRTC.updateTime();

  // Eğer saniye değişmişse, hız hesapla ve hsayac'ı sıfırla
  if (saniye != myRTC.seconds) {
    saniye =myRTC.seconds;
   hiz = (pulseSayac * tekerlekCevresi * 60.0 * 60.0) / 10000;

    Serial.print("Motor Hizi: ");
    Serial.print(hiz);
    Serial.println(" km/h");
    pulseSayac = 0;  
  }

    // CAN Mesajı Hazırlama
    canMsg.can_id = 0x200;
    canMsg.can_dlc = 5;
    canMsg.data[0] = (int)hiz >> 8;
    canMsg.data[1] = (int)hiz & 0xFF;
 
    mcp2515.sendMessage(&canMsg);
  delay(100);
}  

