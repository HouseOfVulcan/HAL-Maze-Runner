#include "motors.h"
#include "main.h"

extern TIM_HandleTypeDef htim4;


static inline void pwm_set_percent_ch(uint8_t ch, uint8_t percent) {

	if (percent > 100) percent = 100;

	// get ARR value from htim4
	uint32_t arr = htim4.Init.Period;

	// calculate CCR
	uint32_t ccr = ((arr + 1) * percent) / 100;

	switch(ch)
	{
	case 1: __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, ccr); break;
	case 2: __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, ccr); break;
	case 3: __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, ccr); break;
	case 4: __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, ccr); break;
	default: break;
	}
}

void motors_set_all_percent(uint8_t percent)
{
    pwm_set_percent_ch(1, percent);
    pwm_set_percent_ch(2, percent);
    pwm_set_percent_ch(3, percent);
    pwm_set_percent_ch(4, percent);
}

void motors_set_each_percent(uint8_t lf, uint8_t lr, uint8_t rf, uint8_t rr)
{
    pwm_set_percent_ch(1, lf);
    pwm_set_percent_ch(2, lr);
    pwm_set_percent_ch(3, rf);
    pwm_set_percent_ch(4, rr);
}

void motors_init(void) {

	// set STBY(PB11)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);

	// Set all direction control pins LOW,
	uint16_t motor_pins[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2,
			                 GPIO_PIN_10, GPIO_PIN_12, GPIO_PIN_13,
							 GPIO_PIN_14, GPIO_PIN_15};

	for (int i = 0; i < 8; i++) {
		HAL_GPIO_WritePin(GPIOB, motor_pins[i], GPIO_PIN_RESET);
	}

	// Start PWM on all 4 channels
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

	// set initial duty cycle to 30%
	motors_set_all_percent(30);

}

/*
Motor	        IN1	    IN2
Left Front------PB0     PB1
Left Rear-------PB2	    PB10
Right Front-----PB12	PB13
Right Rear------PB14	PB15

IN1	 IN2	Motor Behavior
 0	  0   	Brake
 0	  1     Reverse
 1	  0	    Forward
 1	  1	    Brake
*/

void move_forward(void) {

	//array to house all pins and set LOW
	uint16_t motor_pins[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2,
			                 GPIO_PIN_10, GPIO_PIN_12, GPIO_PIN_13,
							 GPIO_PIN_14, GPIO_PIN_15};

	for (int i = 0; i < 8; i++) {
		HAL_GPIO_WritePin(GPIOB, motor_pins[i], GPIO_PIN_RESET);
	}

	// Set IN1 pins HIGH (PB0,2,12,14)
	uint16_t forward_pins[] = {GPIO_PIN_0, GPIO_PIN_2, GPIO_PIN_12, GPIO_PIN_14};

	for (int i = 0; i < 4; i++) {
		HAL_GPIO_WritePin(GPIOB, forward_pins[i], GPIO_PIN_SET);
	}

}

void move_backward(void) {

	//array to house all pins and set LOW
	uint16_t motor_pins[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2,
			                 GPIO_PIN_10, GPIO_PIN_12, GPIO_PIN_13,
							 GPIO_PIN_14, GPIO_PIN_15};

	for (int i = 0; i < 8; i++) {
		HAL_GPIO_WritePin(GPIOB, motor_pins[i], GPIO_PIN_RESET);
	}

	// Set IN2 pins HIGH (PB1, 10, 13, 15)
	uint16_t backward_pins[] = {GPIO_PIN_1, GPIO_PIN_10, GPIO_PIN_13, GPIO_PIN_15};

	for (int i = 0; i < 4; i++) {
		HAL_GPIO_WritePin(GPIOB, backward_pins[i], GPIO_PIN_SET);
	}

}

void turn_right(void){

	//array to house all pins and set LOW
	uint16_t motor_pins[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2,
			                 GPIO_PIN_10, GPIO_PIN_12, GPIO_PIN_13,
							 GPIO_PIN_14, GPIO_PIN_15};

	for (int i = 0; i < 8; i++) {
		HAL_GPIO_WritePin(GPIOB, motor_pins[i], GPIO_PIN_RESET);
	}

	// set LEFT forward, RIGHT backwards
	uint16_t right_pins[] = {GPIO_PIN_0, GPIO_PIN_2, GPIO_PIN_13, GPIO_PIN_15};

	for (int i = 0; i < 4; i++) {
		HAL_GPIO_WritePin(GPIOB, right_pins[i], GPIO_PIN_SET);
	}
}

void turn_left(void){

	//array to house all pins and set LOW
	uint16_t motor_pins[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2,
			                 GPIO_PIN_10, GPIO_PIN_12, GPIO_PIN_13,
							 GPIO_PIN_14, GPIO_PIN_15};

	for (int i = 0; i < 8; i++) {
		HAL_GPIO_WritePin(GPIOB, motor_pins[i], GPIO_PIN_RESET);
	}

	// set LEFT backwards, RIGHT forwards
	uint16_t left_pins[] = {GPIO_PIN_1, GPIO_PIN_10, GPIO_PIN_12, GPIO_PIN_14};

	for (int i = 0; i < 4; i++) {
		HAL_GPIO_WritePin(GPIOB, left_pins[i], GPIO_PIN_SET);
	}
}

void stop_motors(void) {

	//array to house all pins and set LOW
	uint16_t motor_pins[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2,
			                 GPIO_PIN_10, GPIO_PIN_12, GPIO_PIN_13,
							 GPIO_PIN_14, GPIO_PIN_15};

	for (int i = 0; i < 8; i++) {
		HAL_GPIO_WritePin(GPIOB, motor_pins[i], GPIO_PIN_RESET);
	}

}

