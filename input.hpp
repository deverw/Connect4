// Abstraction layer for input device (e.g. joystick, keyboard)
// Handle initialisation of input device with constructor / destructor
// Deliver 8 bit unsigned integer value with detected input (2^0 -> left, 2^1 -> right, 2^2 -> up, 2^3 -> down, 2^4 -> fire)
//
// Variant for RaspberryPi SenseHat joystick
//
// Written by Stefan Abendroth <sab@ab-solut.com>
// Last updated: 2022-02-09

#ifndef _INPUT_
#define _INPUT_

#include <cstdint>          // uint8_t
#include "libs/joystick.h"  // Raspberry Pi SenseHat joystick

#define INPUT_LEFT 1        // 2^0 -> left
#define INPUT_RIGHT 2       // 2^1 -> right
#define INPUT_UP 4          // 2^2 -> up
#define INPUT_DOWN 8        // 2^3 -> down
#define INPUT_FIRE 16       // 2^4 -> fire

class Input
{
    public:
        Input();            // Initialisation of input device
        ~Input();           // Close input device
        uint8_t read();     // Deliver 8 bit unsigned integer value with detected input 
    private:
        js_event ev;        // SenseHat joystick event
};

#endif