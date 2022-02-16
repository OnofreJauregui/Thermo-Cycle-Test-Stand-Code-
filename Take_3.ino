//Initialize
int C1 = 11; //Pin used to open/close relay for air solenoid.
int C2 = 12; //Pin used to open/close relay for air solenoid.
int i = 0;

float btn_new; 
float btn_pin = 5; // Pin used to initiate the program. 

float m1;
float b1;
float m2; 
float b2 ;

int analogPin1=1; // Pins used to read thermocouples. 
int analogPin2=2; // Pins used to read thermocouples. 

//Thermocouple Amplifier calibration 



const int chipSelect = 4; 


// Arduino Library
#include <SD.h>
#include <SPI.h>
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>



// Define LCD Display
#if defined (__AVR_ATmega32U4__) || defined(ARDUINO_SAMD_FEATHER_M0) || defined (__AVR_ATmega328P__) || \
    defined(ARDUINO_SAMD_ZERO) || defined(__SAMD51__) || defined(__SAM3X8E__) || defined(ARDUINO_NRF52840_FEATHER)
   #define STMPE_CS 6
   #define TFT_CS   9
   #define TFT_DC   10
   // #define SD_CS 5    // Leave like this unless you use the screen for the SD card 
#endif
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


void setup() {

  delay(2000);
  Serial.begin(9600); // All the code with Serial. is used to view what the program is doing on the serial monitor ( top right )
  
  tft.begin();
  
  digitalWrite(C1,HIGH);
  digitalWrite(C2,HIGH);
  
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  
  pinMode(btn_pin, INPUT);
  pinMode(1,INPUT_PULLUP);
  pinMode(2,INPUT_PULLUP);
  
 // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

void loop() {
  testText();
  btn_new=analogRead(btn_pin);
      Serial.println(btn_new);
    delay(100);
    
     if(btn_new > 1000){       // Choose the number of cycles you want.
       while (i < 5 ) {
    
         // make a string for assembling the data to log:
        String dataString1 = "";
        String dataString2 = "";
        String cycle = "";
       

  // read three sensors and append to the string:
   float sensor1 = analogRead(analogPin1); // Reads data from pin 1
   float sensor2 = analogRead(analogPin2); // Reads data from pin 2
   // Testing the Code 
     Serial.print("S = ");
     Serial.print(sensor1);
     Serial.print(","); 
     Serial.println(sensor2);

 m1=0.00323529; //(3.3/1020);           // Linear Interpolation to go from bit to voltage.
 b1=-0.00970588; //(3.3-(3375.9/1020));

 m2=0.003230543; //(5/1023);
 b2= -0.004845; //3.3-(3375.9/1021.5));

   Serial.print(m1,5);  //// Trial Stuff ////
   Serial.print(","); 
   Serial.println(b1,5);
   Serial.print(m2,5);  //// Trial Stuff ////
   Serial.print(","); 
   Serial.println(b2,5);
     
     
   float voltage1 = (sensor1*m1+b1);  //Voltage Output from the Amplifier.
   float voltage2 = (sensor2*m2+b2); //Voltage Output from the Amplifier.

   Serial.print("V = ");
   Serial.print(voltage1,5);
   Serial.print(","); 
   Serial.println(voltage2,5);
   
     float TempC1= (voltage1-1.25)/.005; // Equation to go from voltage to temperature.
     float TempC2= (voltage2-1.25)/.005;
     // Serial.print("The temp in C is ");
     //Serial.print(TempC1);
   
   float TempF1= ((TempC1) *(1.8))+32; //Going from Celcius to Fahrenheit
   float TempF2= ((TempC2)*(1.8))+32;
   Serial.print("The temp in F is ");
   Serial.print(TempF1);
   Serial.print(","); 
   Serial.println(TempF2);
   
   dataString1= String(TempF1); 
   dataString2= String(TempF2);
   cycle=String(i);
   
   
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it: // Coping the temperatures and cycles to SD card.
  if (dataFile) {
    dataFile.print(i);
    dataFile.print(",");
    dataFile.print(dataString1);
    dataFile.print(",");
    dataFile.println(dataString2);
    dataFile.close();
    // print to the serial port too:
    //Serial.println(dataString1);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }  
       digitalWrite(C2,LOW); 
     delay(1000);              // determine the time you want PRX summerged 
       digitalWrite(C2,HIGH);
     delay(1000);
       digitalWrite(C1,HIGH);
     delay(1000);
      //
       digitalWrite(C2,LOW);
     delay(1000);
       digitalWrite(C2,HIGH);
     delay(1000);
       digitalWrite(C1,LOW);
     delay(1000);
        // Serial.print("i = ");
        //Serial.print(i);
        //Serial.print(",");
        //Serial.print(dataString1);
       // Serial.print(",");
        //Serial.println(dataString2);

        
       
          testText();
          delay(1000);
          Serial.print("i=");
          Serial.println(i+1);
        i++;
      }
    i=0;
 }
}
unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  tft.fillScreen(ILI9341_RED);
  yield();
  tft.fillScreen(ILI9341_GREEN);
  yield();
  tft.fillScreen(ILI9341_BLUE);
  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);

  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(7);
  tft.println("Cycle");
  tft.println(i+1);

  return micros() - start;
}
