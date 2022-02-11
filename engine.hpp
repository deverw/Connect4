// Connect4 computer player
// providing different agents for selecting a move (random, request server)
//
// Written by Stefan Abendroth <sab@ab-solut.com>
// Last updated: 2022-02-11

#ifndef _ENGINE_
#define _ENGINE_

#include <cstdint>      // uint8_t
#include <string>       // string, substr, replace, c_str, stoi
#include <stdlib.h>     // rand, srand
#include <time.h>       // time
#include <curl/curl.h>     // libcurl (http://curl.haxx.se/libcurl/c)
#include "parameters.h"     // ROWS, COLUMNS

class Engine
{
    public:
        Engine();

        // calculate next move for player according to current tiles
        uint8_t propose_move(uint8_t tiles[ROWS][COLUMNS], uint8_t player, uint8_t level);

    private:
        // return random move
        uint8_t random_agent();

        // get move from server api.codebox.net/connect4
        uint8_t server_agent(uint8_t tiles[ROWS][COLUMNS], uint8_t player);
};

#endif