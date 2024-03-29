//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  tiles.cpp
//
//  Tile drawing routines for ZC.
//
//--------------------------------------------------------

#include "zdefs.h"
#include "tiles.h"
#include "pal.h"

extern uint8_t *tilebuf;
extern newcombo *combobuf;
extern uint16_t
animated_combo_table[MAXCOMBOS][2]; //[0]=position in act2, [1]=original tile
extern uint16_t
animated_combo_table4[MAXCOMBOS][2];   //[0]=combo, [1]=clock
extern uint16_t animated_combos;
extern bool
blank_tile_table[NEWMAXTILES];   //keeps track of blank tiles
extern bool blank_tile_quarters_table[NEWMAXTILES * 4]; //keeps track of blank tiles
extern fix  LinkModifiedX(void);
extern fix  LinkModifiedY(void);

uint8_t unpackbuf[256];

bool isblanktile(int i)
{
   uint8_t *tilestart = tilebuf + (i * 128);
   uint64_t *di = (uint64_t *)tilestart;
   for (int j = 0; j < 16; ++j, ++di)
   {
      if (*di != 0)
         return false;
   }
   return true;
}

void register_blank_tile_quarters(int tile)
{
   uint8_t *tilestart = tilebuf + (tile * 128);
   uint32_t *di = (uint32_t *)tilestart;
   blank_tile_quarters_table[(tile << 2)] = true;
   blank_tile_quarters_table[(tile << 2) + 1] = true;
   blank_tile_quarters_table[(tile << 2) + 2] = true;
   blank_tile_quarters_table[(tile << 2) + 3] = true;

   for (int i = 0; i < 4; ++i)
   {
      for (int j = 0; j < 32; ++j, ++di)
      {
         if (*di != 0)
            blank_tile_quarters_table[(tile << 2) + ((j > 15) ? 2 : 0) +
                                                  ((j & 1) ? 1 : 0)] = false;
      }
   }

}

void register_blank_tiles(void)
{
   for (int i = 0; i < NEWMAXTILES; ++i)
   {
      register_blank_tile_quarters(i);
      blank_tile_table[i] = isblanktile(i);
   }
}

//returns the number of tiles
uint16_t count_tiles(uint8_t *buf)
{
   uint16_t tiles_used;
   bool used;
   int x;
   for (tiles_used = (NEWMAXTILES); tiles_used > 0; --tiles_used)
   {
      used = false;
      for (x = 0; x < 128; ++x)
         used = used || (buf[(tiles_used - 1) * 128 + x] != 0);
      if (used)
         break;
   }
   return tiles_used;
}

//returns the number of combos
uint16_t count_combos(void)
{
   uint16_t combos_used;
   for (combos_used = MAXCOMBOS; combos_used > 0; --combos_used)
   {
      if (combobuf[combos_used - 1].tile != 0)
         break;
   }
   return combos_used;
}

void setup_combo_animations(void)
{
   memset(animated_combo_table, 0, MAXCOMBOS * 2 * 2);
   memset(animated_combo_table4, 0, MAXCOMBOS * 2 * 2);
   int y = 0;
   for (uint16_t x = 0; x < MAXCOMBOS; ++x)
   {
      animated_combo_table[x][0] = y;
      animated_combo_table[x][1] = combobuf[x].tile;
      if (combobuf[x].frames > 1)
      {
         animated_combo_table4[y][0] = x;
         animated_combo_table4[y][1] = 0;
         ++y;
      }
   }
   animated_combos = y;
}

void reset_combo_animation(int c)
{
   for (uint16_t x = 0; x < animated_combos; ++x)
   {
      int y = animated_combo_table4[x][0];   //combo number
      if (y == c)
      {
         combobuf[y].tile = animated_combo_table[y][1];  //reset tile
         animated_combo_table4[x][1] = 0; //reset clock
         return;
      }
   }
}

void reset_combo_animations(void)
{
   for (uint16_t x = 0; x < animated_combos; ++x)
      combobuf[animated_combo_table4[x][0]].tile =
         animated_combo_table[animated_combo_table4[x][0]][1];
}

extern void update_combo_cycling(void);

void animate_combos(void)
{
   update_combo_cycling();
   for (uint16_t x = 0; x < animated_combos; ++x)
   {
      int y = animated_combo_table4[x][0];   //combo number
      if (animated_combo_table4[x][1] >= combobuf[y].speed) //time to animate
      {
         if (combobuf[y].tile - combobuf[y].frames >=
               animated_combo_table[y][1] - 1)
         {
            combobuf[y].tile = animated_combo_table[y][1];  //reset tile
         }
         else
         {
            ++combobuf[y].tile;  //increment tile
         }
         animated_combo_table4[x][1] = 0; //reset clock
      }
      else
      {
         ++animated_combo_table4[x][1];   //increment clock
      }
   }
}

//clears the tile buffer
void clear_tiles(uint8_t *buf)
{
   uint32_t i = 0;
   for (; i < NEWTILE_SIZE / 4; ++i)
      ((uint32_t *)buf)[i] = 0;
}

// unpacks from tilebuf to unpackbuf
void unpack_tile(int tile, int flip, bool force)
{
   static uint8_t *si, *di, *oldtilebuf = tilebuf;
   static int i, j, oldtile = -5, oldflip = -5;
   if (tile == oldtile && (flip & 5) == (oldflip & 5) && oldtilebuf == tilebuf
         && !force)
      return;
   oldtile = tile;
   oldflip = flip;
   oldtilebuf = tilebuf;
   switch (flip & 5)
   {
      case 1:  //horizontal
         si = tilebuf + ((tile + 1) << 7);
         for (i = 15; i >= 0; --i)
         {
            di = unpackbuf + (i << 4) - 1;
            for (j = 7; j >= 0; --j)
            {
               (*(++di)) = (*(--si)) >> 4;
               (*(++di)) = (*si) & 15;
            }
         }
         break;
      case 4:  //rotated
         si = tilebuf + ((tile + 1) << 7);
         for (i = 15; i >= 0; --i)
         {
            di = unpackbuf + 271 - i; //256 + 15 - i
            for (j = 7; j >= 0; --j)
            {
               di -= 16;
               *di = (*(--si)) >> 4;
               di -= 16;
               *di = (*si) & 15;
            }
         }
         break;
      case 5:  //rotated and horizontal
         si = tilebuf + ((tile + 1) << 7);
         for (i = 15; i >= 0; --i)
         {
            di = unpackbuf + 256 + i;
            for (j = 7; j >= 0; --j)
            {
               di -= 16;
               *di = (*(--si)) >> 4;
               di -= 16;
               *di = (*si) & 15;
            }
         }
         break;
      default: //none or invalid
         si = tilebuf + ((tile + 1) << 7);
         di = unpackbuf + 256;
         for (i = 127; i >= 0; --i)
         {
            (*(--di)) = (*(--si)) >> 4;
            (*(--di)) = (*si) & 15;
         }
         break;
   }
}

