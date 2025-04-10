#pragma once
#include <ESP32Time.h>
#include <TFT_eSPI.h>

extern TFT_eSPI tft;
extern int16_t height, width, buttonsHeight, statusBarHeight;
extern ESP32Time rtc;
