
/*THIS IS THE CODE FOR THE OPTIONAL MICROCONTROLLER*/

//LIBRARIES-----------------------
#include <Adafruit_SH110X.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Keyboard.h>
//-------------------------------

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_NEOPIXEL, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800); //INITIALIZE PIXELS
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &SPI1, OLED_DC, OLED_RST, OLED_CS);        //INITIALIZE DISPLAY

void setup(){ //set everything up
  Keyboard.begin(); //keyboard enable 
  Serial.begin(115200); //serial port
  delay(500);  //delay 500ms
  pixels.begin(); //start pixels
  pixels.setBrightness(255); //set brightness to max
  pixels.show(); //show pixels
  display.begin(0, true); //start OLED
  display.display(); //display the display 
  display.setTextSize(1); //text size 
  display.setTextWrap(false); //no text wrap 
  display.setTextColor(SH110X_WHITE, SH110X_BLACK); // white text, black background
  for (uint8_t i=0; i<=12; i++) { //for each key
    pinMode(i, INPUT_PULLUP); //set to input
  }
}
    
void loop(){ //main loop 

  display.clearDisplay(); //clear everything 
  display.setCursor(0,0); //set cursor to top 
  display.println(" ****** TETRIS ****** "); //title 

  pixels.setPixelColor(7, 255, 192, 203); //pink 
  pixels.setPixelColor(9, 255, 192, 203); //pink
  pixels.setPixelColor(10, 255, 192, 203); //pink
  pixels.setPixelColor(11, 255, 192, 203); //pink 

  if(!digitalRead(8)){ //if key 8 (up)
    Keyboard.press(KEY_UP_ARROW);    // press up arrow key
    delay(8);                        // hold key down
    Keyboard.release(KEY_UP_ARROW);  // release key
  }

  if(!digitalRead(10)){ //if key 10 (right)
    Keyboard.press(KEY_LEFT_ARROW);   // press left arrow key
    delay(10);                        // hold key down 
    Keyboard.release(KEY_LEFT_ARROW); // release key
  }
    
  if(!digitalRead(11)){ //if key 11 (down)
    Keyboard.press(KEY_DOWN_ARROW);   // press down arrow key
    delay(10);                        // hold key down
    Keyboard.release(KEY_DOWN_ARROW); // release key
  }

  if(!digitalRead(12)){ //if key 12 (left)
    Keyboard.press(KEY_RIGHT_ARROW);   // press right arrow key
    delay(10);                         // hold key down for
    Keyboard.release(KEY_RIGHT_ARROW); // release key
  }
  
  pixels.show(); //show neopixels
  display.display(); //display oled
  delay(105); //delay is not a bad idea

}
