#include <Arduino.h>
#include <toneAC.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Pins
int lights[] = {3, 4, 5, 6, 7, 8}; // WILL BE RANDOMIZED (POST SETUP)
int ledCount = sizeof(lights) / sizeof(lights[0]);
// Tempo
int tempo = 200;

// Jingle Bells
char sheet_music[] = {"dbagdddbageecbaffddcabdbagdddbageeecbaddddedcag bbbbbbbdgab"}; // https://i.pinimg.com/originals/1d/72/b4/1d72b4eabf3deb08e37173e99caf8a98.gif
int sheet_beats[] = {1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 3, 2};
// G Major... I think?
char notes[] = {'g', 'a', 'b', 'c', 'd', 'e', 'f'};
unsigned long freq[] = {392, 440, 493, 523, 587, 659, 739};
int DEBUG = 0;

// Swaps integers
void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

// A function to generate a random permutation of arr[]
void randomize(int arr[], int n)
{
  // Generate a new randomized seed
  srand(time(NULL));

  // Start from the last element and swap one by one
  for (int i = n - 1; i > 0; i--)
  {
    // Pick a random index from 0 to i
    int j = rand() % (i + 1);

    // Swap arr[i] with the element at random index
    swap(&arr[i], &arr[j]);
  }
}

void setup()
{
  // Set Pins
  for (int i = 0; i <= ledCount; i++)
  {
    pinMode(lights[i], OUTPUT);
  };
  if (DEBUG)
  {
    Serial.begin(9600);
  }
}

// Determines notes from notes[], adjusts lights and plays tone
void play(char note, int time)
{
  int note_size = sizeof(notes) / sizeof(notes[0]);
  for (int n = 0; n <= note_size; n++)
  {
    if (notes[n] == note)
    {
      digitalWrite(lights[n], HIGH);
      if (n + 1 <= ledCount)
      {
        digitalWrite(lights[n + 1], HIGH);
      }
      else if (n - 1 <= ledCount)
      {
        digitalWrite(lights[n - 1], HIGH);
      }
      toneAC(freq[n], 1, time, false);
      for (int l = 0; l <= ledCount; l++)
      {
        digitalWrite(lights[l], LOW);
      }
      toneAC(0);
      if (DEBUG)
      {
        Serial.print("Current Playing: ");
        Serial.println(notes[n]);
        Serial.print("Current Time: ");
        Serial.println(time);
        Serial.print("Current Freq: ");
        Serial.println(freq[n]);
      }
    }
  }
}

void loop()
{
  // Randomize lights[] so the LED pattern changes every loop of the song.
  randomize(lights, ledCount);
  for (int i = 0; i <= sizeof(sheet_music) / sizeof(sheet_music[0]); i++)
  {
    // Rest
    if (sheet_music[i] == ' ')
    {
      toneAC(0);
      delay(sheet_beats[i] * tempo);
    }
    else
    {
      // Tempo = 1 Beat, so tempo * # of beats
      play(sheet_music[i], tempo * sheet_beats[i]);
    }
    // Note Spacing
    delay(tempo / 2);
  };
}