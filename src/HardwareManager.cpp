#include <EEPROM.h>
#include "HardwareManager.h"
#include "defines.h"
#include "Logging.h"


// Global instance
HardwareManager hardware;

volatile int HardwareManager::bitIndex = -1;
volatile uint16_t HardwareManager::sendBuffer = 0;


/* --------------------------------------------------------------
   |  init -- performs one-time initialization of all hardware   |
   |  subsystems: ADC, encoder, buttons, shift registers, MUXes |
   -------------------------------------------------------------- */
void HardwareManager::init() {
    log(LOG_VERBOSE, "Entering HardwareManager->init()");
    
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

    log(LOG_VERBOSE, "Setting up ADC...");

    adc.adc0->setAveraging(4);
    adc.adc0->setConversionSpeed(ADC_CONVERSION_SPEED::MED_SPEED);
    adc.adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED); 

    log(LOG_VERBOSE, "Attaching interrupts...");

    attachInterrupt(digitalPinToInterrupt(CLOCK_IN_PIN), HardwareManager::onPG800ClockFall, FALLING);

    log(LOG_VERBOSE, "Setting up DAC tables...");

    for(uint8_t i=0; i< NUM_MUXES; i++) {
        for(uint8_t j=0;j<16;j++) {
            AnalogValues[j][i] = 0;
            oldAnalogValues[j][i] = 0;
        }
    }

    ledstate = 0b00000000;

    log(LOG_INFO, "Initialized.");
}

HardwareManager::~HardwareManager() {
  delete encoderKnob;
}


/* .------------------------------------------------------------.
   |  loadKnobs() - loads knob settings from EEPROM             |
   '------------------------------------------------------------' */
void HardwareManager::loadKnobs() {
  for (int i = 0; i < NUM_KNOBS; i++) {
    int addr = i * sizeof(knobConfig);
    EEPROM.get(addr, knobConfigurations[i]);
    while(strlen(knobConfigurations[i].name) < 14)
      strcat(knobConfigurations[i].name," ");
  }

  memcpy(knobConfigurations_bkup, knobConfigurations, sizeof(knobConfigurations));
}

/* .-------------------------------------------------------------------.
   |  saveKnobs() - saves knob settings to EEPROM if they have changed |
   '-------------------------------------------------------------------' */
void HardwareManager::saveKnobs() {
  for (int i = 0; i < NUM_KNOBS; i++) {
    while(strlen(knobConfigurations[i].name) < 14)
      strcat(knobConfigurations[i].name," ");
    int addr = i * sizeof(knobConfig);

    if(knobChanged(i)) { // optimization that reduces wear on EEPROM
      EEPROM.put(addr, knobConfigurations[i]);
      log(LOG_INFO, "knob changed:     " + String(i));
    }
    else {
      log(LOG_VERBOSE, "knob not changed: " + String(i));
    }
  }
  memcpy(knobConfigurations_bkup, knobConfigurations, sizeof(knobConfigurations));
}

// check if the knobconfiguration has changed at this location
bool HardwareManager::knobChanged(int i) {
  return memcmp(&knobConfigurations[i], &knobConfigurations_bkup[i], sizeof(knobConfig)) != 0;
}


/* --------------------------------------------------------------
   |  loop -- periodically invokes control polling logic         |
   |  according to the configured polling interval               |
   -------------------------------------------------------------- */
void HardwareManager::loop() {
    if (millis() - lastPollTime < hardwareManagerPollIntervalMS) return;
    lastPollTime = millis();

    gatherControlSettings();
}


static const uint knobXformer[] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
 32,33,34,35,36,37,38,39,40,41, 42, 43, 44, 45, 46, 47,
 16,17,18,19,20,21,22,23,24,25, 26, 27, 28, 29, 30, 31,
 48,49,50,51,52,53,54,55
};


/* --------------------------------------------------------------
   |  getKnobValue -- returns a smoothed ADC value for the       |
   |  specified logical knob index, transformed through wiring   |
   -------------------------------------------------------------- */
int HardwareManager::getKnobValue(uint knobIX) {
  knobIX = knobXformer[knobIX];              // transform this index (necessary because of wiring) 
  uint mux = knobIX >> 4;
  uint mux_ix = knobIX & 0x0F;

  return 255-(this->AnalogValues[mux_ix][mux]);
}

/* --------------------------------------------------------------
   |  isButtonPressed -- returns debounced state of a button     |
   |  specified by logical index (0 through NUM_BUTTONS - 1)     |
   -------------------------------------------------------------- */
bool HardwareManager::isButtonPressed(uint index) {
    if(index >= NUM_BUTTONS) {
      log(LOG_ERROR, "HardwareManager::isButtonPressed() invoked with index larger than number of defined buttons.");
      return buttonStates[0];
    }

    return buttonStates[index];
}


/* --------------------------------------------------------------
   |  gatherControlSettings -- performs a full scan of           |
   |  potentiometers, buttons, and encoder state for UI polling  |
   -------------------------------------------------------------- */
