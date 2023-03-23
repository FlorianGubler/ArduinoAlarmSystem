//Display
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int color_defaultR = 0;
const int color_defaultG = 0;
const int color_defaultB = 255;

const int color_errorR = 255;
const int color_errorG = 0;
const int color_errorB = 0;

const int color_warnR = 255;
const int color_warnG = 165;
const int color_warnB = 0;

const int color_validR = 0;
const int color_validG = 255;
const int color_validB = 0;

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

void setup()
{  
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
    lcd.begin(16, 2);
    lcd.setRGB(color_defaultR, color_defaultG, color_defaultB);
    printToDisplay("READY", 1, true);
}

void loop()
{
   if(client.available()){
        String res = "";
        while (client.available()) {          // loop while the client's available
          char c = client.read();
          res += c;
        }
        String firstline = res.substring(0, res.indexOf('\n'));
        String ResponseCode = firstline.substring(res.indexOf(' ') + 1, firstline.length() - 2); // RM Line Space
   }
}

void verifyRFIDCard(String rfid_UID){
    lcd.setRGB(color_defaultR, color_defaultG, color_defaultB);
    printToDisplay("WAITING...", 1, true);
    Serial.println("Verifying RFID Card " + rfid_UID);
    // if you get a connection, report back via serial:
    
}

void http(String method, String path, char[] server, int port){
  if (client.connect(server, port)) {
      client.println(method + " " + path + " HTTP/1.1");
      client.println("Host: " + String(server));
      client.println("Connection: close");
      client.println();
    } else{
      Serial.println("Couldn't connect to server: " + String(server) + ":" + port);
    }
}

void printToDisplay(String inp, int line, boolean clear){
  if(clear){
      lcd.clear();
  }
  lcd.print(inp);
  lcd.setCursor(0, line);
}
