# zc210-libretro
A Zelda Classic v2.10 port for libretro (quest player only).

This is a work of pure love for ZC and a proof of concept it can be ported to Libretro without dependency on the Allegro library. It runs on 32 bits and 64 bits platforms with no special dependencies to build. Currently only tested on Linux, Windows, Raspberry Pi and Android.

<b>Zelda Classic homepage:</b> https://www.zeldaclassic.com/</br>
<b>Pure ZC homepage (to get compatible Zelda Classic 2.10 quests):</b> https://www.purezc.net/</br>
<b>Armageddon Games homepage (Original creators of Zelda Classic):</b> https://www.armageddongames.com/

# Special Requirements
It requires the data file <b>zcdata.dat</b> to be located in the system folder of the Libretro frontend (RetroArch).</br>
It requires a Sound Font file <b>zcsf.sf2</b> for running and playing MIDI music to be located on the system folder of the frontend (RetroArch). I recommend a small Sound Font like <b>gbfont.sf2</b> or <b>scc1t2.sf2</b> (which sounds very similar to the Windows MIDI engine where it originally runs). Not provided but you can get one on the Internet very easily.</br>
The <b>zc210_libretro.info</b> file is not required but recommended to have it installed on the frontend (RetroArch). Available in the info folder of this project.</br>
If you want to use custom sounds (some quests provide it or you can create one), you can use it by renaming it to <b>zcsfx.dat</b> and place it in the system folder. The sounds from it will replace the original ones. It accepts any version as it is not attached to the engine version (2.10).

# How to run a quest
After the requirements are satisfied, you just need to load a <b>.qst</b> file into the Libretro Frontend (retroArch) and it the quest will automatically start as a custom quest (even the original 1st.qst, 2nd.qst will run as custom quests). The file can be also zipped in any compression format supported by the Libretro frontend (RetroArch).

# Credits
<b>Armageddon Games</b> - Original author of Zelda Classic</br>
<b>Bernhard Schelling</b> - Author of Tiny Sound Font used in this project for MIDI processing</br>
<b>Allegro team</b> - Creators of the library used by ZC and from which some code was ported for this project</br>
<b>netux79</b> - Author of this port

# Q&A
<b>why v2.10 and not a newer version?</b></br>
Zelda Classic is still in development and the authors constantly add new features to it. I selected this old version to be ported since it was very stable and simple enough as it doesn't have all new functionalities that would make this project even more complicated to port. Also, each quest runs better on the version where it was created so it may need a different port for each of the ZC versions to support them all.</br></br>

<b>Does it work on 64-bit?</b></br>
Yes, it can be compiled as a 64-bit or 32-bit libretro core.</br></br>

<b>Does it run on Big Endian platforms?</b></br>
No, it has not been yet ported to work on Big Endian platforms.</br></br>

<b>Is there anything missing from the original Zelda Classic?</b></br>
Yes, there are no menus, no cheats, no exporting of images and MIDI songs, no intro and no ZC Music (external mp3, ogg files) support. Most of these were removed to adapt the engine to something more similar to a console.</br>
Only the player was ported, the ZQuest tool was not ported so you won't be able to create new quests on it. It only is useful to play the existing ones.</br></br>

<b>Do you plan to port a newer version of Zelda Classic to Libretro?</b></br>
No plans of doing it. It is just a POC for the authors so they can see their project can be ported.</br></br>

<b>Do you plan to make it work on Big Endian platforms?</b></br>
Yes, but not sure when it will be done.</br></br>

<b>Are the save files compatible with the original ZC saves?</b></br>
No, but not on purpose. The original ZC 2.10 has a bug that was missing some data been saved which has been corrected making it incompatible.</br></br>

# Programming language
C, C++

# Building
To compile, you will need a C++ compiler and assorted toolchain installed.

	make
