#include <TFT_eSPI.h>

#include <Arduino.h>

TFT_eSPI tft;
long rand1;
long rand2;
// Dinosaur settings
int dinoX = 20;
float dinoY = 140;   // Ground level Y
int dinoHeight = 20;
int dinoWidth = 20;

// Jump variables
bool isJumping = false;
float jumpSpeed = 0;
float gravity = 1;

// Obstacle settings
int cactusX = 320;
int cactusY = 140;
int cactusWidth = 20;
int cactusHeight = 30;
int cactusSpeed;

// Score
long score = 0;

void setup() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_WHITE);

  pinMode(WIO_5S_PRESS, INPUT_PULLUP); // Middle button for jump
  rand2 = random(90, 320);
  rand1 = random(3, 7); // Random speed between 3–6
  cactusSpeed = rand1;

  drawGround();
}

void loop() {
  // Check jump
  if (digitalRead(WIO_5S_PRESS) == LOW && !isJumping) {
    isJumping = true;
    jumpSpeed = -14; // Upwards
  }

  // Update jump
  if (isJumping) {
    dinoY += jumpSpeed;
    jumpSpeed += gravity;
    if (dinoY >= 140) {
      dinoY = 140;
      isJumping = false;
    }
  }

  // Move cactus
  cactusX -= cactusSpeed;
  if (cactusX < -cactusWidth) {
    cactusX = rand2;
    cactusSpeed = random(3, 9);
    score++;
  }

  // Collision detection
  if (dinoX + dinoWidth > cactusX && dinoX < cactusX + cactusWidth &&
      dinoY + dinoHeight > cactusY) {
    gameOver();
  }

  // Draw frame
  tft.fillScreen(TFT_WHITE);
  drawGround();
  tft.fillRect(dinoX, dinoY - dinoHeight, dinoWidth, dinoHeight, TFT_RED);
  tft.fillRect(cactusX, cactusY - cactusHeight, cactusWidth, cactusHeight, TFT_GREEN);

  // Draw score
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setCursor(5, 5);
  tft.setTextSize(2);
  tft.print("Score: ");
  tft.print(score);

  delay(5); // frame delay
}

void drawGround() {
  tft.drawLine(0, 140, 320, 140, TFT_BLACK);
}

void gameOver() {
  tft.fillScreen(TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(50, 60);
  tft.println("GAME OVER!");
  tft.setTextSize(2);
  tft.setCursor(50, 100);
  tft.print("Score: ");
  tft.print(score);
  while (1); // Stop game
}
