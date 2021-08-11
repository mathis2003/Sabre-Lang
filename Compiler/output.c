#include "output.h"

#include "ptlib.h"

#include <stdlib.h>

#include <stdio.h>

#include <time.h>

#include <stdint.h>
uint32_t millisecs;
clock_t start_time;
uint8_t  anon1 (clock_t  start_time , uint32_t  milli_seconds) {
	uint8_t  nested_return;
	(nested_return = (((clock())<(start_time+milli_seconds)) ? (anon1(start_time , milli_seconds)) : 0));

	return nested_return;

}
uint8_t (*nested_loop)(clock_t  , uint32_t ) = &anon1;
uint8_t  anon0 (uint32_t  number_of_seconds) {
	uint8_t  delay_return;
	(start_time = (clock()));
	(millisecs = (number_of_seconds*1000));
	(delay_return = (nested_loop(start_time , millisecs)));

	return delay_return;

}
uint8_t (*delay)(uint32_t ) = &anon0;
ptlRaster screen1;
uint8_t x = 5;
uint8_t y = 5;
uint8_t *prev_x = &(uint8_t ){0};
uint8_t *prev_y = &(uint8_t ){0};
uint32_t keyPressed;
uint8_t  anon2 (ptlRaster  screen1 , uint8_t  x , uint8_t  y , uint8_t * prev_x , uint8_t * prev_y) {
	uint8_t  return_val;
	(ptlDrawPixel(screen1 , '.' , (*prev_x) , (*prev_y)));
	(ptlDrawPixel(screen1 , '#' , x , y));
	(*prev_x = x);
	(*prev_y = y);
	(delay(10));
	(ptlRepaint(screen1));
	(keyPressed = (ptlPressedKey(screen1)));
	(x = (((keyPressed==KEYCODE_LEFT_ARROW)&&(x>0)) ? (x-1) : (((keyPressed==KEYCODE_RIGHT_ARROW)&&(x<24)) ? (x+1) : x)));
	(y = (((keyPressed==KEYCODE_UP_ARROW)&&(y>0)) ? (y-1) : (((keyPressed==KEYCODE_DOWN_ARROW)&&(y<19)) ? (y+1) : y)));
	(return_val = ((keyPressed==KEYCODE_Q) ? 1 : (anon2(screen1 , x , y , prev_x , prev_y))));

	return return_val;

}
uint8_t (*game_loop)(ptlRaster  , uint8_t  , uint8_t  , uint8_t * , uint8_t *) = &anon2;
int main() {
*prev_x = 5;
*prev_y = 5;
	(screen1 = (ptlInitRaster(25 , 20 , '.')));
	(game_loop(screen1 , x , y , prev_x , prev_y));
	(ptlDestroyRaster(screen1));

	return 0;
}