#include <SoftwareSerial.h>
#include <Nextion.h>
#include <SPI.h>
#include <mcp2515.h>

SoftwareSerial HMISerial(10, 11);


#define nexSerial HMISerial

NexNumber x0 = NexNumber(0, 8, "x0");//maxVoltage
NexNumber x1 = NexNumber(0, 9, "x1");//minVoltage
NexNumber x2 = NexNumber(0, 10, "x2");//totalVoltage
NexNumber x3 = NexNumber(0, 16, "x3");//temperature
NexNumber x4 = NexNumber(0, 17, "x4");//current
NexNumber n0 = NexNumber(0, 18, "n0");//soc
NexProgressBar pbar = NexProgressBar(0, 20, "j0"); //soc için progress bar
NexNumber n1 = NexNumber(0, 23, "n1");//hiz
NexGauge z0 = NexGauge(0, 22, "z0");    // Hız göstergesi (gauge)


NexTouch *nex_listen_list[] =
{
  &x0,
  &x1,
  &x2,
  &x3,
  &x4,
  &n0,
  NULL
};

// Nextion bileşenleri oluşturuluyor
 NexPicture p0 = NexPicture(0, 1, "p0"); // Ekranın 0. sayfasında, ID'si 10 olan p8 adlı bir resim bileşeni
 NexPicture p1 = NexPicture(0, 2, "p1");
 NexPicture p2 = NexPicture(0, 3, "p2");
 NexPicture p3 = NexPicture(0, 4, "p3");
 NexPicture p4 = NexPicture(0, 5, "p4");
 NexPicture p5 = NexPicture(0, 6, "p5");
 NexPicture p6 = NexPicture(0, 7, "p6");
MCP2515 mcp2515(53); 
struct can_frame canMsg;

float receivedCellVoltages[4] = {0};

const int button0Pin = 24; //Şarj 
const int button1Pin = 26; //Far
const int button2Pin = 28; //Silecek
const int button3Pin = 30; //Flaşör
const int button4Pin = 32; //Sağ Sinyal
const int button5Pin = 34; //Sol Sinyal
const int button6Pin = 36; // Fren stop

void setup() {
  Serial.begin(115200);
  delay(500);
  HMISerial.begin(9600);

  pinMode(button0Pin, INPUT_PULLUP);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(button4Pin, INPUT_PULLUP);
  pinMode(button5Pin, INPUT_PULLUP);
  pinMode(button6Pin, INPUT_PULLUP);

  Serial.println("Başlatıldı...");

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

  Serial.println("AKS Başlatıldı. Veriler işleniyor...");

  HMISerial.begin(9600); // Nextion ekranı için baudrate
  nexInit();
 Serial.println("Nextion ekranı başlatıldı.");
}

 

