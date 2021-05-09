//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  title.cpp
//
//  Title screen and intro for zelda.cpp
//  Also has game loading and select screen code.
//
//--------------------------------------------------------

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "zdefs.h"
#include "zelda.h"
#include "qst.h"
#include "tiles.h"
#include "pal.h"
#include "sprite.h"
#include "subscr.h"
#include "title.h"

/***********************************/
/****  Game Selection Screens  *****/
/***********************************/

// first the game saving & loading system

static const char *SAVE_HEADER = "Zelda Classic Save File";
static char SAVE_FILE[1048] = {'\0'};

int readsaves(gamedata *savedata, PACKFILE *f)
{
   word item_count;
   word qstpath_len;
   word save_count;

   if (!p_igetw(&save_count, f, true))
      return 1;
   for (int i = 0; i < save_count; i++)
   {
      if (!pfread(savedata[i].name, sizeof(savedata[i].name), f, true))
         return 2;
      if (!p_getc(&(savedata[i].quest), f, true))
         return 3;
      if (!p_igetw(&savedata[i].life, f, true))
         return 4;
      if (!p_igetw(&savedata[i].maxlife, f, true))
         return 5;
      if (!p_igetw(&savedata[i].drupy, f, true))
         return 6;
      if (!p_igetw(&savedata[i].rupies, f, true))
         return 7;
      if (!p_igetw(&savedata[i].deaths, f, true))
         return 8;
      if (!p_getc(&(savedata[i].keys), f, true))
         return 9;
      if (!p_getc(&(savedata[i].maxbombs), f, true))
         return 10;
      if (!p_getc(&(savedata[i].wlevel), f, true))
         return 11;
      if (!p_getc(&(savedata[i].cheat), f, true))
         return 12;
      if (!p_igetw(&item_count, f, true))
         return 13;
      for (int j = 0; j < item_count; ++j)
      {
         if (!p_getc(&(savedata[i].items[j]), f, true))
            return 14;
      }
      if (!pfread(savedata[i].version, sizeof(savedata[i].version), f, true))
         return 15;
      if (!pfread(savedata[i].title, sizeof(savedata[i].title), f, true))
         return 16;
      if (!p_getc(&(savedata[i].hasplayed), f, true))
         return 17;
      if (!p_igetl(&savedata[i].time, f, true))
         return 18;
      if (!p_getc(&(savedata[i].timevalid), f, true))
         return 19;
      for (int j = 0; j < MAXLEVELS; ++j)
      {
         if (!p_getc(&(savedata[i].lvlitems[j]), f, true))
            return 20;
      }
      if (!p_getc(&(savedata[i].HCpieces), f, true))
         return 21;
      if (!p_getc(&(savedata[i].continue_scrn), f, true))
         return 22;
      if (!p_getc(&(savedata[i].continue_dmap), f, true))
         return 23;
      if (!p_igetw(&savedata[i].magic, f, true))
         return 24;
      if (!p_igetw(&savedata[i].maxmagic, f, true))
         return 25;
      if (!p_igetw(&savedata[i].dmagic, f, true))
         return 26;
      if (!p_getc(&(savedata[i].magicdrainrate), f, true))
         return 27;
      if (!p_getc(&(savedata[i].canslash), f, true))
         return 28;
      for (int j = 0; j < MAXDMAPS; ++j)
      {
         if (!p_getc(&(savedata[i].visited[j]), f, true))
            return 29;
      }
      for (int j = 0; j < MAXDMAPS; ++j)
      {
         if (!p_getc(&(savedata[i].bmaps[j]), f, true))
            return 29;
      }
      for (int j = 0; j < MAXMAPS2 * MAPSCRSNORMAL; j++)
      {
         if (!p_igetw(&savedata[i].maps[j], f, true))
            return 31;
      }
      for (int j = 0; j < MAXMAPS2 * MAPSCRSNORMAL; ++j)
      {
         if (!p_getc(&(savedata[i].guys[j]), f, true))
            return 32;
      }
      if (!p_igetw(&qstpath_len, f, true))
         return 33;
      if (!pfread(savedata[i].qstpath, qstpath_len, f, true))
         return 34;
      savedata[i].qstpath[qstpath_len] = 0;
      if (!pfread(savedata[i].icon, sizeof(savedata[i].icon), f, true))
         return 35;
      if (!pfread(savedata[i].pal, sizeof(savedata[i].pal), f, true))
         return 36;
   }

   return 0;
}

