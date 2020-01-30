# Contributing to Pekka Kana 2

![pekka](res/gfx/manual/chick.png?raw=true)
The main goal here is to make this a great code for beginners, who want to experiment new things with the power of C/C++ with SDL2.
The code must be the simplest possible, self explanatory and clean.

Everybody can contribute for this, don't be afraid to send pull requests, we will try to answer it properly.

All the magic happen on the `src/` directory, where you can find all the code of the game (except some platform specific, like the PK2Activity.java for android)

The game is basically two objects: the object `Episode`, started when you init a episode, it contains all the episode specific information, like player name, current level and episode score; and the object `Game`, started when you init a game, it contains (or must contain), all information specific for the game.

Here is a scheme explaining what each file do:

```
src/

    engine/ - It is the interface with the game and the SDL2 or Zlib (they should be included only here)
        PDraw.cpp - It contains a 8 bit 800x480 indexed image and is where all the game is drawn
        PFile.cpp - The PFile class wich is a std::string with some file specific methods
        PFont.cpp - The PFont class, it is used to draw texts on the game 
        PInput.cpp - All the input specific methods
        Piste.cpp - The engine main source, it defines the main loop and calls other engine sources
        PLang.cpp - The PLang class, wich loads Piste Gamez language files
        PLog.cpp - Does the log of the game and saves it on a file
        PSound.cpp - All the sound interface, for music and sfx
        PUtils.cpp - Some platform specific functions, like CreateDir and Setcwd
    
    episode/
        episodeclass.cpp - The definition of the episode class
        mapstore.cpp - Searches episodes on the game and on the zips
    
    game/
        blocks.cpp - Defines the blocks that form the game scenery
        game.cpp - The definition of the game class
        gifts.cpp - The gift management methods
        mapclass.cpp - The structure that defines the .map file
        physics.cpp - Updates all the sprites of the game
        spriteclass.cpp - The structure that defines the .spr file
        sprites.cpp - The sprite list of the game
    
    gfx/
        effect.cpp - Methods that create a lot of particles to create a effect
        particle.cpp - Defines the class for the game particles, like stars and dust
        particles.cpp - The particles list and updater
        text.cpp - The methods to draw different types of texts
    
    screens/ - Each screen on the game have a different loop and does different things
        ending.cpp - The screen played when you end an episode
        intro.cpp - The screen played when you open the game
        map.cpp - The screen played when you select the level
        menu.cpp - The screen played when you are on the menu
        playing.cpp - The screen played when you are playing the game
        score.cpp - The screen played when you finnish the level
        screens.cpp - Select the screen and run it, the game main loop is here

    gui.cpp - The android controls
    language.cpp - Contains almost every text of the game, it changes based on the language
    main.cpp - The starting point of the game, it calls the engine and the game screen
    save.cpp - Load and record the save file
    settings.cpp - Load and record the settings file
    sfx.cpp - The sound effects of the game, it holds the sfx playing to change panning and volume
    system.cpp - Methods and values common to every scene

```