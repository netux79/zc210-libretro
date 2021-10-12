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

/* first the game saving & loading system */
static const char *SAVE_HEADER = "Zelda Classic Save File";
static const char *TEMP_SAVEFILE = "tmpsav.tmp";

int readsaves(gamedata *savedata, PACKFILE *f)
{
   uint16_t item_count;
   uint16_t qstpath_len;
   uint16_t save_count;
   uint16_t save_idx = 0;
   gamedata g;

   if (!p_igetw(&save_count, f, true))
      return 1;

   for (int i = 0; i < save_count; i++)
   {
      if (!pfread(g.name, sizeof(g.name), f, true))
         return 2;
      if (!p_getc(&g.quest, f, true))
         return 3;
      if (!p_igetw(&g.life, f, true))
         return 4;
      if (!p_igetw(&g.maxlife, f, true))
         return 5;
      if (!p_igetw(&g.drupy, f, true))
         return 6;
      if (!p_igetw(&g.rupies, f, true))
         return 7;
      if (!p_igetw(&g.deaths, f, true))
         return 8;
      if (!p_getc(&g.keys, f, true))
         return 9;
      if (!p_getc(&g.maxbombs, f, true))
         return 10;
      if (!p_getc(&g.wlevel, f, true))
         return 11;
      if (!p_getc(&g.cheat, f, true))
         return 12;
      if (!p_igetw(&item_count, f, true))
         return 13;
      for (int j = 0; j < item_count; ++j)
      {
         if (!p_getc(&g.items[j], f, true))
            return 14;
      }
      if (!pfread(g.version, sizeof(g.version), f, true))
         return 15;
      if (!pfread(g.title, sizeof(g.title), f, true))
         return 16;
      if (!p_getc(&g.hasplayed, f, true))
         return 17;
      if (!p_igetl(&g.time, f, true))
         return 18;
      if (!p_getc(&g.timevalid, f, true))
         return 19;
      for (int j = 0; j < MAXLEVELS; ++j)
      {
         if (!p_getc(&g.lvlitems[j], f, true))
            return 20;
      }
      if (!p_getc(&g.HCpieces, f, true))
         return 21;
      if (!p_getc(&g.continue_scrn, f, true))
         return 22;
      if (!p_getc(&g.continue_dmap, f, true))
         return 23;
      if (!p_igetw(&g.magic, f, true))
         return 24;
      if (!p_igetw(&g.maxmagic, f, true))
         return 25;
      if (!p_igetw(&g.dmagic, f, true))
         return 26;
      if (!p_getc(&(g.magicdrainrate), f, true))
         return 27;
      if (!p_getc(&g.canslash, f, true))
         return 28;
      for (int j = 0; j < MAXDMAPS; ++j)
      {
         if (!p_getc(&g.visited[j], f, true))
            return 29;
      }
      for (int j = 0; j < MAXDMAPS * 64; ++j)
      {
         if (!p_getc(&g.bmaps[j], f, true))
            return 29;
      }
      for (int j = 0; j < MAXMAPS2 * MAPSCRSNORMAL; j++)
      {
         if (!p_igetw(&g.maps[j], f, true))
            return 31;
      }
      for (int j = 0; j < MAXMAPS2 * MAPSCRSNORMAL; ++j)
      {
         if (!p_getc(&g.guys[j], f, true))
            return 32;
      }
      if (!p_igetw(&qstpath_len, f, true))
         return 33;
      if (!pfread(g.qstpath, qstpath_len, f, true))
         return 34;
      g.qstpath[qstpath_len] = 0;
      if (!pfread(g.icon, sizeof(g.icon), f, true))
         return 35;
      if (!pfread(g.pal, sizeof(g.pal), f, true))
         return 36;

      /* Confirm the entry matches the loaded quest; skip if error */
      if (g.title[0] && strcmp(g.title, QHeader.title))
      {
         zc_error("Save entry #%d: %s. Skipping", i + 1, qst_error[qe_match]);
         continue;
      }

      /* Confirm the save version is valid; skip if error */
      if (g.version[0] && (strcmp(g.version, QHeader.minver) < 0))
      {
         zc_error("Save entry #%d: %s. Skipping", i + 1, qst_error[qe_minver]);
         continue;
      }

      /* Keep the save entry */
      savedata[save_idx++] = g;

      /* if # of saves gets larger than MAXSAVES, we are done */
      if (save_idx == MAXSAVES)
         break;
   }

   return 0;
}

