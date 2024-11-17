#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include wire.h
#include LCD.h
#include LiquidCrystal_I2C.h
#include pitches.h

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

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintIDs [50];
int prints = 0;
LiquidCrystal_I2C lcd(I2C_ADDR, En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

void setup () {

lcd.begin (16,2);
lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
lcd.setBacklight(HIGH);
lcd.print("Welcome to SmartTable Foosball");
delay(1000);

    int temp_prints = prints;
    while (temp_prints == prints) {
lcd.print("Player 1. Scan your finger");
fingerprintID = getFingerprintID();
delay (50);

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
lcd.print("Player 2. Scan your finger");
fingerprintID = getFingerprintID();
delay (50);

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

int p1_score = 0;
int p2_score = 0;
lcd.print("Starting game.");
delay(1000);

}

void loop () {

  lcd.print ("Player 1: " + String(p1_score));
  lcd.setCursor (0, 1);
  lcd.print ("Player 2: " + String (p2_score));

  int game_update = (!play_game(p1_score, p2_score));

  if (game_update == 1) {
    p1_score++;
    make_sound ();
    cloud_game_score(p1_score, p2_score);
  }
  else if (game_update == 2){
    p2_score++;
    make_sound ();
    cloud_game_score(p1_score, p2_score);
  }

  if (p1_score == 10){
    win_sound();
    lcd.clear();
    lcd.print("Player 1 Wins!");
    player1_wins++;
    cloud_analytics (player1_wins, player2_wins);
    p1_score = 0;
    p2_score = 0;

  }
    else if (p2_score == 10){
    win_sound();
    lcd.clear();
    lcd.print("Player 2 Wins!");
    player2_wins++;
    cloud_analytics (player1_wins, player2_wins);

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
void cloud_analytics (int p1, int p2){
  return void;
}

// Needs to be written to upload game score to cloud
void cloud_game_score (int p1, int p2){
  return void;
}

// Needs to be written with a while loop waiting for beam to be broken
int play_game (int p1, int p2) {
  return 0;
}