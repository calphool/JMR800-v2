/**
 * @file Logging.h
 * @brief Lightweight logging utility for serial output in the JMR800-V2 project.
 *
 * Provides basic log-level filtering and convenience methods for printing to the
 * serial console, allowing consistent diagnostics across the application.
 */

#pragma once
#include <Arduino.h>
#include "defines.h"  // Ensure LogLevel is available project-wide

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
void log(LogLevel level, const String& message, const char* func);

/**
 * @brief Logs a message with an explicit log level.
 * 
 * @param level The severity of the message.
 * @param message The message string to be logged.
 */
void log(const String& message, const char* func); // defaults to VERBOSE