// packs from src[256] to tilebuf
void pack_tile(uint8_t *src, int tile)
{
   uint8_t *di = tilebuf + (tile << 7);

   for (int si = 0; si < 256; si += 2)
   {
      *di = (src[si] & 15) + ((src[si + 1] & 15) << 4);
      ++di;
   }
}

// packs a whole set of tiles from old size to new size
void pack_tiles(uint8_t *buf)
{
   int di = 0;
   for (int si = 0; si < TILEBUF_SIZE; si += 2)
      buf[di++] = (buf[si] & 15) + ((buf[si + 1] & 15) << 4);

   for (; di < NEWTILE_SIZE; ++di)
      buf[di] = 0;
}

int rotate_table[8] =
{
   4, 6, 5, 7, 3, 1, 2, 0
};

int rotate_value(int flip)
{
   return rotate_table[flip & 7];
}

void puttiletranslucent8(BITMAP *dest, int tile, int x, int y, int cset,
                         int flip, int opacity)
{

   if (x < -7 || y < -7)
      return;
   if (y > dest->h)
      return;
   if (y == dest->h && x > dest->w)
      return;

   cset &= 15;
   cset <<= CSET_SHFT;
   unpack_tile(tile >> 2, 0, false);
   uint8_t *si = unpackbuf + ((tile & 2) << 6) + ((tile & 1) << 3);
   if (flip & 1)   //horizontal
      si += 7;

   if ((flip & 2) == 0) //not flipped vertically
   {
      if (y < 0)
         si += (0 - y) << 4;
      for (int dy = (y < 0 ? 0 - y : 0); (dy < 8) && (dy + y < dest->h); ++dy)
      {
         uint8_t *di = &(dest->line[y + dy][x < 0 ? 0 : x]);
         for (int i = 0; i < 8; ++i)
         {
            if (x + i < dest->w)
            {
               *(di) = trans_table.data[(*di)][((*si) + cset)];
               ++di;
            }
            flip & 1 ? --si : ++si;
         }
         if (flip & 1)
            si += 24;

         else
            si += 8;
      }
   }  //flipped vertically
   else
   {
      if (y + 7 >= dest->h)
         si += (8 + y - dest->h) << 4;
      for (int dy = (y + 7 >= dest->h ? dest->h - y - 1 : 7); (dy >= 0)
            && (dy + y >= 0); --dy)
      {
         uint8_t *di = &(dest->line[y + dy][x < 0 ? 0 : x]);
         for (int i = 0; i < 8; ++i)
         {
            if (x + i < dest->w)
            {
               *(di) = trans_table.data[(*di)][((*si) + cset)];
               ++di;
            }
            flip & 1 ? --si : ++si;
         }
         if (flip & 1)
            si += 24;

         else
            si += 8;
      }
   }
}

void overtiletranslucent8(BITMAP *dest, int tile, int x, int y, int cset,
                          int flip, int opacity)
{
   if (x < -7 || y < -7)
      return;
   if (y > dest->h)
      return;
   if (y == dest->h && x > dest->w)
      return;

   if (blank_tile_quarters_table[tile])
      return;

   cset &= 15;
   cset <<= CSET_SHFT;
   unpack_tile(tile >> 2, 0, false);
   uint8_t *si = unpackbuf + ((tile & 2) << 6) + ((tile & 1) << 3);
   if (flip & 1)
      si += 7;

   if ((flip & 2) == 0) //not flipped vertically
   {
      if (y < 0)
         si += (0 - y) << 4;
      for (int dy = (y < 0 ? 0 - y : 0); (dy < 8) && (dy + y < dest->h); ++dy)
      {
         uint8_t *di = &(dest->line[y + dy][x < 0 ? 0 : x]);
         for (int i = 0; i < 8; ++i)
         {
            if (x + i < dest->w)
            {
               if (*si)
                  *(di) = trans_table.data[(*di)][((*si) + cset)];
               ++di;
            }
            flip & 1 ? --si : ++si;
         }
         if (flip & 1)
            si += 24;

         else
            si += 8;
      }
   }  //flipped vertically
   else
   {
      if (y + 7 >= dest->h)
         si += (8 + y - dest->h) << 4;
      for (int dy = (y + 7 >= dest->h ? dest->h - y - 1 : 7); (dy >= 0)
            && (dy + y >= 0); --dy)
      {
         uint8_t *di = &(dest->line[y + dy][x < 0 ? 0 : x]);
         for (int i = 0; i < 8; ++i)
         {
            if (x + i < dest->w)
            {
               if (*si)
                  *(di) = trans_table.data[(*di)][((*si) + cset)];
               ++di;
            }
            flip & 1 ? --si : ++si;
         }
         if (flip & 1)
            si += 24;

         else
            si += 8;
      }
   }
}

