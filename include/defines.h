/**
 * @file defines.h
 * @brief Hardware pin definitions and global constants for JMR800-V2.
 *
 * This header centralizes all board-level hardware definitions, including
 * pin assignments for buttons, encoders, multiplexer select lines, and
 * serial communication lines. It also defines constants for display geometry,
 * button counts, and PG-800 command timing.
 */

#pragma once

#include <stdint.h>


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

/**
 * @typedef knobConfig
 * @brief Structure representing a single knob’s control configuration.
 */
typedef struct {
  char name[15];  ///< Human-readable label (e.g., "VCA Level")
  uint8_t cmdbyte; ///< Command byte to send to PG-800 (analogous to a Midi CC)
  uint8_t typecode; ///< Reserved field or control type classification
} knobConfig;


/**
 * @enum LogLevel
 * @brief Defines logging severity levels.
 *
 * These levels allow filtering of log messages based on verbosity or importance.
 */
enum LogLevel {
    LOG_VERBOSE,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};


#ifndef TARGET_TEENSY
#include <chrono>
#include <thread>

inline uint32_t millis() {
    static auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
    return static_cast<uint32_t>(elapsed);
}

inline void delay(uint32_t ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
#endif
