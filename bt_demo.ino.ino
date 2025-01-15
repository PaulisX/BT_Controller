#include <SoftwareSerial.h>
#include <stdlib.h>

#define MOT_A1_PIN 3
#define MOT_A2_PIN 5
#define MOT_B1_PIN 6
#define MOT_B2_PIN 9
#define SLEEP_PIN 2


SoftwareSerial BT(10,11);
char val;
char c=' ';
int8_t motA = 0;
int8_t motB = 0;
bool autopilot = false;
bool initBT = true;
void setup() {
  // put your setup code here, to run once:
  // delay(1000);
  Serial.begin(9600);
  Serial.println("ready");
  BT.begin(9600);
  // BT.write("AT+PSWD0000");
  // BT.write("AT+BAUD1");

  pinMode(MOT_A1_PIN, OUTPUT);
  pinMode(MOT_A2_PIN, OUTPUT);
  pinMode(MOT_B1_PIN, OUTPUT);
  pinMode(MOT_B2_PIN, OUTPUT);
  pinMode(SLEEP_PIN, OUTPUT);

  digitalWrite(MOT_A1_PIN, LOW);
  digitalWrite(MOT_A2_PIN, LOW);
  digitalWrite(MOT_B1_PIN, LOW);
  digitalWrite(MOT_B2_PIN, LOW);
  digitalWrite(SLEEP_PIN, HIGH);
}
void printBin(byte aByte) {
  for (int8_t aBit = 7; aBit >= 0; aBit--)
    Serial.write(bitRead(aByte, aBit) ? '1' : '0');
}

void swap(int8_t &a, int8_t &b){
  a = a + b;
  b = a - b;
  a = a - b;
}
unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}
void loop() {
  // if(BT.available()){
  //   Serial.write(BT.read());
  //   Serial.write(Serial.read());
  // }
  // if(Serial.available()){
  //   BT.write(Serial.read());
  // }
  // return;
  motA = 255;
  motB = 194;
  // if(initBT && BT.available()>1){
    
  //   motA = BT.read();
  //   motB = BT.read();
  //   if(motA != 0 || motB != 0){
  //     return;
  //   }
  //   Serial.println("Configured");
  //   while(BT.available()>0){
  //     BT.read();
  //   }
  //   Serial.println("Cleared");
  //   initBT = false;
  // }
  if(BT.available()>3){
    Serial.print("r");
    motA = BT.read();
  }
  if(BT.available()>1){
    Serial.print(BT.available());
    Serial.print(", ");
    motA = BT.read();
    motB = BT.read();
    // Serial.println(motA);

    // x1xx xxxx, is motor A
    if(motB & 1 > 0){
      swap(motA, motB);
    }
    
  //   Serial.print("motA: ");
  //   Serial.print(motA);
  //   Serial.print(", motB: ");
  //   Serial.println(motB);
  }
  // motA = 50;
  if(motA & motB == -128){
    autopilot != autopilot;
    Serial.write("Autopilot: ");
    Serial.println(autopilot);
    return;
  }
  if(!autopilot){
    // ignore motor and direction
    uint8_t speed = motA >> 2;
    bool dir = (motA & 3) > 0;
    printBin(motA);
    Serial.print(", ");
    Serial.print(speed);
    Serial.print(", ");
    Serial.print(dir);
    Serial.print(", ");
    printBin(motB);
    Serial.print(", ");
    // 0xxx xxxx is forward, 1x... backward
    // Serial.println(speed);
    if(motA & 2){
      analogWrite(MOT_A1_PIN, speed*4);
      analogWrite(MOT_A2_PIN, 0);
    } else {
      analogWrite(MOT_A1_PIN, 0);
      analogWrite(MOT_A2_PIN, speed*4);
    }
    speed = motB >> 2;
    dir = (motB & 2)  > 0;
    Serial.print(speed);
    Serial.print(", ");
    Serial.println(dir);
    if(dir){
      analogWrite(MOT_B1_PIN, speed*2);
      analogWrite(MOT_B2_PIN, 0);
    } else {
      analogWrite(MOT_B1_PIN, 0);
      analogWrite(MOT_B2_PIN, speed*2);
    }
  }
}
