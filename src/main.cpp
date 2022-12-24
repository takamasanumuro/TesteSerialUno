#include <Arduino.h>
void ReceiveSerial();
void DisplaySerial(); 

constexpr uint8_t g_kBufferSize=32;
char g_bufferInput[g_kBufferSize] = {0};
bool g_hasSerialReceived = false;
bool g_isSerialReceiving = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Begin");
}

void loop() {
  ReceiveSerial();
  DisplaySerial();
}

void ReceiveSerial() {
  static uint8_t buffer_index = 0;
  static constexpr char kStartMarker = '<';
  static constexpr char kStopMarker = '>';
  char received_byte = 0;

  if (!Serial.available()) return;
  while (Serial.available() && g_hasSerialReceived == false) {
    received_byte = Serial.read();
    if (received_byte == kStartMarker && g_isSerialReceiving == false) {
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
    if (received_byte == kStopMarker && g_isSerialReceiving == true) {
      g_hasSerialReceived = true;
      g_isSerialReceiving = false;
      g_bufferInput[buffer_index] = '\0';
      buffer_index = 0;
      return;
    }

    if (g_isSerialReceiving == false) continue;
    g_bufferInput[buffer_index] = received_byte;
    buffer_index++;
  }
}

void DisplaySerial() {
  if (g_hasSerialReceived) {
    g_hasSerialReceived = false;
    Serial.print(g_bufferInput);
  }
}