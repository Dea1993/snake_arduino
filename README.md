# Snake Game on Arduino® UNO R4 WiFi

## Description

This project implements the classic Snake game on an Arduino UNO R4 WiFi using an LED matrix as the display. The game allows you to control the snake and collect food while avoiding collisions with itself.

## Components Required

- Arduino UNO R4 WiFi
- Jumper wires
- 4 buttons to move your snake
- Breadboard (optional)

## Installation

1. **Clone the repository:**
   (if necessary edit Makefile according to your needs)
   ```bash
   git clone git@github.com:Dea1993/snake_arduino.git
   cd snake_arduino
   make compile  #(arduino-cli compile -b arduino:renesas_uno:unor4wifi ./snake_arduino.ino)
   make upload   #(arduino-cli upload -b arduino:renesas_uno:unor4wifi -p /dev/ttyACM0)```
2. compile it on your arduino


## Buttons Cables

- UP: 13
- DOWN: 12
- LEFT: 11
- RIGHT: 10
