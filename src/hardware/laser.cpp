#include "laser.h"
#include <Arduino.h>

void laser_setup(void)
{
    pinMode(LASER_PIN, OUTPUT);
}

void laser_on(void)
{
    digitalWrite(LASER_PIN, HIGH);
}

void laser_off(void)
{
    digitalWrite(LASER_PIN, LOW);
}

