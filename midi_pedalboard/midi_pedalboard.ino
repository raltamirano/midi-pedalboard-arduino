#include <MIDI.h>

// Pin reading when button is OPEN (not pressed)
const int OPEN = HIGH;

// Pin reading when button is CLOSED (pressed)
const int CLOSED = LOW;

// Number of pedals
const int PEDALS = 5;

// Pin number for each pedal button
const int PEDAL_PINS[] = { 2, 3, 4, 5, 6 }; 

// The debounce time; increase if the output flickers
const unsigned long DEBOUNCE_DELAY = 50;

// Last comand sent
int lastCommandSent;

// The current reading from the input pins
int buttonState[PEDALS];   

// The previous reading from the input pins
int lastButtonState[PEDALS];   

// MIDI init
MIDI_CREATE_DEFAULT_INSTANCE();

void setup()
{
   // No last command was sent
   lastCommandSent = -1;
  
   for(int i=0; i<PEDALS; i++) {
     pinMode(PEDAL_PINS[i], INPUT_PULLUP);
     buttonState[i] = OPEN;
     lastButtonState[i] = OPEN;
   }

   // Prepare for MIDI I/O
   MIDI.begin(); 
   Serial.begin(115200);
}


void loop()
{
  for(int i=0; i<PEDALS; i++) {
    if (readPedal(i) == CLOSED) {
      int command = 60 + i; 
      if (command != lastCommandSent) {
        lastCommandSent = command;
        MIDI.sendNoteOn(command, 127, 1);
      }
      break;
    }
  }
}

int readPedal(int pedalIndex) {
  int reading = digitalRead(PEDAL_PINS[pedalIndex]);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState[pedalIndex]) {
    delay(DEBOUNCE_DELAY);
    reading = digitalRead(PEDAL_PINS[pedalIndex]);    
    if (reading != buttonState[pedalIndex]) {
      buttonState[pedalIndex] = reading;
    }
  }

  // save the reading
  lastButtonState[pedalIndex] = reading;  
  
  return buttonState[pedalIndex];
}
