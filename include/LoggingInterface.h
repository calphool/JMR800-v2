/**
 * @file LoggingInterface.h
 * @brief Lightweight logging utility for serial output in the JMR800-V2 project.
 *
 * Provides basic log-level filtering and convenience methods for printing to the
 * serial console, allowing consistent diagnostics across the application.
 */

#pragma once
#include "defines.h"  // Ensure LogLevel is available project-wide

class LoggingInterface {
/**
 * @brief Logs a message with an explicit log level.
 * 
 * @param level The severity of the message.
 * @param message The message string to be logged.
 */
static void log(LogLevel level, const char* message, const char* func);

/**
 * @brief Logs a message with an explicit log level.
 * 
 * @param level The severity of the message.
 * @param message The message string to be logged.
 */
static void log(const char* message, const char* func);

};