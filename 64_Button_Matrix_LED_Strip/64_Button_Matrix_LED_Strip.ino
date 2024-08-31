#include <Bounce2.h>
#include <FastLED.h>

// Set row and column pins
const int numRows = 8;
const int numCols = 8;

// Row pins: Use from 2 to 9
int rowPins[numRows] = {2, 3, 4, 5, 6, 7, 8, 9};

// Column pins: Use from 15 to 22
int colPins[numCols] = {15, 16, 17, 18, 19, 20, 21, 22};

// Set LEDs
const byte _nLED = 64; // Number of LEDs
const byte _dLED = 0; // LED data pins
CRGB _LED[_nLED];

// Maximum brightness
const uint8_t MAX_BRIGHTNESS = 255; // Adjustable 0 ~ 255

// LED update interval
const unsigned long LED_UPDATE_INTERVAL = 10; // Adjustable
unsigned long lastUpdate = 0;

// Array for storing button states (Do Not Touch)
Bounce buttonDebouncer[numRows][numCols]; // Array of Bounce objects
bool lastButtonState[numRows][numCols]; // Last button state

// MIDI start note
const byte _pStart = 36;  // Number of the first note
bool update = false;

// MIDI Notes And LED Mapping
const byte noteToLED[8][8] = {
  {64, 65, 66, 67, 96, 97, 98, 99},
  {60, 61, 62, 63, 92, 93, 94, 95},
  {56, 57, 58, 59, 88, 89, 90, 91},
  {52, 53, 54, 55, 84, 85, 86, 87},
  {48, 49, 50, 51, 80, 81, 82, 83},
  {44, 45, 46, 47, 76, 77, 78, 79},
  {40, 41, 42, 43, 72, 73, 74, 75},
  {36, 37, 38, 39, 68, 69, 70, 71}
};

// Color Palette
const byte _r[128] = {0, 28, 125, 255, 255, 255, 85, 24, 255, 255, 85, 36, 255, 255, 85, 24, 134, 81, 28, 16, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 134, 81, 24, 12, 255, 255, 85, 24, 255, 255, 85, 32, 255, 150, 117, 65, 0, 0, 0, 0, 0, 36, 125, 28, 255, 186, 174, 97, 12, 0, 0, 0, 61, 121, 174, 61, 255, 134, 113, 0, 57, 85, 53, 89, 49, 134, 210, 255, 255, 182, 142, 130, 57, 16, 12, 20, 20, 101, 166, 219, 215, 255, 158, 101, 28, 219, 125, 154, 142, 61, 113, 223, 158, 53, 24, 4, 182, 61, 178, 73};
const byte _g[128] = {0, 28, 125, 255, 73, 0, 0, 0, 186, 81, 28, 24, 255, 255, 85, 24, 255, 255, 85, 40, 255, 255, 85, 24, 255, 255, 85, 24, 255, 255, 85, 28, 255, 255, 85, 24, 194, 166, 65, 12, 134, 85, 28, 4, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 20, 53, 81, 97, 57, 85, 81, 0, 69, 0, 125, 28, 0, 255, 235, 255, 138, 255, 166, 40, 0, 0, 24, 32, 73, 223, 255, 255, 255, 255, 255, 138, 81, 125, 28, 0, 125, 174, 255, 89, 40, 73, 77, 20, 28, 57, 0, 81, 105, 223, 223, 178, 28, 255, 255, 150, 101, 61, 113, 255, 0, 0, 206, 65, 174, 49, 93, 20};
const byte _b[128] = {0, 28, 125, 255, 73, 0, 0, 0, 105, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 93, 24, 12, 0, 134, 85, 28, 16, 182, 150, 53, 16, 255, 255, 81, 24, 255, 255, 85, 24, 255, 255, 85, 24, 255, 255, 97, 45, 255, 255, 85, 24, 134, 81, 28, 16, 0, 0, 0, 0, 0, 53, 125, 255, 77, 202, 125, 28, 0, 45, 4, 8, 0, 134, 255, 255, 255, 255, 121, 0, 0, 4, 20, 0, 36, 109, 202, 255, 194, 231, 255, 89, 0, 0, 0, 4, 0, 12, 53, 40, 89, 24, 8, 61, 24, 36, 45, 12, 45, 105, 186, 255, 255, 61, 113, 255, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  // Row pin initialization
  for (int row = 0; row < numRows; row++) {
    pinMode(rowPins[row], OUTPUT);
    digitalWrite(rowPins[row], HIGH);  // Set row to HIGH
  }

  // Initialize the heat pin and set up the Bounce object
  for (int col = 0; col < numCols; col++) {
    pinMode(colPins[col], INPUT_PULLUP);  // Set the hot pin to a pull-up input
    for (int row = 0; row < numRows; row++) {
      buttonDebouncer[row][col].attach(colPins[col]);
      buttonDebouncer[row][col].interval(50);  // Set debounce time (in milliseconds)
      lastButtonState[row][col] = false;  // Set initial button state
    }
  }

  // LED initialize
  FastLED.addLeds<NEOPIXEL, _dLED>(_LED, _nLED);
  FastLED.setBrightness(MAX_BRIGHTNESS);  // Set Max BBRIGHTNESS
  FastLED.show();

  // MIDI Handler
  usbMIDI.setHandleNoteOn(noteOn);
  usbMIDI.setHandleNoteOff(noteOff);
  usbMIDI.setHandleSystemExclusive(sysEx);
}