void puttiletranslucent16(BITMAP *dest, int tile, int x, int y, int cset,
                          int flip, int opacity)
{
   if (x < -15 || y < -15)
      return;
   if (y > dest->h)
      return;
   if (y == dest->h && x > dest->w)
      return;
   if (tile < 0 || tile >= NEWMAXTILES)
   {
      rectfill(dest, x, y, x + 15, y + 15, 0);
      return;
   }

   cset &= 15;
   cset <<= CSET_SHFT;
   unpack_tile(tile, 0, false);
   uint8_t *si = unpackbuf;
   uint8_t *di;
   if (flip & 1)
      si += 15;

   if ((flip & 2) == 0)
   {
      if (y < 0)
         si += (0 - y) << 4;

      for (int dy = (y < 0 ? 0 - y : 0); (dy < 16) && (dy + y < dest->h); ++dy)
      {
         di = &(dest->line[y + dy][x < 0 ? 0 : x]);
         if (x + 15 < dest->w)
         {
            if (x < 0)
               flip & 1 ? si -= 0 - x : si += 0 - x;
            for (int dx = (x < 0 ? 0 - x : 0); dx < 16; ++dx)
            {
               *di = trans_table.data[(*di)][((*si) + cset)];
               ++di;
               flip & 1 ? --si : ++si;
            }
         }
         else
         {
            for (int i = 0; i < 16; ++i)
            {
               if (x + i < dest->w)
               {
                  *di = trans_table.data[(*di)][((*si) + cset)];
                  ++di;
               }
               flip & 1 ? --si : ++si;
            }
         }
         if (flip & 1)
            si += 32;
      }
   }
   else
   {
      if (y + 15 >= dest->h)
         si += (16 + y - dest->h) << 4;

      for (int dy = (y + 15 >= dest->h ? dest->h - y - 1 : 15); (dy >= 0)
            && (dy + y >= 0); --dy)
      {
         di = &(dest->line[y + dy][x < 0 ? 0 : x]);
         if (x + 15 < dest->w)
         {
            if (x < 0)
               flip & 1 ? si -= 0 - x : si += 0 - x;
            for (int dx = (x < 0 ? 0 - x : 0); dx < 16; ++dx)
            {
               *di = trans_table.data[(*di)][((*si) + cset)];
               ++di;
               flip & 1 ? --si : ++si;
            }
         }
         else
         {
            for (int i = 0; i < 16; ++i)
            {
               if (x + i < dest->w)
               {
                  *di = trans_table.data[(*di)][((*si) + cset)];
                  ++di;
               }
               flip & 1 ? --si : ++si;
            }
         }
         if (flip & 1)
            si += 32;
      }
   }
}

void overtiletranslucent16(BITMAP *dest, int tile, int x, int y, int cset,
                           int flip, int opacity)
{
   if (x < -15 || y < -15)
      return;
   if (y > dest->h)
      return;
   if (y == dest->h && x > dest->w)
      return;
   if (tile < 0 || tile >= NEWMAXTILES)
   {
      rectfill(dest, x, y, x + 15, y + 15, 0);
      return;
   }

   if (blank_tile_table[tile])
      return;
   cset &= 15;
   cset <<= CSET_SHFT;
   unpack_tile(tile, 0, false);
   uint8_t *si = unpackbuf;
   uint8_t *di;
   if (flip & 1)
      si += 15;

   if ((flip & 2) == 0)
   {
      if (y < 0)
         si += (0 - y) << 4;

      for (int dy = (y < 0 ? 0 - y : 0); (dy < 16) && (dy + y < dest->h); ++dy)
      {
         di = &(dest->line[y + dy][x < 0 ? 0 : x]);
         if (x + 15 < dest->w)
         {
            if (x < 0)
               flip & 1 ? si -= 0 - x : si += 0 - x;
            for (int dx = (x < 0 ? 0 - x : 0); dx < 16; ++dx)
            {
               if (*si)
                  *di = trans_table.data[(*di)][((*si) + cset)];
               ++di;
               flip & 1 ? --si : ++si;
            }
         }
         else
         {
            for (int i = 0; i < 16; ++i)
            {
               if (x + i < dest->w)
               {
                  if (*si)
                     *di = trans_table.data[(*di)][((*si) + cset)];
                  ++di;
               }
               flip & 1 ? --si : ++si;
            }
         }
         if (flip & 1)
            si += 32;
      }
   }
   else
   {
      if (y + 15 >= dest->h)
         si += (16 + y - dest->h) << 4;

      for (int dy = (y + 15 >= dest->h ? dest->h - y - 1 : 15); (dy >= 0)
            && (dy + y >= 0); --dy)
      {
         di = &(dest->line[y + dy][x < 0 ? 0 : x]);
         if (x + 15 < dest->w)
         {
            if (x < 0)
               flip & 1 ? si -= 0 - x : si += 0 - x;
            for (int dx = (x < 0 ? 0 - x : 0); dx < 16; ++dx)
            {
               if (*si)
                  *di = trans_table.data[(*di)][((*si) + cset)];
               ++di;
               flip & 1 ? --si : ++si;
            }
         }
         else
         {
            for (int i = 0; i < 16; ++i)
            {
               if (x + i < dest->w)
               {
                  if (*si)
                     *di = trans_table.data[(*di)][((*si) + cset)];
                  ++di;
               }
               flip & 1 ? --si : ++si;
            }
         }
         if (flip & 1)
            si += 32;
      }
   }
}

