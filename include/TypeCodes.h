#pragma once

#include <stdint.h>

#define NUM_TYPE_CODES 9

#define TYPE_CODE_RANGE         0
#define TYPE_CODE_WAVE_FORM     1
#define TYPE_CODE_3_2_1_OFF     2
#define TYPE_CODE_0_TO_10       3
#define TYPE_CODE_OCTAVE        4
#define TYPE_CODE_MODE          5
#define TYPE_CODE_ENV2_GATE     6
#define TYPE_CODE_LFO_WAVE_FORM 7
#define TYPE_CODE_2_1_OFF       8

typedef struct {
  char typeCodeName[15];
  uint8_t cd;
} typeCode;

extern const typeCode typeCodes[NUM_TYPE_CODES];