// call once at startup
int load_savedgames()
{
   int ret;
   PACKFILE *f = NULL;
   const char *tmpfilename = "tmpsav";

   int  section_id;
   int  section_size;
   word section_version;
   word section_cversion;

   // Create the save file path by replacing the ext of qstpath
   replace_extension(SAVE_FILE, qstpath, "sav", sizeof(SAVE_FILE));

   if (saves == NULL)
   {
      saves = (gamedata *)malloc(sizeof(gamedata) * MAXSAVES);
      if (saves == NULL)
         return 1;
   }

   // see if it's there
#ifdef ALLEGRO_DOS
   if (file_size(SAVE_FILE) == 0)
   {
#else
   if (file_size_ex(SAVE_FILE) == 0)
   {
#endif
      goto newdata;
   }

   // decode to temp file
   ret = decode_file_007(SAVE_FILE, tmpfilename, SAVE_HEADER, ENC_METHOD_MAX - 1,
                         strstr(SAVE_FILE, ".dat#") != NULL);
   if (ret)
   {
      delete_file(tmpfilename);
      Z_message("Format error.  Resetting game data...");
      goto init;
   }

   // load the games
   f = pack_fopen(tmpfilename, F_READ_PACKED);
   if (!f)
      goto newdata;

   //section id
   if (!p_mgetl(&section_id, f, true))
      goto reset;

   //section version info
   if (!p_igetw(&section_version, f, true))
      goto reset;
   if (!p_igetw(&section_cversion, f, true))
      goto reset;

   //section size
   if (!p_igetl(&section_size, f, true))
      goto reset;

   if (readsaves(saves, f) != 0)
      goto reset;

   pack_fclose(f);
   delete_file(tmpfilename);
   return 0;

newdata:
   Z_message("Save file not found.  Creating new save file...");
   goto init;

reset:
   pack_fclose(f);
   delete_file(tmpfilename);
   Z_message("Format error.  Resetting game data...");

init:
   int *di = (int *)saves;
   for (unsigned i = 0; i < sizeof(gamedata)*MAXSAVES / sizeof(int); i++)
      *(di++) = 0;

   return 0;
}

int writesaves(gamedata *savedata, PACKFILE *f)
{
   word item_count = iMax;
   word qstpath_len = 0;
   if (!p_iputw(MAXSAVES, f))
      return 1;
   for (int i = 0; i < MAXSAVES; i++)
   {
      qstpath_len = strlen(savedata[i].qstpath);
      if (!pfwrite(savedata[i].name, sizeof(savedata[i].name), f))
         return 2;
      if (!p_putc(savedata[i].quest, f))
         return 3;
      if (!p_iputw(savedata[i].life, f))
         return 4;
      if (!p_iputw(savedata[i].maxlife, f))
         return 5;
      if (!p_iputw(savedata[i].drupy, f))
         return 6;
      if (!p_iputw(savedata[i].rupies, f))
         return 7;
      if (!p_iputw(savedata[i].deaths, f))
         return 8;
      if (!p_putc(savedata[i].keys, f))
         return 9;
      if (!p_putc(savedata[i].maxbombs, f))
         return 10;
      if (!p_putc(savedata[i].wlevel, f))
         return 11;
      if (!p_putc(savedata[i].cheat, f))
         return 12;
      if (!p_iputw(item_count, f))
         return 13;
      if (!pfwrite(savedata[i].items, item_count, f))
         return 14;
      if (!pfwrite(savedata[i].version, sizeof(savedata[i].version), f))
         return 15;
      if (!pfwrite(savedata[i].title, sizeof(savedata[i].title), f))
         return 16;
      if (!p_putc(savedata[i].hasplayed, f))
         return 17;
      if (!p_iputl(savedata[i].time, f))
         return 18;
      if (!p_putc(savedata[i].timevalid, f))
         return 19;
      if (!pfwrite(savedata[i].lvlitems, MAXLEVELS, f))
         return 20;
      if (!p_putc(savedata[i].HCpieces, f))
         return 21;
      if (!p_putc(savedata[i].continue_scrn, f))
         return 22;
      if (!p_putc(savedata[i].continue_dmap, f))
         return 23;
      if (!p_iputw(savedata[i].magic, f))
         return 24;
      if (!p_iputw(savedata[i].maxmagic, f))
         return 25;
      if (!p_iputw(savedata[i].dmagic, f))
         return 26;
      if (!p_putc(savedata[i].magicdrainrate, f))
         return 27;
      if (!p_putc(savedata[i].canslash, f))
         return 28;
      if (!pfwrite(savedata[i].visited, MAXDMAPS, f))
         return 29;
      if (!pfwrite(savedata[i].bmaps, MAXDMAPS, f))
         return 30;
      for (int j = 0; j < MAXMAPS2 * MAPSCRSNORMAL; j++)
      {
         if (!p_iputw(savedata[i].maps[j], f))
            return 31;
      }
      if (!pfwrite(savedata[i].guys, MAXMAPS2 * MAPSCRSNORMAL, f))
         return 32;
      if (!p_iputw(qstpath_len, f))
         return 33;
      if (!pfwrite(savedata[i].qstpath, qstpath_len, f))
         return 34;
      if (!pfwrite(savedata[i].icon, sizeof(savedata[i].icon), f))
         return 35;
      if (!pfwrite(savedata[i].pal, sizeof(savedata[i].pal), f))
         return 36;
   }
   return 0;
}

int save_savedgames(bool freemem)
{
   if (saves == NULL)
      return 1;

   const char *tmpfilename = "tmpsav";

   PACKFILE *f = pack_fopen(tmpfilename, F_WRITE_PACKED);
   if (!f)
   {
      delete_file(tmpfilename);
      return 2;
   }

   int section_id = ID_SAVEGAME;
   int section_version = V_SAVEGAME;
   int section_cversion = CV_SAVEGAME;
   int section_size = 0;

   //section id
   if (!p_mputl(section_id, f))
      return 2;

   //section version info
   if (!p_iputw(section_version, f))
      return 3;
   if (!p_iputw(section_cversion, f))
      return 4;

   //section size
   if (!p_iputl(section_size, f))
      return 5;

   if (writesaves(saves, f) != 0)
   {
      pack_fclose(f);
      delete_file(tmpfilename);
      return 4;
   }

   pack_fclose(f);
   int ret = encode_file_007(tmpfilename, SAVE_FILE, 0x413F0000 + (frame & 0xffff),
                             SAVE_HEADER, ENC_METHOD_MAX - 1);
   if (ret)
      ret += 100;

   delete_file(tmpfilename);
   if (freemem)
      free(saves);

   return ret;
}

void load_game_icon(gamedata *g)
{
   int i = high_item(imax_ring, itype_ring, true, g->items[itype_ring], true);
   int t = QMisc.icons[i];

   if (t < 0 || t >= NEWMAXTILES)
      t = 0;

   byte *si = tilebuf + ((t ? t : 28) << 7);

   for (int j = 0; j < 128; j++)
      g->icon[j] = *(si++);

   if (t)
      si = colordata + CSET(pSprite(i + spICON1)) * 3;

   else
   {
      if (i)
         si = colordata + CSET(pSprite(i - 1 + spBLUE)) * 3;

      else
         si = colordata + CSET(6) * 3;
   }

   for (int j = 0; j < 48; j++)
      g->pal[j] = *(si++);
}

static void select_mode()
{
   textout_ex(scrollbuf, zfont, "REGISTER YOUR NAME", 48, 152, 1, -1);
   textout_ex(scrollbuf, zfont, "COPY FILE", 48, 168, 1, -1);
   textout_ex(scrollbuf, zfont, "DELETE FILE", 48, 184, 1, -1);
}

static void copy_mode()
{
   textout_ex(scrollbuf, zfont, "COPY FILE", 48, 168, 3, -1);
}

static void delete_mode()
{
   textout_ex(scrollbuf, zfont, "DELETE FILE", 48, 184, 3, -1);
}

static void selectscreen()
{
   loadfullpal();
   clear_bitmap(scrollbuf);
   blueframe(scrollbuf, 24, 48, 26, 20);
   textout_ex(scrollbuf, zfont, "- S E L E C T -", 64, 24, 1, 0);
   textout_ex(scrollbuf, zfont, " NAME ", 80, 48, 1, 0);
   textout_ex(scrollbuf, zfont, " LIFE ", 152, 48, 1, 0);
   select_mode();
}

static byte left_arrow_str[] = {132, 0};
static byte right_arrow_str[] = {133, 0};

static int savecnt;

static void list_save(int save_num, int ypos)
{
   bool r = refreshpal;

   if (save_num < savecnt)
   {
      game.maxlife = game.life = saves[save_num].maxlife;
      lifemeter(framebuf, 144,
                ypos + 24 + ((game.maxlife > 16 * (HP_PER_HEART)) ? 8 : 0));
      textout_ex(framebuf, zfont, saves[save_num].name, 72, ypos + 16, 1, -1);

      if (saves[save_num].quest)
         textprintf_ex(framebuf, zfont, 72, ypos + 24, 1, -1, "%3d",
                       saves[save_num].deaths);

      textprintf_ex(framebuf, zfont, 72, ypos + 16, 1, -1, "%s",
                    saves[save_num].name);
   }

   byte *hold = tilebuf;
   tilebuf = saves[save_num].icon;
   overtile16(framebuf, 0, 48, ypos + 17, (save_num % 3) + 10, 0);    //link?
   tilebuf = hold;

   hold = colordata;
   colordata = saves[save_num].pal;
   loadpalset((save_num % 3) + 10, 0);
   colordata = hold;

   textout_ex(framebuf, zfont, "-", 136, ypos + 16, 1, -1);

   refreshpal = r;
}

static void list_saves()
{
   for (int i = 0; i < 3; i++)
      list_save(listpos + i, i * 24 + 56);

   // Draw the arrows above the lifemeter!
   if (savecnt > 3)
   {
      if (listpos >= 3)
         textout_ex(framebuf, zfont, (char *)left_arrow_str, 96, 60, 3, -1);
      if (listpos + 3 < savecnt)
         textout_ex(framebuf, zfont, (char *)right_arrow_str, 176, 60, 3, -1);
      textprintf_ex(framebuf, zfont, 112, 60, 3, -1, "%2d - %-2d", listpos + 1,
                    listpos + 3);
   }
}

static void draw_cursor(int pos, int mode)
{
   if (pos < 3)
      overtile8(framebuf, 0, 40, pos * 24 + 77, 1, 0);

   else
      overtile8(framebuf, 0, 40, (pos - 3) * 16 + 153, 1, 0);
}

static bool register_name()
{
   if (savecnt >= MAXSAVES)
      return false;

   saves[savecnt].maxlife = 3 * HP_PER_HEART;
   saves[savecnt].maxbombs = 8;
   saves[savecnt].continue_dmap = 0;
   saves[savecnt].continue_scrn = 0xFF;

   int s = savecnt;
   ++savecnt;
   listpos = (s / 3) * 3;

   int y = 72;
   int x = 0;
   int spos = 0;
   char name[9] = "        ";

   clear_keybuf();
   refreshpal = true;
   bool done = false;
   bool cancel = false;

   int letter_grid_x = 44;
   int letter_grid_y = 120;
   int letter_grid_offset = 8;
   int letter_grid_width = 11;
   int letter_grid_height = 4;
   int letter_grid_spacing = 16;
   const char *letter_grid = "ABCDEFGHIJKLMNOPQRSTUVWXYZ-.,!'&.0123456789 ";

   clear_bitmap(framebuf);
   blueframe(framebuf, 24, 48, 26, 8);
   textout_ex(framebuf, zfont, " NAME ", 80, 48, 1, 0);
   textout_ex(framebuf, zfont, " LIFE ", 152, 48, 1, 0);

   blueframe(framebuf, letter_grid_x - letter_grid_offset,
             letter_grid_y - letter_grid_offset, 23, 9);

   textout_ex(framebuf, zfont, "A B C D E F G H I J K", letter_grid_x,
              letter_grid_y, 1, -1);
   textout_ex(framebuf, zfont, "L M N O P Q R S T U V", letter_grid_x,
              letter_grid_y + 16, 1, -1);
   textout_ex(framebuf, zfont, "W X Y Z - . , ! ' & .", letter_grid_x,
              letter_grid_y + 32, 1, -1);
   textout_ex(framebuf, zfont, "0 1 2 3 4 5 6 7 8 9  ", letter_grid_x,
              letter_grid_y + 48, 1, -1);

   advanceframe();
   blit(framebuf, scrollbuf, 0, 0, 0, 0, 256, 224);

   int grid_x = 0;
   int grid_y = 0;

   do
   {
      spos = grid_y * letter_grid_width + grid_x;
      if (rLeft())
      {
         --grid_x;
         if (grid_x < 0)
         {
            grid_x = letter_grid_width - 1;
            --grid_y;
            if (grid_y < 0)
               grid_y = letter_grid_height - 1;
         }
         sfx(WAV_CHIME);
      }
      else if (rRight())
      {
         ++grid_x;
         if (grid_x >= letter_grid_width)
         {
            grid_x = 0;
            ++grid_y;
            if (grid_y >= letter_grid_height)
               grid_y = 0;
         }
         sfx(WAV_CHIME);
      }
      else if (rUp())
      {
         --grid_y;
         if (grid_y < 0)
            grid_y = letter_grid_height - 1;
         sfx(WAV_CHIME);
      }
      else if (rDown())
      {
         ++grid_y;
         if (grid_y >= letter_grid_height)
            grid_y = 0;
         sfx(WAV_CHIME);
      }
      else if (rBbtn())
      {
         ++x;
         if (x >= 8)
            x = 0;
      }
      else if (rAbtn())
      {
         name[min(x, 7)] = letter_grid[spos];
         ++x;
         if (x >= 8)
            x = 0;
         sfx(WAV_PLACE);
      }
      else if (rSbtn())
      {
         for (int i = 7; i >= 0; i--)
         {
            if (name[i] != ' ')
            {
               // Found name is not empty
               done = true;
               break;
            }
            else
               name[i] = '\0';
         }

         // cancel if name is empty
         cancel = !done;
      }

      strcpy(saves[s].name, name);
      blit(scrollbuf, framebuf, 0, 0, 0, 0, 256, 224);
      list_save(s, 56);

      int x2 = letter_grid_x + grid_x * letter_grid_spacing;
      int y2 = letter_grid_y + grid_y * letter_grid_spacing;

      if (frame & 8)
      {
         int tx = (min(x, 7) << 3) + 72;
         for (int dy = 0; dy < 8; dy++)
         {
            for (int dx = 0; dx < 8; dx++)
            {
               if (framebuf->line[y + dy][tx + dx] != 1)
                  framebuf->line[y + dy][tx + dx] = 3;

               if (framebuf->line[y2 + dy][x2 + dx] != 1)
                  framebuf->line[y2 + dy][x2 + dx] = 3;
            }
         }
      }

      draw_cursor(0, 0);
      advanceframe();

      if (Status)
         cancel = true;

   }
   while (!done && !cancel);

   if (done)
   {
      saves[s].quest = 0xFF;
      strncpy(saves[s].qstpath, get_filename(qstpath), 80);
      load_game(saves + s);
      saves[s].maxlife = zinit.hc * HP_PER_HEART;
      saves[s].items[itype_ring] = 0;
      if (zinit.ring)
         saves[s].items[itype_ring] = (1 << (min(zinit.ring, 3) - 1));
      ringcolor();
      saves[s].maxbombs = zinit.max_bombs;
      load_game_icon(saves + s);
      saves[s].timevalid = 1;
   }
   else
   {
      for (int i = s; i < MAXSAVES - 1; i++)
         saves[i] = saves[i + 1];
      int *di = (int *)(saves + MAXSAVES - 1);
      for (unsigned i = 0; i < sizeof(gamedata) / sizeof(int); i++)
         *(di++) = 0;
      --savecnt;
      if (listpos > savecnt - 1)
         listpos = max(listpos - 3, 0);
   }

   selectscreen();
   return done;
}

static bool copy_file(int file)
{
   if (savecnt < MAXSAVES && file < savecnt)
   {
      saves[savecnt] = saves[file];
      ++savecnt;
      listpos = ((savecnt - 1) / 3) * 3;
      sfx(WAV_SCALE);
      select_mode();
      return true;
   }
   return false;
}

static bool delete_save(int file)
{
   if (file < savecnt)
   {
      for (int i = file; i < MAXSAVES - 1; i++)
         saves[i] = saves[i + 1];
      int *di = (int *)(saves + MAXSAVES - 1);
      for (unsigned i = 0; i < sizeof(gamedata) / sizeof(int); i++)
         *(di++) = 0;
      --savecnt;
      if (listpos > savecnt - 1)
         listpos = max(listpos - 3, 0);
      sfx(WAV_OUCH);
      select_mode();
      return true;
   }
   return false;
}

static int game_details(int file)
{
   int pos = file % 3;
   if (saves[file].quest == 0)
      return 0;
   BITMAP *info = create_bitmap_ex(8, 160, 26);
   blit(framebuf, info, 48, pos * 24 + 70, 0, 0, 160, 26);
   rectfill(framebuf, 40, 60, 216, 192, 0);
   blueframe(framebuf, 24, 48, 26, 20);
   textout_ex(framebuf, zfont, " NAME ", 80, 48, 1, 0);
   textout_ex(framebuf, zfont, " LIFE ", 152, 48, 1, 0);

   int i = pos * 24 + 70;
   do
   {
      blit(info, framebuf, 0, 0, 48, i, 160, 26);
      advanceframe();
      i -= pos + pos;
   }
   while (pos && i >= 70);
   destroy_bitmap(info);

   char title[23];
   strncpy(title, saves[file].title, 23);
   title[22] = '\0';
   textout_ex(framebuf, zfont, title, 40, 104, 1, -1);

   textout_ex(framebuf, zfont, "FILE NAME", 40, 112, 3, -1);
   textout_ex(framebuf, zfont, "QUEST VER", 40, 120, 3, -1);
   textout_ex(framebuf, zfont, "PLAY TIME", 40, 128, 3, -1);

   textout_ex(framebuf, zfont, saves[file].qstpath, 120, 112, 1, -1);
   textout_ex(framebuf, zfont, saves[file].version, 120, 120, 1, -1);

   if (!saves[file].hasplayed)
      textout_ex(framebuf, zfont, "Empty Game", 120, 128, 1, -1);

   else if (!saves[file].timevalid)
      textout_ex(framebuf, zfont, "Time Unknown", 120, 128, 1, -1);

   else
      textout_ex(framebuf, zfont, time_str(saves[file].time), 120, 128, 1, -1);

   if (saves[file].cheat)
      textout_ex(framebuf, zfont, "(Cheats)", 120, 136, 1, -1);

   textout_ex(framebuf, zfont, "START: PLAY QUEST", 56, 152, 1, -1);
   textout_ex(framebuf, zfont, "    B: CANCEL", 56, 168, 1, -1);

   while (!Status)
   {
      advanceframe();
      if (rBbtn())
         return 0;
      if (rSbtn())
         return 1;
   }
   return 0;
}

static void select_game()
{
   int pos = max(min(currgame - listpos, 3), 0);
   int mode = 0;

   selectscreen();

   savecnt = 0;
   while (savecnt < MAXSAVES && saves[savecnt].quest > 0)
      ++savecnt;

   if (savecnt == 0)
      pos = 3;

   bool done = false;
   refreshpal = true;

   do
   {
      blit(scrollbuf, framebuf, 0, 0, 0, 0, 256, 224);
      list_saves();
      draw_cursor(pos, mode);
      advanceframe();

      if (rSbtn())
         switch (pos)
         {
            case 3:
               if (!register_name())
                  pos = 3;

               else
                  pos = (savecnt - 1) % 3;
               refreshpal = true;
               break;

            case 4:
               if (savecnt && savecnt < MAXSAVES)
               {
                  mode = 2;
                  pos = 0;
                  copy_mode();
               }
               refreshpal = true;
               break;

            case 5:
               if (savecnt)
               {
                  mode = 3;
                  pos = 0;
                  delete_mode();
               }
               refreshpal = true;
               break;

            default:
               switch (mode)
               {
                  case 0:
                     currgame = listpos + pos;
                     if (saves[currgame].quest)
                        done = true;
                     break;

                  case 2:
                     if (copy_file(pos + listpos))
                     {
                        mode = 0;
                        pos = (savecnt - 1) % 3;
                        refreshpal = true;
                     }
                     break;

                  case 3:
                     if (delete_save(pos + listpos))
                     {
                        mode = 0;
                        pos = 3;
                        refreshpal = true;
                     }
                     break;
               }
         }

      if (rUp())
      {
         --pos;
         if (pos < 0)
            pos = (mode) ? 2 : 5;
         sfx(WAV_CHIME);
      }
      if (rDown())
      {
         ++pos;
         if (pos > ((mode) ? 2 : 5))
            pos = 0;
         sfx(WAV_CHIME);
      }
      if (rLeft() && listpos > 2)
      {
         listpos -= 3;
         sfx(WAV_CHIME);
         refreshpal = true;
      }
      if (rRight() && listpos + 3 < savecnt)
      {
         listpos += 3;
         sfx(WAV_CHIME);
         refreshpal = true;
      }
      if (rBbtn() && mode)
      {
         if (mode == 2)
            pos = 4;
         if (mode == 3)
            pos = 5;
         mode = 0;
         select_mode();
      }
      if (rAbtn() && !mode && pos < 3)
      {
         if (game_details(pos + listpos))
         {
            currgame = listpos + pos;
            if (saves[currgame].quest)
               done = true;
         }
      }
   }
   while (!Status && !done);
}

/**************************************/
/****  Main title screen routine  *****/
/**************************************/

void titlescreen()
{
   int q = Status;

   Status = 0;
   Playing = false;

   if (q == qRESUME)
   {
      resume_game();
      return;
   }

   if (q == qCONT)
   {
      cont_game();
      setup_combo_animations();
      return;
   }

   if (q == qRESET)
      reset_status();

   if (!Status)
      select_game();

   if (!Status)
      init_game();

   setup_combo_animations();
}

int selection_menu()
{
   textout_ex(framebuf, zfont, "CONTINUE", 88, 72, QMisc.colors.text, -1);
   textout_ex(framebuf, zfont, "SAVE", 88, 96, QMisc.colors.text, -1);
   textout_ex(framebuf, zfont, "RETRY", 88, 120, QMisc.colors.text, -1);
   textout_ex(framebuf, zfont, "QUIT", 88, 144, QMisc.colors.text, -1);

   int pos = 0;
   int f = -1;
   int htile = 2;
   bool done = false;
   Status = 0;

   do
   {
      if (f == -1)
      {
         if (rUp())
         {
            sfx(WAV_CHINK);
            pos = (pos == 0) ? 3 : pos - 1;
         }
         if (rDown())
         {
            sfx(WAV_CHINK);
            pos = (pos + 1) % 4;
         }
         if (rSbtn())
            ++f;
      }

      if (f >= 0)
      {
         if (++f == 65)
            done = true;
         if (!(f & 3))
         {
            int c = (f & 4) ? QMisc.colors.text : QMisc.colors.caption;
            switch (pos)
            {
               case 0:
                  textout_ex(framebuf, zfont, "CONTINUE", 88, 72, c, -1);
                  break;
               case 1:
                  textout_ex(framebuf, zfont, "SAVE", 88, 96, c, -1);
                  break;
               case 2:
                  textout_ex(framebuf, zfont, "RETRY", 88, 120, c, -1);
                  break;
               case 3:
                  textout_ex(framebuf, zfont, "QUIT", 88, 144, c, -1);
                  break;
            }
         }
      }

      rectfill(framebuf, 72, 72, 79, 151, 0);
      puttile8(framebuf, htile, 72, pos * 24 + 72, 1, 0);
      advanceframe();

      // Need to avoid player hit the exit key
      // by mistake in this menu.
      if (Status == qEXIT)
         Status = 0;

   }
   while (!Status && !done);

   return pos;
}

void game_over()
{
   kill_sfx();
   music_stop();
   jukebox(MUSIC_GAMEOVER);

   clear_to_color(screen, BLACK);
   loadfullpal();
   clear_bitmap(framebuf);

   int pos = selection_menu();

   reset_status();
   clear_bitmap(framebuf);
   advanceframe();

   if (!Status)
   {
      switch (pos)
      {
         case 0:
            Status = qCONT;
            break;
         case 3:
            Status = qEXIT;
            break;
         case 1:
            game.cheat |= cheat;
            saves[currgame] = game;
            load_game_icon(saves + currgame);
            save_savedgames(false);
         // fall thru...
         case 2:
            Status = qQUIT;
            break;
      }
   }
}

void go_quit()
{
   clear_to_color(screen, BLACK);
   clear_bitmap(framebuf);

   int pos = selection_menu();

   // if not resuming...
   if (pos)
      reset_status();
   clear_bitmap(framebuf);
   advanceframe();

   if (!Status)
   {
      switch (pos)
      {
         case 0:
            Status = qRESUME;
            break;
         case 3:
            Status = qEXIT;
            break;
         case 1:
            game.cheat |= cheat;
            saves[currgame] = game;
            load_game_icon(saves + currgame);
            save_savedgames(false);
         // fall thru...
         case 2:
            Status = qQUIT;
            break;
      }
   }
}

/*** end of title.cpp ***/
