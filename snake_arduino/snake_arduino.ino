#include "Arduino_LED_Matrix.h"

#define BTNUP 13
#define BTNBOTTOM 12
#define BTNLEFT 11
#define BTNRIGHT 10

#define GOUP 1
#define GOBOTTOM 2
#define GOLEFT 3
#define GORIGHT 4

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

char direction= GORIGHT;
// used to avoid a double change direction before LED turns on
char new_direction= GORIGHT;

int points= 0;

// apple initial position
int apple_x= 9;
int apple_y= 4;

int loop_delay= 300;
long exec_t2= millis();
bool running= true;

void setup() {
  //Serial.begin(115200);
  Serial.begin(9600);
  // stard LED matrix
  matrix.begin();
  pinMode(BTNUP, INPUT_PULLUP);
  pinMode(BTNRIGHT, INPUT_PULLUP);
  pinMode(BTNBOTTOM, INPUT_PULLUP);
  pinMode(BTNLEFT, INPUT_PULLUP);

  randomSeed(analogRead(0));
}

void game_over() {
  // show gameover frame
  matrix.loadFrame(frame_game_over);
  delay(1000);

  // show points inside the matrix
  // clear the old frame
  for (int i=0; i < 12; i++) {
    for (int j=0; j < 8; j++) {
      frame[j][i]= 0;
    }
  }
  matrix.renderBitmap(frame, 8, 12);

  // show points
  for (int i=0; i < points; i++) {
    frame[0][i]= 1;
    matrix.renderBitmap(frame, 8, 12);
    delay(100);
  }
  Serial.println("Game Over");
  running= false;
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
  if (points % 5 == 0) loop_delay= max(50, loop_delay-=50);
  
  // generate new apple coordinates
  frame[apple_y][apple_x]= 0;
  apple_x= random(11);
  apple_y= random(7);
  
  // increase snake length
  add_tail_block();
}

void move_snake() {
  int y= snake_y[snake_len-1];
  int x= snake_x[snake_len-1];
  frame[y][x]= 0;

  // move and render the snake tail
  for (int i= snake_len-1; i > 0; i--) {
    frame[snake_y[i-1]][snake_x[i-1]]= 1;
    if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i])
      return game_over();
    snake_x[i]= snake_x[i-1];
    snake_y[i]= snake_y[i-1];
  }

  //move the snake head

  if (direction == GORIGHT) {
    snake_x[0]+= 1;
    if (snake_x[0] > 11) snake_x[0]= 0;
  }
  else if (direction == GOBOTTOM) {
    snake_y[0]+= 1;
    if (snake_y[0] > 7) snake_y[0]= 0;
  }
  else if (direction == GOLEFT) {
    snake_x[0]-= 1;
    if (snake_x[0] < 0) snake_x[0]= 11;
  }
  else if (direction == GOUP) {
    snake_y[0]-= 1;
    if (snake_y[0] < 0) snake_y[0]= 7;
  }
  if (snake_y[0] == apple_y && snake_x[0] == apple_x)
    eat_apple();

  // render the snake head
  frame[snake_y[0]][snake_x[0]]= 1;
}

void change_direction() {
  if      (digitalRead(BTNUP) == LOW && direction != GOBOTTOM) new_direction= GOUP;
  else if (digitalRead(BTNRIGHT) == LOW && direction != GOLEFT) new_direction= GORIGHT;
  else if (digitalRead(BTNBOTTOM) == LOW && direction != GOUP) new_direction= GOBOTTOM;
  else if (digitalRead(BTNLEFT) == LOW && direction != GORIGHT) new_direction= GOLEFT;
}


void loop() {
  if (!running) return;
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
