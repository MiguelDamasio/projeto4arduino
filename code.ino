#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);

const int player1ButtonPin = 2;
const int player2ButtonPin = 3;
const int resetButtonPin = 4;
const int buzzerPin = 5;
const int ledPin = 6;

long tempo=0;
int player1Score = 0;
int player2Score = 0;
const int winningScore = 7;
bool gameEnded = false;

void setup() {
  pinMode(player1ButtonPin, INPUT_PULLUP);
  pinMode(player2ButtonPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);
  
  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("P1: 0   P2: 0");
  lcd.setCursor(0, 1);
  lcd.print("First to 7 pts");

  Serial.begin(9600);
}

void loop() {
  if (gameEnded) {
    if (digitalRead(resetButtonPin) == LOW) {
      resetGame();
    }
    return;
  }
  
  if (digitalRead(resetButtonPin) == LOW) {
      resetGame();
   }

  if (digitalRead(player1ButtonPin) == LOW) {
    player1Score++;
    delay(300);
  }


  if (digitalRead(player2ButtonPin) == LOW) {
    player2Score++;
    delay(300);
  }
  if (millis()-tempo>500){
    tempo = millis();
  displayScore(player1Score, player2Score);
  }
  checkWinner();
  delay(1);
}

void resetGame() {
  player1Score = 0;
  player2Score = 0;
  gameEnded = false;
  noTone(buzzerPin);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("P1: 0   P2: 0");
  lcd.setCursor(0, 1);
  lcd.print("First to 7 pts");
}

void checkWinner() {
  if ((player1Score >= winningScore || player2Score >= winningScore) &&
      abs(player1Score - player2Score) >= 2) {
    gameEnded = true;
    lcd.clear();
    if (player1Score > player2Score) {
      lcd.setCursor(0, 0);
      lcd.print("Player 1 Wins!");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Player 2 Wins!");
    }
    lcd.setCursor(0, 1);
    lcd.print("Congrats!");
    for (int i = 0; i < 3; i++) {
      digitalWrite(ledPin, HIGH);
      tone(buzzerPin, 1000, 1000);
      delay(500);
      digitalWrite(ledPin, LOW);
      noTone(buzzerPin);
      delay(500);
    }
    Serial.println("We have a winner!");

  }
}

void displayScore(int score1, int score2) {
  lcd.setCursor(0, 0);
  lcd.print("P1: ");
  lcd.print(score1);
  lcd.print("   P2: ");
  lcd.print(score2);
}
