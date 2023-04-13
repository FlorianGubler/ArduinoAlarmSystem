//Display
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int color_offR = 255;
const int color_offG = 255;
const int color_offB = 255;

const int color_defaultR = 0;
const int color_defaultG = 0;
const int color_defaultB = 255;

const int color_redR = 255;
const int color_redG = 0;
const int color_redB = 0;

const int color_yellowR = 255;
const int color_yellowG = 165;
const int color_yellowB = 0;

const int color_greenR = 0;
const int color_greenG = 255;
const int color_greenB = 0;

//LCD Light
int lcdLightPort = 1;

//Wifi
#include <WiFiNINA.h>

#define SECRET_SSID "iPhone"
#define SECRET_PASS "123456789"

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
char server[] = "172.20.10.4";    // IP Adress of API Host
int port = 8080;          //Port of API
WiFiClient client;

//Alarm sensors
int alarmSensors[] = {2};

//Alarm Button
int alarmButtonPort = 0;
byte entprellzeit = 200;                      // Entprellzeit des Tasters. ACHTUNG BYTE nur bis 255 ms !!!
unsigned long alarmButtonClickedLast = 0;     // Variable für den Timer des Tasters zum entprellen

//Alarm State
boolean alarmState = false;
boolean alarmActive = false;
int alarmBlinkCount = 20;

void setup()
{  
    //Setup Display
    lcd.begin(16, 2);
    lcd.setRGB(color_offR, color_offG, color_offB);
    printToDisplay("SETUP...", 1, true);
        
    //Set LcdLight Mode
    pinMode(lcdLightPort, OUTPUT);

    //Set Alarm Button Mode
    pinMode(alarmButtonPort, INPUT);

    //Attach Interrupt to alarm button
    attachInterrupt(digitalPinToInterrupt(alarmButtonPort), switchAlarmState, FALLING);
    
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE) {
      Serial.println("Communication with WiFi module failed!");
      // don't continue
      while (true);
    }
  
    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
      Serial.println("Please upgrade the firmware");
    }
  
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(ssid, pass);
      // wait 10 seconds for connection:
      delay(10000);
    }
    Serial.println("Connected to wifi");

    //Display
    lcd.setRGB(color_defaultR, color_defaultG, color_defaultB);
    printToDisplay("READY", 1, true);
}

void loop() {
  String res = receiveHttp();
  if(res != ""){
    String firstline = res.substring(0, res.indexOf('\n'));
    String ResponseCode = firstline.substring(res.indexOf(' ') + 1, firstline.length() - 2); // RM Line Space
  }
  checkAlarmSensors();
}

//Util Methods
void http(String method, String path, char server, int port){
  if (client.connect(server, port)) {
      client.println(method + " " + path + " HTTP/1.1");
      client.println("Host: " + String(server));
      client.println("Connection: close");
      client.println();
    } else{
      Serial.println("Couldn't connect to server: " + String(server) + ":" + port);
    }
}

String receiveHttp(){
  String res = "";
  if(client.available()){
        while (client.available()) {          // loop while the client's available
          char c = client.read();
          res += c;
        }
   }
   return res;
}

void printToDisplay(String inp, int line, boolean clear){
  if(clear){
      lcd.clear();
  }
  lcd.print(inp);
  lcd.setCursor(0, line);
}

void turnLCDLight(boolean on){
  digitalWrite(lcdLightPort, on);
}

//Functionality
void switchAlarmState(){
  //If current time is later than last time clicked plus entprellzeit
  if ((millis() - alarmButtonClickedLast) <= entprellzeit){
    return;
  }
  //Set alarmButtonClickedLast
  alarmButtonClickedLast = millis();
  //Update state
  alarmState = !alarmState;
  //Activate LCD light
  turnLCDLight(alarmState);
  String stateStr = "";
  if(alarmState){
    stateStr = "activated";
    lcd.setRGB(color_yellowR, color_yellowG, color_yellowB);
  } else{
    stateStr = "deactivated";
    lcd.setRGB(color_greenR, color_greenG, color_greenB);
  }
  //Log and print to display
  Serial.println("Alarm is now " + stateStr);
  stateStr.toUpperCase();
  printToDisplay(stateStr, 1, true);
  //Send Alarm State to API

}

void checkAlarmSensors(){
  //Alarm activated and alarm not currently active
  if(!alarmState || alarmActive){
    return;
  }
  //Check sensors
  for (int sensor : alarmSensors) { // for each sensor
    if(digitalRead(sensor) == HIGH){
      alarm(sensor);
    }
  }
}

void alarm(int sensor){
  //Set alarm active state
  alarmActive = true;
  //Log
  Serial.println("Alarm on sensor " + sensor);
  //Print to display
  lcd.setRGB(color_redR, color_redG, color_redB);
  printToDisplay("ALARM ON SENSOR " + String(sensor), 1, true);
  //LED Blink
  for(int i = 0; i < alarmBlinkCount; i++){
    digitalWrite(lcdLightPort, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(lcdLightPort, LOW);   // turn the LED off by making the voltage LOW
    delay(100);                       // wait for a second
  }
  //Turn of alarm Active state
  alarmActive = false;
  //Disable alarm
  switchAlarmState();
}