int load_savedgames(void)
{
   int ret;
   PACKFILE *f = NULL;
   char spath[MAX_STRLEN];
   char tpath[MAX_STRLEN];

   int  section_id;
   int  section_size;
   uint16_t section_version;
   uint16_t section_cversion;
   
   /* Calculate the save path to use */
   sprintf(spath, "%s%c%s", save_path, OTHER_PATH_SEPARATOR, qst_name);
   replace_extension(spath, spath, "sav");
   
   /* Calculate the temp file path */
   replace_filename(tpath, spath, TEMP_SAVEFILE);

   saves = (gamedata *)calloc(MAXSAVES, sizeof(gamedata));
   if (saves == NULL)
      return -1;

   /* see if it already exists */
   if (!file_exists(spath))
      goto newdata;

   /* decode to temp file */
   ret = decode_file_007(spath, tpath, SAVE_HEADER, ENC_METHOD_MAX - 1,
                         strstr(spath, ".dat#") != NULL);
   if (ret)
   {
      delete_file(tpath);
      zc_message("Format error. Resetting game data...");
      goto init;
   }

   /* load the games */
   f = pack_fopen(tpath, F_READ_PACKED);
   if (!f)
      goto newdata;

   /* section id */
   if (!p_mgetl(&section_id, f, true))
      goto reset;
   /* section version info */
   if (!p_igetw(&section_version, f, true))
      goto reset;
   if (!p_igetw(&section_cversion, f, true))
      goto reset;
   /* section size */
   if (!p_igetl(&section_size, f, true))
      goto reset;
   /* actual save data */
   if (readsaves(saves, f) != 0)
      goto reset;

   pack_fclose(f);
   delete_file(tpath);
   return 0;

newdata:
   zc_message("Save file not found. Creating new save file...");
   goto init;

reset:
   pack_fclose(f);
   delete_file(tpath);
   zc_error("Format error. Resetting game data...");

init:
   /* Clear all data with zeroes */
   memset(saves, 0, sizeof(gamedata) * MAXSAVES);

   return 0;
}

