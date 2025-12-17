#include "sensor.h"
#include "main.h"

extern TIM_HandleTypeDef htim2;

void sensor_init(void) {

	// Start input capture on TIM2CH2
	HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_2);

}

static void delay_us(uint32_t us) {

	volatile uint32_t count = us * 42;
	while (count--) {
		__asm__("nop");
	}
}

static uint32_t measure_distance_cm(void) {

	uint32_t start_time, end_time, pulse_width;

	// Step 1: Reset timer counter to 0 so we know where we are starting from
	__HAL_TIM_SET_COUNTER(&htim2, 0);


	// Step 2: Set rising edge
	// CC2P bit(bit 5) controls polarity, 0 = rising, 1 = falling
	htim2.Instance->CCER &= ~(1 << 5);

	// Step 3: Send 10 us trigger pulse
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); // TRIG HIGH
	delay_us(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); // TRIG LOW

	// Step 4: Wait for rising edge, detect when ECHO goes HIGH
	uint32_t timeout;
	timeout = 10000;
	while(__HAL_TIM_GET_COUNTER(&htim2) < 5000 && timeout > 0){
		timeout--;
	}
	if (timeout == 0) return 0xFFFF; // Need incase something goes wrong

	// Step 5: Capture start time, marks when pulse was sent
	start_time = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_2);

	// Step 6: switch to falling edge, need to detect when ECHO goes LOW
	htim2.Instance->CCER |= (1 << 5);

	// Step 7: Wait for falling edge
	timeout = 10000;
	uint32_t start_wait = __HAL_TIM_GET_COUNTER(&htim2);
	while((__HAL_TIM_GET_COUNTER(&htim2) - start_wait) < 30000 && timeout > 0){
		timeout--;
	}
	if (timeout == 0) return 0xFFFF; // Need incase something goes wrong

	// Step 8: capture end time
	end_time = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_2);

	// Step 9: Calculate pulse width
	if (end_time >= start_time) {
		pulse_width = end_time - start_time;
	}
	else {
		pulse_width = (0xFFFF - start_time) + end_time + 1; // wrap around, overflow
	}

	// Stpe 10: convert pulse width to distance
	// Distance = (pulse width * 0.0343) / 2
	uint32_t distance_cm = (pulse_width * 343) / 2000; //integer math

	return distance_cm;

}

uint32_t get_distance_cm(void){

	return measure_distance_cm();
}
