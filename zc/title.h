//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  title.h
//
//  Title screen and intro for zelda.cpp
//  Also has game loading and select screen code.
//
//--------------------------------------------------------

#ifndef _ZC_TITLE_H_
#define _ZC_TITLE_H_

int  load_savedgames();
int  save_savedgames(void);
void free_savedgames(void);
void load_game_icon(gamedata *g);
void titlescreen();
void zc_gameover();
void zc_quit();
#endif
