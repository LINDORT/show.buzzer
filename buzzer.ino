/*
 * MIDIUSB_test.ino
 *
 * Created: 4/6/2015 10:47:08 AM
 * Author: gurbrinder grewal
 * Modified by Arduino LLC (2015)
 */ 

#include "MIDIUSB.h"

#define DEBOUNCE 20

const unsigned char buzzers[2] = {A0, A1};
unsigned int prev_buzzer_time[2];
int prev_buzzer_state[2] = {HIGH, HIGH};

void setup() {
  Serial.begin(115200);
  while(!Serial);

  for (int i = 0; i < sizeof(buzzers); i++) {
    Serial.println(buzzers[i]);
    pinMode(buzzers[i], INPUT_PULLUP);
  }
}

void loop() {
  for (int i = 0; i < sizeof(buzzers); i++) {
    if ((millis() - prev_buzzer_time[i]) > DEBOUNCE) {
      if (digitalRead(buzzers[i]) == LOW && prev_buzzer_state[i] == HIGH) {
        send_midi_buzzer(i);
  
        prev_buzzer_time[i] = millis();
        prev_buzzer_state[i] = LOW;
      } else if (digitalRead(buzzers[i]) == HIGH && prev_buzzer_state[i] == LOW) {
        prev_buzzer_state[i] = HIGH;
      }
    }
  }
}

void send_midi_buzzer(int id) {
  Serial.println(String("PRESSED: ") + id);
  midiEventPacket_t noteOn = {0x09, 0x90 | 0, 0, id}; // Send channel: 0, pitch: 0; velocity: buzzer index
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}
