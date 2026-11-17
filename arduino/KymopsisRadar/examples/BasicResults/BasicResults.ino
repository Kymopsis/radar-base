// Print the radar app's results. Wiring (Radar Base carrier, ESP32-S3): radar UART0 TX -> GPIO18, RX <- GPIO17.
// The radar app must start from the radar module's own flash (dev-kit modules ship that way).
#include <KymopsisRadar.h>

HardwareSerial RadarSerial(1);
KymopsisRadar radar(RadarSerial);

const int RADAR_RESET = 4, BOOT_HIGH = 5, BOOT_LOW = 6;   // start the app from the module's flash

void setup() {
  Serial.begin(115200);
  pinMode(BOOT_HIGH, OUTPUT); digitalWrite(BOOT_HIGH, HIGH);
  pinMode(BOOT_LOW, OUTPUT); digitalWrite(BOOT_LOW, LOW);
  pinMode(RADAR_RESET, OUTPUT); digitalWrite(RADAR_RESET, LOW); delay(20); digitalWrite(RADAR_RESET, HIGH);
  RadarSerial.begin(115200, SERIAL_8N1, 18, 17);   // RX, TX
}

void loop() {
  static uint32_t last = 0;
  radar.update();
  if (millis() - last < 500) return;
  last = millis();
  Serial.printf("people %d  nearest %.2f m  areas %d%d%d%d  fall %d  breathing %.0f  heart %.0f\n",
                radar.people(), radar.nearest(), radar.area(0), radar.area(1), radar.area(2), radar.area(3),
                radar.fall(), radar.breathing(), radar.heartRate());
}