void overtilecloaked16(BITMAP *dest, int tile, int x, int y, int flip)
{
   if (x < -15 || y < -15)
      return;
   if (y > dest->h)
      return;
   if (y == dest->h && x > dest->w)
      return;
   if (tile < 0 || tile >= NEWMAXTILES)
   {
      rectfill(dest, x, y, x + 15, y + 15, 0);
      return;
   }

   unpack_tile(tile, 0, false);
   uint8_t *si = unpackbuf;
   uint8_t *di;
   if (flip & 1)
      si += 15;

   if ((flip & 2) == 0)
   {
      if (y < 0)
         si += (0 - y) << 4;

      for (int dy = (y < 0 ? 0 - y : 0); (dy < 16) && (dy + y < dest->h); ++dy)
      {
         di = &(dest->line[y + dy][x < 0 ? 0 : x]);
         if (x + 15 < dest->w)
         {
            if (x < 0)
               flip & 1 ? si -= 0 - x : si += 0 - x;
            for (int dx = (x < 0 ? 0 - x : 0); dx < 16; ++dx)
            {
               if (*si)
                  *di = dest->line[((y + dy) ^ 1)][((x + dx) ^ 1)];
               ++di;
               flip & 1 ? --si : ++si;
            }
         }
         else
         {
            for (int i = 0; i < 16; ++i)
            {
               if (x + i < dest->w)
               {
                  if (*si)
                     *di = dest->line[((y + dy) ^ 1)][(x ^ 1)];
                  ++di;
               }
               flip & 1 ? --si : ++si;
            }
         }
         if (flip & 1)
            si += 32;
      }
   }
   else
   {
      if (y + 15 >= dest->h)
         si += (16 + y - dest->h) << 4;

      for (int dy = (y + 15 >= dest->h ? dest->h - y - 1 : 15); (dy >= 0)
            && (dy + y >= 0); --dy)
      {
         di = &(dest->line[y + dy][x < 0 ? 0 : x]);
         if (x + 15 < dest->w)
         {
            if (x < 0)
               flip & 1 ? si -= 0 - x : si += 0 - x;
            for (int dx = (x < 0 ? 0 - x : 0); dx < 16; ++dx)
            {
               if (*si)
                  *di = dest->line[((y + dy) ^ 1)][((x + dx) ^ 1)];
               ++di;
               flip & 1 ? --si : ++si;
            }
         }
         else
         {
            for (int i = 0; i < 16; ++i)
            {
               if (x + i < dest->w)
               {
                  if (*si)
                     *di = dest->line[((y + dy) ^ 1)][(x ^ 1)];
                  ++di;
               }
               flip & 1 ? --si : ++si;
            }
         }
         if (flip & 1)
            si += 32;
      }
   }
}

void putblocktranslucent8(BITMAP *dest, int tile, int x, int y, int csets[],
                          int flip, int mask, int opacity)
{
   int t[4];
   for (int i = 0; i < 4; ++i)
      t[i] = tile + i;

   switch (mask)
   {
      case 1:
         puttiletranslucent8(dest, tile, x, y, csets[0], flip, opacity);
         break;

      case 3:
         if (flip & 2)
            swap(t[0], t[1]);
         puttiletranslucent8(dest, t[0], x, y,  csets[0], flip, opacity);
         puttiletranslucent8(dest, t[1], x, y + 8, csets[1], flip, opacity);
         break;

      case 5:
         if (flip & 1)
            swap(t[0], t[1]);
         puttiletranslucent8(dest, t[0], x,  y, csets[0], flip, opacity);
         puttiletranslucent8(dest, t[1], x + 8, y, csets[1], flip, opacity);
         break;

      case 15:
         if (flip & 1)
         {
            swap(t[0], t[1]);
            swap(t[2], t[3]);
         }
         if (flip & 2)
         {
            swap(t[0], t[2]);
            swap(t[1], t[3]);
         }
         puttiletranslucent8(dest, t[0], x,  y,  csets[0], flip, opacity);
         puttiletranslucent8(dest, t[1], x + 8, y,  csets[1], flip, opacity);
         puttiletranslucent8(dest, t[2], x,  y + 8, csets[2], flip, opacity);
         puttiletranslucent8(dest, t[3], x + 8, y + 8, csets[3], flip, opacity);
         break;
   }
}

void overblocktranslucent8(BITMAP *dest, int tile, int x, int y, int csets[],
                           int flip, int mask, int opacity)
{
   int t[4];
   for (int i = 0; i < 4; ++i)
      t[i] = tile + i;

   switch (mask)
   {
      case 1:
         overtiletranslucent8(dest, tile, x, y, csets[0], flip, opacity);
         break;

      case 3:
         if (flip & 2)
         {
            swap(t[0], t[1]);
            swap(csets[0], csets[1]);
         }
         overtiletranslucent8(dest, t[0], x, y,  csets[0], flip, opacity);
         overtiletranslucent8(dest, t[1], x, y + 8, csets[1], flip, opacity);
         break;

      case 5:
         if (flip & 1)
            swap(t[0], t[1]);
         overtiletranslucent8(dest, t[0], x,  y, csets[0], flip, opacity);
         overtiletranslucent8(dest, t[1], x + 8, y, csets[1], flip, opacity);
         break;

      case 15:
         if (flip & 1)
         {
            swap(t[0], t[1]);
            swap(t[2], t[3]);
         }
         if (flip & 2)
         {
            swap(t[0], t[2]);
            swap(t[1], t[3]);
         }
         overtiletranslucent8(dest, t[0], x,  y,  csets[0], flip, opacity);
         overtiletranslucent8(dest, t[1], x + 8, y,  csets[1], flip, opacity);
         overtiletranslucent8(dest, t[2], x,  y + 8, csets[2], flip, opacity);
         overtiletranslucent8(dest, t[3], x + 8, y + 8, csets[3], flip, opacity);
         break;
   }
}

