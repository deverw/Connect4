#include <iostream>     // std::cout, std::endl
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
  uint8_t config[2]={1,0};    // game configuration {player1, player2}
                              // {0,0}=computer vs. computer
                              // {1,0}=human vs. computer (default)
                              // {0,1}=computer vs. human
                              // {1,1}=human vs. human
  uint8_t win_pattern[4][2]={0};  // coordinates of winning stones
  uint8_t winner=0;   // 0: no winner, 1: player 1, 2: player 2

  void move()
  {
    uint8_t last_input=0;
    if (config[player-1])
    {   // human player?
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
    else  // computer player
      position=engine.propose_move(tiles,player);
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
      winner=engine.check_winner(tiles,win_pattern);
      if (winner==0)
        // next player
        player^=3;  // 1 XOR 3 = 2 ; 2 XOR 3 = 1
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
  Display display;
  Input input;
  Engine engine;
  uint8_t tiles[ROWS][COLUMNS] = {0};
  uint8_t player=1;
  uint8_t position=(COLUMNS-1)/2;
};

int main(int argc, char* argv[])
{
    Game game;
    // parse arguments
    if (argc==3)
    {
      if (*argv[1]=='c')
        game.config[0]=0;
      else game.config[0]=1;
      if (*argv[2]=='c')
        game.config[1]=0;
      else game.config[1]=1;
    }
    // game loop
    do
    {
      game.move();
    }
    while (game.move_count<(ROWS*COLUMNS) && (game.winner==0));
    if (game.winner)
    {
      std::cout<<"Player "<<unsigned(game.winner)<<" wins after "<<unsigned(game.move_count)<<" moves!"<<std::endl;
      game.show_winner();
    }
    else
      std::cout<<"Draw!"<<std::endl;
    sleep(1);
    return 0;
}