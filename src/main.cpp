#include <Arduino.h>

uint8_t g_inputChar; // Serial input character
bool g_newData=false; // Flag to indicate new serial data received

void ShowDataParam(int32_t received_data);
int32_t ReceiveData();

void setup() {
  Serial.begin(115200);
  Serial.println("Begin");
}

void loop() {
  //ReceiveData();
  //ShowData();
  ShowDataParam(ReceiveData());
}

int32_t ReceiveData(){
  if (!Serial.available()) return 0;
  if (Serial.peek()<'0' || Serial.peek()>'9') {Serial.read(); return 0;}
  
  delayMicroseconds(5000);
  g_newData = true;
  int32_t received_data = 0;
 
  while(Serial.available()){
    //delayMicroseconds(20000);
    g_inputChar = Serial.read();
    if (g_inputChar<'0' || g_inputChar>'9') break;
    received_data = 10*received_data + g_inputChar - '0';
    Serial.println(received_data);
  }
  return received_data;
}

void ShowData(){
  if (g_newData) {
    g_newData = false;
    Serial.println(g_inputChar-'0'); 
  }
}

void ShowDataParam(int32_t received_data){
  if (g_newData) {
    g_newData = false;
    Serial.print("Show: "); Serial.println(received_data);
  }
}


