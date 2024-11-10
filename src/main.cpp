#include <Arduino.h>
#include <WiFi.h>

// extern "C" void cyw43_set_pio_clock_divisor(uint16_t clock_div_int, uint8_t clock_div_frac);

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Pico W running at " + String(rp2040.f_cpu() / 1000000) + " MHz");
  Serial.flush();
  delay(1000);
  //cyw43_set_pio_clock_divisor(4, 0); // is done in picow_init.cpp already
  // blink LED (is on WIFI module)
  pinMode(LED_BUILTIN, OUTPUT);
  for(int i = 0; i < 6; i++) {
    digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) ^ 1);
    delay(500);
  }
}

const char *macToString(uint8_t mac[6]) {
  static char s[20];
  sprintf(s, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return s;
}

const char *encToString(uint8_t enc) {
  switch (enc) {
    case ENC_TYPE_NONE: return "NONE";
    case ENC_TYPE_TKIP: return "WPA";
    case ENC_TYPE_CCMP: return "WPA2";
    case ENC_TYPE_AUTO: return "AUTO";
  }
  return "UNKN";
}

void loop() {
  delay(5000);
  Serial.printf("Beginning scan at %lu\n", millis());
  auto cnt = WiFi.scanNetworks();
  if (!cnt) {
    Serial.printf("No networks found\n");
  } else {
    Serial.printf("Found %d networks\n\n", cnt);
    Serial.printf("%32s %5s %17s %2s %4s\n", "SSID", "ENC", "BSSID        ", "CH", "RSSI");
    for (auto i = 0; i < cnt; i++) {
      uint8_t bssid[6];
      WiFi.BSSID(i, bssid);
      Serial.printf("%32s %5s %17s %2d %4ld\n", WiFi.SSID(i), encToString(WiFi.encryptionType(i)), macToString(bssid), WiFi.channel(i), WiFi.RSSI(i));
    }
  }
  Serial.printf("\n--- Sleeping ---\n\n\n");
  delay(5000);
}
