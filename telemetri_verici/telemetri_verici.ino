#include <SPI.h>
#include <mcp2515.h> // MCP2515 kütüphanesi

struct can_frame canMsg; // CAN mesajı yapısı
MCP2515 mcp2515(10);     // MCP2515 modülünün CS pini

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); // 500 Kbps hız, 8 MHz kristal
  mcp2515.setNormalMode();

  Serial.println("CAN Alıcı Başladı...");
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    // Gelen veriyi okuma
    if (canMsg.can_id == 0x202) { // 0x200 ID'li mesajı dinle
      uint8_t hiz = canMsg.data[0];                        // Hız verisi
      uint8_t soc = canMsg.data[1];                        // SoC verisi
      uint8_t sicaklik = canMsg.data[2];                   // Sıcaklık verisi
      uint16_t kalanEnerji = (canMsg.data[3] << 8) | canMsg.data[4]; // 2 byte kalan enerji verisi

      // Veriyi Seri Monitörde Göster
      Serial.println("Gelen Veriler:");
      Serial.print("Hız: "); Serial.print(hiz); Serial.println(" km/h");
      Serial.print("SoC: "); Serial.print(soc); Serial.println(" %");
      Serial.print("Sıcaklık: "); Serial.print(sicaklik); Serial.println(" °C");
      Serial.print("Kalan Enerji: "); Serial.print(kalanEnerji); Serial.println(" Wh");
      Serial.println("-----------------------");
    }
  }
}


