#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RotaryEncoder.h>
  
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

// OLED FeatherWing buttons map to different pins depending on board:
#if defined(ESP8266)
 #define BUTTON_A  0
 #define BUTTON_B 16
 #define BUTTON_C  2
#elif defined(ESP32)
  #define BUTTON_A 15
  #define BUTTON_B 32
  #define BUTTON_C 14
#elif defined(ARDUINO_STM32_FEATHER)
  #define BUTTON_A PA15
  #define BUTTON_B PC7
  #define BUTTON_C PC5
#elif defined(TEENSYDUINO)
  #define BUTTON_A  4
  #define BUTTON_B  3
  #define BUTTON_C  8
#elif defined(ARDUINO_FEATHER52832)
  #define BUTTON_A 31
  #define BUTTON_B 30
  #define BUTTON_C 27
#else // 32u4, M0, M4, nrf52840 and 328p
  #define BUTTON_A  9
  #define BUTTON_B  6
  #define BUTTON_C  5
#endif

#define outputA 12
#define outputB 13

volatile float counter = 0;
const float encoderRadius = 25.4; // Radius of encoder in mm ***Double check  this value***
const float mmToInches = 0.0393701; // 0.0393701 inches per mm 
int distanceInches = 0;
int distanceFeet = 0;

void setup() {
  Serial.begin(9600);

  Serial.println("OLED FeatherWing test");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  Serial.println("IO test");

  pinMode(outputA, INPUT_PULLUP);
  pinMode(outputB, INPUT_PULLUP);  

  // text display tests
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.setCursor(0,0);
  display.display(); // actually display all of the above

  display.clearDisplay();
  display.println("Welcome to the \nunderground!");
  display.display();
  delay(3000);
  display.setTextSize(2);
  display.setCursor(0,0);
  
  attachInterrupt(digitalPinToInterrupt(12), count, RISING); // interrupt used to count rising edges
}

void loop() {
  rotationsToImperial(counter/2048);
  display.clearDisplay();
  display.print("Feet: ");
  display.println((int)distanceFeet);
  display.print("In: ");
  display.print((int)(distanceInches - (((int)distanceFeet)*12))); //Take inch count and subtract the feet that have already been counted
  display.display();
  display.setCursor(0, 0);
}

void count(){ // interrupt service routine
  if(digitalRead(12) == digitalRead(13)) counter++;
  else counter--;
}

void rotationsToImperial(float rotations) {
  distanceInches = rotations * 3.1415 * (encoderRadius) * mmToInches;
  distanceFeet = distanceInches / 12;
}
