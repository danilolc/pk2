# Pekka Kana 2 "Greta engine"

![pekka](res/gfx/manual/pekka.png?raw=true)

Pekka Kana 2 (Pekka the Rooster 2) is a 2D platformer game where you control a rooster, made in the spirit of classic platformers such as Super Mario, SuperTux, Jazz Jackrabbit, Super Frog and so on. <br>
https://www.pistegamez.net/game_pk2.html


This repository contains the official open-source source code and resource files
for Pekka Kana 2, maintained by SaturninTheAlien with contributions
from the Piste Gamez community.

## About this repository

This repository represents the current official continuation of Pekka Kana 2.
It preserves the original gameplay and content while allowing technical
improvements, ports, refactoring, and new features for creators and developers.

## Project history

- **2003** – Original game created by **Janne Kivilahti**  
  https://github.com/poppijanne  
  https://github.com/pistegamez

- **2010** – SDL port by **Samuli Tuomola**  
  https://github.com/stt/pk2

- **2016–2023** – SDL2 port, refactoring, and maintenance by **Danilo Lemos**  
  https://github.com/danilolc/pk2

- **2023–present** – Ongoing maintenance, refactoring, and new features by
  **SaturninTheAlien**, with the knowledge and general approval of the  original author
  
## Our community
* Discord: https://discord.gg/kqDJfYX
* Old forum: http://pistegamez.proboards.com/
* Pistegamez site: https://www.pistegamez.net/

## ⚠️ Warning!
- If you have **photosensitive epilepsy**, Pekka Kana 2 (including both vanilla and 3rd party content) may not be suitable for you.\
The game may contain screen shaking, flashing lights, repetitive patterns, and motion effects that could trigger seizures.\
Proceed at your own risk.

- Pekka Kana 2 (including both vanilla and 3rd party content) contains content that some players may find disturbing, such as **fantasy violence** and other potentially sensitive elements. We are not responsible for any potential psychological distress caused by the game. 

- Pekka Kana 2 was created for entertainment and artistic purposes only. It is not intended to offend anyone.


## Platform support

The game has been tested on:

- Windows
- GNU/Linux
- macOS (Apple Silicon)

Pekka Kana 2 can also be run on older Intel-based Macs 
using the Windows portable version together with Wine.
This configuration is not officially tested or supported.

## Binary distibution

See the <a href="https://github.com/SaturninTheAlien/pk2_greta/releases">Releases</a> tab!

MS Windows binaries were compiled using MS Visual Studio Community 2022 on MS Windows 11,
so if there is any missing DLL file you perhaps need to install the latest Microsoft Visual C++ Redistributable:\
https://aka.ms/vs/17/release/vc_redist.x64.exe


## Compiling

Begin by cloning the repository using Git:

```sh
git clone https://github.com/SaturninTheAlien/pk2_greta
```

### Linux

To compile **Pekka Kana 2** you need:
- C++ compiler (GCC or Clang)
- make
- pkg-config
- SDL2
- SDL2_image
- SDL2_mixer
- libzip
- Lua5.4 or newer

On Linux with "apt-get" package manager, you can install every dependency with a command:
```sh
sudo apt install build-essential libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libzip-dev liblua5.4-dev
```

Then run the "**make**" command inside the "**Pekka Kana 2**" directory.
```sh
make all
```

The PK2 binary will be in "**bin/**" directory, it can be run with:
```sh
./bin/pekka-kana-2
```

To make the removal, within the compiled directory, execute this command:
```sh
make clean
```

To update the game to the latest version:
```sh
git pull
make clean
make all
```

Parallel builds using `make -j` are supported.
For example, on a system with 8 CPU cores,
```sh
make -j8 all
```
can be used to speed up the compilation process.


### macOS on Apple Silicon
Use [Homebrew](https://brew.sh/) to install SDL2 packages:
```
brew install pkgconf sdl2 sdl2_gfx sdl2_image sdl2_mixer libzip lua
```

Clone the repository and build the project using `make all`.
The game can be run in the same way as on Linux:

```sh
./bin/pekka-kana-2
```

## User-made episodes

Community-created episodes are available here:
https://sites.google.com/view/pekka-kana-fanpage/pk2-makyunis-mapstore

To install an episode, download the ZIP file and place it in the `data/mapstore`
directory.

### Mapstore directory location

The location of the `data` directory (including `mapstore`) depends on how
Pekka Kana 2 is installed or run. This is expected behavior.

- **Source build (running from the repository):**  
  `res/data/mapstore`

- **Windows (portable build):**  
  `data/mapstore`

- **Windows (installer):**  
  `C:\Users\<user>\AppData\Roaming\piste-gamez\pekka-kana-2\mapstore`

- **Linux (installed package or `sudo make install`):**  
  `~/.local/share/piste-gamez/pekka-kana-2/mapstore`



## Advanced options

### Commandline arguments 

The game can be started with the "--dev" argument to enable the
cheats and "--test" followed by "episode_name/level_filename" to play/test a particular level. 

This example starts the level13.map (the robot boss fight) on dev mode:
```sh
./pekka-kana-2 --dev --test "rooster island 2/level13.map"
```
Absolute paths to custom locations are also accepted, such as:
```sh
./pekka-kana-2 --test /home/saturnin/pk2_episodes/my_episode/my_level.map
```

### Config.txt

Advanced configuration options can be found in `data/config.txt`.
This file is created automatically when the game is run for the first time.

## Internal formats documentation
Documentation of internal file formats is available on Makyuni's fan page:
https://sites.google.com/view/pekka-kana-fanpage/pekka-kana-2/useful-lists


## Development notes
- Translation of all the Finnish variable names and comments to English is planned.

## Known bugs
- Sprites sometimes do not detect edges correctly near `BLOCK_BARRIER_DOWN`
  (legacy behavior).


## Used libraries

- **nlohmann/json** – JSON for Modern C++  
  https://github.com/nlohmann/json

- **SDL2**, **SDL2_image**, **SDL2_mixer**  
  https://www.libsdl.org/

- **libzip**  
  https://libzip.org/

- **sol2**  
  https://github.com/ThePhD/sol2