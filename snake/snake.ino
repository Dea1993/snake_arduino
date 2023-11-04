#include "Arduino_LED_Matrix.h"

// create LED matrix object
ArduinoLEDMatrix matrix;

// initial snake frame
byte frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

const uint32_t game_over[] = {
  0x20410809,
  0x600600,
  0x90108204,
};

// snake initial coordinates
int snake_x[4]= {4, 3, 2, 1};
int snake_y[4]= {3, 3, 3, 3};
int snake_len= 4;
// direction 1 right, 2 bottom, 3 left, 4 top
int direction= 1;

// apple initial position
//int apple_x= 9;
//int apple_y= 4;

void setup() {
  Serial.begin(115200);
  // stard LED matrix
  matrix.begin();
}

void move_snake() {
  /*
    starting from head, save curr position w and h
    then for all the snake length the previuos
    body block needs to take the previous block place
    the current tail pos will be set to 0
  */

  int i=0;
  frame[snake_y[snake_len-1]][snake_x[snake_len-1]]= 0;
  // move and render the snake tail
  for (i= snake_len-1; i > 0; i--) {
    frame[snake_y[i-1]][snake_x[i-1]]= 1;
    snake_x[i]= snake_x[i-1];
    snake_y[i]= snake_y[i-1];
  }

  //move the snake head

  //snake_y[0]= snake_y[0];
  // go right
  if (direction == 1) {
    snake_x[0]+= 1;
    if (snake_x[0] > 11) snake_x[0]= 0;
  }
  // go down
  else if (direction == 2) {
    snake_y[0]+= 1;
    if (snake_y[0] > 7) snake_y[0]= 0;
  }
  // go left
  else if (direction == 3) {
    snake_x[0]-= 1;
    if (snake_x[0] < 0) snake_x[0]= 11;
  }
  // go up
  else if (direction == 4) {
    snake_y[0]-= 1;
    if (snake_y[0] < 0) snake_y[0]= 7;
  }

  // render the snake head
  frame[snake_y[0]][snake_x[0]]= 1;
}

void loop() {
  matrix.renderBitmap(frame, 8, 12);
  delay(300);
  move_snake();
  direction= 2;
}
