#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, 6, NEO_GRB + NEO_KHZ800);
SoftwareSerial BTSerial(2, 3);

int redTemp = 0;
int blueTemp = 0;
int greenTemp = 0;
char modeA = 0; //초음파센서
char modeB = 0; //사운드센서
  
const int trigPin = 7;  // Trig(송신부)를 7번으로 변수 선언
const int echoPin = 10; // Echo(수신부)를 6번으로 변수 선언

void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0 ; i<strip.numPixels() ; i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait); 
  }
}

void controlUltrawave() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  long distance = (duration*340)/2/10000;

  if(distance<60) {
    colorWipe(strip.Color(redTemp, greenTemp, blueTemp),20);
  } else {
    colorWipe(strip.Color(0, 0, 0),20);
  }
  delay(200);
}

void controlSOS() {
  for(int i=0; i<3; i++) {
    colorWipe(strip.Color(255,255,255), 0);
    delay(150);
    colorWipe(strip.Color(0,0,0), 0);
    delay(100);
  }
  delay(200);
  for(int j=0; j<3; j++) {
    colorWipe(strip.Color(255,255,255), 0);
    delay(300);
    colorWipe(strip.Color(0,0,0), 0);
    delay(300);
  }
  for(int k=0; k<3; k++) {
    colorWipe(strip.Color(255,255,255), 0);
    delay(150);
    colorWipe(strip.Color(0,0,0), 0);
    delay(100);
  }
  delay(2000);
}

void controlSound(){
  int statusSensor = analogRead(A2);
  int mapping = map(statusSensor, 0, 400, 0, 23);
  if(mapping == 0) {
    for(int i=0; i<24; i++) {
      strip.setPixelColor(i, 0, 0, 0);
      strip.show();
    }
  } else {
    for(int j=0; j< mapping; j++)
    {
      strip.setPixelColor(j, redTemp, greenTemp, blueTemp);
      strip.show();
    }
  }
}

void setup()
{
  BTSerial.begin(9600);
  pinMode(A2, INPUT); // 사운드 센서
  pinMode(5, INPUT_PULLUP);//푸시 버튼
  pinMode(trigPin, OUTPUT);//7번 핀
  pinMode(echoPin, INPUT);//10번 핀
  strip.begin();
  strip.setBrightness(255);
  strip.show();
  colorWipe(strip.Color(255, 0, 0),50);
  colorWipe(strip.Color(0, 255, 0),50);
  colorWipe(strip.Color(0, 0, 255),50);
  colorWipe(strip.Color(0, 0, 0),0);
}

void loop()
{
  char data;
  if (BTSerial.available()) {
    data= BTSerial.read();
  if (BTSerial.find("r")) {
    redTemp = BTSerial.parseInt();
  }
  if (BTSerial.find("g")) {
    greenTemp = BTSerial.parseInt();
  }
  if (BTSerial.find("b")) {
    blueTemp = BTSerial.parseInt();
    colorWipe(strip.Color(redTemp, greenTemp, blueTemp),20);
  }
  if (data=='q') {
      modeA= 1;
      modeB = 0;
  }
  if (data=='a') {
      modeA = 0;
      modeB = 1;
  }
  if(data=='w') {
    modeA = 0;
    modeB = 0;
  }
}
if(digitalRead(5)==LOW) {
  modeA = 0;
  modeB = 0;
  controlSOS();
}
if(modeB == 1){
  controlUltrawave();
  }
if(modeA == 1){
  controlSound();
}
}
