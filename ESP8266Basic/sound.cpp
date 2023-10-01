#include <Arduino.h>
#include "Sound.h"

void Sound::setPin(int pin) {
  this->pin = pin;
}

void Sound::loop() {
  if (!isPlaying) return;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= pauseBetweenNotes) {
    previousMillis = currentMillis;
    // stop the tone playing:
    noTone(pin);
    thisNote++;
    if (thisNote < len) {
      playTone(notes[thisNote], durations[thisNote]);
    }
    else {
      stop();
      if (myCallback != NULL) {
        myCallback();
      }
    }
  }
}

void Sound::playTone(int note, int duration, bool wait) {
  // to calculate the note duration, take one second divided by the note type.
  //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
  //int noteDuration = 1000 / duration;
  int noteDuration = duration;
  tone(pin, note, noteDuration);
  // to distinguish the notes, set a minimum time between them.
  // the note's duration + 30% seems to work well:
  pauseBetweenNotes = noteDuration; // * 1.30;
  if (wait) {
    delay(pauseBetweenNotes);
  }
  else {
    previousMillis = millis();
    isPlaying = true;
  }
}

void Sound::playMelody(int *notes, int *durations, int len, bool wait) {
  if (isPlaying) return;
  if (wait) {
    for (int i = 0; i < len; i++) {
      playTone(notes[i], durations[i], true);
    }
  } else {
    this->len = len;
    this->notes = (int*)calloc(len, sizeof(int));
    for (int i = 0; i < len; i++) {
      this->notes[i] = notes[i];
    }
    this->durations = (int*)calloc(len, sizeof(int));
    for (int i = 0; i < len; i++) {
      this->durations[i] = durations[i];
    }
    this->thisNote = 0;
    playTone(notes[thisNote], durations[thisNote]);
  }
}

void Sound::playMelody(String song, bool wait) {
  if (isPlaying) return;
  int i = 0;
  int num;
  byte default_dur = 4;
  byte default_oct = 6;
  int bpm = 63;
  byte note;
  byte scale;
  long duration;
  long wholenote;
  unsigned int nextFrequency;
  unsigned int nextDuration;
  int notes_array[49] = {0,
                         NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
                         NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
                         NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
                         NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
                        };
  int *notes2 = (int*)malloc(sizeof(int));
  int *durations2 = (int*)malloc(sizeof(int));
  int note_index = 0;

  while (song[i] != ':') i++;  // ignore name
  i++; // skip ':'

  while (song[i] == ' ') i++;  // ignore space

  // get default duration
  if (song[i] == 'd') {
    i++;
    i++; // skip "d="
    num = 0;
    while (isdigit(song[i])) {
      num = (num * 10) + (song[i++] - '0');
    }
    if (num > 0) default_dur = num;
    i++; // skip comma
  }

  while (song[i] == ' ') i++;  // ignore space

  // get default octave
  if (song[i] == 'o') {
    i++;
    i++; // skip "o="
    num = song[i++] - '0';
    if (num >= 3 && num <= 7) default_oct = num;
    i++; // skip comma
  }

  while (song[i] == ' ') i++;  // ignore space

  // get BPM
  if (song[i] == 'b') {
    i++;
    i++; // skip "b="
    num = 0;
    while (isdigit(song[i])) {
      num = (num * 10) + (song[i++] - '0');
    }
    bpm = num;
    i++; // skip colon
  }

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4; // this is the time for whole note (in milliseconds)

  while (i <= song.length()) {

    while (song[i] == ' ') i++;  // ignore space

    // first, get note duration, if available
    num = 0;
    while (isdigit(song[i])) {
      num = (num * 10) + (song[i++] - '0');
    }

    if (num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch (song[i]) {
      case 'c': note = 1; break;
      case 'd': note = 3; break;
      case 'e': note = 5; break;
      case 'f': note = 6; break;
      case 'g': note = 8; break;
      case 'a': note = 10; break;
      case 'b': note = 12;  break;
      case 'p':
      default: note = 0;
    }
    i++;

    // now, get optional '#' sharp
    if (song[i] == '#') {
      note++;
      i++;
    }

    // now, get optional '.' dotted note
    if (song[i] == '.') {
      duration += duration / 2;
      i++;
    }

    // now, get scale
    if (isdigit(song[i])) {
      scale = song[i] - '0';
      i++;
    } else {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if (song[i] == ',')
      i++;  // skip comma for next note (or we may be at the end)

    // now play the note
    if (note) {
      nextFrequency = notes_array[(scale - 4) * 12 + note];
      nextDuration = duration;
    }
    else {
      nextFrequency = 0;
      nextDuration = duration;
    }

    notes2 = (int*)realloc(notes2, (note_index + 1) * sizeof(int));
    durations2 = (int*)realloc(durations2, (note_index + 1) * sizeof(int));
    notes2[note_index] = nextFrequency;
    durations2[note_index] = nextDuration;
    note_index++;
  }

  playMelody(notes2, durations2, note_index, wait);

  free(notes2);
  free(durations2);
}

void Sound::stop() {
  if (isPlaying) {
    free(notes);
    free(durations);
  }
  isPlaying = false;
}

void Sound::setCallback(void (*callback)()) {
  myCallback = callback;
}
