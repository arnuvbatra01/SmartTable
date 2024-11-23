#include <Adafruit_Fingerprint.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "pitches.h"

#define I2C_ADDR 0x27 //Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

// Analytics: 

int player1_wins = 0;
int player2_wins = 0;

int p1_score = 0;
int p2_score = 0;

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintIDs [50];
int prints = 0;
LiquidCrystal_I2C lcd(0x27,16,2);


void setup () {

  Serial.begin(9600);
  //startFingerprintSensor();
  lcd.begin();
  lcd.setCursor(0,0);
  dprint("Welcome to Smart");
  lcd.setCursor(0,1);
  dprint("Table Foosball!");
  delay(3000);

  // Fingerprint setup:

finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }

  // Player setup:

  lcd.clear();

    int temp_prints = prints;
    while (1) {
lcd.clear();
lcd.setCursor(0,0);
dprint("Player 1.");
lcd.setCursor (0,1);
dprint("Scan your finger.");
uint8_t fingerprintID = getFingerprintID();
Serial.println("Fingerprint ID: " + String(fingerprintID));
//int fingerprintID = 1;
delay (1000);

if (fingerprintID == FINGERPRINT_NOFINGER){
  fingerprintID = 0;
  continue;
}

else if (fingerprintID == 1 || fingerprintID == 2 || fingerprintID == 3 || fingerprintID == 4){
    fingerprintIDs[prints] = fingerprintID;
    prints++;
    break;
}
    }

    temp_prints = prints;
    lcd.clear();
    while (1) {
lcd.setCursor(0,0);
dprint("Player 2.");
lcd.setCursor (0,1);
dprint("Scan your finger");
uint8_t fingerprintID = getFingerprintID();
Serial.println("Fingerprint ID: " + String(fingerprintID));
//int fingerprintID = 2;
delay (1000);

if (fingerprintID == FINGERPRINT_NOFINGER){
  fingerprintID = 0;
  continue;
}

else if (fingerprintID == 1 || fingerprintID == 2 || fingerprintID == 3 || fingerprintID == 4){
    fingerprintIDs[prints] = fingerprintID;
    prints++;
    break;
}
    }

//Initialize Game: 

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Starting game...");
make_sound();
delay(1000);
lcd.clear();

}

void loop () {

  lcd.setCursor(0,0);
  lcd.print ("Player 1: " + String(p1_score));
  lcd.setCursor (0, 1);
  lcd.print ("Player 2: " + String (p2_score));

  int game_update = (play_game(p1_score, p2_score));

  if (game_update == 1) {
    p1_score++;
    make_sound ();
    //cloud_game_score(p1_score, p2_score);
  }
  else if (game_update == 2){
    p2_score++;
    make_sound ();
    //cloud_game_score(p1_score, p2_score);
  }

  if (p1_score == 10){
    win_sound();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Player 1 Wins!");
    delay(5000);
    player1_wins++;
    //cloud_analytics (player1_wins, player2_wins);
    p1_score = 0;
    p2_score = 0;
    lcd.clear();
    delay(2000);

  }
    else if (p2_score == 10){
    win_sound();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Player 2 Wins!");
    delay(5000);
    player2_wins++;
    //cloud_analytics (player1_wins, player2_wins);
    p1_score = 0;
    p2_score = 0;
    lcd.clear();
    delay(2000);

  }



}

void make_sound () {
  tone(8,NOTE_B5,100);
  delay(100);
  tone(8,NOTE_E6,850);
  delay(800);
  noTone(8); 
  
  delay(1000);
  
}

void win_sound () {
  tone(8,NOTE_G4,35);
  delay(35);
  tone(8,NOTE_G5,35);
  delay(35);
  tone(8,NOTE_G6,35);
  delay(35);
  noTone(8);
}


// Needs to be written to upload analytics to cloud
/*
void cloud_analytics (int p1, int p2){
  return void;
}

// Needs to be written to upload game score to cloud
void cloud_game_score (int p1, int p2){
  return void;
}
 */
// Needs to be written with a while loop waiting for beam to be broken
int play_game (int p1, int p2) {
  return 1;
}

void dprint (String s){
  Serial.println(s);
  lcd.print(s);
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
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
  switch (p) {
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
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
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
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}
