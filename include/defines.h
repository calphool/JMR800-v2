#pragma once

#ifndef DEFINES_h
#define DEFINES_h


// MUX enable pins
#define U2_ENABLE_PIN 4
#define U3_ENABLE_PIN 5
#define U4_ENABLE_PIN 6
#define U6_ENABLE_PIN 7

// ADC input pins
#define U2_ANALOG_IN_PIN 23
#define U3_ANALOG_IN_PIN 22
#define U4_ANALOG_IN_PIN 21
#define U6_ANALOG_IN_PIN 20

// MUX address pins
#define S0_PIN 0
#define S1_PIN 1
#define S2_PIN 2 
#define S3_PIN 3

#define NUM_MUXES 4

// synthesizer input pins
#define READYOUT_PIN 10
#define CLOCK_IN_PIN 11
#define DATA_OUT_PIN 12

// OLED screen I2C address
#define OLED_I2C_ADDRESS 0x3c

// Button pins
#define PUSH_BTN_SW4_PIN 29
#define PUSH_BTN_SW3_PIN 28
#define PUSH_BTN_SW2_PIN 27
#define PUSH_BTN_SW1_PIN 26

// array of button pins
#define NUM_BUTTONS 4

// button lights shift register pins
#define SHIFT_REG_SER 32
#define SHIFT_REG_SRCLK 31
#define SHIFT_REG_RCLK 30

// Encoder pins
#define ENCODER_CLK_PIN 13
#define ENCODER_DT_PIN 14
#define ENCODER_SW_PIN 15

// OLED related stuff
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// used in send command functionality
#define COMMAND_TIMEOUT  250

#define NUM_KNOBS 56

// knob configuration structure
typedef struct {
  char name[15];
  uint8_t cmdbyte;
  uint8_t typecode;
} knobConfig;


#endif