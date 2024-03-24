// define infrared buttons code
#define IR_UP 0x18
#define IR_DOWN 0x4A
#define IR_LEFT 0x10
#define IR_RIGHT 0x5A
#define IR_OK 0x38
#define IR_1 0xA2
#define IR_2 0x62
#define IR_3 0xE2
#define IR_0 0x98

#include "navi_subdisplay.h"
#include <iarduino_RTC.h>
#include <EEPROM.h>

#include <NecDecoder.h>
NecDecoder ir;

NaviSubDisplay subDisplay;

unsigned int CURRENT_TEXT = 0;
const unsigned int MAX_MESSAGE_LENGTH = 12;
const unsigned int MESSAGE_MASK_LENGTH = 3;                                     // length of search mask
byte MESSAGE_MASK[MESSAGE_MASK_LENGTH] = {46, 1, 8};                            // equivalent of 2E 1 8 in message, mask to search by
static byte CLIMATE_TEMP = 0;
static byte CLIMATE_ENABLED = 0;

iarduino_RTC watch(RTC_DS1307);

void setup() {
  EEPROM.get(0, CURRENT_TEXT);   
  attachInterrupt(1, irIsr, FALLING);
  bool parse = false;
  bool config = false;
  watch.begin();
  subDisplay.begin();
  subDisplay.clear();
  subDisplay.render();
  Serial.begin(9600);
}

void irIsr() {
  // callback for IR interrupt
  ir.tick();
}

void loop() {
  while (Serial.available() > 0) {
    static byte message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;
    byte inByte = Serial.read();
    if (!Serial.availableForWrite()) {
      Serial.flush();
    }
    Serial.write(inByte);
    if (message_pos < MESSAGE_MASK_LENGTH && inByte != MESSAGE_MASK[message_pos] ) {
      message_pos = 0;
      continue;
    }
    if (message_pos < MAX_MESSAGE_LENGTH - 1) {
      message[message_pos] = inByte;
      message_pos++;
    } else {
      CLIMATE_TEMP = message[5];
      CLIMATE_ENABLED = message[6];
      message_pos = 0;
    }
  }
  if (ir.available()) {
    ir_callback(ir.readCommand());
  }
  renderSubdisplay();
}


void renderSubdisplay() {
  watch.gettime();
  subDisplay.clear();
  if (CLIMATE_ENABLED != 0) {
    subDisplay.setClimatTemp(getClimateTemp(CLIMATE_TEMP, 1), getClimateTemp(CLIMATE_TEMP, 0));
  }
  bool show_semicolon;
  if (watch.seconds % 2 == 0) {
    show_semicolon = true;
  } else {
    show_semicolon = false;
  }
  subDisplay.clock(watch.Hours, watch.minutes, show_semicolon);
  switch (CURRENT_TEXT) {
    case 0:
      break;
    case 1:
      subDisplay.text("UNS8T3D");
      break;
    case 2:
      subDisplay.text("HONDA");
      break;
    case 3:
      subDisplay.text("ACCORD");
      break;
    default:
      break;
  }  
  subDisplay.render();
}

int getClimateTemp(byte Value, int position) {
  if (position == 0) {
    return (ceil(Value / 16)) - 1;
  } else {
    return (Value % 16) - 1;
  }
}

void ir_callback(byte command) {
  watch.gettime();
  volatile int minutes = watch.minutes;
  volatile int hours = watch.Hours;
  volatile bool setTime = false;
  switch (command) {
    case IR_UP:
      setTime = true;
      if (hours == 23) {
        hours = 0;
      } else {
        hours++;
      }
      break;
    case IR_DOWN:
      setTime = true;
      if (hours == 0) {
        hours = 23;
      } else {
        hours--;
      }
      break;
    case IR_LEFT:
      setTime = true;
      if (minutes == 0) {
        minutes = 59;
      } else {
        minutes--;
      }
      break;
    case IR_RIGHT: 
      setTime = true;
      if (minutes == 59) {
        minutes = 0;
      } else {
        minutes++;
      }
      break;
    case IR_OK:
      break;
    case IR_1:
      EEPROM.put(0, 1);
      CURRENT_TEXT = 1;
      break;
    case IR_2:
      EEPROM.put(0, 2);
      CURRENT_TEXT = 2;
      break;
    case IR_3:
      EEPROM.put(0, 3);
      CURRENT_TEXT = 3;
      break;
    case IR_0:
      EEPROM.put(0, 0);
      CURRENT_TEXT = 0;
      break;
  }
  if (setTime) {
    watch.settime(0, minutes, hours);
    setTime = false;
  }
}