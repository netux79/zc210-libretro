# zc210-libretro
A Zelda Classic v2.10 port for libretro (quest player only).

This is a work of pure love for ZC and a proof of concept it can be ported to Libretro without dependency on the Allegro library. It runs on 32 bits and 64 bits platforms with no special dependencies to build. Currently only tested on Linux, Windows, Raspberry Pi, Android and Nintendo Wii (for Big-Endian support).

- **Zelda Classic homepage:** https://www.zeldaclassic.com/
- **Pure ZC homepage (to get compatible Zelda Classic 2.10 quests):** https://www.purezc.net/
- **Armageddon Games homepage (Original creators of Zelda Classic):** https://www.armageddongames.com/

### Special Requirements
It requires the data file ***zcdata.dat*** to be located in the ***system/zc210*** folder of the Libretro frontend (RetroArch) This file is provided here in the GitHub project, located in the ***datfile*** folder of the project.

It requires a Sound Font (SF2 format) file named ***default.sf2*** for running and playing MIDI music to be located on the ***system/zc210/sf2*** folder of the frontend (RetroArch). Also, you can install other soundfont files in the same folder renaming them to ***customX.sf2*** where "X" is a number between 0 and 9. You will be able to select which one to use from the core options menu. I'm providing a basic set of soundfonts here in GitHub, located in the ***datfile/sf2*** folder of the project.

The ***zc210_libretro.info*** file is not required but recommended to have it installed on the frontend (RetroArch). It will tell you the status of all required and optional files for this core and their location. It is available in the ***info*** folder of this project.

If you want to use _optional_ custom sounds (some Zelda Classic quests come with a custom one or you can create one), you can use it by renaming it to ***Custom.dat*** and place it in the ***system/zc210/sfx*** folder of RetroArch. The sounds from it will replace the original ones for any quest. It accepts any version as it is not attached to the Zelda Classic engine version (2.10). I'm providing a set of sound effects files in the ***datfile/sfx*** folder of this project (GitHub) that you can install in the mentioned folder and then select which one to use from the core options menu.

If you simply want to go the easy way, just install the full ***datfile*** folder in the RetroArch ***system*** folder and rename it to ***zc210***.

### How to run a quest
After the requirements are satisfied, you just need to load a ***.qst*** file into the Libretro Frontend (RetroArch) and the quest will automatically start as a custom quest (even the original _1st.qst_, _2nd.qst_ will run as custom quests). The file can be also zipped in any compression format supported by the Libretro frontend (RetroArch) but not much gain will be obtained since the _.qst_ format is already a compressed file.

### Credits
- **Armageddon Games** - Original author of Zelda Classic
- **Bernhard Schelling** - Author of Tiny Sound Font used in this project for MIDI processing
- **Allegro team** - Creators of the library used by ZC and from which some code was ported for this project
- **netux79** - Author of this port

### Q&A
***Why v2.10 and not a newer version?***

Zelda Classic is still in development and the authors constantly add new features to it. I selected this old version to be ported since it was very stable and simple enough as it doesn't have all new functionalities that would make this project even more complicated to port. Also, each quest runs better on the version where it was created so it may need a different port for each of the ZC versions to support them all.

***Does it work on 64-bit?***

Yes, it can be compiled as a 64-bit or 32-bit libretro core.

***Does it run on Big-Endian platforms?***

Yes, it has already been ported to work on Big-Endian platforms but not much testing has been done since I just own 1 Big-Endian device (Nintendo Wii) so I cannot ensure it will work everywhere.

***Is there anything missing from the original Zelda Classic?***

Yes, there are no menus, no cheats (just a small-basic set using a combination of buttons), no image and MIDI songs export feature, no Zelda intro and no ZC Music support (no external mp3 and ogg files - which had very little use in this version of Zelda Classic but MIDI music is fully supported). Most of these were removed to adapt the engine to something more similar to a console.

Only the player was ported, the ZQuest tool was not ported so you won't be able to create new quests on it. It only is useful to play the existing ones.

***Do you plan to port a newer version of Zelda Classic to Libretro?***

No current plans of doing it. It is just a _Proof-of-Concept_ for the authors so they can see their project can be ported to Libretro. Also, I am only interested in the old quests from back in the day when I discovered Zelda Classic and all these can be played on this core.

***Are the save files compatible with the original ZC saves?***

No, but not on purpose. The original ZC 2.10 has a bug that was missing some data been saved which has been corrected making it incompatible.

### Programming language
C, C++

### Building
To compile, you will need a C++ compiler and assorted toolchain installed.

	make
