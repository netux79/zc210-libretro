//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  pal.cpp
//
//  Palette code for ZC.
//
//--------------------------------------------------------

#include "zdefs.h"
#include "maps.h"
#include "zelda.h"
#include "pal.h"
#include "subscr.h"

RGB_MAP rgb_table;
COLOR_MAP trans_table;

const byte nes_pal[] =
{
   31, 31, 31,                                               //  0
   0, 0, 63,                                                 //  1
   0, 0, 47,                                                 //  2
   17, 10, 47,                                               //  3
   37, 0, 33,                                                //  4
   42, 0, 8,                                                 //  5
   42, 4, 0,                                                 //  6
   34, 5, 0,                                                 //  7
   20, 12, 0,                                                //  8
   0, 30, 0,                                                 //  9
   0, 26, 0,                                                 // 10
   0, 22, 0,                                                 // 11
   0, 16, 22,                                                // 12
   0, 0, 0,                                                  // 13 0D
   0, 0, 0,                                                  // 14 0E
   0, 0, 0,                                                  // 15 0F
   47, 47, 47,                                               // 16
   0, 30, 62,                                                // 17
   0, 22, 62,                                                // 18
   26, 17, 63,                                               // 19
   54, 0, 51,                                                // 20
   57, 0, 22,                                                // 21
   62, 14, 0,                                                // 22
   57, 23, 4,                                                // 23
   43, 31, 0,                                                // 24
   0, 46, 0,                                                 // 25
   0, 42, 0,                                                 // 26
   0, 42, 17,                                                // 27
   0, 34, 34,                                                // 28
   0, 0, 0,                                                  // 29
   0, 0, 0,                                                  // 30
   0, 0, 0,                                                  // 31
   62, 62, 62,                                               // 32
   15, 47, 63,                                               // 33
   26, 34, 63,                                               // 34
   38, 30, 62,                                               // 35
   62, 30, 62,                                               // 36
   62, 22, 38,                                               // 37
   62, 30, 22,                                               // 38
   63, 40, 17,                                               // 39
   62, 46, 0,                                                // 40
   46, 62, 6,                                                // 41
   22, 54, 21,                                               // 42
   22, 62, 38,                                               // 43
   0, 58, 54,                                                // 44
   30, 30, 30,                                               // 45
   0, 0, 0,                                                  // 46
   0, 0, 0,                                                  // 47
   63, 63, 63,                                               // 48 !
   41, 57, 63,                                               // 49
   52, 52, 62,                                               // 50
   54, 46, 62,                                               // 51
   62, 46, 62,                                               // 52
   62, 41, 48,                                               // 53
   60, 52, 44,                                               // 54
   63, 56, 42,                                               // 55
   62, 54, 30,                                               // 56
   54, 62, 30,                                               // 57
   46, 62, 46,                                               // 58
   46, 62, 54,                                               // 59
   0, 63, 63,                                                // 60
   62, 54, 62,                                               // 61
   0, 54, 50,                                                // 62
   31, 63, 63                                                // 63
};

RGB _RGB(const byte *si)
{
   RGB x;
   x.r = si[0];
   x.g = si[1];
   x.b = si[2];
   return x;
}

RGB _RGB(int r, int g, int b)
{
   RGB x;
   x.r = r;
   x.g = g;
   x.b = b;
   return x;
}

void loadfullpal()
{
   for (int i = 0; i < 240; i++)
      RAMpal[i] = _RGB(colordata + i * 3);
   for (int i = 240; i < 256; i++)
      RAMpal[i] = ((RGB *)data[PAL_GUI].dat)[i];
   zc_sync_pal = true;
}

/* create_zc_trans_table:
 *  Constructs a translucency color table for the specified palette. The
 *  r, g, and b parameters specifiy the solidity of each color component,
 *  ranging from 0 (totally transparent) to 255 (totally solid).
 */