void putcombotranslucent(BITMAP *dest, int x, int y, int cmbdat, int cset,
                         int opacity)
{
   newcombo c = combobuf[cmbdat];   //strip the flags
   int drawtile = c.tile;
   int tframes = max(1, combobuf[cmbdat].frames);
   double ddir = atan2(double(y - LinkModifiedY()), double(LinkModifiedX() - x));
   switch (combobuf[cmbdat].type)
   {
      case cEYEBALL_A:
         if ((ddir <= (((-5) * PI) / 8)) && (ddir > (((-7) * PI) / 8)))
         {
            drawtile += tframes * 5;   //dl
         }
         else if ((ddir <= (((-3) * PI) / 8)) && (ddir > (((-5) * PI) / 8)))
         {
            drawtile += tframes * 4;   //d
         }
         else if ((ddir <= (((-1) * PI) / 8)) && (ddir > (((-3) * PI) / 8)))
         {
            drawtile += tframes * 3;   //dr
         }
         else if ((ddir <= (((1) * PI) / 8)) && (ddir > (((-1) * PI) / 8)))
         {
            drawtile += tframes * 2;   //r
         }
         else if ((ddir <= (((3) * PI) / 8)) && (ddir > (((1) * PI) / 8)))
         {
            drawtile += tframes * 1;   //ur
         }
         else if ((ddir <= (((5) * PI) / 8)) && (ddir > (((3) * PI) / 8)))
         {
            drawtile += tframes * 0;   //u
         }
         else if ((ddir <= (((7) * PI) / 8)) && (ddir > (((5) * PI) / 8)))
         {
            drawtile += tframes * 7;   //ul
         }
         else
         {
            drawtile += tframes * 6;   //l
         }
         break;
      case cEYEBALL_B:
         if ((ddir <= (((-6) * PI) / 8)) && (ddir > (((-8) * PI) / 8)))
         {
            drawtile += tframes * 5;   //dl
         }
         else if ((ddir <= (((-4) * PI) / 8)) && (ddir > (((-6) * PI) / 8)))
         {
            drawtile += tframes * 4;   //d
         }
         else if ((ddir <= (((-2) * PI) / 8)) && (ddir > (((-4) * PI) / 8)))
         {
            drawtile += tframes * 3;   //dr
         }
         else if ((ddir <= (((0) * PI) / 8)) && (ddir > (((-2) * PI) / 8)))
         {
            drawtile += tframes * 2;   //r
         }
         else if ((ddir <= (((2) * PI) / 8)) && (ddir > (((0) * PI) / 8)))
         {
            drawtile += tframes * 1;   //ur
         }
         else if ((ddir <= (((4) * PI) / 8)) && (ddir > (((2) * PI) / 8)))
         {
            drawtile += tframes * 0;   //u
         }
         else if ((ddir <= (((6) * PI) / 8)) && (ddir > (((4) * PI) / 8)))
         {
            drawtile += tframes * 7;   //ul
         }
         else
         {
            drawtile += tframes * 6;   //l
         }
         break;
   }
   if (!(c.csets & 0xF0) || !(c.csets & 0x0F))
      puttiletranslucent16(dest, drawtile, x, y, cset, c.flip, opacity);

   else
   {
      int csets[4];
      int cofs = c.csets & 15;
      if (cofs & 8)
         cofs |= ~int(0xF);

      for (int i = 0; i < 4; ++i)
         csets[i] = c.csets & (16 << i) ? cset + cofs : cset;

      putblocktranslucent8(dest, drawtile << 2, x, y, csets, c.flip, 15, opacity);
   }
}

void overcombotranslucent(BITMAP *dest, int x, int y, int cmbdat, int cset,
                          int opacity)
{
   newcombo c = combobuf[cmbdat];   //strip the flags
   int drawtile = c.tile;
   int tframes = max(1, combobuf[cmbdat].frames);
   double ddir = atan2(double(y - LinkModifiedY()), double(LinkModifiedX() - x));
   switch (combobuf[cmbdat].type)
   {
      case cEYEBALL_A:
         if ((ddir <= (((-5) * PI) / 8)) && (ddir > (((-7) * PI) / 8)))
         {
            drawtile += tframes * 5;   //dl
         }
         else if ((ddir <= (((-3) * PI) / 8)) && (ddir > (((-5) * PI) / 8)))
         {
            drawtile += tframes * 4;   //d
         }
         else if ((ddir <= (((-1) * PI) / 8)) && (ddir > (((-3) * PI) / 8)))
         {
            drawtile += tframes * 3;   //dr
         }
         else if ((ddir <= (((1) * PI) / 8)) && (ddir > (((-1) * PI) / 8)))
         {
            drawtile += tframes * 2;   //r
         }
         else if ((ddir <= (((3) * PI) / 8)) && (ddir > (((1) * PI) / 8)))
         {
            drawtile += tframes * 1;   //ur
         }
         else if ((ddir <= (((5) * PI) / 8)) && (ddir > (((3) * PI) / 8)))
         {
            drawtile += tframes * 0;   //u
         }
         else if ((ddir <= (((7) * PI) / 8)) && (ddir > (((5) * PI) / 8)))
         {
            drawtile += tframes * 7;   //ul
         }
         else
         {
            drawtile += tframes * 6;   //l
         }
         break;
      case cEYEBALL_B:
         if ((ddir <= (((-6) * PI) / 8)) && (ddir > (((-8) * PI) / 8)))
         {
            drawtile += tframes * 5;   //dl
         }
         else if ((ddir <= (((-4) * PI) / 8)) && (ddir > (((-6) * PI) / 8)))
         {
            drawtile += tframes * 4;   //d
         }
         else if ((ddir <= (((-2) * PI) / 8)) && (ddir > (((-4) * PI) / 8)))
         {
            drawtile += tframes * 3;   //dr
         }
         else if ((ddir <= (((0) * PI) / 8)) && (ddir > (((-2) * PI) / 8)))
         {
            drawtile += tframes * 2;   //r
         }
         else if ((ddir <= (((2) * PI) / 8)) && (ddir > (((0) * PI) / 8)))
         {
            drawtile += tframes * 1;   //ur
         }
         else if ((ddir <= (((4) * PI) / 8)) && (ddir > (((2) * PI) / 8)))
         {
            drawtile += tframes * 0;   //u
         }
         else if ((ddir <= (((6) * PI) / 8)) && (ddir > (((4) * PI) / 8)))
         {
            drawtile += tframes * 7;   //ul
         }
         else
         {
            drawtile += tframes * 6;   //l
         }
         break;
   }
   if (!(c.csets & 0xF0) || !(c.csets & 0x0F))
      overtiletranslucent16(dest, drawtile, x, y, cset, c.flip, opacity);

   else
   {
      int csets[4];
      int cofs = c.csets & 15;
      if (cofs & 8)
         cofs |= ~int(0xF);

      for (int i = 0; i < 4; ++i)
         csets[i] = c.csets & (16 << i) ? cset + cofs : cset;

      overblocktranslucent8(dest, drawtile << 2, x, y, csets, c.flip, 15, opacity);
   }
}

