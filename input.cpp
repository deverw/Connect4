// Abstraction layer for input device (e.g. joystick, keyboard)
// Handle initialisation of input device with constructor / destructor
// Deliver 8 bit unsigned integer value with detected input (2^0 -> left, 2^1 -> right, 2^2 -> up, 2^3 -> down, 2^4 -> fire)
//
// Variant for RaspberryPi SenseHat joystick
// Only one bit is set (e.g. no combination of directions in case of diagonal movement of joystick)
//
// Written by Stefan Abendroth <sab@ab-solut.com>
// Last updated: 2022-02-09

#include "input.hpp"

Input::Input()
{
    open_joystick_device();
}

Input::~Input()
{
    close_joystick_device();
}

uint8_t Input::read()
{
    uint8_t result=0;
    ev=read_joystick_input();
    if (ev.type!=JOYSTICK_RELEASE)
    {
        switch (ev.direction)
        {
            case DIRECTION_WEST:
                result|=INPUT_LEFT;
                break;
            case DIRECTION_EAST:
                result|=INPUT_RIGHT;
                break;
            case DIRECTION_NORTH:
                result|=INPUT_UP;
                break;
            case DIRECTION_SOUTH:
                result|=INPUT_DOWN;
                break;
            case DIRECTION_DOWN:
                result|=INPUT_FIRE;
                break;
        }
    }
    return result;     
}