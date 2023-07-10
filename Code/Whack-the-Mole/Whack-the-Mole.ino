/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Whack-the-Mole.ino                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ababdelo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 09:13:52 by ababdelo          #+#    #+#             */
/*   Updated: 2023/07/10 11:25:56 by ababdelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Button pin connections
const int buttonPinsPlayer1[] = {A0, A1, A2, A3, A4}; // Player 1 button pins
const int buttonPinsPlayer2[] = {A5, A6, A7, A8, A9}; // Player 2 button pins
const int btnstart = A10;
const int buttonCount = sizeof(buttonPinsPlayer1) / sizeof(buttonPinsPlayer1[0]);

// Mole pin connections
const int molePinsPlayer1[] = {2, 3, 4, 5, 6}; // Player 1 mole pins
const int molePinsPlayer2[] = {7, 8, 9, 10, 11}; // Player 2 mole pins

// Game variables
int scorePlayer1 = 0;
int scorePlayer2 = 0;
int currentMoleIndexPlayer1 = 0; // Index of the current mole being active for Player 1
int currentMoleIndexPlayer2 = 0; // Index of the current mole being active for Player 2
bool gameRunning = false;

void setup() {
  currentMoleIndexPlayer2 = random(0, 5);
  currentMoleIndexPlayer1 = random(0, 5);
  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight(); // Initialize the LCD
  //  randomSeed(analogRead(0)); // Seed the random number generator
  //  pinMode(LED_BUILTIN, OUTPUT); // Built-in LED for hit indication

  for (int i = 0; i < buttonCount; i++) {
    pinMode(buttonPinsPlayer1[i], INPUT_PULLUP); // Set player 1 button pins as input with internal pull-up resistors
    pinMode(buttonPinsPlayer2[i], INPUT_PULLUP); // Set player 2 button pins as input with internal pull-up resistors
  }
  pinMode(btnstart, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print(" Whack the Mole ");
  lcd.setCursor(0, 1);
  lcd.print(" Press Start Btn");
  delay(500);
}

void loop() {
  if (!gameRunning) {
    if (digitalRead(btnstart) == LOW) {
      startGame();
    }
  } else {
    playGame();
  }
}

void startGame() {
  lcd.clear();
  scorePlayer1 = 0;
  scorePlayer2 = 0;
  gameRunning = true;
  lcd.setCursor(0, 0);
  lcd.print(" Game Started ! ");
  spawnMole();
}

void spawnMole() {
  int molePinPlayer1 = molePinsPlayer1[currentMoleIndexPlayer1];
  int molePinPlayer2 = molePinsPlayer2[currentMoleIndexPlayer2];

  digitalWrite(molePinPlayer1, HIGH); // Activate the mole for Player 1
  digitalWrite(molePinPlayer2, HIGH); // Activate the mole for Player 2

  if (digitalRead(buttonPinsPlayer1[currentMoleIndexPlayer1]) == LOW )
  {
    digitalWrite(molePinPlayer1, LOW); // Deactivate the mole for Player 1
    //    while (digitalRead(buttonPinsPlayer1[currentMoleIndexPlayer1]) == LOW)
    //      delay(50);
    currentMoleIndexPlayer1 = random(0, 5);
    scorePlayer1++;
  }

  if (digitalRead(buttonPinsPlayer2[currentMoleIndexPlayer2]) == LOW )
  {
    digitalWrite(molePinPlayer2, LOW); // Deactivate the mole for Player 2
    //    while (digitalRead(buttonPinsPlayer2[currentMoleIndexPlayer2]) == LOW)
    //      delay(50);
    currentMoleIndexPlayer2 = random(0, 5);
    scorePlayer2++;
  }
  lcd.setCursor(0, 0);
  lcd.print(" Player   Hit ! ");
  lcd.setCursor(0, 1);
  lcd.print("  P1: ");
  lcd.print(scorePlayer1);
  lcd.setCursor(7, 1);
  lcd.print("  P2: ");
  lcd.print(scorePlayer2);
  delay(250);
}

void playGame() {
  if (scorePlayer1 == 10 || scorePlayer2 == 10) {
    endGame();
  } else {
    spawnMole();
  }
}

void endGame() {
  for (int x = 0; x < buttonCount; x++)
  {
    digitalWrite(molePinsPlayer1[x], LOW); // Deactivate the mole for Player 1
    digitalWrite(molePinsPlayer2[x], LOW); // Deactivate the mole for Player 2
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Game  Over !  ");
  if (scorePlayer1 > scorePlayer2) {
    lcd.setCursor(0, 1);
    lcd.print(" Player 1 Wins! ");
    celebrate(1);
  } else if (scorePlayer1 < scorePlayer2) {
    lcd.setCursor(0, 1);
    lcd.print(" Player 2 Wins! ");
    celebrate(2);
  } else {
    lcd.setCursor(0, 1);
    lcd.print("It's a tie!");
    celebrate(0);
  }
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Whack the Mole ");
  lcd.setCursor(0, 1);
  lcd.print(" Press Start Btn");
  gameRunning = false;
}

void celebrate(int player)
{
  int i = 250;
  for (int x = 0; x < buttonCount; x++)
  {
    digitalWrite(molePinsPlayer1[x], LOW); // Deactivate the mole for Player 1
    digitalWrite(molePinsPlayer2[x], LOW); // Deactivate the mole for Player 2
  }
  if (player == 0)
  {
    while (i)
    {
      for (int x = 0; x < buttonCount; x++)
      {
        digitalWrite(molePinsPlayer1[x], HIGH); // Deactivate the mole for Player 1
        digitalWrite(molePinsPlayer2[x], HIGH); // Deactivate the mole for Player 1
      }
      delay(i);
      for (int x = 0; x < buttonCount; x++)
      {
        digitalWrite(molePinsPlayer1[x], LOW); // Deactivate the mole for Player 1
        digitalWrite(molePinsPlayer2[x], LOW); // Deactivate the mole for Player 1
      }
      delay(i);
      i = i - 25;
    }
  }

  else if (player == 1)
  {
    while (i)
    {
      for (int x = 0; x < buttonCount; x++)
      {
        digitalWrite(molePinsPlayer1[x], HIGH); // Deactivate the mole for Player 1
      }
      delay(i);
      for (int x = 0; x < buttonCount; x++)
      {
        digitalWrite(molePinsPlayer1[x], LOW); // Deactivate the mole for Player 1
      }
      delay(i);
      i = i - 25;
    }
  }

  else if (player == 2)
  {
    while (i)
    {
      for (int x = 0; x < buttonCount; x++)
      {
        digitalWrite(molePinsPlayer2[x], HIGH); // Deactivate the mole for Player 1
      }
      delay(i);
      for (int x = 0; x < buttonCount; x++)
      {
        digitalWrite(molePinsPlayer2[x], LOW); // Deactivate the mole for Player 1
      }
      delay(i);
      i = i - 25;
    }
  }
}
