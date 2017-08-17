//*****************************************************************************
//
// ExoYunGettingStarted 
//   This example is can be used with the "Arduino Yun Compatible"
//   device available in portals.exosite.com.  It uses provisioning
//   to activate with Exosite with it's MAC Address instead of using a hard-coded CIK.
//   By default it does two things, one is reads the A0 analog value and sends
//   this to a dataport called A0.  It also makes a read request for a dataport 
//   called D13 and sets the Digital pin 13 to 1 or 0 based on that value, which  
//   controls the LED on the YUN board.  
//   
//   Developers can feel free to create new dataports to read and write to on the 
//   Exosite platform for their device's client and also then add the code below to 
//   interact with those dataports.
//
//   Tested with Arduino Yun
//
// Copyright (c) 2015 Exosite LLC.  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

//  * Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of Exosite LLC nor the names of its contributors may
//    be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
//*****************************************************************************
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Bridge.h>
#include <YunClient.h>
#include <Exosite.h>
#include <Process.h>
//#include <Console.h>

/*==============================================================================
* LED Configuration
*=============================================================================*/
#define DATA_PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, DATA_PIN, NEO_GRB + NEO_KHZ800);
/*==============================================================================
* End of LED Configuration
*=============================================================================*/



/*==============================================================================
* Configuration Variables
*
* Change these variables to your own settings.
*=============================================================================*/
// Use these variables to customize what datasources are read and written to.
const String readString = "b&g&r&l&t&e&p";
String returnString;

// Number of Errors before we try a reprovision.
const unsigned char reprovisionAfter = 3;

const int REPORT_INTERVAL = 1000; //milliseconds period for reporting to Exosite.com


#define productId "q1s1sg51mja4g0wwo"
#define cik "KZI2b9Qo3FcFDCU6OI50gf5HKHbnbdxq21fsFVQP"
/*==============================================================================
* End of Configuration Variables
*=============================================================================*/
YunClient client;
Exosite exosite(&client);

unsigned char errorCount = reprovisionAfter;  // Force Provision On First Loop
char macString[18];  // Used to store a formatted version of the MAC Address

String tempString;

int analog0Pin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED

int exosite_comm_errors = 0;
int comm_errors = 0;
long unsigned int prevSendTime = 0;

//========================================================================
uint32_t currentColor = strip.Color(0, 0, 0);
int setupDelay = 600;

//disco helper



int8_t toggle, passwordStatus;
uint16_t brightness, extraValue, redness, greenness, blueness;
//========================================================================
void getYunMAC(char* MACptr, byte bufSize) {
  Process p;    // Create a process and call it "p"
  p.runShellCommand("/sbin/ifconfig -a | /bin/grep HWaddr |/bin/grep wlan0 | /usr/bin/awk '{print $5}'");

  for(int i = 0; i < (bufSize-1) && p.available() > 0; i++) {
    MACptr[i] = p.read();
    MACptr[i+1] = 0;
  }
}

/*==============================================================================
* Basic Controlls
*=============================================================================*/
void setColor(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c); 
    }
  strip.show();
}

void fade(uint32_t b, int wait) {
  strip.setBrightness(b);
  strip.show();
  delay(wait);
}

