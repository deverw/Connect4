# connect4
C++ implementation of the game Connect4, using Raspberry Pi SenseHat as input and display device. Computer moves are requested from server (api.codebox.net/connect4).
You need to install libcurl (http://curl.haxx.se/libcurl/c) in order to compile the files:
sudo apt install libcurl4-openssl-dev

After cloning the repository, start with:
<code>
make
./connect4
</code>

This will start a game human player (red) against computer (yellow).
Move the joystick left and right to choose your column, move down or push to drop the current tile.

You can change the roles by calling the program with parameters:
<code>
./connect4 c h (computer to move first)
./connect4 h h (human vs. human)
./connect4 c c (computer vs. computer)
</code>
