#include <Arduino_FreeRTOS.h>
#include <math.h>
#include "mario.h"

#define IDLE_MODE 0
#define POWER_DOWN_MODE 1
#define POWER_SAVE_MODE 2
#define STANDBY_MODE 3
#define EXT_STANDBY_MODE 4

// Button interrupt pin
const byte interruptPin = 2;
#define melodyPin 3

volatile long global_sample_counter = 0;
volatile long sum_adc_values = 0;
volatile bool stopRead = false;
volatile bool releaseOutput = false;

void TaskAdcOutput();
void TaskButtonClick();
void TaskMusicPlayer();

void setup() {
  Serial.begin(9600);

  // Initialize LED and set as turned off
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Button interruptions
  attachInterrupt(digitalPinToInterrupt(interruptPin), onButtonClick, FALLING);

  // Buzzer config
  pinMode(melodyPin, OUTPUT);

  // initialize ADC configuration
  ADCSRA = (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //set ADC clock with 128 prescaler so we can have 10k samples / s
  ADMUX = (1 << REFS0);  // use AVcc as the reference
  ADCSRA |= (1 << ADATE); //enabble auto trigger
  ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
  ADCSRA |= (1 << ADSC); //start ADC measurements

  // Now set up the tasks to run independently.
  xTaskCreate(
    TaskAdcOutput
    ,  (const portCHAR *)"ADC Output"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskButtonClick
    ,  (const portCHAR *)"Button"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskMusicPlayer
    ,  (const portCHAR *)"Music Player"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  sei(); //start all interrupts
}

void loop() {
  // put your main code here, to run repeatedly:
  set_sleep_mode(IDLE_MODE);
  sleep_cpu();
}

void sleep_cpu() {
  SMCR |= (1 << SE);
  asm("sleep");
}

void set_sleep_mode(char mode) {
  switch (mode) {
    case IDLE_MODE:
      SMCR &= ~(1 << SM0);
      SMCR &= ~(1 << SM1);
      SMCR &= ~(1 << SM2);
      break;
    case POWER_DOWN_MODE:
      SMCR &= ~(1 << SM0);
      SMCR |= (1 << SM1);
      break;
    case POWER_SAVE_MODE:
      SMCR |= (1 << SM1) | (1 << SM0);
      break;
    case STANDBY_MODE:
      SMCR &= ~(1 << SM0);
      SMCR |= (1 << SM2) | (1 << SM1);
      break;
    case EXT_STANDBY_MODE:
      SMCR |= (1 << SM2) | (1 << SM1) | (1 << SM0);
      break;
  }
}

/**
   This covers the TASK 3
*/
void TaskAdcOutput() {
  while (true) {
    vTaskDelay( 5000 / portTICK_PERIOD_MS ); // Wait 5 seconds
    stopRead = true; // stop reading for a while
    long average = sum_adc_values / global_sample_counter;
    long R = ((1023 - average) * 10) / average;
    long lux = 350 * pow(R, -1.43);
    sum_adc_values = 0;
    global_sample_counter = 0;
    Serial.print("Average value: ");
    Serial.println(average);
    Serial.print("Lux value: ");
    Serial.println(lux);
    stopRead = false; // start sampling again
  }
}

/**
   This covers the TASK 2
*/
void TaskButtonClick() {
  while (true) {
    if (releaseOutput) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // change LED state
      Serial.print("Counter: ");
      Serial.println(global_sample_counter);
      Serial.print("ADC sum: ");
      Serial.println(sum_adc_values);
      releaseOutput = false;
    }
    vTaskDelay( 200 / portTICK_PERIOD_MS ); // Wait 100ms
  }
}

void onButtonClick() {
  releaseOutput = true;
}

/**
   This covers the TASK 1
*/
ISR(ADC_vect) {
  if (!stopRead) {
    global_sample_counter++;
    sum_adc_values += ADC; //get value from A0
  }
}

/**
   This covers the TASK 3
*/
void TaskMusicPlayer() {
  while (true) {
    play_melody();
  }
}

