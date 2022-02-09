// Abstraction layer for Connect Four display
//
// Variant for Raspberry Pi SenseHat LED display
//
// Written by Stefan Abendroth <sab@ab-solut.com>
// Last updated: 2022-02-09

#include "display.hpp"
#include <unistd.h>             // usleep
#include "libs/led_matrix.h"    // Raspberry Pi SenseHat LED display

Display::Display()
{
    open_led_matrix();
    show_empty_board();
}

Display::~Display()
{
    hide_board();
    close_led_matrix();
}

void Display::show_empty_board()
{
    clear_leds();
    for (uint8_t row=0; row<ROWS; row++)
        for (uint8_t col=0; col<COLUMNS; col++)
            set_led(row+2,col,BOARD_COLOR);
}

void Display::hide_board()
{
    clear_leds();
}

void Display::show_entry(uint8_t column, uint8_t player)
{
    hide_entry();
    switch (player)
    {
        case 1:
            set_led(1,column,PLAYER1_COLOR);
            break;
        case 2:
            set_led(1,column,PLAYER2_COLOR);
            break;
    }
}

void Display::hide_entry()
{
    for (uint8_t col=0; col<COLUMNS; col++)
        set_led(1,col,RGB565_OFF);
}

void Display::drop_tile(uint8_t column, int8_t target_row, uint8_t player)
{
    uint16_t color=BOARD_COLOR;
    switch (player)
    {
        case 1:
            color=PLAYER1_COLOR;
            break;
        case 2:
            color=PLAYER2_COLOR;
            break;
    }
    uint8_t row=0;
    usleep(100000);
    hide_entry();
    set_led(row+2,column,color);
    while (row<target_row)
    {
        usleep(100000);
        set_led(row+2,column,BOARD_COLOR);
        row++;
        set_led(row+2,column,color);
    }
}

void Display::show_tile(uint8_t row, uint8_t column, uint8_t player)
{
    switch (player)
    {
        case 1:
            set_led(row+2,column,PLAYER1_COLOR);
            break;
        case 2:
            set_led(row+2,column,PLAYER2_COLOR);
            break;
    }
}

void Display::hide_tile(uint8_t row, uint8_t column)
{
    set_led(row+2,column,BOARD_COLOR);
}