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

const uint32_t frame_game_over[] = {
  0x20410809,
  0x600600,
  0x90108204,
};

// snake initial coordinates
int snake_x[96]= {4, 3, 2, 1};
int snake_y[96]= {3, 3, 3, 3};
int snake_len= 4;

char direction= 'r';
char new_direction= 'r';

int points= 0;

// apple initial position
int apple_x= 9;
int apple_y= 4;

int loop_delay= 300;
long exec_t2= millis();

void setup() {
  //Serial.begin(115200);
  Serial.begin(9600);
  // stard LED matrix
  matrix.begin();
  // btn up
  pinMode(13, INPUT_PULLUP);
  // btn right
  pinMode(12, INPUT_PULLUP);
  // btn bottom
  pinMode(11, INPUT_PULLUP);
  // btn left
  pinMode(10, INPUT_PULLUP);

  randomSeed(analogRead(0));
}

void add_tail_block() {
  snake_len++;
  snake_y[snake_len-1]= snake_y[snake_len-2];
  snake_x[snake_len-1]= snake_x[snake_len-2];
}

void show_apple() {
  frame[apple_y][apple_x]= 1;
}

void eat_apple() {
  points++;
  Serial.println(points);
  // increase difficult each 5 apples
  if (points % 5 == 0) loop_delay-= 50;
  
  // generate new apple coordinates
  frame[apple_y][apple_x]= 0;
  apple_x= random(11);
  apple_y= random(7);
  show_apple();
  
  // increase snake length
  add_tail_block();
}

void game_over() {
  // show gameover frame
  matrix.loadFrame(frame_game_over);
  delay(1000);

  // show points inside the matrix
  int i= 0;
  int j= 0;
  // clear the old frame
  for (i=0; i < 12; i++) {
    for (j=0; j < 8; j++) {
      frame[j][i]= 0;
    }
  }
  matrix.renderBitmap(frame, 8, 12);

  // show points
  for (i=0; i < points; i++) {
    frame[0][i]= 1;
    matrix.renderBitmap(frame, 8, 12);
    delay(100);
  }
  Serial.println("Game Over");
  while (true) delay(10000);
}

void move_snake() {
  int i=0;

  int y= snake_y[snake_len-1];
  int x= snake_x[snake_len-1];
  frame[y][x]= 0;

  // move and render the snake tail
  for (i= snake_len-1; i > 0; i--) {
    frame[snake_y[i-1]][snake_x[i-1]]= 1;
    if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i])
      game_over();
    snake_x[i]= snake_x[i-1];
    snake_y[i]= snake_y[i-1];
  }

  //move the snake head

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
  
  if      (u == LOW && direction != 'b') new_direction= 'u';
  else if (r == LOW && direction != 'l') new_direction= 'r';
  else if (b == LOW && direction != 'u') new_direction= 'b';
  else if (l == LOW && direction != 'r') new_direction= 'l';
}


void loop() {
  long exec_t1= millis();
  change_direction();
  if (exec_t1 - exec_t2 > loop_delay) {
    direction= new_direction;
    show_apple();
    move_snake();
    matrix.renderBitmap(frame, 8, 12);
    exec_t2= exec_t1;
  }
  delay(10);
}
