#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
 
#define PIN 14
  
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, 1, PIN,
  NEO_TILE_BOTTOM   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_PROGRESSIVE +
  NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(255, 255, 0),matrix.Color(0, 0, 255), matrix.Color(255, 0, 255), matrix.Color(0, 255, 255), matrix.Color(255, 255, 255)};

//------------------------------------------------------------------------------------------------------------------------//

char subtle = 'S';
char mediocre = 'M';
char intensive = 'I';

char mode = subtle;

//------------------------------------------------------------------------------------------------------------------------//

int speed;

void setup() {

  switch( mode ){
    case 'I':
      speed = 50;
      matrix.setBrightness(50);
      break;
    case 'M':
      speed = 75;
      matrix.setBrightness(20);
      break;
    default: // subtle
      speed = 100;
      matrix.setBrightness(5);
  }
  
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setTextColor(colors[0]);
}

int x    = matrix.width();
int pass = 0;


void loop() {
  // Some example procedures showing how to display to the pixels:
//  colorWipe(matrix.Color(255, 0, 0), 20); // Red
//  colorWipe(matrix.Color(0, 255, 0), 20); // Green
//  colorWipe(matrix.Color(0, 0, 255), 20); // Blue

  
  // Send a theater pixel chase in...
//  theaterChase(matrix.Color(127, 127, 127), 50); // White
//  theaterChase(matrix.Color(127,   0,   0), 50); // Red
//  theaterChase(matrix.Color(  0,   0, 127), 50); // Blue

  rainbow(20);
//  rainbowCycle(20);
//  theaterChaseRainbow(50);
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
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<294; i++) {
      matrix.drawPixel(0, i, Wheel((i+j) & 255));
    }
    matrix.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< 294; i++) {
      matrix.drawPixel(0, i, Wheel(((i * 256 / 294) + j) & 255));
    }
    matrix.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
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
        for (int i=0; i < 294; i=i+3) {
          matrix.drawPixel(0, i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        matrix.show();
       
        delay(wait);
       
        for (int i=0; i < 294; i=i+3) {
          matrix.drawPixel(0, i+q, 0);        //turn every third pixel off
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

//void loop() {
////  display_words( "Stop being nosey" );
//  rainbowCycle(20);
//}
//
//void display_words( char* message ){
//  matrix.fillScreen(0);
//  matrix.setCursor(x, 0);
//  
//  char str[50];
//  strcpy(str, message);
//  int len = strlen(str);
//  
//  matrix.print(str);
//
//  if(--x < (-(len*6 + 10))
//  ) {
//    x = matrix.width();
//
//    if(++pass >= 8) pass = 0;
//    matrix.setTextColor(colors[pass]);
//  }
//  matrix.show();
//  delay( speed );
//}
//
//uint32_t Wheel(byte WheelPos) {
//  WheelPos = 255 - WheelPos;
//  if(WheelPos < 85) {
//    return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//  }
//  if(WheelPos < 170) {
//    WheelPos -= 85;
//    return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//  }
//  WheelPos -= 170;
//  return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//}
//
//void rainbowCycle(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
//    for(i=0; i< matrix.numPixels(); i++) {
//      matrix.setPixelColor(i, Wheel(((i * 256 / matrix.numPixels()) + j) & 255));
//    }
//    matrix.show();
//    delay(wait);
//  }
//}

