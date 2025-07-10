/**
 * @file TeensyHardwareManager.cpp
 * @brief Implements the TeensyHardwareManager class which controls all low-level hardware interfaces.
 * 
 * This includes initialization of ADC, encoder, digital buttons, shift registers, MUX selection logic,
 * EEPROM configuration storage, and serial parameter transmission to the Roland JX-8P synthesizer.
 */

#ifdef TARGET_TEENSY


#include "IHardwareManager.h"
#include "defines.h"
#include "Logging.h"
#include <EEPROM.h>
#include "TeensyHardwareManager.h"
volatile int TeensyHardwareManager::bitIndex = -1;
volatile uint16_t TeensyHardwareManager::sendBuffer = 0;




/**
 * @brief Initializes all hardware subsystems.
 * 
 * Configures pin modes, initializes the encoder, sets up ADC parameters, and attaches necessary interrupts.
 * Also initializes the MUX channel state and LED register system.
 */
void TeensyHardwareManager::init() {
    log(LOG_VERBOSE, "Entering TeensyHardwareManager->init()", __func__);
    
    EEPROM.begin();
    loadKnobs(); 

    encoderKnob = new Encoder(ENCODER_CLK_PIN, ENCODER_DT_PIN);

    pinMode(PUSH_BTN_SW4_PIN, INPUT_PULLUP);
    pinMode(PUSH_BTN_SW3_PIN, INPUT_PULLUP);
    pinMode(PUSH_BTN_SW2_PIN, INPUT_PULLUP);
    pinMode(PUSH_BTN_SW1_PIN, INPUT_PULLUP);

    pinMode(ENCODER_SW_PIN,   INPUT_PULLUP);
    pinMode(ENCODER_CLK_PIN,  INPUT);
    pinMode(ENCODER_DT_PIN,   INPUT);

    pinMode(SHIFT_REG_SER,    OUTPUT);
    pinMode(SHIFT_REG_SRCLK,  OUTPUT);
    pinMode(SHIFT_REG_RCLK,   OUTPUT);

    for(uint i = 0; i < NUM_MUXES; i++) {
      pinMode(this->enablePins[i], OUTPUT);             // set enable pins as output
      digitalWrite(this->enablePins[i], HIGH);          // disable enable pins
    }
    for(uint i = 0; i < 4; i++) {
      pinMode(this->MuxAddressPins[i], OUTPUT);         // set mux pins as output
      digitalWrite(this->MuxAddressPins[i], LOW);       // set them all low
    }

    pinMode(READYOUT_PIN, OUTPUT);
    digitalWrite(READYOUT_PIN, LOW);

    pinMode(CLOCK_IN_PIN, INPUT_PULLUP);

    pinMode(DATA_OUT_PIN, OUTPUT);
    digitalWrite(READYOUT_PIN, LOW);

    log(LOG_VERBOSE, "Setting up ADC...", __func__);

    adc.adc0->setAveraging(4);
    adc.adc0->setConversionSpeed(ADC_CONVERSION_SPEED::MED_SPEED);
    adc.adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED); 

    log(LOG_VERBOSE, "Attaching interrupts...", __func__);

    attachInterrupt(digitalPinToInterrupt(CLOCK_IN_PIN), TeensyHardwareManager::onPG800ClockFall, FALLING);

    log(LOG_VERBOSE, "Setting up DAC tables...", __func__);

    for(uint8_t i=0; i< NUM_MUXES; i++) {
        for(uint8_t j=0;j<16;j++) {
            AnalogValues[j][i] = 0;
            oldAnalogValues[j][i] = 0;
        }
    }

    ledstate = 0b00000000;

    log(LOG_INFO, "Initialized.", __func__);
}

/**
 * @brief Destructor for TeensyHardwareManager, responsible for freeing allocated memory.
 */
TeensyHardwareManager::~TeensyHardwareManager() {
  delete encoderKnob;
}

/**
 * @brief Retrieves the encoder position as an integer scaled to a 0..divisor range.
 * 
 * @param divisor Maximum number for the scaled value.
 * @return long Scaled encoder position.
 */
