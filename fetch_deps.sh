#!/bin/bash

REPO_URL="https://github.com/Bodmer/TFT_eSPI.git"

set -e

git clone "$REPO_URL" lib/TFT_eSPI

cp files/User_Setup.h lib/TFT_eSPI/User_Setup.h
cp files/ST7796_Init.h lib/TFT_eSPI/TFT_Drivers/ST7796_Init.h