void create_zc_trans_table(COLOR_MAP *table, const PALETTE pal, int r, int g,
                           int b)
{
   int tmp[768], *q;
   int x, y, i, j, k;
   unsigned char *p;
   RGB c;

   for (x = 0; x < 256; x++)
   {
      tmp[x * 3]   = pal[x].r * (255 - r) / 255;
      tmp[x * 3 + 1] = pal[x].g * (255 - g) / 255;
      tmp[x * 3 + 2] = pal[x].b * (255 - b) / 255;
   }

   for (x = 0; x < PAL_SIZE; x++)
   {
      i = pal[x].r * r / 255;
      j = pal[x].g * g / 255;
      k = pal[x].b * b / 255;

      p = table->data[x];
      q = tmp;

      if (rgb_map)
      {
         for (y = 0; y < PAL_SIZE; y++)
         {
            c.r = i + *(q++);
            c.g = j + *(q++);
            c.b = k + *(q++);
            p[y] = rgb_map->data[c.r >> 1][c.g >> 1][c.b >> 1];
         }
      }
      else
      {
         for (y = 0; y < PAL_SIZE; y++)
         {
            c.r = i + *(q++);
            c.g = j + *(q++);
            c.b = k + *(q++);
            p[y] = bestfit_color(pal, c.r, c.g, c.b);
         }
      }
   }
}

void loadlvlpal(int level)
{
   byte *si = colordata + CSET(level * pdLEVEL + poLEVEL) * 3;

   for (int i = 0; i < 16 * 3; i++)
   {
      RAMpal[CSET(2) + i] = _RGB(si);
      si += 3;
   }

   for (int i = 0; i < 16; i++)
   {
      RAMpal[CSET(9) + i] = _RGB(si);
      si += 3;
   }

   if (!get_bit(quest_rules, qr_NOLEVEL3FIX) && level == 3)
      RAMpal[CSET(6) + 2] = NESpal(0x37);

   create_rgb_table(&rgb_table, RAMpal);
   rgb_map = &rgb_table;
   create_zc_trans_table(&trans_table, RAMpal, 128, 128, 128);

   zc_sync_pal = true;
}

void loadpalset(int cset, int dataset)
{
   byte *si = colordata + CSET(dataset) * 3;
   for (int i = 0; i < 16; i++)
   {
      RAMpal[CSET(cset) + i] = _RGB(si);
      si += 3;
   }
   zc_sync_pal = true;
}

void ringcolor()
{
   switch (current_item(itype_ring, true))
   {
      case 0:
         loadpalset(6, 6);
         break;
      case 1:
         loadpalset(6, pSprite(6));
         break;
      case 2:
         loadpalset(6, pSprite(7));
         break;
      case 3:
         loadpalset(6, pSprite(8));
         break;
   }
   zc_sync_pal = true;
}

void loadfadepal(int dataset)
{
   byte *si = colordata + CSET(dataset) * 3;

   for (int i = 0; i < pdFADE * 16; i++)
   {
      RAMpal[CSET(2) + i] = _RGB(si);
      si += 3;
   }

   zc_sync_pal = true;
}

void fade(int level, bool blackall, bool fromblack, bool total)
{
   int cx = fromblack ? 30 : 0;
   for (int i = 0; i <= 30; i += (get_bit(quest_rules, qr_FADE)) ? 2 : 1)
   {
      if (get_bit(quest_rules, qr_FADE))
      {
         int dpos = (cx << 6) / 30;
         int lpos = min(dpos, blackall ? 64 : 32);
         int last = CSET(5) - 1;

         if (get_bit(quest_rules, qr_FADECS5))
         {
            last += 16;
            loadpalset(5, 5);
         }

         loadlvlpal(level);
         byte *si = colordata + CSET(level * pdLEVEL + poFADE1) * 3;
         for (int i = 0; i < 16; i++)
         {
            int light = si[0] + si[1] + si[2];
            si += 3;
            fade_interpolate(RAMpal, black_palette, RAMpal, light ? lpos : dpos,
                             CSET(2) + i, CSET(2) + i);
         }
         fade_interpolate(RAMpal, black_palette, RAMpal, dpos, CSET(3), last);
         zc_sync_pal = true;
      }
      else
      {
         switch (cx)
         {
            case 0:
               loadlvlpal(level);
               break;
            case 10:
               loadfadepal(level * pdLEVEL + poFADE1);
               break;
            case 20:
               loadfadepal(level * pdLEVEL + poFADE2);
               break;
            case 30:
               if (blackall)
               {
                  for (int i = 0; i < pdFADE * 16; i++)
                     RAMpal[CSET(2) + i] = black_palette[0];
                  zc_sync_pal = true;
               }
               else
                  loadfadepal(level * pdLEVEL + poFADE3);
               break;
         }
      }

      if (!get_bit(quest_rules, qr_NOLEVEL3FIX) && level == 3)
         RAMpal[CSET(6) + 2] = NESpal(0x37);

      putsubscr(framebuf, 0, 0);
      advanceframe();
      if (zc_state)
         break;
      fromblack ? --cx : ++cx;
      if (get_bit(quest_rules, qr_FADE))
         fromblack ? --cx : ++cx;
   }
}