void advancedFade(uint8_t b, uint8_t wait) {
  uint32_t x [255];
  for(uint16_t i=0; i<strip.numPixels(); i++)
   x[i] =  strip.getPixelColor(i);
  for(uint16_t i=0; i<strip.numPixels(); i++)
    Serial.println(x[i]);
  strip.setBrightness(b);
  for(uint16_t i=0; i<strip.numPixels(); i++)
    strip.setPixelColor(i, x[i]);

  strip.show();
  delay(wait);  
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


/*==============================================================================
* Macros
*=============================================================================*/
void startupSequence(uint8_t mark) {
Serial.print("initializing startupSequence...");
Serial.println(mark);
  strip.setBrightness(255);
  if(mark==1) {
    wipeFade(strip.Color(0, 255, 255), 255, 15);
  }
  if(mark==2) {
    strip.setBrightness(50);
    delay(setupDelay/2);
    rainbow(5, 0);

  }
  if(mark==3) {
    setColor(strip.Color(0, 0, 0));
    delay(setupDelay);
    setColor(strip.Color(255, 0, 0));
    delay(setupDelay);
    setColor(strip.Color(0, 0, 0));
    delay(setupDelay);
    setColor(strip.Color(255, 120, 0));
    delay(setupDelay);
    setColor(strip.Color(0, 0, 0));
    delay(setupDelay);
    setColor(strip.Color(255, 255, 0));
    delay(setupDelay);
    setColor(strip.Color(0, 0, 0));
    delay(setupDelay);
    setColor(strip.Color(0, 255, 0));
    delay(setupDelay*4);
    setColor(strip.Color(0, 0, 0));
    delay(setupDelay*4);
  }
  Serial.print("ending startupSequence...");
  Serial.println(mark);
}

void CMY(uint8_t b) {
Serial.println("initializing CMY()");
  for(int i=0; i<3; i++) {
    setColor(strip.Color(0, 0, 0));
    strip.setBrightness(1);
    strip.show();
    if(i%3==0)
    {
      setColor(strip.Color(0, 255, 255));
    }
    if(i%3==1)
    {
      setColor(strip.Color(255, 0, 255));
    }
    if(i%3==2)
    {
      setColor(strip.Color(255, 255, 0));
    }
      
    delay(30);
    //fade up
    for(int j=1; j<=50; j++)
      fade(j, b);
    for(int j=50; j>=0; j--)
      fade(j, b);
  }
Serial.println("ending CMY()");
}
/*
void alarm() {
  strip.setBrightness(255);
  setColor(strip.Color(255, 0, 0));
  delay(200);
  strip.setBrightness(0);
  strip.show();
  delay(200);
}
*/

void pulse(uint32_t c, uint16_t b, int wait) {
  strip.setBrightness(1);
    setColor(c);
    for(int i=1; i<=b; i++)
      fade(i, wait);
    for(int i=b; i>=1; i--)
      fade(i, wait);
    delay(30);
}

void randomPulse(int num, uint16_t b, int wait) {
  for(int i=0; i<num; i++) {
  strip.setBrightness(1);
    setColor(strip.Color(random(255), random(255), random(255)));
    for(int j=1; j<=b; j++)
      fade(j, wait);
    for(int j=b; j>=0; j--)
      fade(j, wait);
    delay(30);
  }
}

void wipeFade(uint32_t c, uint16_t b, int wait) {
  strip.setBrightness(b);
    colorWipe(c, 10);
    for(int i=b; i>=0; i--)
    {
      fade(i, wait);
    }
}

void setRainbow() {

  double modifier = 255.0/(double) strip.numPixels();
  for(int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i*modifier)));
    }
  strip.show();
}

void rainbow(int num, uint8_t wait) {
  uint16_t i, j;
  for(int k=0; k<num; k++) {
    for(j=0; j<256; j++) {
      for(i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i+j) & 255));
      }
      strip.show();
      delay(wait);
    }
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void guide(uint32_t c, uint8_t wait) {
  for(int i=0; i<strip.numPixels()+5; i++) {
    strip.setPixelColor(i, c);
    strip.setPixelColor(i-5, strip.Color(0, 0, 0));
    strip.show();
    delay(wait);
  }
}

void guideBounce(uint32_t c, uint8_t wait) {
  for(int i=0; i<strip.numPixels()+5; i++) {
    strip.setPixelColor(i, c);
    strip.setPixelColor(i-5, strip.Color(0, 0, 0));
    strip.show();
    delay(wait);
  }
  for(int i=strip.numPixels(); i>0-5; i--) {
    strip.setPixelColor(i, c);
    strip.setPixelColor(i+5, strip.Color(0, 0, 0));
    strip.show();
    delay(wait);
  }
}

void guideStack(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
    delay(wait);
  }
  for(int i=0; i<strip.numPixels(); i+=5) {
    for(int j=0; j<strip.numPixels()-(i); j++) {
      strip.setPixelColor(j, c);
      strip.setPixelColor(j-5, strip.Color(0, 0, 0));
      strip.show();
      delay(wait);
    }
  }
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<1; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// twinkle random number of pixels
void disco(int num, uint32_t bg, int wait) {
  // set background
  setColor(bg);
  uint32_t discoPixelNumber [num];
  // for each num
  for (int i=0; i<4; i++) {
    for (int j=0; j<num; j++) {
      strip.setPixelColor(discoPixelNumber[j], strip.Color(0, 0, 0));
      discoPixelNumber[j] = random(strip.numPixels());
      strip.setPixelColor(discoPixelNumber[j], strip.Color(random(255), random(255), random(255)));
      strip.show();
      delay(wait);
    }
  }
}

