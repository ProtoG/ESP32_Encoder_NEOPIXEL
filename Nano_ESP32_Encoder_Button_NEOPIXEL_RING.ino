//Proto G Engineering 07/08/2025
//IMPORTANT!
//Go to Tools->Pin Numbering->By GPIO Number(legacy)
//Change it from "By Arduino pin (default)"


// Rotary Encoder Inputs
#define CLK D11
#define DT D10
#define SW D9

#include <Adafruit_NeoPixel.h>


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        A6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int RGBCounter = 0;
int PixelCounter = 0;
int R = 0;
int G = 150;
int B = 0;
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;

void setup() {
    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  // Set encoder pins as inputs
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup Serial Monitor
  Serial.begin(9600);
  
  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);
  pixels.clear(); // Set all pixel colors to 'off'
}

void toggleLED() {
  if (digitalRead(LED_BUILTIN) == LOW){
    digitalWrite(LED_BUILTIN, HIGH);
    if (RGBCounter == 0){
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
    R = 150;
    G = 0;
    B = 0;
  }
      else if (RGBCounter == 1){
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, HIGH);
    R = 0;
    G = 0;
    B = 150;
  }
      else if (RGBCounter == 2){
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, LOW);
    R = 150;
    G = 150;
    B = 0;
  }
      else if (RGBCounter == 3){
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
    R = 150;
    G = 0;
    B = 150;
  }
      else if (RGBCounter == 4){
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, LOW);
    R = 0;
    G = 150;
    B = 150;
  }
      else if (RGBCounter == 5){
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, HIGH);
    R = 150;
    G = 150;
    B = 150;
  }
      else if (RGBCounter == 6){
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, HIGH);
    R = 0;
    G = 150;
    B = 0;
  }

  }
  else
  {
   digitalWrite(LED_BUILTIN, LOW);
   digitalWrite(LED_RED, HIGH);
   digitalWrite(LED_GREEN, HIGH);
   digitalWrite(LED_BLUE, HIGH);
   RGBCounter++;
   if (RGBCounter == 7){
    RGBCounter = 0;
  }
}
}

void loop() {

  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      counter++;
      currentDir = "CW";
      //toggleLED();
      pixels.setPixelColor(PixelCounter, pixels.Color(R, G, B));
      pixels.show();   // Send the updated pixel colors to the hardware.
      PixelCounter--;
      if(PixelCounter == -1){
        PixelCounter = 15;
      }
     

    } else {
      // Encoder is rotating CW so increment
      counter--;
      currentDir = "CCW";
      //toggleLED();
      pixels.setPixelColor(PixelCounter, pixels.Color(0, 0, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      PixelCounter++;
      if(PixelCounter == 16){
        PixelCounter = 0;
      }
      
    }

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;

  // Read the button state
  int btnState = digitalRead(SW);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
      toggleLED();
      pixels.clear(); // Set all pixel colors to 'off'
      pixels.show();   // Send the updated pixel colors to the hardware.

    }

    // Remember last button press event
    lastButtonPress = millis();
  }

  // Put in a slight delay to help debounce the reading
  delay(1);
}