void lighting(int funct, int dir)
// 1 = use candle
// 2 = exit room
// 3 = enter room
// 4 = scrolling warp exit
{
   switch (funct)
   {
      case 1:
         if (darkroom)
         {
            fade(DMaps[currdmap].color, false, true, false);
            darkroom = false;
         }
         break;
      case 2:                                                 //currently light.  room you are going to is supposed to be dark.
         if (tmpscr->flags & fDARK && !darkroom)
         {
            fade(DMaps[currdmap].color, false, false, false);
            darkroom = true;

         }
         break;
      case 3:                                                 //currently dark.  room you are going to is supposed to be light.
         if ((!(tmpscr->flags & fDARK) && darkroom)
               || (DMaps[currdmap].color != currcset))
         {
            fade(DMaps[currdmap].color, false, true, false);
            darkroom = false;
         }
         currcset = DMaps[currdmap].color;
         break;
      case 4:                                                 //currently light.  room you are going to is supposed to be dark.
         if ((tmpscr->flags & fDARK && !darkroom)
               || (DMaps[(tmpscr[((currscr < 128) ? 0 : 1)].sidewarpdmap)].color != currcset))
         {
            fade(DMaps[currdmap].color, true, false, true);
            darkroom = true;
         }
         break;
   }
}

byte drycolors[11] = {0x12, 0x11, 0x22, 0x21, 0x31, 0x32, 0x33, 0x35, 0x34, 0x36, 0x37};

void dryuplake()
{
   if (whistleclk < 0 || whistleclk >= 88)
      return;
   if ((++whistleclk) & 7)
      return;
   if (whistleclk < 88)
   {
      RAMpal[CSET(3) + 3] = NESpal(drycolors[whistleclk >> 3]);
      zc_sync_pal = true;
   }
   else
   {
      if ((hiddenstair(0, true)) && (!nosecretsounds))
         sfx(SFX_SECRET);
   }
}

void rehydratelake()
{
   if (whistleclk == -1)
      return;
   whistleclk &= 0xF8;
   do
   {
      whistleclk -= 8;
      RAMpal[CSET(3) + 3] = NESpal(drycolors[whistleclk >> 3]);
      zc_sync_pal = true;
      advanceframe();
      if (((whistleclk >> 3) & 3) == 1)
         for (int i = 0; i < 4 && !zc_state; i++)
            advanceframe();
   }
   while (whistleclk != 0 && !zc_state);
   whistleclk = -1;
   loadpalset(3, 3);
}

static int palclk[3];
static int palpos[3];

void reset_pal_cycling()
{
   for (int i = 0; i < 3; i++)
      palclk[i] = palpos[i] = 0;
}

void cycle_palette()
{
   if (!get_bit(quest_rules, qr_FADE) || darkroom)
      return;

   int level = (currscr < 128) ? DMaps[currdmap].color : (isdungeon() ? 11 : 10);

   for (int i = 0; i < 3; i++)
   {
      palcycle c = QMisc.cycles[level][i];
      if (c.count & 0xF0)
      {
         if (++palclk[i] >= c.speed)
         {
            palclk[i] = 0;
            if (++palpos[i] >= (c.count >> 4))
               palpos[i] = 0;

            byte *si = colordata + CSET(level * pdLEVEL + poFADE1 + 1 + palpos[i]) * 3;
            si += (c.first & 15) * 3;

            for (int col = c.first & 15; col <= (c.count & 15); col++)
            {
               RAMpal[CSET(c.first >> 4) + col] = _RGB(si);
               si += 3;
            }
            zc_sync_pal = true;
         }
      }
   }
}

int reverse_NESpal(RGB c)
{
   int dist = 12000;
   int index = 0;

   for (int i = 0; (i < 64) && (dist != 0); i++)
   {
      int r = (c.r - NESpal(i).r);
      int g = (c.g - NESpal(i).g);
      int b = (c.b - NESpal(i).b);
      int d = r * r + g * g + b * b;

      if (d < dist)
      {
         dist = d;
         index = i;
      }
   }
   return index;
}

/* end of pal.cpp */