int writesaves(gamedata *savedata, PACKFILE *f)
{
   uint16_t item_count = iMax;
   uint16_t qstpath_len = 0;
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
      if (!pfwrite(savedata[i].bmaps, MAXDMAPS * 64, f))
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

int save_savedgames(void)
{
   char spath[MAX_STRLEN];
   char tpath[MAX_STRLEN];
   
   if (saves == NULL)
      return 1;

   /* Calculate the save path to use */
   sprintf(spath, "%s%c%s", save_path, OTHER_PATH_SEPARATOR, qst_name);
   replace_extension(spath, spath, "sav");
   
   /* Calculate the temp file path */
   replace_filename(tpath, spath, TEMP_SAVEFILE);

   PACKFILE *f = pack_fopen(tpath, F_WRITE_PACKED);
   if (!f)
   {
      delete_file(tpath);
      return 2;
   }

   int section_id = ID_SAVEGAME;
   int section_version = V_SAVEGAME;
   int section_cversion = CV_SAVEGAME;
   int section_size = 0;

   /* section id */
   if (!p_mputl(section_id, f))
      return 2;
   /* section version info */
   if (!p_iputw(section_version, f))
      return 3;
   if (!p_iputw(section_cversion, f))
      return 4;
   /* section size */
   if (!p_iputl(section_size, f))
      return 5;
   /* write the actual save data */
   if (writesaves(saves, f) != 0)
   {
      pack_fclose(f);
      delete_file(tpath);
      return 4;
   }

   pack_fclose(f);
   int ret = encode_file_007(tpath, spath, 0x413F0000 + (frame & 0xffff),
                             SAVE_HEADER, ENC_METHOD_MAX - 1);
   if (ret)
      ret += 100;

   delete_file(tpath);

   return ret;
}

void free_savedgames(void)
{
   if (saves)
   {
      free(saves);
      saves = NULL;
   }
}

void load_game_icon(gamedata *g)
{
   int i = high_item(imax_ring, itype_ring, true, g->items[itype_ring], true);
   int t = QMisc.icons[i];

   if (t < 0 || t >= NEWMAXTILES)
      t = 0;

   uint8_t *si = tilebuf + ((t ? t : 28) << 7);

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

static void select_mode(void)
{
   textout_ex(scrollbuf, zfont, "REGISTER YOUR NAME", 48, 152, 1, -1);
   textout_ex(scrollbuf, zfont, "COPY FILE", 48, 168, 1, -1);
   textout_ex(scrollbuf, zfont, "DELETE FILE", 48, 184, 1, -1);
}

static void copy_mode(void)
{
   textout_ex(scrollbuf, zfont, "COPY FILE", 48, 168, 3, -1);
}

static void delete_mode(void)
{
   textout_ex(scrollbuf, zfont, "DELETE FILE", 48, 184, 3, -1);
}

static void selectscreen(void)
{
   clear_bitmap(scrollbuf);
   blueframe(scrollbuf, 24, 48, 26, 20);
   textout_ex(scrollbuf, zfont, "- S E L E C T -", 64, 24, 1, 0);
   textout_ex(scrollbuf, zfont, " NAME ", 80, 48, 1, 0);
   textout_ex(scrollbuf, zfont, " LIFE ", 152, 48, 1, 0);
   select_mode();
}

static uint8_t left_arrow_str[] = {132, 0};
static uint8_t right_arrow_str[] = {133, 0};

static int savecnt;

static void list_save(int save_num, int ypos)
{
   if (save_num < savecnt)
   {
      game.maxlife = game.life = saves[save_num].maxlife;
      lifemeter(framebuf, 144,
                ypos + 24 + ((game.maxlife > 16 * (HP_PER_HEART)) ? 8 : 0));
      textout_ex(framebuf, zfont, saves[save_num].name, 72, ypos + 16, 1, -1);

      if (saves[save_num].quest)
         textprintf_ex(framebuf, zfont, 72, ypos + 24, 1, -1, "%3d",
                       saves[save_num].deaths);
   }
   
   /* Display Link icon */
   uint8_t *hold = tilebuf;
   tilebuf = saves[save_num].icon;
   overtile16(framebuf, 0, 48, ypos + 17, (save_num % 3) + csICON, 0);
   tilebuf = hold;
   
   if (zc_sync_pal)
   {
      /* Apply the icon palette from the save data */
      hold = colordata;
      colordata = saves[save_num].pal;
      loadpalset((save_num % 3) + csICON, 0);
      colordata = hold;
   }

   textout_ex(framebuf, zfont, "-", 136, ypos + 16, 1, -1);
}

static void list_saves(void)
{
   for (int i = 0; i < 3; i++)
      list_save(listpos + i, i * 24 + 56);

   /* Draw the arrows above the lifemeter! */
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

static void delete_save_entry(int entry)
{
   for (int i = entry; i < MAXSAVES - 1; i++)
      saves[i] = saves[i + 1];
   memset(&saves[MAXSAVES - 1], 0, sizeof(gamedata));
   --savecnt;
   if (listpos > savecnt - 1)
      listpos = max(listpos - 3, 0);
}

static bool register_name(void)
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
         sfx(SFX_CHIME);
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
         sfx(SFX_CHIME);
      }
      else if (rUp())
      {
         --grid_y;
         if (grid_y < 0)
            grid_y = letter_grid_height - 1;
         sfx(SFX_CHIME);
      }
      else if (rDown())
      {
         ++grid_y;
         if (grid_y >= letter_grid_height)
            grid_y = 0;
         sfx(SFX_CHIME);
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
         sfx(SFX_PLACE);
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

      if (zc_state)
         cancel = true;

   }
   while (!done && !cancel);

   if (done)
   {
      saves[s].quest = 0xFF; /* Always a custom quest for libretro */
      strncpy(saves[s].qstpath, qst_name, sizeof(saves[s].qstpath));
      strncpy(saves[s].version, QHeader.version, sizeof(saves[s].version));
      strncpy(saves[s].title, QHeader.title, sizeof(saves[s].title));
      saves[s].maxlife = zinit.hc * HP_PER_HEART;
      saves[s].items[itype_ring] = 0;
      if (zinit.ring)
         saves[s].items[itype_ring] = (1 << (min(zinit.ring, 3) - 1));
      ringcolor();
      saves[s].maxbombs = zinit.max_bombs;
      load_game_icon(saves + s);
      saves[s].timevalid = 1;
      save_savedgames();
   }
   else
      delete_save_entry(s);

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
      save_savedgames();
      sfx(SFX_SCALE);
      select_mode();
      return true;
   }
   return false;
}