void puttile8(BITMAP *dest, int tile, int x, int y, int cset, int flip)
{
   if (x < 0 || y < 0)
      return;
   if (y > dest->h - 8)
      return;
   if (y == dest->h - 8 && x > dest->w - 8)
      return;

   cset &= 15;
   cset <<= CSET_SHFT;
   uint32_t lcset = (cset << 24) + (cset << 16) + (cset << 8) + cset;
   unpack_tile(tile >> 2, 0, false);

   switch (flip & 3)
   {
      case 1:  // 1 byte at a time
      {
         uint8_t *si = unpackbuf + ((tile & 2) << 6) + ((tile & 1) << 3);
         for (int dy = 0; dy < 8; ++dy)
         {
            uint8_t *di = &(dest->line[y + dy][x + 7]);
            for (int i = 0; i < 8; ++i)
               *(di--) = *(si++) + cset;
            si += 8;
         }
      }
      break;

      case 2:  // 4 bytes at a time
      {
         uint32_t *si = ((uint32_t *)unpackbuf) + ((tile & 2) << 4) + ((tile & 1) << 1);

         for (int dy = 7; dy >= 0; --dy)
         {
            uint32_t *di = &((uint32_t *)dest->line[y + dy])[x >> 2];
            *(di++) = *(si++) + lcset;
            *(di++) = *(si++) + lcset;
            si += 2;
         }
      }
      break;

      case 3:  // 1 byte at a time
      {
         uint8_t *si = unpackbuf + ((tile & 2) << 6) + ((tile & 1) << 3);
         for (int dy = 7; dy >= 0; --dy)
         {
            uint8_t *di = &(dest->line[y + dy][x + 7]);
            for (int i = 0; i < 8; ++i)
               *(di--) = *(si++) + cset;
            si += 8;
         }
      }
      break;

      default: // 4 bytes at a time
      {
         uint32_t *si = ((uint32_t *)unpackbuf) + ((tile & 2) << 4) + ((tile & 1) << 1);
         for (int dy = 0; dy < 8; ++dy)
         {
            uint32_t *di = &((uint32_t *)dest->line[y + dy])[x >> 2];
            *(di++) = *(si++) + lcset;
            *(di++) = *(si++) + lcset;
            si += 2;
         }
      }
      break;
   }
}

void overtile8(BITMAP *dest, int tile, int x, int y, int cset, int flip)
{
   if (x < -7 || y < -7)
      return;
   if (y > dest->h)
      return;
   if (y == dest->h && x > dest->w)
      return;

   if (blank_tile_quarters_table[tile])
      return;
   cset &= 15;
   cset <<= CSET_SHFT;
   unpack_tile(tile >> 2, 0, false);
   uint8_t *si = unpackbuf + ((tile & 2) << 6) + ((tile & 1) << 3);
   if (flip & 1)
      si += 7;

   if ((flip & 2) == 0) //not flipped vertically
   {
      if (y < 0)
         si += (0 - y) << 4;
      for (int dy = (y < 0 ? 0 - y : 0); (dy < 8) && (dy + y < dest->h); ++dy)
      {
         uint8_t *di = &(dest->line[y + dy][x < 0 ? 0 : x]);
         for (int i = 0; i < 8; ++i)
         {
            if (x + i < dest->w)
            {
               if (*si)
                  *(di) = (*si) + cset;
               ++di;
            }
            flip & 1 ? --si : ++si;
         }
         if (flip & 1)
            si += 24;

         else
            si += 8;

      }
   }  //flipped vertically
   else
   {
      if (y + 7 >= dest->h)
         si += (8 + y - dest->h) << 4;
      for (int dy = (y + 7 >= dest->h ? dest->h - y - 1 : 7); (dy >= 0)
            && (dy + y >= 0); --dy)
      {
         uint8_t *di = &(dest->line[y + dy][x < 0 ? 0 : x]);
         for (int i = 0; i < 8; ++i)
         {
            if (x + i < dest->w)
            {
               if (*si)
                  *(di) = (*si) + cset;
               ++di;
            }
            flip & 1 ? --si : ++si;
         }
         if (flip & 1)
            si += 24;

         else
            si += 8;
      }
   }
}

void puttile16(BITMAP *dest, int tile, int x, int y, int cset, int flip)
{
   if (x < 0 || y < 0)
      return;
   if (y > dest->h - 16)
      return;
   if ((y == dest->h - 16) && (x > dest->w - 16))
      return;
   if (tile < 0 || tile >= NEWMAXTILES)
   {
      rectfill(dest, x, y, x + 15, y + 15, 0);
      return;
   }
   cset &= 15;
   cset <<= CSET_SHFT;

   unpack_tile(tile, flip & 5, false);

   switch (flip & 2)
   {
      case 2:   //vertical
      {
         uint64_t llcset = (((uint64_t)cset) << 56) + (((uint64_t)cset) << 48) +
                           (((uint64_t)cset) << 40) + (((uint64_t)cset) << 32) +
                           (((uint64_t)cset) << 24) + (cset << 16) + (cset << 8) +
                           cset;
         uint64_t *si = (uint64_t *)unpackbuf;
         for (int dy = 15; dy >= 0; --dy)
         {
            // 4 bytes at a time
            uint64_t *di = (uint64_t *)(dest->line[y + dy] + x);
            for (int i = 0; i < 16; i += 8)
               * (di++) = *(si++) + llcset;
         }
      }
      break;
      default:   //none or invalid
      {
         uint64_t llcset = (((uint64_t)cset) << 56) + (((uint64_t)cset) << 48) +
                           (((uint64_t)cset) << 40) + (((uint64_t)cset) << 32) +
                           (((uint64_t)cset) << 24) + (cset << 16) + (cset << 8) +
                           cset;
         uint64_t *si = (uint64_t *)unpackbuf;
         for (int dy = 0; dy < 16; ++dy)
         {
            // 4 bytes at a time
            uint64_t *di = (uint64_t *)(dest->line[y + dy] + x);
            for (int i = 0; i < 16; i += 8)
               * (di++) = *(si++) + llcset;
         }
      }
      break;
   }
}

