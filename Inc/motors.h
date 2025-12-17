/*
 * motors.h
 *
 *  Created on: Dec 16, 2025
 *      Author: jamesgallegos
 */

#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

#include <stdint.h>

void motors_init(void);
void move_forward(void);
void move_backward(void);
void turn_left(void);
void turn_right(void);
void stop_motors(void);
void motors_set_all_percent(uint8_t percent);
void motors_set_each_percent(uint8_t  lf, uint8_t lr, uint8_t rf, uint8_t rr);


#endif /* INC_MOTORS_H_ */
