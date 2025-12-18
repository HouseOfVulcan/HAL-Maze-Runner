# Maze Runner Robot - HAL Version

HAL-based conversion of the [bare-metal maze runner project]([link-to-bare-metal-repo](https://github.com/HouseOfVulcan/Maze-Runner-Robot)). This version demonstrates proficiency with STM32 Hardware Abstraction Layer (HAL) while maintaining the same obstacle-avoidance FSM logic.

## Project Status

**Currently Operational:**
- Motor control fully functional (4 DC motors with independent PWM speed control)
- HAL timer configuration (TIM4 for PWM, TIM2 for input capture)
- GPIO management via HAL drivers
- Finite State Machine logic implementation
- Wiring verified and tested

**In Progress:**
- Ultrasonic sensor integration (HC-SR04 timing calibration in progress)

This project served as a practical learning exercise in HAL development before advancing to more complex embedded projects like self-balancing robots.

## Overview

Autonomous obstacle-avoiding robot using:
- STM32 HAL drivers for peripheral management
- Finite State Machine (FSM) for decision-making
- Ultrasonic sensing for distance measurement (HC-SR04)
- TB6612FNG motor drivers for 4-wheel control
- CubeMX-generated initialization code

## Hardware
- **Microcontroller:** STM32F407VG Discovery
- **Motor Driver:** TB6612FNG (x2, one per side)
- **Motors:** 6V DC gear motors (48:1 ratio)
- **Sensor:** HC-SR04 Ultrasonic (with voltage divider for 5V→3.3V level shifting)
- **Power:** 5× AA batteries (7.5V for motors, 3.3V logic from STM32)

**Pin Assignments:**
```
Motor Control:
  Left Front:  IN1=PB0,  IN2=PB1,  PWM=PB6 (TIM4_CH1)
  Left Rear:   IN1=PB2,  IN2=PB10, PWM=PB7 (TIM4_CH2)
  Right Front: IN1=PB12, IN2=PB13, PWM=PB8 (TIM4_CH3)
  Right Rear:  IN1=PB14, IN2=PB15, PWM=PB9 (TIM4_CH4)
  STBY: PB11 (shared between both TB6612FNG drivers)

Sensor:
  TRIG: PA0 (GPIO Output)
  ECHO: PA1 (TIM2_CH2, Input Capture with voltage divider)
```

## Bare-Metal vs HAL: Key Differences

### Initialization Approach

**Bare-Metal:**
```c
// Direct register manipulation
RCC_AHB1ENR |= (1 << 1);           // Enable GPIOB clock
GPIOB_MODER &= ~(0x3 << (6*2));    // Clear mode bits
GPIOB_MODER |=  (0x2 << (6*2));    // Set alternate function
```

**HAL:**
```c
// HAL function calls with structs
__HAL_RCC_GPIOB_CLK_ENABLE();
GPIO_InitStruct.Pin = GPIO_PIN_6;
GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
```

### PWM Configuration

**Bare-Metal:**
- Manual calculation of prescaler/ARR values
- Direct register writes to TIM4_PSC, TIM4_ARR, TIM4_CCMR1/2
- Bit manipulation for PWM mode and preload settings

**HAL:**
- CubeMX generates timer configuration
- `HAL_TIM_PWM_Start()` handles peripheral enabling
- `__HAL_TIM_SET_COMPARE()` macro for duty cycle updates

### Code Complexity

**Bare-Metal Version:**
- `motors.c`: ~210 lines (including register definitions and PWM setup)
- Full control over every register bit
- Requires deep understanding of timer peripherals

**HAL Version:**
- `motors.c`: ~120 lines (57% reduction)
- Higher-level abstraction
- CubeMX-generated `main.c` handles low-level initialization

### Tradeoffs

| Aspect | Bare-Metal | HAL |
|--------|-----------|-----|
| **Code Size** | Smaller binary | Larger (HAL overhead) |
| **Development Speed** | Slower | Faster (CubeMX) |
| **Portability** | MCU-specific | Easier across STM32 families |
| **Learning Value** | Deep hardware understanding | Industry-standard practices |
| **Debugging** | More transparent | Abstraction can hide issues |

## Behavior

The FSM implements a left-hand wall-following algorithm:

**States:**
- `STATE_FORWARD` – Default movement, checks distance continuously
- `STATE_TURN_LEFT` – First obstacle response (90° left turn)
- `STATE_TURN_RIGHT` – Secondary response (180° if consecutive obstacle, 90° right if third)
- `STATE_ERROR` – Triggered by sensor timeout, auto-recovers after 2s

**Escape Sequence:**
1. Obstacle detected → Turn left 90°
2. If blocked again → Turn right 180° (face opposite direction)
3. If still blocked → Turn right 90° (dead-end escape)

**Timing Constants:**
```c
#define SAFE_DISTANCE_CM  7
#define TURN_LEFT_90      90    // ms
#define TURN_RIGHT_90     90
#define TURN_RIGHT_180    180
#define CRUISE_DUTY       20    // % PWM
#define TURN_DUTY         30
```

## Project Structure
```
HAL_Maze_Runner/
├── .ioc                          # STM32CubeMX project file
├── Inc/
│   ├── main.h                    # HAL-generated headers
│   ├── stm32f4xx_hal_conf.h      # HAL configuration
│   ├── motors.h                  # Motor control API
│   ├── sensor.h                  # Ultrasonic sensor API
│   └── logic.h                   # FSM states and constants
├── Src/
│   ├── main.c                    # HAL initialization + main loop
│   ├── stm32f4xx_hal_msp.c       # HAL MSP (peripheral low-level init)
│   ├── motors.c                  # HAL-based motor control
│   ├── sensor.c                  # HAL-based distance measurement
│   └── logic.c                   # FSM implementation
└── Drivers/                      # STM32 HAL library files (CubeMX-generated)
```

## Development Tools
- **IDE:** STM32CubeIDE
- **Configuration:** STM32CubeMX (generates `.ioc` file)
- **HAL Version:** STM32Cube FW_F4 V1.28.3
- **Debugger:** ST-LINK (onboard)

## Future Improvements
- Complete sensor calibration for reliable distance readings
- Implement UART debugging output for real-time FSM state monitoring
- Add multiple ultrasonic sensors for 360° obstacle detection
- Explore DMA for PWM updates to reduce CPU load

## Related Projects
- [Bare-Metal Maze Runner](link-to-bare-metal-repo) - Original implementation using direct register manipulation
- Self-Balancing Robot (upcoming) - Next project applying HAL + advanced control theory

---

**Note:** This is part of an ongoing embedded systems learning portfolio. The conversion from bare-metal to HAL demonstrates understanding of both low-level hardware control and industry-standard abstraction layers.
