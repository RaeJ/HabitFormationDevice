#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
 
#define PIN 14
const int buttonPin = 2;     // the number of the pushbutton pin

Servo hinge;  // create servo object to control a servo

int angle = 0;
bool open = false;
  
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, 1, PIN,
  NEO_TILE_BOTTOM   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_PROGRESSIVE +
  NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

//------------------------------------------------------------------------------------------------------------------------//

// Colours defined
const uint16_t none = matrix.Color(0, 0, 0);
const uint16_t red = matrix.Color(255, 0, 0);
const uint16_t green = matrix.Color(0, 255, 0);
const uint16_t blue = matrix.Color(0, 0, 255);
const uint16_t white = matrix.Color(255, 255, 255);
const uint16_t yellow = matrix.Color(255, 255, 0);
const uint16_t purple = matrix.Color(255, 0, 255);
const uint16_t turquiose = matrix.Color(0, 255, 255);

//------------------------------------------------------------------------------------------------------------------------//

// Mode of intensity
char subtle = 'S';
char mediocre = 'M';
char intensive = 'I';

char mode = subtle;

//------------------------------------------------------------------------------------------------------------------------//

char* activity = "Meditate";

//------------------------------------------------------------------------------------------------------------------------//

void setup() {

  switch( mode ){
    case 'I':
      matrix.setBrightness(75);
      break;
    case 'M':
      matrix.setBrightness(30);
      break;
    default: // subtle
      matrix.setBrightness(15);
  }
  
  pinMode(buttonPin, INPUT);  // initialize the pushbutton pin as an input
  hinge.attach(5);  // attaches the servo on pin 12 to the servo object
  hinge.write(angle);
  matrix.begin();
  matrix.setTextWrap(false);
  Serial.begin(9600);
}

void open_close(){
  while(digitalRead(buttonPin) == HIGH){
    //Do nothing
  }
  if(open){
    angle = 0;
    open = false;
  } else {
    angle = 100;
    open = true;
  }

  hinge.write(angle);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH){
    open_close();
  }
  
//  switch( mode ){
//    case 'I':
//      intense_mode();
//      break;
//    case 'M':
//      medi_mode();
//      break;
//    default: // subtle
//      subtle_mode();
//  }
}


void intense_mode(){
  
}

void medi_mode(){
  
}

void subtle_mode(){

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<matrix.height(); i++) {
    for(uint16_t j=0; j<matrix.width(); j++) {
        matrix.drawPixel(j, i, c);
        matrix.show();
        delay(wait);
    }
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j, x;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< matrix.height(); i++) {
      for(x=0; x < matrix.width(); x++){
        matrix.drawPixel(x, i, Wheel(((i * 256 / matrix.height()) + j) & 255));
      }
    }
    matrix.show();
    delay(wait);
  }
}

void rainbowPulse(uint8_t wait) {
  uint16_t i, j, x;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< matrix.height(); i++) {
      for(x=0; x < matrix.width(); x++){
        matrix.drawPixel(x, i, Wheel(((i * x * 256 / ( matrix.height() * matrix.width() )) + j) & 255));
      }
    }
    matrix.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait, int cycles) {
  for (int j=0; j<cycles; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int x=0; x < matrix.height(); x++) {
        for (int i=0; i < matrix.width(); i=i+3) {
          matrix.drawPixel(i+q, x, c);
//          matrix.drawPixel(i+q+(x%3), x, c);
        }
      }
      matrix.show();
     
      delay(wait);

      for (int x=0; x < matrix.height(); x++) {
        for (int i=0; i < matrix.width(); i=i+3) {
          matrix.drawPixel(i+q, x, 0);
//          matrix.drawPixel(i+q+(x%3), x, 0);
        }
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int x=0; x < matrix.width(); x++) {
        for (int i=0; i < matrix.height(); i=i+3) {
          matrix.drawPixel(x, i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
      }
        matrix.show();
       
        delay(wait);
        for (int x=0; x < matrix.width(); x++) {
          for (int i=0; i < matrix.height(); i=i+3) {
            matrix.drawPixel(x, i+q, 0);        //turn every third pixel off
          }
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void display_words( char* message, uint32_t c, uint8_t wait ){
    matrix.setTextColor(c);
  
    char str[50];
    strcpy(str, message);
    int len = strlen(str);
    
  for(int x = matrix.width(); x > -(len*6 + 10); x--){
    matrix.fillScreen(0);
    matrix.setCursor(x, 0);
    
    matrix.print(str);
  
    if(--x < (-(len*6 + 10))
    ) {
      x = matrix.width();
    }
    matrix.show();
    delay( wait );
  }
  
}


