#include "Arduino_LED_Matrix.h"

// create LED matrix object
ArduinoLEDMatrix matrix;

// initial snake frame
byte frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
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
int snake_x[96]= {4, 3, 2, 1};
int snake_y[96]= {3, 3, 3, 3};
int snake_len= 4;
//int *snake_x;
//int *snake_y;
//snake_x= (int *)malloc(snake_len * sizeof(int));
//snake_y= (int *)malloc(snake_len * sizeof(int));

char direction= 'r';

int points= 0;

// apple initial position
int apple_x= 9;
int apple_y= 3;

void setup() {
  //Serial.begin(115200);
  Serial.begin(9600);
  // stard LED matrix
  matrix.begin();
  // btn up
  pinMode(13, INPUT);
  // btn right
  pinMode(12, INPUT);
  // btn bottom
  pinMode(11, INPUT);
  // btn left
  pinMode(10, INPUT);
}

void add_tail_block() {
  snake_len++;
  snake_y[snake_len-1]= snake_y[snake_len-2];
  snake_x[snake_len-1]= snake_x[snake_len-2];
}

void eat_apple() {
  points++;
  //XXX new apple coordinates
  frame[apple_y][apple_x]= 0;
  apple_x= random(11);
  apple_y= random(7);
  frame[apple_y][apple_x]= 1;

  add_tail_block();
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
  if (direction == 'r') {
    snake_x[0]+= 1;
    if (snake_x[0] > 11) snake_x[0]= 0;
  }
  // go bottom
  else if (direction == 'b') {
    snake_y[0]+= 1;
    if (snake_y[0] > 7) snake_y[0]= 0;
  }
  // go left
  else if (direction == 'l') {
    snake_x[0]-= 1;
    if (snake_x[0] < 0) snake_x[0]= 11;
  }
  // go up
  else if (direction == 'u') {
    snake_y[0]-= 1;
    if (snake_y[0] < 0) snake_y[0]= 7;
  }
  if (snake_y[0] == apple_y && snake_x[0] == apple_x) eat_apple();

  // render the snake head
  frame[snake_y[0]][snake_x[0]]= 1;
}

void change_direction() {
  // up
  int u= digitalRead(13);
  // right
  int r= digitalRead(12);
  // bottom
  int b= digitalRead(11);
  // left
  int l= digitalRead(10);
  
  if      (u == HIGH && direction != 'b') direction= 'u';
  else if (r == HIGH && direction != 'l') direction= 'r';
  else if (b == HIGH && direction != 'u') direction= 'b';
  else if (l == HIGH && direction != 'r') direction= 'l';
}

void loop() {
  Serial.println(points);
  change_direction();
  matrix.renderBitmap(frame, 8, 12);
  delay(300);
  move_snake();


  // XXX to remove, test only
  //direction= 'b';
  //eat_apple();
  //add_tail_block();
}