static bool delete_file(int file)
{
   if (file < savecnt)
   {
      delete_save_entry(file);
      save_savedgames();
      sfx(SFX_OUCH);
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
   BITMAP *info = create_bitmap(160, 26);
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
   while (pos && i >= 70 && !zc_state);
   destroy_bitmap(info);

   char temp[23];
   strncpy(temp, saves[file].title, 23);
   temp[22] = '\0';
   textout_ex(framebuf, zfont, temp, 40, 104, 1, -1);

   textout_ex(framebuf, zfont, "CREATOR", 40, 112, 3, -1);
   textout_ex(framebuf, zfont, "ZELDA VER", 40, 120, 3, -1);
   textout_ex(framebuf, zfont, "PLAY TIME", 40, 128, 3, -1);

   strncpy(temp, QHeader.author, 14);
   temp[13] = '\0';
   textout_ex(framebuf, zfont, temp, 120, 112, 1, -1);
   textout_ex(framebuf, zfont, VerStr(QHeader.zelda_version), 120, 120, 1, -1);

   if (!saves[file].hasplayed)
      textout_ex(framebuf, zfont, "Empty Game", 120, 128, 1, -1);
   else if (!saves[file].timevalid)
      textout_ex(framebuf, zfont, "Time Unknown", 120, 128, 1, -1);
   else
      textout_ex(framebuf, zfont, time_str(saves[file].time), 120, 128, 1, -1);

   textout_ex(framebuf, zfont, "START: PLAY QUEST", 56, 152, 1, -1);
   textout_ex(framebuf, zfont, "    B: CANCEL", 56, 168, 1, -1);

   while (!zc_state)
   {
      advanceframe();
      if (rBbtn())
         return 0;
      if (rSbtn())
         return 1;
   }
   return 0;
}

static void select_game(void)
{
   int mode = 0;
   bool done = false;

   /* Use zelda title music in the selection screen */
   jukebox(MUSIC_TITLE);

   loadfullpal();
   selectscreen();

   savecnt = 0;
   while (savecnt < MAXSAVES && saves[savecnt].quest > 0)
      ++savecnt;

   /* if no saves, position the cursor at the register option */
   int pos = (savecnt == 0) ? 3 : max(min(currgame - listpos, 3), 0);

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

               zc_sync_pal = true;
               break;

            case 4:
               if (savecnt && savecnt < MAXSAVES)
               {
                  mode = 2;
                  pos = 0;
                  copy_mode();
               }
               zc_sync_pal = true;
               break;

            case 5:
               if (savecnt)
               {
                  mode = 3;
                  pos = 0;
                  delete_mode();
               }
               zc_sync_pal = true;
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
                        zc_sync_pal = true;
                     }
                     break;

                  case 3:
                     if (delete_file(pos + listpos))
                     {
                        mode = 0;
                        pos = 3;
                        zc_sync_pal = true;
                     }
                     break;
               }
         }

      if (rUp())
      {
         --pos;
         if (pos < 0)
            pos = (mode) ? 2 : 5;
         sfx(SFX_CHIME);
      }
      if (rDown())
      {
         ++pos;
         if (pos > ((mode) ? 2 : 5))
            pos = 0;
         sfx(SFX_CHIME);
      }
      if (rLeft() && listpos > 2)
      {
         listpos -= 3;
         sfx(SFX_CHIME);
         zc_sync_pal = true;
      }
      if (rRight() && listpos + 3 < savecnt)
      {
         listpos += 3;
         sfx(SFX_CHIME);
         zc_sync_pal = true;
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
   while (!zc_state && !done);

   /* stop selection screen music */
   music_stop();
}

