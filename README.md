# FightStick

This project was made for cs362 at UIC. This assignments main requirement was that 2 arduino's must be used to communicate, and must involve original code. 

Our decision was to make a standard fightstick (8 face button, digital stick, start/select) with LED's. 

## Demo/Informational Video

https://www.youtube.com/watch?v=jdNJM4W2_w4

## Tools

Unojoy is used for driver recognition, which can be found here: https://github.com/AlanChatham/UnoJoy

## Warning

Our code is designed for the arduino to communicate in a way to meet the project requirements, and should not be used for general purpose (as it adds a small amount of input lag). 

Moderately trivial changes can be made to allow the same functionality with reduced input lag (change the communication to I2C and use an interrupt to trigger remmaping).

## Programming Environment

Arduino IDE, Windows 7 and 10

## Authors

Peter Bengtson, David Fulmer

## Date Created

November, 2017