long TeensyHardwareManager::getEncoderZeroTo(long divisor) {
    long i = (lastEncoderPosition>>2) % divisor; 
    if(i < 0)
      i = 0;
    if(i > divisor)
      i = divisor-1;
    
    return i;
}

/**
 * @brief Loads knob configuration data from EEPROM.
 * 
 * Restores knob mappings and names from persistent storage, padding names if necessary.
 */
void TeensyHardwareManager::loadKnobs() {
  for (int i = 0; i < NUM_KNOBS; i++) {
    int addr = i * sizeof(knobConfig);
    EEPROM.get(addr, knobConfigurations[i]);
    while(strlen(knobConfigurations[i].name) < 14)
      strcat(knobConfigurations[i].name," ");
  }

  memcpy(knobConfigurations_bkup, knobConfigurations, sizeof(knobConfigurations));
}

/**
 * @brief Saves knob configurations to EEPROM only if modified.
 */
void TeensyHardwareManager::saveKnobs() {
  for (int i = 0; i < NUM_KNOBS; i++) {
    while(strlen(knobConfigurations[i].name) < 14)
      strcat(knobConfigurations[i].name," ");
    int addr = i * sizeof(knobConfig);

    if(knobChanged(i)) { // optimization that reduces wear on EEPROM
      EEPROM.put(addr, knobConfigurations[i]);
      char buf[40];
      sprintf(buf, "knob changed:     %d", i);
      log(LOG_INFO, buf, __func__);
    }
    else {
      char buf[40];
      sprintf(buf, "knob not changed:     %d", i);
      log(LOG_VERBOSE, buf, __func__);
    }
  }
  memcpy(knobConfigurations_bkup, knobConfigurations, sizeof(knobConfigurations));
}


/**
 * @brief Compares current and backup knob configurations to detect changes.
 * 
 * @param i Index of the knob configuration to compare.
 * @return true if the configuration has changed.
 */
bool TeensyHardwareManager::knobChanged(int i) const {
  return memcmp(&knobConfigurations[i], &knobConfigurations_bkup[i], sizeof(knobConfig)) != 0;
}


/**
 * @brief Main loop that periodically polls input states.
 */
void TeensyHardwareManager::loop() {
    if (millis() - lastPollTime < TeensyHardwareManagerPollIntervalMS) return;
    lastPollTime = millis();

    gatherControlSettings();
}


static const uint knobXformer[] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
 32,33,34,35,36,37,38,39,40,41, 42, 43, 44, 45, 46, 47,
 16,17,18,19,20,21,22,23,24,25, 26, 27, 28, 29, 30, 31,
 48,49,50,51,52,53,54,55
};


/**
 * @brief Returns a transformed and smoothed ADC value for the specified knob.
 * 
 * @param knobIX Logical knob index.
 * @return int Smoothed value from 0-127.
 */
int TeensyHardwareManager::getKnobValue(uint knobIX) {
  knobIX = knobXformer[knobIX];              // transform this index (necessary because of wiring) 
  uint mux = knobIX >> 4;
  uint mux_ix = knobIX & 0x0F;

  return this->AnalogValues[mux_ix][mux] >> 1;
}

/**
 * @brief Checks whether a specified button is currently pressed.
 * 
 * @param index Button index.
 * @return true if pressed, false otherwise.
 */
bool TeensyHardwareManager::isButtonPressed(uint index) {
    if(index >= NUM_BUTTONS) {
      log(LOG_ERROR, "TeensyHardwareManager::isButtonPressed() invoked with index larger than number of defined buttons.", __func__);
      return buttonStates[0];
    }

    return buttonStates[index];
}


/**
 * @brief  Polls all control surfaces: knobs, buttons, encoder - performs a full scan of potentiometers, buttons, and 
 * encoder state for UI polling
 * 
 * This method reads the current state of all buttons, the encoder switch, and the potentiometer values.
 * It updates the internal state variables accordingly, including previous states for change detection.
 * It also updates the encoder position and checks for any changes in button states.
 * This method is typically called periodically to gather the latest control settings.
 */