/*
void twinkle(int num, int rounds, int wait) {

  int toggleMod = 0, brightnessMod = 0, pixelMod = 0;
  int randomNum = 0, randomMod = 30;
  uint32_t twinklePixelNumber [num];
  setColor(strip.Color(0, 0, 0));
  delay(2000);

  for (int i=0; i<num; i++) {
    twinklePixelNumber[i] = random(strip.numPixels());
    strip.setPixelColor(twinklePixelNumber[i], strip.Color(255, 255, 255));
    strip.show();
  }
  for (int i=0; i<rounds; i++) {
    for (int j=0; j<num; j++) {
      randomNum = random(randomMod+toggleMod);
      if(randomNum<3)
      {
        toggleMod=0;
        if(randomNum==0)
          strip.setPixelColor(twinklePixelNumber[j], strip.Color(0, 0, 0));
        else
          strip.setPixelColor(twinklePixelNumber[j], strip.Color(255, 255, 255));
        strip.show();
      }
      else {
        toggleMod--;
      }
        
      randomNum = random(randomMod+pixelMod);
      if(randomNum<2)
      {
        strip.setPixelColor(twinklePixelNumber[j], strip.Color(0, 0, 0));
        twinklePixelNumber[j] = random(strip.numPixels());
        strip.setPixelColor(twinklePixelNumber[j], strip.Color(255, 255, 255));
        strip.show();
        pixelMod=0;
      }
      else {
        pixelMod--;
      }
      
      randomNum = random(randomMod+brightnessMod);
      if(randomNum<2)
      {
        strip.setBrightness(strip.getBrightness()+10);
        strip.show();
        brightnessMod=0;
      }
      else if(randomNum<4)
      {
        strip.setBrightness(strip.getBrightness()-10);
        strip.show();
        brightnessMod=0;
      }
      else {
        brightnessMod--;
      }
      delay(wait);
      Serial.print("Toggle Mod");
      Serial.print(toggleMod);
      Serial.print("...");
      Serial.print("Brightness Mod");
      Serial.print(brightnessMod);
      Serial.print("...");
      Serial.print("pixelMod");
      Serial.print(pixelMod);
      Serial.println();
    }
  }
}
*/
/*
void cherries(int wait) {
  for(int i=0; i<strip.numPixels()+19; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
    strip.setPixelColor(i-5, strip.Color(0, 0, 0));
    strip.setPixelColor(i-12, strip.Color(0, 0, 255));
    strip.setPixelColor(i-17, strip.Color(0, 0, 0));
    strip.show();
    delay(wait);
  }
  for(int i=strip.numPixels(); i>-19; i--) {
    strip.setPixelColor(i, strip.Color(0, 0, 255));
    strip.setPixelColor(i+5, strip.Color(0, 0, 0));
    strip.setPixelColor(i+12, strip.Color(255, 0, 0));
    strip.setPixelColor(i+17, strip.Color(0, 0, 0));
    strip.show();
    delay(wait);
  }
}
*/


/*==============================================================================
* setup
*
* Arduino setup function.
*=============================================================================*/
void setup() {
  
  
  Serial.begin(115200);
  Serial.println(F("Boot"));
  delay(1000);

  strip.begin();
  strip.show();

  startupSequence(1);
  
  Serial.println("\r\n\r\nStarting Yun WiFi...");
  pinMode(ledPin, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);  // Led on pin 13 turns on when the bridge is ready

  getYunMAC(macString, 18);

  startupSequence(2);

  // Print Some Useful Info
  Serial.print(F("MAC Address: "));
  Serial.println(macString);
  
  Serial.print(F("Exo Arduino Lib Ver: "));
  Serial.println(ACTIVATOR_VERSION);

  exosite.setDomain(productId".devmode-m2.exosite.io");

  if (exosite.provision(productId, productId, "90:A2:DA:F3:1E:1E")) {
        Serial.println("setup: Provision Succeeded");
  } else {
        Serial.println("setup: Provision Check Done");
  }
  startupSequence(3);
}

