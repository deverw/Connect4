# Connect4
C++ implementation of the game Connect4, using Raspberry Pi SenseHat as input and display device. Computer moves are requested from an external server (api.codebox.net/connect4) or placed randomly.
The implementation follows a strictly modular approach, so the game logic is completely decoupled from the input and display device. In order to run the game in a different setting, the classes Input (implemented in input.cpp, reading the SenseHat joystick) and Display (implemented in display.cpp, switching the SenseHat LED matrix) need to be replaced without changing the headers.
The class Engine (implemented in engine.cpp) is responsible for providing computer moves and can be extended further by your own logic or AI. I am curious to learn what you come up with and whether you are able to beat the API!

Since C++ comes without native support of web based services, libcurl (http://curl.haxx.se/libcurl/c) needs to be installed before compilation:

<code>sudo apt install libcurl4-openssl-dev</code>

After cloning the repository, start with:
<br><code>
make
</code><br><code>
./connect4
</code><br><br>
This will start a game human player (red) against computer (yellow).
Move the joystick left and right to choose your column, move down or push to drop the current tile.

You can change the roles by calling the program with parameters:

<code>
./connect4 c h
</code> (computer to move first)

<code>
./connect4 h h
</code> (human vs. human)

<code>
./connect4 c c
</code> (computer vs. computer)
<br><br>
On execution on a RaspberryPi without the SenseHat extension or a normal PC, an
<code>Error on call to open(): No such file or directory</code>
will be thrown, since the devices defined in libs/joystick.h and libs/led_matrix.h (/dev/input/event0 and /dev/fb1) do not exist.<br>
The SenseHat libraries have been provided by Pontus Ekberg (pontus.ekberg@it.uu.se).
