#include <SoftwareSerial.h>
#include <Nextion.h>
SoftwareSerial HMISerial(10,11);

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

  Serial.println("Baslatildi...");

}

void loop() {
   
 
  if(digitalRead(button0Pin) == HIGH){  
    Serial.println("Sarj butonuna basiniz.");
   }
  else if(digitalRead(button0Pin) == LOW){
    Serial.println("Şarj butonu basıldı.");
    HMISerial.println("Şarj Butonu Basıldı");}

  if(digitalRead(button1Pin) == HIGH){ 
    Serial.println("Far butonuna basiniz.");
  }
  else if(digitalRead(button1Pin) == LOW){ 
    Serial.println("Far butonu basıldı.");
    HMISerial.println("Far Butonu Basıldı");
  }

  if(digitalRead(button2Pin) == HIGH){ 
    Serial.println("Silecek butonuna basiniz.");
  }
  else if(digitalRead(button2Pin) == LOW){
    Serial.println("Silecek butonu basıldı.");
    HMISerial.println("Silecek Butonu Basıldı");
   }

  if(digitalRead(button3Pin) == HIGH){ 
    Serial.println("Flasor butonuna basiniz.");
  }
  else if(digitalRead(button3Pin) == LOW){ 
    Serial.println("Flasor butonu basıldı.");
    HMISerial.println("Flasor Butonu Basıldı");
  }

  if(digitalRead(button4Pin) == HIGH){ 
    Serial.println("Sag sinyal butonuna basiniz.");
  }
  else if(digitalRead(button4Pin) == LOW){ 
    Serial.println("Sag sinyal butonu basıldı.");
    HMISerial.println("Sag sinyal Butonu Basıldı");
  }

  if(digitalRead(button5Pin) == HIGH){ 
    Serial.println("Sol sinyal butonuna basiniz.");
  }
  else if(digitalRead(button5Pin) == LOW){ 
    Serial.println("Sol sinyal butonu basıldı.");
    HMISerial.println("Sol sinyal Butonu Basıldı");
  }

  if(digitalRead(button6Pin) == HIGH){
    Serial.println("Fren stop butonuna basiniz.");
   }
  else if(digitalRead(button6Pin) == LOW){
    Serial.println("Fren stop butonu basıldı.");
    HMISerial.println("Fren stop Butonu Basıldı");
   }
  delay(1000);
}
