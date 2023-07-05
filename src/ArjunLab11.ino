#include "Particle.h"
#include "oled-wing-adafruit.h"

#include "LIS3DH.h"

SYSTEM_THREAD(ENABLED);

SYSTEM_MODE(MANUAL);

const unsigned long PRINT_SAMPLE_PERIOD = 100;

LIS3DHSPI accel(SPI, D3, WKP);
OledWingAdafruit display;

unsigned long lastPrintSample = 0;

void setup()
{
  display.setup();
  resetDisplay();
  display.display();

  Serial.begin(9600);

  LIS3DHConfig config;
  config.setAccelMode(LIS3DH::RATE_100_HZ);

  bool setupSuccess = accel.setup(config);
  Serial.printlnf("setupSuccess=%d", setupSuccess);
}

void loop()
{
  display.loop();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  if (millis() - lastPrintSample >= PRINT_SAMPLE_PERIOD)
  {
    lastPrintSample = millis();

    LIS3DHSample sample;
    if (accel.getSample(sample))
    {
      Serial.printlnf("%d,%d,%d", sample.x, sample.y, sample.z);
      display.println("X, Y, Z:");
      display.printf("%d,%d,%d", sample.x, sample.y, sample.z);
      display.display();
    }
    else
    {
      Serial.println("no sample");
    }
  }
}