const int BASE_NOTE = 36;  // Set BASE MIDI note number

void loop() {
  for (int row = 0; row < numRows; row++) {
    // Set current row to LOW
    for (int i = 0; i < numRows; i++) {
      digitalWrite(rowPins[i], i == row ? LOW : HIGH);
    }

    for (int col = 0; col < numCols; col++) {
      buttonDebouncer[row][col].update();
      bool currentButtonState = !buttonDebouncer[row][col].read();  // Check if the button is pressed

      if (currentButtonState != lastButtonState[row][col]) {
        // Get MIDI note numbers using the noteToLED array.
        int midiNote = noteToLED[row][col];

        if (currentButtonState) {
          usbMIDI.sendNoteOn(midiNote, 127, 1);  //  Note, Velocity, Channel
        } else {
          usbMIDI.sendNoteOff(midiNote, 0, 1);  // Note, Velocity, Channel
        }

        lastButtonState[row][col] = currentButtonState;  // Status update
      }
    }
  }

  while (usbMIDI.read());

  unsigned long currentMillis = millis();
  if (update && currentMillis - lastUpdate >= LED_UPDATE_INTERVAL) {
    FastLED.show();
    lastUpdate = currentMillis;
    update = false;
  }
}



// MIDI Note ON Handler
void noteOn(byte ch, byte p, byte v) {
  for (byte row = 0; row < 8; row++) {
    for (byte col = 0; col < 8; col++) {
      if (noteToLED[row][col] == p) {
        _LED[row * 8 + col] = CRGB(_r[v], _g[v], _b[v]);
        update = true;
        return;
      }
    }
  }
}

// MIDI Note OFF Handler
void noteOff(byte ch, byte p, byte v) {
  for (byte row = 0; row < 8; row++) {
    for (byte col = 0; col < 8; col++) {
      if (noteToLED[row][col] == p) {
        _LED[row * 8 + col] = CRGB::Black;
        update = true;
        return;
      }
    }
  }
}

// SYSEX Handler
void sysEx(byte *data, unsigned int length) {
  if (length == 6) {
    for (byte row = 0; row < 8; row++) {
      for (byte col = 0; col < 8; col++) {
        if (noteToLED[row][col] == *(data+1)) {
          _LED[row * 8 + col] = CRGB(*(data+2), *(data+3), *(data+4));
          update = true;
          return;
        }
      }
    }
  }
}