/*==============================================================================
* loop
*
* Arduino loop function.
*=============================================================================*/
void loop() {
/*
if(startup) {
  startupSequence();
  startup = false;
}
*/
 int8_t index = 0;
 int8_t lastIndex = -1;
 float h,t;
  
 if (prevSendTime + REPORT_INTERVAL < millis() || millis() < prevSendTime || isnan(h) || isnan(t) ) {
    int uptime = millis()/1000;

    //Loop marker
    Serial.println("==========================");
    if (exosite.read(readString, returnString)) {

      Serial.println("~~~~~~~");
      Serial.println(returnString);
      Serial.println("~~~~~~~");
      exosite_comm_errors = 0;
      comm_errors = 0;
      prevSendTime = millis();
      //Handle Response Message
      for(;;) {
 
          Serial.println("======");
          index = returnString.indexOf("p=");
          if(index!=-1){
            index+=2;
            passwordStatus = returnString.substring(index, returnString.indexOf("&", index)).toInt();
            Serial.print("passwordStatus: ");
            Serial.println(passwordStatus);
          }

          index = returnString.indexOf("e=");
          if(index!=-1){
            index+=2;
            extraValue = returnString.substring(index, returnString.indexOf("&", index)).toInt();
            Serial.print("extraValue: ");
            Serial.println(extraValue);
          }
  
          index = returnString.indexOf("t=");
          if(index!=-1){
            index+=2;
            toggle = returnString.substring(index, returnString.indexOf("&", index)).toInt();
            Serial.print("toggle: ");
            Serial.println(toggle);
          }
  
          index = returnString.indexOf("l=");
          if(index!=-1){
            index+=2;
            brightness = returnString.substring(index, returnString.indexOf("&", index)).toInt();
            Serial.print("brightness: ");
            Serial.println(brightness);
          }
  
          index = returnString.indexOf("r=");
          if(index!=-1){
            index+=2;
            redness = returnString.substring(index, returnString.indexOf("&", index)).toInt();
            Serial.print("redness: ");
            Serial.println(redness);
          }
  
          index = returnString.indexOf("g=");
          if(index!=-1){
            index+=2;
            greenness = returnString.substring(index, returnString.indexOf("&", index)).toInt();
            Serial.print("greenness: ");
            Serial.println(greenness);
          }
  
          index = returnString.indexOf("b=");
          if(index!=-1){
            index+=2;
            blueness = returnString.substring(index, returnString.indexOf("&", index)).toInt();
            Serial.print("blueness: ");
            Serial.println(blueness);
          }


        //Set's LED's based on data from Exosite
          if(extraValue>0)
          {
            
            currentColor == strip.Color(redness, greenness, blueness);
            strip.setBrightness(brightness);
            /*
            if(extraValue==1)
              alarm();
            else if(extraValue==911)
              cherries(10);
              */
            if(extraValue==1337)
              rainbowCycle(10);
              
            else if(extraValue==1338)
              rainbow(30, 0);
            
            else if(extraValue==1970)
              disco(40, currentColor, 90);
              /*
            else if(extraValue==10010)
              twinkle(10, 40, 100);
              */
            else if(extraValue==12321)
              pulse(currentColor, brightness, 50);
            else if(extraValue==12340)
              wipeFade(currentColor, brightness, 20);
            else if(extraValue==12821)
              randomPulse(10, brightness, 50);
            else if(extraValue==31325)
              CMY(brightness);
            else if(extraValue==45654)
              guideBounce(currentColor, 20);
            else if(extraValue==45678)
              guide(currentColor, 20);
            else if(extraValue==45918)
              guideStack(currentColor, 20);
              
          }
          else
          {
            if(toggle==1)
            {
              currentColor = strip.Color(redness, greenness, blueness);
              strip.setBrightness(brightness);
              setColor(currentColor);
            }
            else
            {
              strip.setBrightness(0);
              strip.show();
            }
          }
        
            
          Serial.println("======");
          
          if(index != 0) {
            break;
  
          } else{
            Serial.println(F("No Index"));
            break;
          }
        }
      

    } else {
      //Serial.println("HANDLER: network or platform api request issue");
            
      exosite_comm_errors++;
      comm_errors++;

    }


    if (exosite_comm_errors == 5) {

      strip.setBrightness(10);
      guide(strip.Color(255, 0, 0), 20);
      
      Serial.print("exo: Check Provisioning - ");
      Serial.print("Murano Product ID: ");
      Serial.print(productId);
      Serial.print(F(", Device Identifier (MAC Address): "));
      Serial.println(macString);

      if (exosite.provision(productId, productId, macString)) {
        Serial.println("exo: Activate: New Activation");
      } else {
        Serial.println("exo: Activate: No New Activation");

      }
      exosite_comm_errors = 0;
    }
    if (comm_errors == 20) {
      delay(2000);
      // If not able to communicate, test restarting WiFi Router Connection
      Serial.println("wifi: Setup WiFi Network Connection Again - ");
      comm_errors = 0;
    }
  }
  delay(0);
}


