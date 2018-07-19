#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
 
#define PIN 14
const int buttonPin = 2;     // the number of the pushbutton pin

Servo hinge;  // create servo object to control a servo

int angle = 0;
bool open = false;
int hingeSpeed = 0;
  
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
const uint16_t turq = matrix.Color(0, 255, 255);

//------------------------------------------------------------------------------------------------------------------------//

// Mode of intensity
char subtle = 'S';
char mediocre = 'M';
char intensive = 'I';

char mode = mediocre;

//------------------------------------------------------------------------------------------------------------------------//

char* activity = "Meditate";

//------------------------------------------------------------------------------------------------------------------------//

void setup() {

  switch( mode ){
    case 'I':
      matrix.setBrightness(75);
      hingeSpeed = 15;
      break;
    case 'M':
      matrix.setBrightness(30);
      hingeSpeed = 40;
      break;
    default: // subtle
      matrix.setBrightness(15);
      hingeSpeed = 75;
  }
  
  pinMode(buttonPin, INPUT);  // initialize the pushbutton pin as an input
  hinge.attach(5);  // attaches the servo on pin 12 to the servo object
  hinge.write(angle);
  matrix.begin();
  matrix.setTextWrap(false);
  Serial.begin(9600);
  colorWipe(none, 30);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH && !open){
    // TODO: Add a line so that if the button is pressed it just opens without fandangled things
    Serial.println("Button pressed");
    switch( mode ){
      case 'I':
        intense_mode();
        break;
      case 'M':
        medi_mode();
        break;
      default: // subtle
        subtle_mode();
    }
  } else if(buttonState == HIGH){
    open_close();
  }
  
}

bool respond_to_button(){
  if (digitalRead(buttonPin) == HIGH){
    while(digitalRead(buttonPin) == HIGH){
      //Do nowt
    }
    colorWipe(blue, 10);
    colorWipe(none, 10);
    open_close();
    return true;
  }
  return false;
}

void intense_mode(){
  intense_movement();
  open_close();
  
  while(true){
    if(theaterChase(white, 50, 50)){
      break;
    }
    if(colorWipe(red, 10)){
      break;
    }
    if(theaterChase(red, 50, 50)){
      break;
    }
    if(colorWipe(purple, 10)){
      break;
    }
    if(colorWipe(none, 10)){
      break;
    }
    if(display_words(activity, blue, 150)){
      break;
    }
    if(colorWipe(matrix.Color(93, 37, 166), 10)){
      break;
    }
    if(colorWipe(none, 10)){
      break;
    }
    if(theaterChaseRainbow(50)){
      break;
    }
    if(colorWipe(white, 10)){
      break;
    }
  }
}

void medi_mode(){
  some_movement();
  open_close();

   while(true){
    if(colorWipe(green, 30)){
      break;
    }
    if(theaterChase(green, 90, 30)){
      break;
    }
    if(colorWipe(blue, 30)){
      break;
    }
    if(theaterChase(blue, 70, 40)){
      break;
    }
    if(colorWipe(matrix.Color(111, 0, 255), 30)){
      break;
    }
    if(colorWipe(none, 30)){
      break;
    }
    if(display_words(activity, matrix.Color(111, 0, 255), 150)){
      break;
    }
    if(colorWipe(matrix.Color(146, 146, 30), 30)){
      break;
    }
    if(colorWipe(yellow, 30)){
      break;
    }
    if(colorWipe(matrix.Color(171, 255, 0), 30)){
      break;
    }
  }
}

void subtle_mode(){
  open_close();

  while(true){
    if(colorWipe(turq, 30)){
      break;
    }
    if(colorWipe(blue, 30)){
      break;
    }
    if(colorWipe(matrix.Color(111, 0, 255), 30)){
      break;
    }
    if(colorWipe(matrix.Color(93, 37, 166), 30)){
      break;
    }
    if(colorWipe(matrix.Color(188, 0, 255), 30)){
      break;
    }
    if(colorWipe(purple, 30)){
      break;
    }
    if(colorWipe(matrix.Color(255, 164, 255), 30)){
      break;
    }
    if(colorWipe(white, 30)){
      break;
    }
    
  }
}

void some_movement(){
  for (int pos = 0; pos <= 60 ; pos += 1) {
    hinge.write(pos);
    delay(hingeSpeed); 
  }
  hinge.write(0);
  delay(500);
}

void intense_movement(){
  for(int i=0; i<3; i++){
    hinge.write(30);
    delay(300);
    hinge.write(0);
    delay(300);
  }
  delay(200);
}

void open_close(){
  while(digitalRead(buttonPin) == HIGH){
    //Do nothing
  }
  if(open){
    for (int pos = angle; pos >= 0 ; pos -= 1) {
      hinge.write(pos);
      delay(hingeSpeed); 
    }
    angle = 0;
    open = false;
  } else {
    for (int pos = angle; pos <= 100 ; pos += 1) {
      hinge.write(pos);
      delay(hingeSpeed); 
    }
    angle = 100;
    open = true;
  }
}

// Fill the dots one after the other with a color
bool colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<matrix.height(); i++) {
    if(respond_to_button()){
      return true;
    }
    for(uint16_t j=0; j<matrix.width(); j++) {
        matrix.drawPixel(j, i, c);
        matrix.show();
        delay(wait);
    }
  }
  return false;
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

bool rainbowPulse(uint8_t wait, int cycles) {
  uint16_t i, j, x;

  for(j=0; j<256*cycles; j++) {
    if(respond_to_button()){
      return true;
    }
    for(i=0; i< matrix.height(); i++) {
      for(x=0; x < matrix.width(); x++){
        matrix.drawPixel(x, i, Wheel(((i * x * 256 / ( matrix.height() * matrix.width() )) + j) & 255));
      }
    }
    matrix.show();
    delay(wait);
  }
  return false;
}

//Theatre-style crawling lights.
bool theaterChase(uint32_t c, uint8_t wait, int cycles) {
  for (int j=0; j<cycles; j++) {  //do 10 cycles of chasing
    if(respond_to_button()){
      return true;
    }
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
  return false;
}

//Theatre-style crawling lights with rainbow effect
bool theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    if(respond_to_button()){
      return true;
    }
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
  return false;
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

bool display_words( char* message, uint32_t c, uint8_t wait ){
    matrix.setTextColor(c);
  
    char str[50];
    strcpy(str, message);
    int len = strlen(str);
    
  for(int x = matrix.width(); x > -(len*6 + 10); x--){
    if(respond_to_button()){
      return true;
    }
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
  return false;
}