void overtile16(BITMAP *dest, int tile, int x, int y, int cset, int flip)
{
   if (x < -15 || y < -15)
      return;
   if (y > dest->h)
      return;
   if (y == dest->h && x > dest->w)
      return;
   if (tile < 0 || tile >= NEWMAXTILES)
   {
      rectfill(dest, x, y, x + 15, y + 15, 0);
      return;
   }

   if (blank_tile_table[tile])
      return;
   cset &= 15;
   cset <<= CSET_SHFT;
   unpack_tile(tile, flip & 5, false);
   uint8_t *si = unpackbuf;
   uint8_t *di;

   if ((flip & 2) == 0)
   {
      if (y < 0)
         si += (0 - y) << 4;

      for (int dy = (y < 0 ? 0 - y : 0); (dy < 16) && (dy + y < dest->h); ++dy)
      {
         di = &(dest->line[y + dy][x < 0 ? 0 : x]);
         if (x + 15 < dest->w)
         {
            if (x < 0)
               si += 0 - x;
            for (int dx = (x < 0 ? 0 - x : 0); dx < 16; ++dx)
            {
               if (*si)
                  *di = *si + cset;
               ++di;
               ++si;
            }
         }
         else
         {
            for (int i = 0; i < 16; ++i)
            {
               if (x + i < dest->w)
               {
                  if (*si)
                     *di = *si + cset;
                  ++di;
               }
               ++si;
            }
         }
      }
   }
   else
   {
      if (y + 15 >= dest->h)
         si += (16 + y - dest->h) << 4;

      for (int dy = (y + 15 >= dest->h ? dest->h - y - 1 : 15); (dy >= 0)
            && (dy + y >= 0); --dy)
      {
         di = &(dest->line[y + dy][x < 0 ? 0 : x]);
         if (x + 15 < dest->w)
         {
            if (x < 0)
               si += 0 - x;
            for (int dx = (x < 0 ? 0 - x : 0); dx < 16; ++dx)
            {
               if (*si)
                  *di = *si + cset;
               ++di;
               ++si;
            }
         }
         else
         {
            for (int i = 0; i < 16; ++i)
            {
               if (x + i < dest->w)
               {
                  if (*si)
                     *di = *si + cset;
                  ++di;
               }
               ++si;
            }
         }
      }
   }
}

void putblock8(BITMAP *dest, int tile, int x, int y, int csets[], int flip,
               int mask)
{
   int t[4];
   for (int i = 0; i < 4; ++i)
      t[i] = tile + i;

   switch (mask)
   {
      case 1: //top-left quarter
         puttile8(dest, tile, x, y, csets[0], flip);
         break;

      case 3: //vertical
         if (flip & 2)
            swap(t[0], t[1]);
         puttile8(dest, t[0], x, y,  csets[0], flip);
         puttile8(dest, t[1], x, y + 8, csets[1], flip);
         break;

      case 5: //horizontal
         if (flip & 1)
            swap(t[0], t[1]);
         puttile8(dest, t[0], x,  y, csets[0], flip);
         puttile8(dest, t[1], x + 8, y, csets[1], flip);
         break;

      case 15: //all 4 quarters
         if (flip & 1)
         {
            swap(t[0], t[1]);
            swap(t[2], t[3]);
         }
         if (flip & 2)
         {

            swap(t[0], t[2]);
            swap(t[1], t[3]);
         }
         puttile8(dest, t[0], x,  y,  csets[0], flip);
         puttile8(dest, t[1], x + 8, y,  csets[1], flip);
         puttile8(dest, t[2], x,  y + 8, csets[2], flip);
         puttile8(dest, t[3], x + 8, y + 8, csets[3], flip);
         break;
   }
}

void overblock8(BITMAP *dest, int tile, int x, int y, int csets[], int flip,
                int mask)
{
   int t[4];
   for (int i = 0; i < 4; ++i)
      t[i] = tile + i;

   switch (mask)
   {
      case 1:
         overtile8(dest, tile, x, y, csets[0], flip);
         break;

      case 3:
         if (flip & 2)
            swap(t[0], t[1]);
         overtile8(dest, t[0], x, y,  csets[0], flip);
         overtile8(dest, t[1], x, y + 8, csets[1], flip);
         break;

      case 5:
         if (flip & 1)
            swap(t[0], t[1]);
         overtile8(dest, t[0], x,  y, csets[0], flip);
         overtile8(dest, t[1], x + 8, y, csets[1], flip);
         break;

      case 15:
         if (flip & 1)
         {
            swap(t[0], t[1]);
            swap(t[2], t[3]);
         }
         if (flip & 2)
         {
            swap(t[0], t[2]);
            swap(t[1], t[3]);
         }
         overtile8(dest, t[0], x,  y,  csets[0], flip);
         overtile8(dest, t[1], x + 8, y,  csets[1], flip);
         overtile8(dest, t[2], x,  y + 8, csets[2], flip);
         overtile8(dest, t[3], x + 8, y + 8, csets[3], flip);
         break;
   }
}

//  cmbdat: fffffsss cccccccc
//          (f:flags, s:cset, c:combo)

