#include <TinyGPS++.h> 
#define BLYNK_PRINT Serial
#define BLYNK_HEARTBEAT 30
#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp32.h> 
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#define BOT_TOKEN "1433088692:AAHqNCLAKVH84tYBieaRL-04LLmXfCb2YE8" //kode token untuk membuat bot di aplikasi telegram
#define CHAT_ID "829873264" //id chat dari bot telegram yang sudah dibuat

// Variables for storing GPS Data
float latitude;
float longitude;
float speed;
float satellites;
String direction;

       
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

char auth[] = "UEyaKBO0kkAewQrLoaoY8-YZqzrT2pzr";  //authentifikasi untuk terhubung ke aplikasi Blynk
char ssid[] = "bulll"; //nama dari hotspot(jaringan seluler)/wifi
char pass[] = "buala119809999"; //password hotspot/wifi

TinyGPSPlus gps;
WidgetMap myMap(V0);
BlynkTimer timer;

unsigned int move_index = 1;

void setup()
{
  // Set console baud rate
  Serial.begin(9600);
  delay(10);
  Serial1.begin(9600, SERIAL_8N1, 14, 27); //rx, tx
  WiFi.begin(ssid, pass);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  //Blynk.begin(auth, ssid, pass,"blynk-cloud.com",8080); //jika menngunakan wifi
  Blynk.begin(auth, ssid, pass); //menggunakan hotspot
}

void loop()
{
  while( Serial1.available() > 0)
  if (gps.encode( Serial1.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring.")); //mengecek gps untuk terdeteksi
    while(true);
  }
   if(gps.speed.kmph()>= 5){  //jika kecepatan gps lebih dari 5km/jam
        
      String Send  = "Long: " + String(gps.location.lng(), 6) + "\n"; //mengirimkan pesan longitude
             Send += "Lat: " + String(gps.location.lat(), 6) + "\n"; // mengirimkan pesan latitude
             Send += "Motion detected!!\n"; //pesan peringatan berupa "motion detected"
    bot.sendMessage(CHAT_ID, Send,"");
   
  }
      Blynk.run();
      timer.run();
}

void displayInfo()
{
  if (gps.location.isValid())
  {
//    Serial.println(gps.location.lat(), 6);
//    Serial.println(gps.location.lng(), 6);
    
    latitude = (gps.location.lat());     //Storing the Lat. and Lon.
    longitude = (gps.location.lng());

    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    
    Blynk.virtualWrite(V1, String(latitude, 6));
    Blynk.virtualWrite(V2, String(longitude, 6));
    
    myMap.location(move_index, latitude, longitude, "GPS_Location");
    
    speed = gps.speed.kmph();               //mengambil data speed
    Serial.print("speed: ");
    Serial.println(speed);
    Blynk.virtualWrite(V3, speed);

    direction = TinyGPSPlus::cardinal(gps.course.value()); //mengambil data direction
    Blynk.virtualWrite(V4, direction);
    
    satellites = gps.satellites.value();    //get number of satellites
    Blynk.virtualWrite(V5, satellites);
  }
  else
  {
    Serial.println(F("INVALID"));
  }
  
}
