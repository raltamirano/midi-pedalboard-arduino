#include <MIDI.h>

// Pin reading when pedal is OPEN (not pressed)
const int OPEN = HIGH;

// Pin reading when pedal is CLOSED (pressed)
const int CLOSED = LOW;

// Number of pedals
const int PEDALS = 5;

// Pin number for each pedal button
const int PEDAL_PINS[] = { 2, 4, 7, 9, 12 }; 

// The debounce time; increase if the output flickers
const unsigned long DEBOUNCE_DELAY = 20;

// Last comand sent
int lastCommandSent;

// The current reading from the input pins
int pedalState[PEDALS];   

// The previous reading from the input pins
int lastPedalReading[PEDALS];   

// MIDI init
MIDI_CREATE_DEFAULT_INSTANCE();

void setup()
{
   // No last command was sent
   lastCommandSent = -1;
  
   for(int i=0; i<PEDALS; i++) {
     pinMode(PEDAL_PINS[i], INPUT_PULLUP);
     pedalState[i] = OPEN;
     lastPedalReading[i] = OPEN;
   }

   // Prepare for MIDI I/O
   MIDI.begin(); 
   Serial.begin(115200);
}


void loop()
{
  for(int i=0; i<PEDALS; i++) {
    int lastPedalState = pedalState[i];
    int currentPedalState = readPedal(i);   
    if (currentPedalState != lastPedalState) {
      int command = 60 + i; 

      if (currentPedalState == CLOSED)
        MIDI.sendNoteOn(command, 127, 1);
      else
        MIDI.sendNoteOff(command, 0, 1);

      break;
    }
  }
}

int readPedal(int pedalIndex) {
  int reading = digitalRead(PEDAL_PINS[pedalIndex]) == OPEN ? OPEN : CLOSED;

  if (reading != lastPedalReading[pedalIndex]) {
    delay(DEBOUNCE_DELAY);
    reading = digitalRead(PEDAL_PINS[pedalIndex]) == OPEN ? OPEN : CLOSED;    
    if (reading != pedalState[pedalIndex]) {
      pedalState[pedalIndex] = reading;
    }
  }

  // save the reading
  lastPedalReading[pedalIndex] = reading;  
  
  return pedalState[pedalIndex];
}
 
