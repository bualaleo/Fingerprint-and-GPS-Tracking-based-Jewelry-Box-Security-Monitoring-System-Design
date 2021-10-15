#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

int getFingerprintIDez();
SoftwareSerial mySerial(3,4);// tx, rx


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
 void buzzeropen()
 {
  digitalWrite(11,HIGH); //pin buzzer untuk bunyi
  delay(500);
 }

 void buzzerClose()
{ 
  digitalWrite(11,LOW); //pin buzzer untuk mati
}

 void doorOpen()
{
  if(finger.fingerID==1) //jika kode sidik jari #1 yang terdaftar untuk benar
  {
  digitalWrite(8,HIGH); //pin untuk solenoid terbuka
  digitalWrite(9,HIGH);
  digitalWrite(10,LOW);
  delay(3000);
  
 
  }    
}

void doorClose()
{
  digitalWrite(8,LOW); //pin untuk solenoid tertutup
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH);  
  digitalWrite(11,LOW);
}


void setup()  
{
  pinMode(9,OUTPUT);  //memanggil pin yang tadi sudah didefinisikan
  pinMode(10,OUTPUT); 
  pinMode(11,OUTPUT);  
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH);
 
  Serial.begin(9600);
  Serial.println("fingertest");
  finger.begin(57600);
  pinMode(8,OUTPUT); //Pin connected to relay
  
  if (finger.verifyPassword()) //mengecek apakah fingerprint tersebut sudah terhubung
  {
    Serial.println("Found fingerprint sensor!"); //jika iya maka fingerprint tersebut sudah terhubung
  } else 
  {
    Serial.println("Did not find fingerprint sensor :("); //jika tidak maka fingerprint tersebut tidak terhubung
    while (1);
  }
  Serial.println("No valid finger found,waiting for valid finger..."); //menunggu pemilik untuk menempelkan sidik jari yang terdaftar
  }

  
  
  void loop()                     // saat running
{
      
  if(getFingerprintIDez()>=0)
  {
        doorOpen();
        buzzeropen();
        buzzerClose();
        doorClose();
  }
  
    
}
uint8_t getFingerprintID()
{
  uint8_t p = finger.getImage();
  switch (p)
  {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
        default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) 
  {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); //kode sidik jari ditemukan
  Serial.print(" with confidence of "); Serial.println(finger.confidence); //melihat tingkat akurasi
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