void TeensyHardwareManager::gatherControlSettings() {
  this->gatherPotentiometerValues();

  for(int i = 0; i < NUM_BUTTONS; i++)
    prevButtonStates[i] = buttonStates[i];

  for (int i = 0; i < NUM_BUTTONS; i++)
    buttonStates[i] = !digitalRead(buttonPins[i]); // Active low

  bPrevEncoderBtn = bEncoderBtn;
  bEncoderBtn = !digitalRead(ENCODER_SW_PIN);
  this->updateEncoder();  
}


/**
 * @brief Returns true if the red LED is currently lit for the given button.
 * 
 * @param buttonId Logical button ID.
 * @return true if lit.
 */
bool TeensyHardwareManager::redIsLit(uint buttonId) {
  if(buttonId >= NUM_BUTTONS) {
    log(LOG_ERROR, "invalid button number passed to TeensyHardwareManager::redIsLit()", __func__);
    return false;
  }
  
  buttonId = NUM_BUTTONS - 1 - buttonId;
  uint bitPos = buttonId * 2; // Red LED bit
  return (ledstate & (1 << bitPos)) != 0;
}


/**
 * @brief Returns true if the green LED is currently lit for the given button.
 * 
 * @param buttonId Logical button ID.
 * @return true if lit.
 */
bool TeensyHardwareManager::greenIsLit(uint buttonId) {
  if(buttonId >= NUM_BUTTONS) {
    log(LOG_ERROR, "invalid button number passed to TeensyHardwareManager::greenIsLit()", __func__);
    return false;
  }

  buttonId = NUM_BUTTONS - 1 - buttonId;
  uint bitPos = buttonId * 2 + 1; // Green LED bit
  return (ledstate & (1 << bitPos)) != 0;
}


void TeensyHardwareManager::clearEncoderButton() {
  bPrevEncoderBtn = bEncoderBtn = false;
}

void TeensyHardwareManager::setKnobConfiguration(uint knobIndex, const char* name, uint8_t cmdbyte, uint8_t typecode) {
  if(knobIndex >= NUM_KNOBS) {
    log(LOG_ERROR, "invalid knob number passed to TeensyHardwareManager::setKnobConfiguration()", __func__);
    return;
  }

  strncpy(knobConfigurations[knobIndex].name, name, sizeof(knobConfigurations[knobIndex].name) - 1);
  knobConfigurations[knobIndex].name[sizeof(knobConfigurations[knobIndex].name) - 1] = '\0'; // Ensure null termination
  knobConfigurations[knobIndex].cmdbyte = cmdbyte;
  knobConfigurations[knobIndex].typecode = typecode;
}

/**
 * @brief Checks if the encoder switch changed state in the specified direction.
 * 
 * @param upThenDown Whether to detect a falling or rising edge.
 * @param clearFlag Whether to reset internal edge-tracking state.
 * @return true if state change matches the condition.
 */
bool TeensyHardwareManager::encoderSwitchStateChanged(bool upThenDown, bool clearFlag) {
  if(bEncoderBtn != bPrevEncoderBtn) {
    if(upThenDown) {
      if(bEncoderBtn == LOW && bPrevEncoderBtn == HIGH) {
        if(clearFlag) bPrevEncoderBtn = bEncoderBtn;
        return true;
      }
    }
    else {
      if(bEncoderBtn == HIGH && bPrevEncoderBtn == LOW) {
        if(clearFlag) bPrevEncoderBtn = bEncoderBtn;
        return true;
      }
    }
  }
  return false;
}


/**
 * @brief Checks if a specified button changed state in the given direction.
 * 
 * @param index Button index.
 * @param upThenDown Detect up->down or down->up transition.
 * @param clearFlag Whether to clear the state-tracking flag.
 * @return true if transition occurred.
 */
bool TeensyHardwareManager::buttonStateChanged(uint index, bool upThenDown, bool clearFlag) {
  if(index >= NUM_BUTTONS) {
    log(LOG_ERROR, "invalid button number passed to TeensyHardwareManager::buttonStateChanged()", __func__);
    return false;
  }

  if(buttonStates[index] != prevButtonStates[index]) {
    if(upThenDown) {
      if(buttonStates[index] == LOW && prevButtonStates[index] == HIGH) {
        if(clearFlag) prevButtonStates[index] = buttonStates[index];
        return true; 
      }
    }
    else {
      if(buttonStates[index] == HIGH && prevButtonStates[index] == LOW) {
        if(clearFlag) prevButtonStates[index] = buttonStates[index];        
        return true;
      }
    }
  }

  return false;
}


