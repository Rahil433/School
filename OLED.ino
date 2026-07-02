#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define screen dimensions (Change 64 to 32 if you have the smaller OLED)
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The parameter -1 means the display shares the Arduino's reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);
  while(!Serial); // Wait for Serial Monitor to open (mainly for Leonardo/Micro/ESP32)

  Serial.println(F("Initializing OLED..."));

  // Initialize OLED (Try 0x3C first, change to 0x3D if it fails)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed! Check your wiring or I2C address."));
    while (true); // Halt program execution if it fails
  }

  Serial.println(F("OLED Initialized Successfully!"));

  // Clear the buffer
  display.clearDisplay();

  // Set text properties
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner
  
  // Write a message to the buffer
  display.println(F("OLED IS WORKING!"));
  display.println(F("-------------------"));
  display.println(F("If you see this,"));
  display.println(F("your wiring and"));
  display.println(F("code are correct!"));

  // Push the buffer to the hardware display
  display.display();
}

void loop() {
  // Nothing needed here for a static test display
}