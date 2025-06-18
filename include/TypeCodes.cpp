#include "TypeCodes.h"

const typeCode typeCodes[NUM_TYPE_CODES] = {
  [TYPE_CODE_RANGE]         = { "Range",         0x00 },
  [TYPE_CODE_WAVE_FORM]     = { "WaveForm",      0x01 },
  [TYPE_CODE_3_2_1_OFF]     = { "3-2-1-Off",     0x02 },
  [TYPE_CODE_0_TO_10]       = { "0 to 10",       0x03 },
  [TYPE_CODE_OCTAVE]        = { "Octave",        0x04 },
  [TYPE_CODE_MODE]          = { "Mode",          0x05 },
  [TYPE_CODE_ENV2_GATE]     = { "Env2 Gate",     0x06 },
  [TYPE_CODE_LFO_WAVE_FORM] = { "LFO Wave",      0x07 },
  [TYPE_CODE_2_1_OFF]       = { "2-1-Off",       0x08 }
};