/**
 * @brief Scans all multiplexer channels and updates smoothed analog values.
 * 
 * iterates through all MUX channels to update smoothed ADC values for each knob
 * This method is called periodically to gather the latest potentiometer values.
 * It reads each channel, applies smoothing, and stores the results in AnalogValues.
 * It also maintains a backup of the last values for comparison.
 */
void TeensyHardwareManager::gatherPotentiometerValues() {
  iGatherCtr++;
  lastPotScanTime = millis();
  for(uint8_t muxCtr = 0; muxCtr < NUM_MUXES; muxCtr++) {
      digitalWrite(this->enablePins[muxCtr], LOW);  // enable this mux
      for(uint8_t addr = 0; addr < 16; addr++) {
        this->setAddressPins(addr);
        uint16_t raw = adc.adc0->analogRead(analogInPins[muxCtr]);
        raw = raw >> 2;          // (1024 / 4 = 256)
        raw = ((this->AnalogValues[addr][muxCtr]) * 7 + raw) >> 3;  // smoothed value (multiplies original value by 7 then adds new, then averages)
        this->AnalogValues[addr][muxCtr] = raw;
        if(iGatherCtr % 10 == 0) // store a copy of the current AnalogValues() periodically
            oldAnalogValues[addr][muxCtr] = this->AnalogValues[addr][muxCtr];
      }
      digitalWrite(this->enablePins[muxCtr], HIGH); // disable this mux
      delayMicroseconds(5);
  }
}


/**
 * @brief Writes a 4-bit address to the MUX selector pins to select an analog channel
 * 
 * @param val 4-bit address.
 */
void TeensyHardwareManager::setAddressPins(uint val) {
  if(val & 0x01) digitalWrite(S0_PIN, HIGH); else digitalWrite(S0_PIN, LOW);
  if(val & 0x02) digitalWrite(S1_PIN, HIGH); else digitalWrite(S1_PIN, LOW);
  if(val & 0x04) digitalWrite(S2_PIN, HIGH); else digitalWrite(S2_PIN, LOW);
  if(val & 0x08) digitalWrite(S3_PIN, HIGH); else digitalWrite(S3_PIN, LOW);
  delayMicroseconds(5);
}

/**
 * @brief Resets the encoder to a specified position.
 * 
 * @param i New encoder value.
 */
void TeensyHardwareManager::resetEncoder(uint i) {
  encoderKnob->write(i);
  lastEncoderPosition = i;
}

int TeensyHardwareManager::AsciiToEncoder(char c) {
  int v = ((((int)c) - 65) * 4) + 132;
  return v;
}


/**
 * @brief Reads and updates the encoder position, logs movement.
 */
void TeensyHardwareManager::updateEncoder() {
  long newPosition = encoderKnob->read();

  if(newPosition < 0) {
    encoderKnob->write(0);
    newPosition = 0;
  }

  if (newPosition != lastEncoderPosition) {
    if (newPosition > lastEncoderPosition) {
      char buf[40];
      sprintf(buf, "CCW %ld", lastEncoderPosition>>2);
      log(LOG_VERBOSE, buf, __func__);
    } else {
      char buf[40];
      sprintf(buf, "CW %ld", lastEncoderPosition>>2);
      log(LOG_VERBOSE, buf, __func__);
    }

    lastEncoderPosition = newPosition;
  }
}

/**
 * @brief Gets the raw encoder value.
 * 
 * @return long Internal encoder position.
 */
long TeensyHardwareManager::getEncoderValue() {
  return lastEncoderPosition;
}


long TeensyHardwareManager::getEncoderModdedBy(long divisor) {
    long i = (lastEncoderPosition>>2) % divisor; 
    if(i < 0)
      i = 0;
    if(i > divisor)
      i = divisor-1;
    
    return i;
}


/**
 * @brief Sends LED register data to the hardware shift register.
 */
