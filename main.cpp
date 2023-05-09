#include <string>
#include <WiFi.h>
#include <HttpClient.h>
#include <typeinfo>
#include "time.h"
using namespace std;

// GPIO
// Switch pins for days of the week
const int satPin = 36;
const int friPin = 37;
const int thursPin = 38;
const int wedsPin = 39;
const int tuesPin = 32;
const int monPin = 33;
const int sunPin = 25;

// Time setup
const char* nptServer = "pool.ntp.org";
const long gmtOffset_sec = -28800;
const int daylightOffset_sec = 3600;

// HttpClient lib setup
char ssid[] = "RREEEE";    // your network SSID (name) 
char pass[] = "cumguac69"; // your network password (use for WPA, or use as key for WEP)
const char kHostname[] = "3.144.121.227"; // Name of the server we want to connect to
const int kPort = 5000; // Port of server we want to connect to
const char kPath[] = "/?var="; // Path 
const int kNetworkTimeout = 30*1000; // Number of milliseconds to wait without receiving any data before we give up
const int kNetworkDelay = 1000; // Number of milliseconds to wait if no data is available before trying again

// Time interval
const int ti_lengthM = 30; // Length of time interval in minutes
const int ti_startTimeH = 18; // Hours of start of time interval (0-23)
const int ti_startTimeM = 42; // Minutes of start of time interval
const int ti_startTimeS = 0; // Seconds of start of time interval

// We will be working completely with seconds here, as conversion to base 12 is annoying
// 1h=60m, 1m=60s, 24h=86400s || 00:00am: 0s | 12:00pm: 43200s | 11:49:59pm: 86399s
const int ti_startTimeSec = (((ti_startTimeH * 60) + ti_startTimeM) * 60) + ti_startTimeS; // Start time (in seconds)
const int ti_totalSec = ti_lengthM * 60; // length of time interval in seconds

int tiflag = 0; // flag for time interval entered

// general function to convert hours and minutes into seconds since day started
int convertToSec(int h, int m, int s) {
  return ((((h * 60) + m) * 60) + s);
}

// check if current time is within time interval

int localTimeH() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return 0;
  }
  int h = timeinfo.tm_hour;
  return h;
}

int localTimeM() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return 0;
  }
  int m = timeinfo.tm_min;
  return m;
}

int localTimeS() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return 0;
  }
  int s = timeinfo.tm_sec;
  return s;
}

int localTimeSec() {
  return convertToSec(localTimeH(), localTimeM(), localTimeS());
}

bool isTimeInterval(int ti_start, int ti_length) {
  if (localTimeSec() >= ti_start && localTimeSec() <= (ti_start + ti_length)) {
    return true;
  } else {
    return false;
  }
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}

void setup() {
  Serial.begin(9600);

  // Set pins
  pinMode(monPin, INPUT);
  pinMode(tuesPin, INPUT);
  pinMode(wedsPin, INPUT);
  pinMode(thursPin, INPUT);
  pinMode(friPin, INPUT);
  pinMode(satPin, INPUT);
  pinMode(sunPin, INPUT);

  // Connect to Wifi Network
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());

  configTime(gmtOffset_sec, daylightOffset_sec, nptServer); // set time

  delay(1000);
}

void loop() {

  WiFiClient c;
  HttpClient http(c);
  
  int day = 0;
  if (isTimeInterval(ti_startTimeSec, ti_totalSec) == true) {
    if (digitalRead(sunPin) == LOW) {
      //Serial.println("Sunday");
      day = 1;
      tiflag++;
    } else if (digitalRead(sunPin) == HIGH) {
      //Serial.println("Closed");
    }
    if (digitalRead(monPin) == LOW) {
      //Serial.println("Monday");
      day = 2;
      tiflag++;
    } else if (digitalRead(monPin) == HIGH) {
      //Serial.println("Closed");
    }
    if (digitalRead(tuesPin) == LOW) {
      //Serial.println("Tuesday");
      day = 3;
      tiflag++;
    } else if (digitalRead(tuesPin) == HIGH) {
      //Serial.println("Closed");
    }
    if (digitalRead(wedsPin) == LOW) {
      //Serial.println("Wednesday");
      day = 4;
      tiflag++;
    } else if (digitalRead(wedsPin) == HIGH) {
      //Serial.println("Closed");
    }
    if (digitalRead(thursPin) == LOW) {
      //Serial.println("Thursday");
      day = 5;
      tiflag++;
    } else if (digitalRead(thursPin) == HIGH) {
      //Serial.println("Closed");
    }
    if (digitalRead(friPin) == LOW) {
      //Serial.println("Friday");
      day = 6;
      tiflag++;
    } else if (digitalRead(friPin) == HIGH) {
      //Serial.println("Closed");
    }
    if (digitalRead(satPin) == LOW) {
      //Serial.println("Saturday");
      day = 7;
      tiflag++;
    }  else if (digitalRead(satPin) == HIGH) {
      //Serial.println("Closed");
    }
  
  }
  char dayStr[2];
  sprintf(dayStr, "%d", day);
  
  
  if (tiflag == 1) {
    char *kPathNew = new char[strlen(kPath) + 2];
    strcpy(kPathNew, kPath);
    strcat(kPathNew, dayStr);
    int err = 0;
    err = http.get(kHostname, kPort, kPathNew);
    if (err == 0) {
      Serial.println(kPathNew);
    } else {
      Serial.println("Connection failed");
    }
    delay(10000);

    http.stop();
    tiflag++;
  } else {
    Serial.println("tiflag: " + String(tiflag));
    delay(10000);
  }
}
