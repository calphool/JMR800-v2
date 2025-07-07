#include "screens/RunModeWidgets.h"

#include "widgets/RunModeWidget0to10.h"
#include "widgets/RunModeWidgetRange.h"
#include "widgets/RunModeWidgetWaveform.h"
#include "widgets/RunModeWidgetOctave.h"
#include "widgets/RunModeWidget321Off.h"
#include "widgets/RunModeWidgetEnvMode.h"
#include "widgets/RunModeWidgetEnv2Gate.h"
#include "widgets/RunModeWidgetLFOWave.h"
#include "widgets/RunModeWidget21Off.h"

std::unique_ptr<CenterWidget> makeWidget(uint8_t typecode) {
    switch (typecode) {
        case TYPE_CODE_0_TO_10:       return std::make_unique<RunModeWidget0to10>();
        case TYPE_CODE_RANGE:         return std::make_unique<RunModeWidgetRange>();
        case TYPE_CODE_WAVE_FORM:     return std::make_unique<RunModeWidgetWaveform>();
        case TYPE_CODE_OCTAVE:        return std::make_unique<RunModeWidgetOctave>();
        case TYPE_CODE_3_2_1_OFF:     return std::make_unique<RunModeWidget321Off>();
        case TYPE_CODE_MODE:          return std::make_unique<RunModeWidgetEnvMode>();
        case TYPE_CODE_ENV2_GATE:     return std::make_unique<RunModeWidgetEnv2Gate>();
        case TYPE_CODE_LFO_WAVE_FORM: return std::make_unique<RunModeWidgetLFOWave>();
        case TYPE_CODE_2_1_OFF:       return std::make_unique<RunModeWidget21Off>();
        default:                      return nullptr;
    }
}