void TeensyHardwareManager::writeLedRegistersToHardware() const {
  // make sure to check Serial before doing any logging here.  This is a low level method that gets used 
  // before we know that logging is available.

  byte data = ledstate;

  digitalWrite(SHIFT_REG_RCLK, LOW);  // Start by disabling latch

  // Shift out 8 bits, MSB first
  for (int8_t i = 7; i >= 0; i--) {
    digitalWrite(SHIFT_REG_SRCLK, LOW);  // Prepare for clock

    bool bitVal = data & (1 << i);       // Extract current bit
    digitalWrite(SHIFT_REG_SER, bitVal); // Set data line

    digitalWrite(SHIFT_REG_SRCLK, HIGH); // Rising edge clocks bit into register
  }

  digitalWrite(SHIFT_REG_RCLK, HIGH);    // Latch data to output pins
}



/**
 * @brief encodes and queues a PG-800-style message for bit-serialized transmission to the JX-8P synth with deduplication and timeout control
 * 
 * @param paramID JX-8P parameter ID.
 * @param value Parameter value.
 */
void TeensyHardwareManager::sendParameter(uint8_t paramID, uint8_t value) {
  unsigned long now = millis();

  // Remove expired entries
  this->recentCommands.erase(
    std::remove_if(recentCommands.begin(), recentCommands.end(),
      [now](const SentCommand& cmd) {
        return now - cmd.timestamp > COMMAND_TIMEOUT;
      }),
    this->recentCommands.end()
  );

  // Check for duplicate
  for (const auto& cmd : recentCommands) {
    if (cmd.paramID == paramID && cmd.value == value) {
      return;  // Duplicate within timeout window; do not resend
    }
  }

  // Record this command
  recentCommands.push_back({paramID, value, now});

  while (bitIndex != -1 && (millis() - now < 250) );  // Wait until previous transfer is complete
  if(millis() - now >= 250) {
    log(LOG_ERROR, "problem waiting for bitIndex in sendParameter()", __func__);
    return;
  }

  noInterrupts();
  sendBuffer = ((uint16_t)paramID << 8) | value;
  bitIndex = 15;
  bool bit = (sendBuffer >> bitIndex) & 1;
  digitalWrite(DATA_OUT_PIN, bit ? LOW : HIGH);
  bitIndex--;
  
  digitalWrite(READYOUT_PIN, HIGH);  // Begin transmission
  //delayMicroseconds(5);
  interrupts();
}


/**
 * @brief interrupt routine triggered by a falling clock edge, used to clock out serial bits one by one to the JX-8P using inverted logic signaling
 * 
 * This function is called when the clock pin goes low, indicating that the next bit should be sent.
 * It shifts out the next bit from the sendBuffer and updates the DATA_OUT_PIN accordingly.
 * If all bits have been sent, it resets the bitIndex and sets the READYOUT_PIN low to indicate the end of transmission.
 * This function is designed to be called in an interrupt context, so it should be as fast as possible.
 */
void TeensyHardwareManager::onPG800ClockFall() {
  if (bitIndex < 0) return;  // Not currently sending

  delayMicroseconds(10);
  // DATA is negative logic, so we invert the bit
  bool bit = (sendBuffer >> bitIndex) & 1;
  digitalWrite(DATA_OUT_PIN, bit ? LOW : HIGH);  // inverted logic

  bitIndex--;
  if (bitIndex < 0) {
    delayMicroseconds(5);
    digitalWrite(READYOUT_PIN, LOW);           // End of transfer
    delayMicroseconds(10);
    digitalWrite(DATA_OUT_PIN, LOW);           // Idle state
  }
}

/**
 * @brief Returns the current state of the encoder switch.
 */
bool TeensyHardwareManager::getEncoderSwitchStatus() {
  return bEncoderBtn;
}

/**
 * @brief Saves the current LED state to a backup register.
 */
void TeensyHardwareManager::saveLedState() {
  prevLedState = ledstate;
}

/**
 * @brief Restores the LED state from the previously saved value.
 */
void TeensyHardwareManager::restoreLedState() {
  ledstate = prevLedState;
}

/**
 * @brief Retrieves the configuration of a knob at a given index.
 * 
 * @param index Knob index.
 * @return knobConfig Copy of the configuration struct.
 */
