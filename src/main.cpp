#include <Arduino.h>
void ReceiveSerial();
void ParseSerialData();
void DisplaySerial(); 
int32_t SerialBufferToInt();
int32_t DecimalPower(int32_t power);

constexpr uint8_t g_kBufferSize=32;
char g_bufferInput[g_kBufferSize] = {0};
char g_bufferAux[g_kBufferSize] = {0};
char g_serialMessage[g_kBufferSize] = {0};
bool g_hasSerialReceived = false;
bool g_isSerialReceiving = false;
int g_serialInteger = 0;
float g_serialFloat = 0.0;

void setup() {
  Serial.begin(115200);
  Serial.println("Begin");
}

void loop() {
  ReceiveSerial();
  ParseSerialData();
  DisplaySerial();
}

void ReceiveSerial() {
  static uint8_t buffer_index = 0;
  static constexpr char kStartMarker = '<';
  static constexpr char kStopMarker = '>';
  char received_byte = 0;

  if (!Serial.available()) return;
  while (Serial.available() && !g_hasSerialReceived) {
    received_byte = Serial.read();
    if (received_byte == kStartMarker && !g_isSerialReceiving) {
      g_isSerialReceiving = true;
      buffer_index = 0;
      continue;
    }
    if (buffer_index >= g_kBufferSize) {
      buffer_index = g_kBufferSize - 1;
      g_bufferInput[buffer_index] = '\0';
      g_hasSerialReceived = true;
      g_isSerialReceiving = false;
      buffer_index=0;
      return;
    }
    if (received_byte == kStopMarker && g_isSerialReceiving) {
      g_hasSerialReceived = true;
      g_isSerialReceiving = false;
      g_bufferInput[buffer_index] = '\0';
      buffer_index = 0;
      return;
    }

    if (!g_isSerialReceiving) continue;
    g_bufferInput[buffer_index] = received_byte;
    buffer_index++;
  }
}

int32_t SerialBufferToInt(){
  int decimalPos = 0;
  int32_t sum = 0;
  for (; decimalPos < g_kBufferSize; ++decimalPos) {
    if (g_bufferInput[decimalPos] == '\0') {--decimalPos; break;}
    if (g_bufferInput[decimalPos] < '0' || g_bufferInput[decimalPos] > '9') return -1;  
  }
  for (int i = 0; i <= decimalPos; i++) {
    sum += (g_bufferInput[i] -'0') * DecimalPower(decimalPos-i);
    Serial.print("Sum: "); Serial.println(sum);
  }
  return sum;
}

void ParseSerialData(){
  if(!g_hasSerialReceived) return;
  strncpy(g_bufferAux, g_bufferInput, g_kBufferSize);
  char* parsingIndex = strtok(g_bufferAux, ",");
  strncpy(g_serialMessage, parsingIndex, g_kBufferSize);

  parsingIndex = strtok(NULL, ",");
  g_serialInteger = atoi(parsingIndex);

  parsingIndex = strtok(NULL, ",");
  g_serialFloat = atof(parsingIndex);
}

void DisplaySerial() {
  if (!g_hasSerialReceived) return;
  g_hasSerialReceived = false;
  Serial.print("ReceivedConv: "); Serial.println(SerialBufferToInt());
  Serial.print("Message: "); Serial.println(g_serialMessage);
  Serial.print("Integer: "); Serial.println(g_serialInteger);
  Serial.print("Float: ");  Serial.println(g_serialFloat);
  Serial.print("Buffer: "); Serial.println(g_bufferInput);
}

int32_t DecimalPower(int32_t exponent){
  int32_t result =1;
  for (int i = 0; i < exponent; i++) {
    result *= 10;
  }
  return result;
}