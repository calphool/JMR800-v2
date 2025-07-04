/**
 * @file Logging.h
 * @brief Lightweight logging utility for serial output in the JMR800-V2 project.
 *
 * Provides basic log-level filtering and convenience methods for printing to the
 * serial console, allowing consistent diagnostics across the application.
 */

#pragma once

#ifdef TARGET_TEENSY
#include <Arduino.h>
#endif
#include "defines.h"  // Ensure LogLevel is available project-wide



/**
 * @brief Threshold for filtering messages by severity.
 *
 * Only messages with a severity equal to or higher than this value will be printed.
 * Adjust this constant to control global log verbosity.
 */
constexpr LogLevel LOG_LEVEL_THRESHOLD = LOG_INFO;

/**
 * @brief Timestamp of the last attempt to initialize serial logging.
 *
 * Used to throttle or time retry attempts in serial setup code.
 */
extern unsigned long lastSerialInitAttemptMillis;

/**
 * @brief Logs a message with an explicit log level.
 * 
 * @param level The severity of the message.
 * @param message The message string to be logged.
 */
void log(LogLevel level, const char* message, const char* _func_name);

/**
 * @brief Logs a message with an explicit log level.
 * 
 * @param level The severity of the message.
 * @param message The message string to be logged.
 */
void log(const char* message, const char* _func_name); // defaults to VERBOSE