/**************************************/
/****  Main title screen routine  *****/
/**************************************/

void titlescreen(void)
{
   int state = zc_state;

   zc_state = ZC_RUN;
   is_playing = false;

   if (state == ZC_RESUME)
   {
      resume_game();
      return;
   }

   if (state == ZC_CONT)
   {
      cont_game();
      setup_combo_animations();
      return;
   }

   if (state == ZC_RESET)
      reset_status();

   if (!zc_state)
      select_game();

   if (!zc_state)
      init_game();

   setup_combo_animations();
}

int selection_menu(void)
{
   textout_ex(framebuf, zfont, "CONTINUE", 88, 72, QMisc.colors.text, -1);
   textout_ex(framebuf, zfont, "SAVE", 88, 96, QMisc.colors.text, -1);
   textout_ex(framebuf, zfont, "RETRY", 88, 120, QMisc.colors.text, -1);

   int pos = 0;
   int f = -1;
   int htile = 2;
   bool done = false;
   zc_state = ZC_RUN;

   do
   {
      if (f == -1)
      {
         if (rUp())
         {
            sfx(SFX_CHINK);
            pos = (pos == 0) ? 2 : pos - 1;
         }
         if (rDown())
         {
            sfx(SFX_CHINK);
            pos = (pos + 1) % 3;
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
            }
         }
      }

      rectfill(framebuf, 72, 72, 79, 127, 0);
      puttile8(framebuf, htile, 72, pos * 24 + 72, 1, 0);
      advanceframe();
   }
   while (!zc_state && !done);

   return pos;
}

void zc_gameover(void)
{
   kill_sfx();
   music_stop();
   jukebox(MUSIC_GAMEOVER);

   loadfullpal();
   clear_bitmap(framebuf);

   int pos = selection_menu();

   reset_status();
   clear_bitmap(framebuf);
   advanceframe();

   if (!zc_state)
   {
      switch (pos)
      {
         case 0:
            zc_state = ZC_CONT;
            break;
         case 1:
            game.cheat |= cheat;
            saves[currgame] = game;
            load_game_icon(saves + currgame);
            save_savedgames();
         // fall thru...
         case 2:
            zc_state = ZC_QUIT;
            break;
      }
   }
}

void zc_quit(void)
{
   clear_bitmap(framebuf);

   int pos = selection_menu();

   /* if not resuming... */
   if (pos)
      reset_status();

   clear_bitmap(framebuf);
   advanceframe();

   if (!zc_state)
   {
      switch (pos)
      {
         case 0:
            zc_state = ZC_RESUME;
            break;
         case 1:
            game.cheat |= cheat;
            saves[currgame] = game;
            load_game_icon(saves + currgame);
            save_savedgames();
         // fall thru...
         case 2:
            zc_state = ZC_QUIT;
            break;
      }
   }
}

/*** end of title.cpp ***/
 
