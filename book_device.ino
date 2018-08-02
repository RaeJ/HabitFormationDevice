#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>

#include <i2s.h>
#include "wavspiffs.h"

void respondToData();
void prepareIds();
boolean connectWifi();
boolean connectUDP();
void startHttpServer();
void turnOnDevice();
void turnOffDevice();
void sendDeviceState();

const char* ssid = "NameOfNetwork";  // CHANGE: Wifi name
const char* password = "AardvarkBadgerHedgehog";  // CHANGE: Wifi password 
String friendlyName = "TheDevice";        // CHANGE: Alexa device name
#define PIN 14
const int buttonPin = 16;     // the number of the pushbutton pin

Servo hinge;  // create servo object to control a servo

WiFiUDP UDP;
IPAddress ipMulti(239, 255, 255, 250);
ESP8266WebServer HTTP(80);
boolean udpConnected = false;
unsigned int portMulti = 1900;      // local port to listen on
unsigned int localPort = 1900;      // local port to listen on
boolean wifiConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
String serial;
String persistent_uuid;
boolean cannotConnectToWifi = false;

int angle = 0;
bool opened = false;
bool soundPlayed = false;
bool finishedPlaying = false;
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

char mode = subtle;

//------------------------------------------------------------------------------------------------------------------------//

char* activity = "Just do it";

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
  
  hinge.attach(5);  // attaches the servo on pin 12 to the servo object
  hinge.write(angle);
  matrix.begin();
  matrix.setTextWrap(false);
  colorWipe(none, 30);

  pinMode(buttonPin, INPUT);
  Serial.begin(115200); Serial.println();
  Serial.println(F("\nESP8266 Sound Effects Web Trigger"));

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS.begin() failed");
    return;
  }
  // Confirm track files are present in SPIFFS
  showDir();

  wav_setup();

  prepareIds();
  
  // Initialise wifi connection
  wifiConnected = connectWifi();

  // only proceed if wifi connection successful
  if(wifiConnected){
    Serial.println("Ask Alexa to discover devices");
    udpConnected = connectUDP();
    
    if (udpConnected){
      // initialise pins if needed 
      startHttpServer();
    }
  }
}

void loop() {
  colorWipe(none, 30);
  simple_open_close();

  respondToData();
}

void simple_open_close(){
  if(digitalRead(buttonPin) == HIGH && !opened){
    wait_for_release();
    open_book();
  } else if(digitalRead(buttonPin) == HIGH){
    wait_for_release();
    colorWipe(none, 30);
    close_book();
  }
}

void wait_for_release(){
  while(digitalRead(buttonPin) == HIGH){
      //Do nowt
    }
}

bool respond_to_button(){
  if (digitalRead(buttonPin) == HIGH){
    wait_for_release();
    colorWipe(blue, 10);
    colorWipe(none, 10);
    close_book();
    return true;
  }
  return false;
}

