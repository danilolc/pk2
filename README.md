# Pekka Kana 2
by Janne Kivilahti from Piste Gamez (2003)

Ported to SDL by Samuli Tuomola (2010)
https://github.com/stt/pk2

Translated and ported to SDL2 by Danilo Lemos (2016/2017)

"Pekka Kana 2 (Pekka the Rooster 2) is a jump 'n run game made in the spirit of old classic platformers such as Super Mario, Sonic the Hedgehog, Jazz Jackrabbit, Super Frog and so on."
http://kiwi.mbnet.fi/pistegamez/pk2/


# Runing

### Windows:
There is a compiled binary with the "r2" version.
https://files.fm/f/d6mvf3qr

If you want to compile it yourself, the win32 directory have the solution files for VisualStudio. It will also need the SDL2, SDL2_mixer and SDL2_image library.

### Ubuntu:
To compile PK2 on Ubuntu you will need the SDL2, SDL2_image and SDL2_mixer library instaled on the system. To get them just run the following code:
```
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev
```
Now just clone the code and run `make` on pk2 directory, the PK2 binary will be in bin/ directory.


# User made episodes:
http://mapstore.the.cz/
To add a new episode, just download the zip file and extract it to its respective path inside "res/".

It can be started with the "dev" argument to start the
cheats and "test" following by the episode and level to
open directly on the level.

This example starts the level13.map on dev mode:
```
./PK2 dev test rooster\ island\ 2/level13.map
```


# TODOs and FIXMEs:
* Better port to Android (with resources)
* Consider Sprite in Water when the water is on BG or outside the screen
* Fix the mikmod music loop
* Finish the touch UI
* Recover the rain
* Recover the original physics
* Find a solution to change sfx frequency without delay
* Increase txt buttons area
* Fix the time counting to the real time
* Make the code cleaner (and remove all the Finnish things)
* Do a better resolution change
* Set the touch screen mouse in correct area
* Set energy to update before draw on screen