void loop() {
  // Tuş durumlarını kontrol et
 if (digitalRead(button0Pin) == HIGH) {
    Serial.println("Şarj butonuna basınız.");
    p0.setPic(8);
  } else if (digitalRead(button0Pin) == LOW) {
  
   // Serial.println("Şarj butonu basıldı.");
      p0.setPic(1);
   // HMISerial.println("Şarj Butonu Basıldı");
  }

  if (digitalRead(button1Pin) == HIGH) {

    Serial.println("Far butonuna basınız.");
        p5.setPic(12);
  } else if (digitalRead(button1Pin) == LOW) {
   // Serial.println("Far butonu basıldı.");
    p5.setPic(6);
   // HMISerial.println("Far Butonu Basıldı");
  }

  if (digitalRead(button2Pin) == HIGH) {

    Serial.println("Silecek butonuna basınız.");
     p4.setPic(11);
  } else if (digitalRead(button2Pin) == LOW) {
  //  Serial.println("Silecek butonu basıldı.");
     p4.setPic(5);
   // HMISerial.println("Silecek Butonu Basıldı");
  }

  if (digitalRead(button3Pin) == HIGH) {
    Serial.println("Flaşör butonuna basınız.");
     p3.setPic(10);
  } else if (digitalRead(button3Pin) == LOW) {
   // Serial.println("Flaşör butonu basıldı.");
     p3.setPic(4);
   // HMISerial.println("Flaşör Butonu Basıldı");
  }

  if (digitalRead(button4Pin) == HIGH) {
    Serial.println("Sağ sinyal butonuna basınız.");
     p2.setPic(9);
  } else if (digitalRead(button4Pin) == LOW) {
   // Serial.println("Sağ sinyal butonu basıldı.");
     p2.setPic(3);
   // HMISerial.println("Sağ sinyal Butonu Basıldı");
  }

  if (digitalRead(button5Pin) == HIGH) {
    Serial.println("Sol sinyal butonuna basınız.");
     p1.setPic(8);
  } else if (digitalRead(button5Pin) == LOW) {
   // Serial.println("Sol sinyal butonu basıldı.");
     p1.setPic(2);
   // HMISerial.println("Sol sinyal Butonu Basıldı");
  }

  if (digitalRead(button6Pin) == HIGH) {
    Serial.println("Fren stop butonuna basınız.");
     p6.setPic(13);
  } else if (digitalRead(button6Pin) == LOW) {
   // Serial.println("Fren stop butonu basıldı.");
     p6.setPic(7);
   // HMISerial.println("Fren stop Butonu Basıldı");
  } 

  // CAN mesajlarını kontrol et
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    Serial.println("Mesaj alındı!");

    } else if (canMsg.can_id == 0x101) {
     int rawTemperature = (canMsg.data[0] << 8) | canMsg.data[1];
      float temperatureC = rawTemperature / 100.0;
      Serial.print("BMS Sıcaklık: ");
      Serial.println(temperatureC, 2);
       x3.setValue(temperatureC);

    } else if (canMsg.can_id == 0x100) {
     for (int i = 0; i < 4; i++) {
        uint16_t scaledVoltage = (canMsg.data[i * 2] << 8) | canMsg.data[i * 2 + 1];
        receivedCellVoltages[i] = scaledVoltage / 100.0;
      }
      float sumVoltage = 0;
      float minVoltage = receivedCellVoltages[0];
      float maxVoltage = receivedCellVoltages[0];

      for (int i = 0; i < 4; i++) {
        sumVoltage += receivedCellVoltages[i];

        if (receivedCellVoltages[i] < minVoltage) {
          minVoltage = receivedCellVoltages[i];
        }

        if (receivedCellVoltages[i] > maxVoltage) {
          maxVoltage = receivedCellVoltages[i];
          
        }
       
      }
      // Toplam, min ve max voltaj bilgilerini yazdır
      Serial.print("Toplam pil voltajı: ");
      Serial.println(sumVoltage, 2);

      Serial.print("Minimum voltaj: ");
      Serial.println(minVoltage, 2);

      Serial.print("Maksimum voltaj: ");
      Serial.println(maxVoltage, 2);
      x0.setValue(maxVoltage);      
      x1.setValue(minVoltage); 
      x2.setValue(sumVoltage);
    
    }
     else if (canMsg.can_id == 0x102) {
     int currentRaw = (canMsg.data[0] << 8) | canMsg.data[1];
      float current = currentRaw * 0.001;

      Serial.print("BMS Akım: ");
      Serial.println(current);
      Serial.println(" A");
     
      x4.setValue(current); 
   
    } 
    
    else if (canMsg.can_id == 0x103) {
      uint16_t scaledSOC = (canMsg.data[0] << 8) | canMsg.data[1];
      float soc = scaledSOC / 100.0; // SoC'yi orijinal formata çevir

      Serial.print("Alınan SoC: ");
      Serial.print(soc);
      Serial.println(" %");
      n0.setValue(soc);   
      pbar.setValue(soc);
    } 
    else if (canMsg.can_id == 0x201) {
       int buttonState = canMsg.data[0];
      if (buttonState == HIGH){
      Serial.println("1");
      p0.setPic(0);
      }
      else{
      Serial.println("0");
      p0.setPic(1);
      }
    } else if (canMsg.can_id == 0x200) {
     float hiz = (canMsg.data[0] << 5) | canMsg.data[1];  
      Serial.print("Motor Hız: ");
      Serial.println(hiz);
      // Hız verisini Gauge'e gönder
      if (hiz < 0) hiz = 0;       // Negatif hız değerlerini 0'a ayarla
      if (hiz > 180) hiz = 180;   // Hız maksimum 180 olacak şekilde sınırlandırıldı
      z0.setValue((int)hiz);      // Gauge'e tam sayı değer gönder

      // Hız sayısını yazdır
      n1.setValue((int)hiz);      // Hız sayısını yazdır (Nextion Number bileşeni)

    } else {
      Serial.println("Bilinmeyen ID!");
    }
  

  delay(100);
}
