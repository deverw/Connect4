// Connect Four computer player

#include "engine.hpp"

using namespace std;

size_t curl_callback(void *ptr, size_t size, size_t nmemb, string* data)
// write callback function for POST request
{
	data->append((char*)ptr, size * nmemb);
	return size * nmemb;
}

Engine::Engine()
{
    srand(time(NULL));
}

uint8_t Engine::random_agent()
// Generate random move without looking at the board
// move may be invalid (full column)
{
    return rand()%7;
}

uint8_t Engine::server_agent(uint8_t tiles[ROWS][COLUMNS], uint8_t player)
// Get move from api.codebox.net/connect4
// Only works with standard board size (6 rows, 7 columns)
// Command line example: curl -X POST api.codebox.net/connect4 -H 'Content-Type: application/json' -d '{"board":["...0...","...1...","...01..","...10..","...01..","...110."]}'
// Server expects board state in JSON format like {["...0...","...1...","...01..","...10..","...01..","...110."]},
// where "." is empty, "0" is server and "1" is human player, from top to bottom
// If no error, server responds with JSON format, e.g. {"move": 1, "winner": null}.
// "move" data is extracted and returned. "winner" data is ignored (winner is determined by local function). In case of error, random_agent is called.
{
    uint8_t result;
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL; // linked-list string structure

    string response_string;     // will contain server response
    string tile_char;
    string move_char;
 
    // create server input string
    string post_string = "{\"board\":[\".......\",\".......\",\".......\",\".......\",\".......\",\".......\"]}";       // empty board
    for (uint8_t row=0; row<ROWS; row++)
        for (uint8_t col=0; col<COLUMNS; col++)
            if (tiles[row][col])
            {
                if (tiles[row][col]==player)                             // server expects "0" for own tiles and "1" for opponent
                    tile_char="0";
                else
                    tile_char="1";
                post_string.replace (11+10*row+col,1,tile_char);        // replace single character at right string position
            }
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "api.codebox.net/connect4");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_string.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
 
 
        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);
        if(res == CURLE_OK)
        {
            // extract server response
            move_char=response_string.substr(9,1);      // 9th character contains selected column
            result=stoi(move_char);
        }
        else
            result=random_agent();
        curl_easy_cleanup(curl);
    }
    else
        result=random_agent();
    return result;
}

uint8_t Engine::propose_move(uint8_t tiles[ROWS][COLUMNS], uint8_t player)
// deliver move to game layer
// possible agents:
// return server_agent(tiles, player);
// return random_agent();
{
    return server_agent(tiles, player);
}

uint8_t Engine::check_winner(uint8_t tiles[ROWS][COLUMNS], uint8_t win_pattern[4][2])
// search 4 adjacent tiles, write coordinates into win_pattern and return winning player (or 0)
// algorithm: for each tile in board, search for same color in all 4 orientations (/,|,\,-)
{   
    uint8_t win_count=0;    // count to 4
    uint8_t player=0;   // winning player
    uint8_t cr=0;        // current check row
    uint8_t cc=0;        // current check column
    uint8_t winner=0;

    // for all tiles
    for (uint8_t row=0; row<ROWS; row++)
    {
        for (uint8_t col=0; col<COLUMNS; col++)
        {
            player=tiles[row][col];

            // search southwest
            win_count=0;
            cr=row;
            cc=col;
            while ((winner==0) && player && (tiles[cr][cc]==player) && (cr>=0) && (cc>=0) && (cr<ROWS) && (cc<COLUMNS) && (win_count<4))
            {
                win_pattern[win_count][0]=cr;
                win_pattern[win_count][1]=cc;
                win_count++;
                cr+=1;
                cc-=1;
            }
            if (win_count>=4) winner=player;

            // search south
            win_count=0;
            cr=row;
            cc=col;
            while ((winner==0) && player && (tiles[cr][cc]==player) && (cr>=0) && (cc>=0) && (cr<ROWS) && (cc<COLUMNS) && (win_count<4))
            {
                win_pattern[win_count][0]=cr;
                win_pattern[win_count][1]=cc;
                win_count++;
                cr+=1;
            }
            if (win_count>=4) winner=player;

            // search southeast
            win_count=0;
            cr=row;
            cc=col;
            while ((winner==0) && player && (tiles[cr][cc]==player) && (cr>=0) && (cc>=0) && (cr<ROWS) && (cc<COLUMNS) && (win_count<4))
            {
                win_pattern[win_count][0]=cr;
                win_pattern[win_count][1]=cc;
                win_count++;
                cr+=1;
                cc+=1;
            }
            if (win_count>=4) winner=player;

            // search east
            win_count=0;
            cr=row;
            cc=col;
            while ((winner==0) && player && (tiles[cr][cc]==player) && (cr>=0) && (cc>=0) && (cr<ROWS) && (cc<COLUMNS) && (win_count<4))
            {
                win_pattern[win_count][0]=cr;
                win_pattern[win_count][1]=cc;
                win_count++;
                cc+=1;
            }
            if (win_count>=4) winner=player;

            if (winner) break;  // no need to complete for-loop
        }
        if (winner) break;   // no need to complete for-loop
    }
    return winner;
}