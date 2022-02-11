// Connect4 game
// Call with two parameters: h for human player, c for computer player (default h c).
// For computer players, a number can be attached to c defining the level from 0-9 (default c9).
// e.g. ./connect4 c5 h
//
// Modules for input device, display and computer engine can be exchanged and extended
// without interference with game logic.
//
// Written by Stefan Abendroth <sab@ab-solut.com>
// Last updated: 2022-02-11

#include <iostream>     // cout, endl
#include <unistd.h>     // sleep, usleep
#include "display.hpp"  // display abstraction
#include "input.hpp"    // input abstraction
#include "engine.hpp"   // game intelligence
#include "parameters.h"   // ROWS, COLUMNS
 
int8_t limit(int8_t value, int8_t min=0, int8_t max=COLUMNS-1)
{
  int16_t result=value;
  if (value<min) result=min;
  if (value>max) result=max;
  return result;
}

class Game
{
  public:
  Game() {}
  
  uint8_t move_count=0;    // counter for moves
  uint8_t config[2]={255,9};  // game configuration {player1, player2}
                              // {255,255}=human vs. human
                              // {255,n}=human vs. computer level n (n=1..10, default n=10)
                              // {n,255}=computer level n vs. human
                              // {n,n}=computer level n vs. computer level n
  uint8_t winner=0;   // 0: no winner, 1: player 1, 2: player 2

  void move()
  {
    uint8_t last_input=0;
    if (config[player-1]<10)    // computer player
      position=engine.propose_move(tiles,player,config[player-1]);
    else    // human player
    {   
      position=(COLUMNS-1)/2;
      display.show_entry(position,player);
      do
      {
        last_input=input.read();
        if (last_input&INPUT_LEFT)
        {
          position=limit(position-1);
          display.show_entry(position,player);
        }
        if (last_input&INPUT_RIGHT)
        {
          position=limit(position+1);
          display.show_entry(position,player);
        }
      } while ((last_input&(INPUT_DOWN|INPUT_FIRE))==0);
    }
    drop();
  }

  void drop()
  {
    // determine landing row
    int8_t landing_row=-1;
    while ((landing_row<(ROWS-1)) && (tiles[landing_row+1][position]==0))
      landing_row++;
    if (landing_row>=0)    // space left?
    {
      // drop
      display.hide_entry();
      display.drop_tile(position,landing_row,player);
      // land
      tiles[landing_row][position]=player;
      move_count++;
      // check for winner
      check_winner();
      if (winner==0)
        // next player
        player^=3;  // 1 XOR 3 = 2 ; 2 XOR 3 = 1
    }
  }

  void check_winner()
  // search 4 adjacent tiles, write tile coordinates into win_pattern and set winner
  // (0 for nobody, 1 or 2 for respective player)
  // algorithm: for each tile in board, search in all 4 orientations (/,|,\,-) in single steps
  // as long as same color is found and edge of board is not hit. After first sequence of 4, we have a winner.
  {   
      uint8_t win_count=0;  // count to 4
      uint8_t cp=0;         // player being checked
      uint8_t cr=0;         // current check row
      uint8_t cc=0;         // current check column

      // for all tiles
      for (uint8_t row=0; row<ROWS; row++)
      {
          for (uint8_t col=0; col<COLUMNS; col++)
          {
              cp=tiles[row][col];

              // search southwest
              win_count=0;
              cr=row;
              cc=col;
              while ((winner==0) && cp && (tiles[cr][cc]==player) && (cr>=0) && (cc>=0) && (cr<ROWS) && (cc<COLUMNS) && (win_count<4))
              {
                  win_pattern[win_count][0]=cr;
                  win_pattern[win_count][1]=cc;
                  win_count++;
                  cr+=1;
                  cc-=1;
              }
              if (win_count>=4) winner=cp;

              // search south
              win_count=0;
              cr=row;
              cc=col;
              while ((winner==0) && cp && (tiles[cr][cc]==player) && (cr>=0) && (cc>=0) && (cr<ROWS) && (cc<COLUMNS) && (win_count<4))
              {
                  win_pattern[win_count][0]=cr;
                  win_pattern[win_count][1]=cc;
                  win_count++;
                  cr+=1;
              }
              if (win_count>=4) winner=cp;

              // search southeast
              win_count=0;
              cr=row;
              cc=col;
              while ((winner==0) && cp && (tiles[cr][cc]==player) && (cr>=0) && (cc>=0) && (cr<ROWS) && (cc<COLUMNS) && (win_count<4))
              {
                  win_pattern[win_count][0]=cr;
                  win_pattern[win_count][1]=cc;
                  win_count++;
                  cr+=1;
                  cc+=1;
              }
              if (win_count>=4) winner=cp;

              // search east
              win_count=0;
              cr=row;
              cc=col;
              while ((winner==0) && cp && (tiles[cr][cc]==player) && (cr>=0) && (cc>=0) && (cr<ROWS) && (cc<COLUMNS) && (win_count<4))
              {
                  win_pattern[win_count][0]=cr;
                  win_pattern[win_count][1]=cc;
                  win_count++;
                  cc+=1;
              }
              if (win_count>=4) winner=cp;

              if (winner) break;  // no need to complete for-loop
          }
          if (winner) break;   // no need to complete for-loop
      }
  }

  void show_winner()
  {
    for (uint8_t i=0; i<4; i++)   // flash 4 times
    {
      usleep(500000);
         // flash 4 tiles off - on
      for (uint8_t j=0; j<4; j++)
        display.hide_tile(win_pattern[j][0],win_pattern[j][1]);
      usleep(500000);
      for (uint8_t j=0; j<4; j++)
        display.show_tile(win_pattern[j][0],win_pattern[j][1],winner);
    }
  }

  private:
  Display display;                      // details of display in separate class
  Input input;                          // details of input device in separate class
  Engine engine;                        // details of computer moves in separate class
  uint8_t tiles[ROWS][COLUMNS] = {0};   // state of board (0: empty, 1: player 1, 2: player 2)
  uint8_t player=1;                     // current player, start with 1
  uint8_t position=(COLUMNS-1)/2;       // default position for human player
  uint8_t win_pattern[4][2] = {0};      // coordinates of winning tiles, only valid if winner>0
};

int main(int argc, char* argv[])
{
    using namespace std;
    Game game;
    // parse command line arguments
    for (uint8_t i=1; i<limit(argc,1,3); i++)
    {
      if (argv[i][0]=='c')    // computer player only if 'c' is selected
        if (argv[i][1]>='0' && argv[i][1]<='9')
          game.config[i-1]=argv[i][1]-'0';  // subtraction of ASCII values
        else game.config[i-1]=9;   // default level 9
      else game.config[i-1]=255;
    }
    // game loop
    while (game.move_count<(ROWS*COLUMNS) && (game.winner==0))
    {
      game.move();
    }
    if (game.winner)
    {
      cout<<"Player "<<unsigned(game.winner)<<" wins after "<<unsigned(game.move_count)<<" moves!"<<endl;
      game.show_winner();
    }
    else
      cout<<"Draw!"<<endl;
    sleep(1);
    return game.winner;
}