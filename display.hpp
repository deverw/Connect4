// Abstraction layer for Connect Four display
//
// Variant for Raspberry Pi SenseHat LED display
//
// Written by Stefan Abendroth <sab@ab-solut.com>
// Last updated: 2022-02-09

#ifndef _DISPLAY_
#define _DISPLAY_

#include <cstdint>      // uint8_t
#include "parameters.h"     // ROWS, COLUMNS

#define PLAYER1_COLOR 0xF800    // red
#define PLAYER2_COLOR 0xFFE0    // yellow
#define BOARD_COLOR 0x0007      // dark blue

class Display
{
    public:
        Display();                  // Initialize input device
        ~Display();                 // Close input device
        void show_empty_board();    // Draw empty board
        void hide_board();          // Hide board
        void show_entry(uint8_t column, uint8_t player);    // Show entry line with new tile
        void hide_entry();                                  // Hide entry line
        void drop_tile(uint8_t column, int8_t target_row, uint8_t player);      // Drop new tile into board
        void show_tile(uint8_t row, uint8_t column, uint8_t player);        // Show single tile
        void hide_tile(uint8_t row, uint8_t column);        // Hide single tile
};

#endif