void putcombo(BITMAP *dest, int x, int y, int cmbdat, int cset)
{
   newcombo c = combobuf[cmbdat];   //strip the flags
   int drawtile = c.tile;
   int tframes = max(1, combobuf[cmbdat].frames);
   double ddir = atan2(double(y - LinkModifiedY()), double(LinkModifiedX() - x));
   switch (combobuf[cmbdat].type)
   {
      case cEYEBALL_A:
         if ((ddir <= (((-5) * PI) / 8)) && (ddir > (((-7) * PI) / 8)))
         {
            drawtile += tframes * 5;   //dl
         }
         else if ((ddir <= (((-3) * PI) / 8)) && (ddir > (((-5) * PI) / 8)))
         {
            drawtile += tframes * 4;   //d
         }
         else if ((ddir <= (((-1) * PI) / 8)) && (ddir > (((-3) * PI) / 8)))
         {
            drawtile += tframes * 3;   //dr
         }
         else if ((ddir <= (((1) * PI) / 8)) && (ddir > (((-1) * PI) / 8)))
         {
            drawtile += tframes * 2;   //r
         }
         else if ((ddir <= (((3) * PI) / 8)) && (ddir > (((1) * PI) / 8)))
         {
            drawtile += tframes * 1;   //ur
         }
         else if ((ddir <= (((5) * PI) / 8)) && (ddir > (((3) * PI) / 8)))
         {
            drawtile += tframes * 0;   //u
         }
         else if ((ddir <= (((7) * PI) / 8)) && (ddir > (((5) * PI) / 8)))
         {
            drawtile += tframes * 7;   //ul
         }
         else
         {
            drawtile += tframes * 6;   //l
         }
         break;
      case cEYEBALL_B:
         if ((ddir <= (((-6) * PI) / 8)) && (ddir > (((-8) * PI) / 8)))
         {
            drawtile += tframes * 5;   //dl
         }
         else if ((ddir <= (((-4) * PI) / 8)) && (ddir > (((-6) * PI) / 8)))
         {
            drawtile += tframes * 4;   //d
         }
         else if ((ddir <= (((-2) * PI) / 8)) && (ddir > (((-4) * PI) / 8)))
         {
            drawtile += tframes * 3;   //dr
         }
         else if ((ddir <= (((0) * PI) / 8)) && (ddir > (((-2) * PI) / 8)))
         {
            drawtile += tframes * 2;   //r
         }
         else if ((ddir <= (((2) * PI) / 8)) && (ddir > (((0) * PI) / 8)))
         {
            drawtile += tframes * 1;   //ur
         }
         else if ((ddir <= (((4) * PI) / 8)) && (ddir > (((2) * PI) / 8)))
         {
            drawtile += tframes * 0;   //u
         }
         else if ((ddir <= (((6) * PI) / 8)) && (ddir > (((4) * PI) / 8)))
         {
            drawtile += tframes * 7;   //ul
         }
         else
         {
            drawtile += tframes * 6;   //l
         }
         break;
   }
   if (!(c.csets & 0xF0) || !(c.csets & 0x0F))
      puttile16(dest, drawtile, x, y, cset, c.flip);

   else
   {
      int csets[4];
      int cofs = c.csets & 15;
      if (cofs & 8)
         cofs |= ~int(0xF);

      for (int i = 0; i < 4; ++i)
         csets[i] = c.csets & (16 << i) ? cset + cofs : cset;

      putblock8(dest, drawtile << 2, x, y, csets, c.flip, 15);
   }
}

void overcombo(BITMAP *dest, int x, int y, int cmbdat, int cset)
{
   newcombo c = combobuf[cmbdat];   //strip the flags
   int drawtile = c.tile;
   int tframes = max(1, combobuf[cmbdat].frames);
   double ddir = atan2(double(y - LinkModifiedY()), double(LinkModifiedX() - x));
   switch (combobuf[cmbdat].type)
   {
      case cEYEBALL_A:
         if ((ddir <= (((-5) * PI) / 8)) && (ddir > (((-7) * PI) / 8)))
         {
            drawtile += tframes * 5;   //dl
         }
         else if ((ddir <= (((-3) * PI) / 8)) && (ddir > (((-5) * PI) / 8)))
         {
            drawtile += tframes * 4;   //d
         }
         else if ((ddir <= (((-1) * PI) / 8)) && (ddir > (((-3) * PI) / 8)))
         {
            drawtile += tframes * 3;   //dr
         }
         else if ((ddir <= (((1) * PI) / 8)) && (ddir > (((-1) * PI) / 8)))
         {
            drawtile += tframes * 2;   //r
         }
         else if ((ddir <= (((3) * PI) / 8)) && (ddir > (((1) * PI) / 8)))
         {
            drawtile += tframes * 1;   //ur
         }
         else if ((ddir <= (((5) * PI) / 8)) && (ddir > (((3) * PI) / 8)))
         {
            drawtile += tframes * 0;   //u
         }
         else if ((ddir <= (((7) * PI) / 8)) && (ddir > (((5) * PI) / 8)))
         {
            drawtile += tframes * 7;   //ul
         }
         else
         {
            drawtile += tframes * 6;   //l
         }
         break;
      case cEYEBALL_B:
         if ((ddir <= (((-6) * PI) / 8)) && (ddir > (((-8) * PI) / 8)))
         {
            drawtile += tframes * 5;   //dl
         }
         else if ((ddir <= (((-4) * PI) / 8)) && (ddir > (((-6) * PI) / 8)))
         {
            drawtile += tframes * 4;   //d
         }
         else if ((ddir <= (((-2) * PI) / 8)) && (ddir > (((-4) * PI) / 8)))
         {
            drawtile += tframes * 3;   //dr
         }
         else if ((ddir <= (((0) * PI) / 8)) && (ddir > (((-2) * PI) / 8)))
         {
            drawtile += tframes * 2;   //r
         }
         else if ((ddir <= (((2) * PI) / 8)) && (ddir > (((0) * PI) / 8)))
         {
            drawtile += tframes * 1;                            //ur
         }
         else if ((ddir <= (((4) * PI) / 8)) && (ddir > (((2) * PI) / 8)))
         {
            drawtile += tframes * 0;   //u
         }
         else if ((ddir <= (((6) * PI) / 8)) && (ddir > (((4) * PI) / 8)))
         {
            drawtile += tframes * 7;   //ul
         }
         else
         {
            drawtile += tframes * 6;   //l
         }
         break;
   }
   if (!(c.csets & 0xF0) || !(c.csets & 0x0F))
      overtile16(dest, drawtile, x, y, cset, c.flip);

   else
   {
      int csets[4];
      int cofs = c.csets & 15;
      if (cofs & 8)
         cofs |= ~int(0xF);

      for (int i = 0; i < 4; ++i)
         csets[i] = c.csets & (16 << i) ? cset + cofs : cset;

      overblock8(dest, drawtile << 2, x, y, csets, c.flip, 15);
   }
}

void overcombo2(BITMAP *dest, int x, int y, int cmbdat, int cset)
{
   if (cmbdat != 0)
      overcombo(dest, x, y, cmbdat, cset);
}

/* end of tiles.cpp */