knobConfig TeensyHardwareManager::getKnobConfiguration(uint index) {
  if(index >= NUM_KNOBS) {
    log(LOG_ERROR, "TeensyHardwareManager::getKnobConfiguration() invoked with an invalid knob index", __func__);
    return knobConfigurations[NUM_KNOBS % index];
  }
  return knobConfigurations[index];
}

/**
 * @brief Sets the red and green lights of a button by index.
 * 
 * @param buttonId Logical ID of the button.
 * @param red Whether to turn on the red LED.
 * @param green Whether to turn on the green LED.
 */
void TeensyHardwareManager::setButtonLights(uint buttonId, bool red, bool green) {
  // all logging here must check whether Serial is defined because this 
  // code gets used in the logging setup
  if(buttonId >= NUM_BUTTONS) {
    if(Serial) log(LOG_ERROR, "TeensyHardwareManager::setButtonLights() invoked with an invalid button id", __func__);
    return;
  }

  buttonId = NUM_BUTTONS - 1 - buttonId;

  int shift = buttonId * 2;

  // Clear the two bits for this button
  ledstate &= ~(0b11 << shift);

  // Pack red (MSB) and green (LSB) into two bits
  uint8_t bitsToSet = ((green ? 1 : 0) << 1) | (red ? 1 : 0);

  // Set the new bits at the proper location
  ledstate |= (bitsToSet << shift);
  writeLedRegistersToHardware();
}

bool TeensyHardwareManager::knobValueChanged(uint knobIX) {
    knobIX = knobXformer[knobIX];  // Transform this index (necessary because of wiring) 
    uint mux = knobIX >> 4;
    uint mux_ix = knobIX & 0x0F;

    uint16_t currentValue = AnalogValues[mux_ix][mux];
    uint16_t oldValue = oldAnalogValues[mux_ix][mux];

    // Apply hysteresis (ignore small changes)
    int delta = (int)currentValue - (int)oldValue;
    if (abs(delta) < 4)  // Adjust the threshold as needed (3-10 is typical)
        return false;

    // Update the stored value if the change is significant
    oldAnalogValues[mux_ix][mux] = currentValue;
    return true;
}


void TeensyHardwareManager::sendParameterToSynth(uint knob)
{
    knobConfig cfg = this->getKnobConfiguration(knob);
    if (cfg.cmdbyte == 0x00)
        return;

    uint8_t kv = getKnobValue(knob);

    switch (cfg.typecode)
    {
        case TYPE_CODE_0_TO_10:
        case TYPE_CODE_OCTAVE:
            // Reverse value: 0 = max, 127 = min
            this->sendParameter(cfg.cmdbyte, 127 - kv);
            return;

        case TYPE_CODE_LFO_WAVE_FORM:
        case TYPE_CODE_2_1_OFF:
            if (kv < 32)
                this->sendParameter(cfg.cmdbyte, 16);   // Random / OFF
            else if (kv < 64)
                this->sendParameter(cfg.cmdbyte, 48);   // Square / 1
            else
                this->sendParameter(cfg.cmdbyte, 96);   // Triangle / 2
            return;

        case TYPE_CODE_ENV2_GATE:
            if (kv < 64)
                this->sendParameter(cfg.cmdbyte, 32);   // Gate
            else
                this->sendParameter(cfg.cmdbyte, 96);   // Normal
            return;

        case TYPE_CODE_RANGE:
        case TYPE_CODE_WAVE_FORM:
        case TYPE_CODE_3_2_1_OFF:
        case TYPE_CODE_MODE:
            if (kv < 32)
                this->sendParameter(cfg.cmdbyte, 16);   // Range 16', Noise, OFF, Env-2-Inverted
            else if (kv < 64)
                this->sendParameter(cfg.cmdbyte, 48);   // Range 8', Sawtooth, 1, Env-2-Normal
            else if (kv < 96)
                this->sendParameter(cfg.cmdbyte, 80);   // Range 4', Pulse, 2, Env-1-Inverted
            else
                this->sendParameter(cfg.cmdbyte, 112);  // Range 2', Square, 3, Env-1-Normal
            return;

        default:
            // Fallback: pass raw value (no transformation)
            this->sendParameter(cfg.cmdbyte, kv);
            return;
    }
}


#endif