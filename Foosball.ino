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

  lcd.begin();
  lcd.setCursor(0,0);
  lcd.print("Welcome to Smart");
  lcd.setCursor(0,1);
  lcd.print("Table Foosball!");
  delay(3000);
  lcd.clear();

    int temp_prints = prints;
    while (temp_prints == prints) {
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Player 1.");
lcd.setCursor (0,1);
lcd.print("Scan your finger");
//fingerprintID = getFingerprintID();
int fingerprintID = 1;
delay (1000);

if (fingerprintID == 1){
    fingerprintIDs[prints] = 1;
    prints++;
}
else if (fingerprintID == 2){
    fingerprintIDs[prints] = 2;
    prints++;
}
    }

    temp_prints = prints;
    lcd.clear();
    while (temp_prints == prints) {
lcd.setCursor(0,0);
lcd.print("Player 2");
lcd.setCursor (0,1);
lcd.print("Scan your finger");
//fingerprintID = getFingerprintID();
int fingerprintID = 2;
delay (1000);

if (fingerprintID == 1){
    fingerprintIDs[prints] = 1;
    prints++;
}
else if (fingerprintID == 2){
    fingerprintIDs[prints] = 2;
    prints++;
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