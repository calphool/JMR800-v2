#pragma once
#include <Arduino.h>
#include "defines.h"  // or wherever LogLevel is defined


enum LogLevel {
    LOG_VERBOSE,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

// Change this to filter messages below a certain level
constexpr LogLevel LOG_LEVEL_THRESHOLD = LOG_INFO;

extern unsigned long lastSerialInitAttemptMillis;


void log(LogLevel level, const String& message);
void log(const String& message); // defaults to INFO