void intense_mode(){
  bool buttonPressed = false;
  intense_movement();
  open_book();
  
  while(opened){
    if(colorWipe(red, 10)){
      break;
    }
    
    while(!finishedPlaying){
      if(!soundPlayed){
        wav_startPlayingFile("/ZapFX001.wav");
        soundPlayed = true;  
      }
      if(wav_loop()){
        buttonPressed = true;
      }
    }
    soundPlayed = false;
    finishedPlaying = false;
    
    if(buttonPressed){
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
    if(theaterChase(white, 50, 50)){
      break;
    }
  }
}

void medi_mode(){
  bool buttonPressed = false;
  some_movement();
  open_book();

  while(!finishedPlaying){
      if(!soundPlayed){
        wav_startPlayingFile("/ZapFX001.wav");
        soundPlayed = true;  
      }
      if(wav_loop()){
        buttonPressed = true;
      }
    }
    soundPlayed = false;
    finishedPlaying = false;

   while(!buttonPressed && opened){
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
  open_book();

  while(opened){
    
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

void open_book(){
  wait_for_release();
  for (int pos = angle; pos <= 100 ; pos += 1) {
    hinge.write(pos);
    delay(hingeSpeed); 
  }
  angle = 100;
  opened = true;
  onMsg();
  respondToData();
}

void close_book(){
  wait_for_release();
  for (int pos = angle; pos >= 0 ; pos -= 1) {
    hinge.write(pos);
    delay(hingeSpeed);
  }
  angle = 0;
  opened = false;
  offMsg();
  respondToData(); 
  delay(5000);
}

//------------------------------------------------------------------------------------------------------------------------//

// Connection functions

void prepareIds() {
  uint32_t chipId = ESP.getChipId();
  char uuid[64];
  sprintf_P(uuid, PSTR("38323636-4558-4dda-9188-cda0e6%02x%02x%02x"),
        (uint16_t) ((chipId >> 16) & 0xff),
        (uint16_t) ((chipId >>  8) & 0xff),
        (uint16_t)   chipId        & 0xff);

  serial = String(uuid);
  persistent_uuid = "Socket-1_0-" + serial;
}

void respondToSearch() {
    Serial.println("");
    Serial.print("Sending response to ");
    Serial.println(UDP.remoteIP());
    Serial.print("Port : ");
    Serial.println(UDP.remotePort());

    IPAddress localIP = WiFi.localIP();
    char s[16];
    sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

    String response = 
         "HTTP/1.1 200 OK\r\n"
         "CACHE-CONTROL: max-age=86400\r\n"
         "DATE: Fri, 15 Apr 2016 04:56:29 GMT\r\n"
         "EXT:\r\n"
         "LOCATION: http://" + String(s) + ":80/setup.xml\r\n"
         "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
         "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
         "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
         "ST: urn:Belkin:device:**\r\n"
         "USN: uuid:" + persistent_uuid + "::urn:Belkin:device:**\r\n"
         "X-User-Agent: redsonic\r\n\r\n";

    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    UDP.write(response.c_str());
    UDP.endPacket();                    

     Serial.println("Response sent !");
}

void startHttpServer() {
    HTTP.on("/index.html", HTTP_GET, [](){
      Serial.println("Got Request index.html ...\n");
      HTTP.send(200, "text/plain", "Hello World!");
    });

    HTTP.on("/upnp/control/basicevent1", HTTP_POST, []() {
      Serial.println("########## Responding to  /upnp/control/basicevent1 ... ##########");      

      //for (int x=0; x <= HTTP.args(); x++) {
      //  Serial.println(HTTP.arg(x));
      //}
  
      String request = HTTP.arg(0);      
      Serial.print("request:");
      Serial.println(request);
 
      if(request.indexOf("SetBinaryState") >= 0) {
        if(request.indexOf("<BinaryState>1</BinaryState>") >= 0) {
            Serial.println("Got Turn on request");
//            colorWipe(red, 10);
            turnOnDevice();
        }
  
        if(request.indexOf("<BinaryState>0</BinaryState>") >= 0) {
            Serial.println("Got Turn off request");
            colorWipe(none, 30);
            turnOffDevice();
        }
      }

      if(request.indexOf("GetBinaryState") >= 0) {
        Serial.println("Got binary state request");
        sendDeviceState();
      }
            
      HTTP.send(200, "text/plain", "");
    });

    HTTP.on("/eventservice.xml", HTTP_GET, [](){
      Serial.println(" ########## Responding to eventservice.xml ... ########\n");
      
      String eventservice_xml = "<scpd xmlns=\"urn:Belkin:service-1-0\">"
        "<actionList>"
          "<action>"
            "<name>SetBinaryState</name>"
            "<argumentList>"
              "<argument>"
                "<retval/>"
                "<name>BinaryState</name>"
                "<relatedStateVariable>BinaryState</relatedStateVariable>"
                "<direction>in</direction>"
                "</argument>"
            "</argumentList>"
          "</action>"
          "<action>"
            "<name>GetBinaryState</name>"
            "<argumentList>"
              "<argument>"
                "<retval/>"
                "<name>BinaryState</name>"
                "<relatedStateVariable>BinaryState</relatedStateVariable>"
                "<direction>out</direction>"
                "</argument>"
            "</argumentList>"
          "</action>"
      "</actionList>"
        "<serviceStateTable>"
          "<stateVariable sendEvents=\"yes\">"
            "<name>BinaryState</name>"
            "<dataType>Boolean</dataType>"
            "<defaultValue>0</defaultValue>"
           "</stateVariable>"
           "<stateVariable sendEvents=\"yes\">"
              "<name>level</name>"
              "<dataType>string</dataType>"
              "<defaultValue>0</defaultValue>"
           "</stateVariable>"
        "</serviceStateTable>"
        "</scpd>\r\n"
        "\r\n";
            
      HTTP.send(200, "text/plain", eventservice_xml.c_str());
    });
    
    HTTP.on("/setup.xml", HTTP_GET, [](){
      Serial.println(" ########## Responding to setup.xml ... ########\n");

      IPAddress localIP = WiFi.localIP();
      char s[16];
      sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);
    
      String setup_xml = "<?xml version=\"1.0\"?>"
            "<root>"
             "<device>"
                "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
                "<friendlyName>"+ friendlyName +"</friendlyName>"
                "<manufacturer>Belkin International Inc.</manufacturer>"
                "<modelName>Socket</modelName>"
                "<modelNumber>3.1415</modelNumber>"
                "<modelDescription>Belkin Plugin Socket 1.0</modelDescription>\r\n"
                "<UDN>uuid:"+ persistent_uuid +"</UDN>"
                "<serialNumber>221517K0101769</serialNumber>"
                "<binaryState>0</binaryState>"
                "<serviceList>"
                  "<service>"
                      "<serviceType>urn:Belkin:service:basicevent:1</serviceType>"
                      "<serviceId>urn:Belkin:serviceId:basicevent1</serviceId>"
                      "<controlURL>/upnp/control/basicevent1</controlURL>"
                      "<eventSubURL>/upnp/event/basicevent1</eventSubURL>"
                      "<SCPDURL>/eventservice.xml</SCPDURL>"
                  "</service>"
              "</serviceList>" 
              "</device>"
            "</root>\r\n"
            "\r\n";
            
        HTTP.send(200, "text/xml", setup_xml.c_str());
        
        Serial.print("Sending :");
        Serial.println(setup_xml);
    });

    // openHAB support
    HTTP.on("/on.html", HTTP_GET, [](){
         Serial.println("Got Turn on request");
         HTTP.send(200, "text/plain", "turned on");
//         colorWipe(green, 10);
         turnOnDevice();
       });
 
     HTTP.on("/off.html", HTTP_GET, [](){
        Serial.println("Got Turn off request");
        HTTP.send(200, "text/plain", "turned off");
        colorWipe(none, 30);
        turnOffDevice();
       });
 
      HTTP.on("/status.html", HTTP_GET, [](){
        Serial.println("Got status request");
 
        String staterespone = "0"; 
        if (opened) {
          staterespone = "1"; 
        }
        HTTP.send(200, "text/plain", staterespone);
      
    });
    
    HTTP.begin();  
    Serial.println("HTTP Server started ..");
}
      
// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}

boolean connectUDP(){
  boolean state = false;
  
  Serial.println("");
  Serial.println("Connecting to UDP");
  
  if(UDP.beginMulticast(WiFi.localIP(), ipMulti, portMulti)) {
    Serial.println("Connection successful");
    state = true;
  }
  else{
    Serial.println("Connection failed");
  }
  
  return state;
}

void onMsg(){
  String body = 
      "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>\r\n"
      "<u:SetBinaryStateResponse xmlns:u=\"urn:Belkin:service:basicevent:1\">\r\n"
      "<BinaryState>1</BinaryState>\r\n"
      "</u:SetBinaryStateResponse>\r\n"
      "</s:Body> </s:Envelope>";

  HTTP.send(200, "text/xml", body.c_str());
        
  Serial.print("Sending :");
  Serial.println(body);
}

void turnOnDevice() {
  if(!opened){
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
  }
}

void offMsg(){
  String body = 
      "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>\r\n"
      "<u:SetBinaryStateResponse xmlns:u=\"urn:Belkin:service:basicevent:1\">\r\n"
      "<BinaryState>0</BinaryState>\r\n"
      "</u:SetBinaryStateResponse>\r\n"
      "</s:Body> </s:Envelope>";

  HTTP.send(200, "text/xml", body.c_str());
        
  Serial.print("Sending :");
  Serial.println(body);
}

void turnOffDevice() {
  colorWipe(none, 10);
  close_book();
}

void sendDeviceState() {
  
  String body = 
      "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>\r\n"
      "<u:GetBinaryStateResponse xmlns:u=\"urn:Belkin:service:basicevent:1\">\r\n"
      "<BinaryState>";
      
  body += (opened ? "1" : "0");
  
  body += "</BinaryState>\r\n"
      "</u:GetBinaryStateResponse>\r\n"
      "</s:Body> </s:Envelope>\r\n";
 
   HTTP.send(200, "text/xml", body.c_str());
}

void respondToData(){
  HTTP.handleClient();
  delay(1);
  
  // if there's data available, read a packet
  // check if the WiFi and UDP connections were successful
  if(wifiConnected){
    if(udpConnected){    
      // if there’s data available, read a packet
      int packetSize = UDP.parsePacket();
      
      if(packetSize) {
        //Serial.println("");
        //Serial.print("Received packet of size ");
        //Serial.println(packetSize);
        //Serial.print("From ");
        IPAddress remote = UDP.remoteIP();
        
        for (int i =0; i < 4; i++) {
          Serial.print(remote[i], DEC);
          if (i < 3) {
            Serial.print(".");
          }
        }
        
        Serial.print(", port ");
        Serial.println(UDP.remotePort());
        
        int len = UDP.read(packetBuffer, 255);
        
        if (len > 0) {
            packetBuffer[len] = 0;
        }

        String request = packetBuffer;
        //Serial.println("Request:");
        //Serial.println(request);
        
        // Issue https://github.com/kakopappa/arduino-esp8266-alexa-wemo-switch/issues/24 fix
        if(request.indexOf("M-SEARCH") >= 0) {
            // Issue https://github.com/kakopappa/arduino-esp8266-alexa-multiple-wemo-switch/issues/22 fix
            //if(request.indexOf("urn:Belkin:device:**") > 0) {
             if((request.indexOf("urn:Belkin:device:**") > 0) || (request.indexOf("ssdp:all") > 0) || (request.indexOf("upnp:rootdevice") > 0)) {
                Serial.println("Responding to search request ...");
                respondToSearch();
             }
        }
      } else{
        simple_open_close();
      }
      
      delay(10);
    }
  } else {
      Serial.println("Cannot connect to Wifi");
  }
}

//------------------------------------------------------------------------------------------------------------------------//

//Neopixel Matrix Functions

// Fill the dots one after the other with a color
bool colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<matrix.height(); i++) {
    respondToData();
    if(respond_to_button() || !opened){
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
    respondToData();
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
    respondToData();
    if(respond_to_button() || !opened){
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
    respondToData();
    if(respond_to_button() || !opened){
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
    respondToData();
    if(respond_to_button() || !opened){
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

//------------------------------------------------------------------------------------------------------------------------//

//Sound Functions

// Non-blocking I2S write for left and right 16-bit PCM
bool ICACHE_FLASH_ATTR i2s_write_lr_nb(int16_t left, int16_t right){
  int sample = right & 0xFFFF;
  sample = sample << 16;
  sample |= left & 0xFFFF;
  return i2s_write_sample_nb(sample);
}

struct I2S_status_s {
  wavFILE_t wf;
  int16_t buffer[512];
  int bufferlen;
  int buffer_index;
  int playing;
} I2S_WAV;

void wav_stopPlaying()
{
  i2s_end();
  I2S_WAV.playing = false;
  wavClose(&I2S_WAV.wf);
  finishedPlaying = true;
}

bool wav_playing()
{
  return I2S_WAV.playing;
}

void wav_setup()
{
  Serial.println(F("wav_setup"));
  I2S_WAV.bufferlen = -1;
  I2S_WAV.buffer_index = 0;
  I2S_WAV.playing = false;
}

bool wav_loop()
{
  bool i2s_full = false;
  int rc;
  while (I2S_WAV.playing && !i2s_full) {
    respondToData();
    if((respond_to_button() || !opened)){
      Serial.println(F("Stop playing"));
      wav_stopPlaying();
      return true;
    }
    while (I2S_WAV.buffer_index < I2S_WAV.bufferlen) {
      
      int16_t pcm = I2S_WAV.buffer[I2S_WAV.buffer_index];
      if (i2s_write_lr_nb(pcm, pcm)) {
        I2S_WAV.buffer_index++;
      }
      else {
       
        i2s_full = true;
        break;
      }
      if ((I2S_WAV.buffer_index & 0x3F) == 0){
        Serial.println("Yielding");
        yield();
      }
    }
    if (i2s_full) break;

    rc = wavRead(&I2S_WAV.wf, I2S_WAV.buffer, sizeof(I2S_WAV.buffer));
    if (rc > 0) {
      //Serial.printf("wavRead %d\r\n", rc);
      I2S_WAV.bufferlen = rc / sizeof(I2S_WAV.buffer[0]);
      I2S_WAV.buffer_index = 0;
    }
    else {
      Serial.println(F("Stop playing"));
      wav_stopPlaying();
      break;
    }
  }
  return false;
}

void wav_startPlayingFile(const char *wavfilename)
{
  wavProperties_t wProps;
  int rc;

  Serial.printf("wav_startPlayingFile(%s)\r\n", wavfilename);
  i2s_begin();
  rc = wavOpen(wavfilename, &I2S_WAV.wf, &wProps);
  Serial.printf("wavopen %d\r\n", rc);
  if (rc != 0) {
    Serial.println("wavopen failed");
    return;
  }
  Serial.printf("audioFormat %d\r\n", wProps.audioFormat);
  Serial.printf("numChannels %d\r\n", wProps.numChannels);
  Serial.printf("sampleRate %d\r\n", wProps.sampleRate);
  Serial.printf("byteRate %d\r\n", wProps.byteRate);
  Serial.printf("blockAlign %d\r\n", wProps.blockAlign);
  Serial.printf("bitsPerSample %d\r\n", wProps.bitsPerSample);

  i2s_set_rate(wProps.sampleRate);

  I2S_WAV.bufferlen = -1;
  I2S_WAV.buffer_index = 0;
  I2S_WAV.playing = true;
  wav_loop();
}

void showDir(void)
{
  wavFILE_t wFile;
  wavProperties_t wProps;
  int rc;

  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    Serial.println(dir.fileName());
    rc = wavOpen(dir.fileName().c_str(), &wFile, &wProps);
    if (rc == 0) {
      Serial.printf("  audioFormat %d\r\n", wProps.audioFormat);
      Serial.printf("  numChannels %d\r\n", wProps.numChannels);
      Serial.printf("  sampleRate %d\r\n", wProps.sampleRate);
      Serial.printf("  byteRate %d\r\n", wProps.byteRate);
      Serial.printf("  blockAlign %d\r\n", wProps.blockAlign);
      Serial.printf("  bitsPerSample %d\r\n", wProps.bitsPerSample);
      Serial.println();
      wavClose(&wFile);
    }
  }
}