void HardwareManager::gatherControlSettings() {
  this->gatherPotentiometerValues();

  for(int i = 0; i < NUM_BUTTONS; i++)
    prevButtonStates[i] = buttonStates[i];

  for (int i = 0; i < NUM_BUTTONS; i++)
    buttonStates[i] = !digitalRead(buttonPins[i]); // Active low

  bPrevEncoderBtn = bEncoderBtn;
  bEncoderBtn = !digitalRead(ENCODER_SW_PIN);
  this->updateEncoder();  
}

bool HardwareManager::redIsLit(uint buttonId) {
  if(buttonId >= NUM_BUTTONS) {
    log(LOG_ERROR, "invalid button number passed to HardwareManager::redIsLit()");
    return false;
  }
  
  buttonId = NUM_BUTTONS - 1 - buttonId;
  uint bitPos = buttonId * 2; // Red LED bit
  return (ledstate & (1 << bitPos)) != 0;
}

bool HardwareManager::greenIsLit(uint buttonId) {
  if(buttonId >= NUM_BUTTONS) {
    log(LOG_ERROR, "invalid button number passed to HardwareManager::greenIsLit()");
    return false;
  }

  buttonId = NUM_BUTTONS - 1 - buttonId;
  uint bitPos = buttonId * 2 + 1; // Green LED bit
  return (ledstate & (1 << bitPos)) != 0;
}

bool HardwareManager::buttonStateChanged(uint index, bool upThenDown, bool clearFlag) {
  if(index >= NUM_BUTTONS) {
    log(LOG_ERROR, "invalid button number passed to HardwareManager::buttonStateChanged()");
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


/* --------------------------------------------------------------
   |  gatherPotentiometerValues -- iterates through all MUX      |
   |  channels to update smoothed ADC values for each knob       |
   -------------------------------------------------------------- */
void HardwareManager::gatherPotentiometerValues() {
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

/* --------------------------------------------------------------
   |  setAddressPins -- writes a 4-bit address to the MUX        |
   |  selector pins to select an analog channel                  |
   -------------------------------------------------------------- */
void HardwareManager::setAddressPins(uint val) {
  if(val & 0x01) digitalWrite(S0_PIN, HIGH); else digitalWrite(S0_PIN, LOW);
  if(val & 0x02) digitalWrite(S1_PIN, HIGH); else digitalWrite(S1_PIN, LOW);
  if(val & 0x04) digitalWrite(S2_PIN, HIGH); else digitalWrite(S2_PIN, LOW);
  if(val & 0x08) digitalWrite(S3_PIN, HIGH); else digitalWrite(S3_PIN, LOW);
  delayMicroseconds(5);
}


/* --------------------------------------------------------------
   |  updateEncoder -- reads and normalizes the encoder position |
   |  and logs directional movement (CW / CCW) if changed        |
   -------------------------------------------------------------- */
void HardwareManager::updateEncoder() {
  long newPosition = encoderKnob->read();

  if(newPosition < 0) {
    encoderKnob->write(0);
    newPosition = 0;
  }

  if (newPosition != lastEncoderPosition) {
    if (newPosition > lastEncoderPosition) {
      log(LOG_VERBOSE, "CCW " + String(lastEncoderPosition>>2));
    } else {
      log(LOG_VERBOSE, "CW  " + String(lastEncoderPosition>>2));
    }

    lastEncoderPosition = newPosition;
  }
}

long HardwareManager::getEncoderValue() {
  return lastEncoderPosition;
}


/* --------------------------------------------------------------
   |  setLEDs -- shifts out a single 8-bit value to the button   |
   |  LED shift register to update their visual state            |
   -------------------------------------------------------------- */
void HardwareManager::writeLedRegistersToHardware() {
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


/* --------------------------------------------------------------
   |  sendParameter -- encodes and queues a PG-800-style message |
   |  for bit-serialized transmission to the JX-8P synth         |
   |  with deduplication and timeout control                     |
   -------------------------------------------------------------- */
void HardwareManager::sendParameter(uint8_t paramID, uint8_t value) {
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
    log(LOG_ERROR, "problem waiting for bitIndex in sendParameter()");
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

/* --------------------------------------------------------------
   |  onPG800ClockFall -- interrupt routine triggered by a       |
   |  falling clock edge, used to clock out serial bits one by   |
   |  one to the JX-8P using inverted logic signaling             |
   -------------------------------------------------------------- */
void HardwareManager::onPG800ClockFall() {
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

bool HardwareManager::getEncoderSwitchStatus() {
  return bEncoderBtn;
}

void HardwareManager::saveLedState() {
  prevLedState = ledstate;
}

void HardwareManager::restoreLedState() {
  ledstate = prevLedState;
}

void HardwareManager::setButtonLights(uint buttonId, bool red, bool green) {
  // all logging here must check whether Serial is defined because this 
  // code gets used in the logging setup
  if(buttonId >= NUM_BUTTONS) {
    if(Serial) log(LOG_ERROR, "HardwareManager::setButtonLights() invoked with an invalid button id");
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