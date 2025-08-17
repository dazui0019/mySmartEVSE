#include <Arduino.h>
#include "driver/ledc.h"
#include "driver/adc.h"

#define PIN_CP_OUT 19
#define PIN_CPOFF 15
#define PIN_TEMP 36
#define PIN_CP_IN 39
#define PIN_PP_IN 34

hw_timer_t * timerA = NULL;

void IRAM_ATTR onTimerA() {
    Serial.println("timer1");
}

// put function declarations here:
void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("Hello, world!");

    pinMode(PIN_CP_OUT, OUTPUT);
    pinMode(PIN_CPOFF, OUTPUT);
    digitalWrite(PIN_CPOFF, HIGH);

    // setup timer, and one shot timer interrupt to 50us
    timerA = timerBegin(0, 80, true);
    timerAttachInterrupt(timerA, &onTimerA, false);
    // we start in STATE A, with a static +12V CP signal
    // set alarm to trigger every 1mS, and let it reload every 1ms
    timerAlarmWrite(timerA, 1000, true);
    // when PWM is active, we sample the CP pin after 5% 
    timerAlarmEnable(timerA);

    // Setup ADC on CP, PP and Temperature pin
    adc1_config_width(ADC_WIDTH_BIT_10);                                    // 10 bits ADC resolution is enough
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_12);             // setup the CP pin input attenuation to 11db
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_6);              // setup the PP pin input attenuation to 6db
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_6);              // setup the Temperature input attenuation to 6db

    ledcSetup(0, 1000, 10);
    ledcAttachPin(PIN_CP_OUT, 0);
    ledcWrite(0, 500);
}

void loop() {
    // put your main code here, to run repeatedly:
    delay(1000);
    Serial.println("Hello, world!");
}
