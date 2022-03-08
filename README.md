![pekka](res/gfx/manual/pekka.png?raw=true)

Copyright (c) 2003 Janne Kivilahti

Ported to SDL by Samuli Tuomola (2010)
https://github.com/stt/pk2

Translated and ported to SDL2 by Danilo Lemos (2016-2020)

"Pekka Kana 2 (Pekka the Rooster 2) is a jump 'n run game made in the spirit of old classic platformers such as Super Mario, Sonic the Hedgehog, Jazz Jackrabbit, Super Frog and so on."
http://kiwi.mbnet.fi/pistegamez/pk2/

This repository contains the source code and resource files for the game Pekka Kana 2 created by Janne Kivilahti in 2003.
The source code was released by the original author on a forum
[thread](http://pistegamez.proboards.com/thread/543/level-editor-source-codes-available).

Since then a lot of things changed, including an initial SDL port by Samuli Tuomola in 2010 and then the SDL2 port, bug fixes,
and code redesign by Danilo Lemos (2016-2020).

This game was tested on Windows, GNU/Linux and Android.

# Binary distibution

### Windows
There is a compiled binary with the version 1.3.5.
https://files.fm/f/9q2np9y6h

### Android

[<img src="https://fdroid.gitlab.io/artwork/badge/get-it-on.png"
     alt="Get it on F-Droid"
     height="80">](https://f-droid.org/packages/org.pgnapps.pk2/)
[<img src="https://play.google.com/intl/en_us/badges/images/generic/en-play-badge.png"
     alt="Get it on Google Play"
     height="80">](https://play.google.com/store/apps/details?id=org.pgnapps.pk2)

# Compiling

### Windows
To compile on Windows, you will need the mingw c++ compiler with SDL2 and Zip (optional) libs.
There is a Makefile on `/mingw/i686/` that defines the needed stuff and include `/mingw/mingw.mk`.
Edit the `/mingw/i686/Makefile` as you need and run Make on this directory.

### Debian
To compile **Pekka Kana 2** into Debian GNU/Linux based distributions,
you need the **SDL2**, **SDL2_image** and **SDL2_mixer** libraries installed on
the system. To get everything you need, just run the following command on the terminal:

    $ apt install g++ make libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev

Then run the "**make**" command inside the "**Pekka Kana 2**" directory.

    $ make

The PK2 binary will be in "**bin/**" directory.

    $ ./bin/pekka-kana-2

To make the removal, within the compiled directory, execute this command:

    $ make clean

### Android
Run `git submodule init` then `git submodule update` to get the SDL libs. Then `./gradlew build`
on `android/` with the SDK and the NDK installed may work.

### macOS - not tested
Use [Homebrew](https://brew.sh/) to install SDL2 packages:
```
brew install sdl2 sdl2_gfx sdl2_image sdl2_mixer
```
Now just clone the code and run `make` on pk2 directory, the PK2 binary will be in `bin/` directory.

# User made episodes:
http://mapstore.the.cz/
To add a new episode, just download the zip and put it inside "mapstore/" on local data.

The game can be started with the "dev" argument to enable the
cheats and "test" following by the episode and level to
open directly on the level.

This example starts the level13.map on dev mode:
```
./pekka-kana-2 dev test "rooster island 2/level13.map"
```

# FIXMEs
* Android import zip, delete zip, zip file on save (can't load save with no zip)
* New graphics and controllers menu
* SDL_QUIT on Android
* Autosave settings option, ask when overwriting save, select save on new game
* Remove repeated languages
* Low volume sounds not played on low volume - music playing on volume 0
* Jump frames when fps is low
* Better sound quality

# PLANS
* Speed-run mode (frame counter, ghost)
* Better documentation
* Implement new layer on .map (regions)
* Find image (load .bmp or .png)
* New weather
* OpenGL ES
