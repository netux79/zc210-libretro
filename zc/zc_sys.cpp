//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  zc_sys.cpp
//
//  System functions, input handlers, GUI stuff, etc.
//  for Zelda Classic.
//
//--------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include "zdefs.h"
#include "zelda.h"
#include "tiles.h"
#include "pal.h"
#include "qst.h"
#include "zc_sys.h"
#include "subscr.h"
#include "maps.h"
#include "sprite.h"
#include "guys.h"
#include "link.h"
#include "title.h"
#include "particles.h"

extern LinkClass Link;
extern sprite_list  guys, items, Ewpns, Lwpns, Sitems, chainlinks, decorations,
       particles;

/**********************************/
/******** System functions ********/
/**********************************/

int black_opening_count = 0;
int black_opening_x, black_opening_y;

void close_black_opening(int x, int y, bool wait)
{
   black_opening_count = 66;
   black_opening_x = x;
   black_opening_y = y;
   lensclk = 0;

   if (wait)
   {
      for (int i = 0; i < 66; i++)
      {
         draw_screen(tmpscr, 0, 0);
         putsubscr(framebuf, 0, 0);
         advanceframe();
         if (zc_state)
            break;
      }
   }
}

void open_black_opening(int x, int y, bool wait)
{
   black_opening_count = -66;
   black_opening_x = x;
   black_opening_y = y;
   lensclk = 0;

   if (wait)
   {
      for (int i = 0; i < 66; i++)
      {
         draw_screen(tmpscr, 0, 0);
         putsubscr(framebuf, 0, 0);
         advanceframe();
         if (zc_state)
            break;
      }
   }
}

void black_opening(BITMAP *dest, int x, int y, int a, int max_a)
{
   clear_to_color(tempbuf, BLACK);
   int w = 256, h = 224;

   double new_w = (w / 2) + abs(w / 2 - x);
   double new_h = (h / 2) + abs(h / 2 - y);
   int r = int(sqrt((new_w * new_w) + (new_h * new_h)) * a / max_a);
   circlefill(tempbuf, x, y, r, 0);
   masked_blit(tempbuf, dest, 0, 0, 0, 0, w, h);
}

//----------------------------------------------------------------

bool item_disabled(int item_type,
                   int item)                 //is this item disabled?
{
   return false;
}

bool can_use_item(int item_type,
                  int item)                  //can Link use this item?
{
   if (has_item(item_type, item) && !item_disabled(item_type, item))
      return true;
   return false;
}

bool has_item(int item_type, int it)   /* does Link possess this item? */
{
   switch (item_type)
   {
      case itype_bomb:
      case itype_sbomb:
         return (game.items[item_type] > 0);
         break;
      case itype_clock:
         return Link.getClock() ? 1 : 0;
      case itype_key:
         return (game.keys > 0);
      case itype_magiccontainer:
         return (game.maxmagic >= MAGICPERBLOCK);
      case itype_triforcepiece:
         //it: -2=any, -1=current level, other=that level
      {
         switch (it)
         {
            case -2:
            {
               for (int i = 0; i < MAXLEVELS; i++)
               {
                  if (game.lvlitems[i] | liTRIFORCE)

                     return true;
               }
               return false;
               break;
            }
            case -1:
               return (game.lvlitems[dlevel] | liTRIFORCE);
               break;
            default:
               if (it >= 0 && it < MAXLEVELS)
                  return (game.lvlitems[it] | liTRIFORCE);
               break;
         }
         return 0;
      }
      case itype_map:
         //it: -2=any, -1=current level, other=that level
      {
         switch (it)
         {
            case -2:
            {
               for (int i = 0; i < MAXLEVELS; i++)
               {
                  if (game.lvlitems[i] | liMAP)
                     return true;
               }
               return false;
            }
            break;
            case -1:
               return (game.lvlitems[dlevel] | liMAP);
               break;
            default:
               if (it >= 0 && it < MAXLEVELS)
                  return (game.lvlitems[it] | liMAP);
               break;
         }
         return 0;
      }
      case itype_compass:
         //it: -2=any, -1=current level, other=that level
      {
         switch (it)
         {
            case -2:
            {
               for (int i = 0; i < MAXLEVELS; i++)
               {
                  if (game.lvlitems[i] | liCOMPASS)
                     return true;
               }
               return false;
               break;
            }
            case -1:
               return (game.lvlitems[dlevel] | liCOMPASS);
               break;
            default:
               if (it >= 0 && it < MAXLEVELS)
                  return (game.lvlitems[it] | liCOMPASS);
               break;
         }
         return 0;
      }
      case itype_bosskey:
         //it: -2=any, -1=current level, other=that level
      {
         switch (it)
         {
            case -2:
            {
               for (int i = 0; i < MAXLEVELS; i++)
               {
                  if (game.lvlitems[i] | liBOSSKEY)
                     return true;
               }
               return false;
               break;
            }
            case -1:
               return (game.lvlitems[dlevel] | liBOSSKEY) ? 1 : 0;
               break;
            default:
               if (it >= 0 && it < MAXLEVELS)
                  return (game.lvlitems[it] | liBOSSKEY) ? 1 : 0;
               break;
         }
         return 0;
      }
      default:
         it = (1 << (it - 1));
         if (item_type >= itype_max)
         {
            zc_error("Error - has_item() exception.");
            return false;
         }
         if (game.items[item_type]&it)
            return true;
         break;
   }
   return false;
}

int high_item(int jmax, int item_type, bool consecutive, int itemcluster,
              bool usecluster)
{

   if (usecluster)
   {
      for (int j = jmax - 1; j > 0; j--)
      {
         if (itemcluster & (1 << (j - 1)))
            return consecutive ? j : (1 << (j - 1));
      }
   }
   else
   {
      for (int j = jmax - 1; j > 0; j--)
      {
         if (can_use_item(item_type, j))

            return consecutive ? j : (1 << (j - 1));
      }
   }

   return 0;
}

int current_item(int item_type, bool consecutive)/* item currently being used */
{
   int jmax = 0;
   switch (item_type)
   {
      case itype_sword:
         jmax = imax_sword;
         break;
      case itype_brang:
         jmax = imax_brang;
         break;
      case itype_arrow:
         jmax = imax_arrow;
         break;
      case itype_candle:
         jmax = imax_candle;
         break;
      case itype_whistle:
         jmax = imax_whistle;
         break;
      case itype_bait:
         jmax = imax_bait;
         break;
      case itype_letter:
         jmax = imax_letter;
         break;
      case itype_potion:
         jmax = imax_potion;
         break;
      case itype_wand:
         jmax = imax_wand;
         break;
      case itype_ring:
         jmax = imax_ring;
         break;
      case itype_wallet:
         jmax = imax_wallet;
         break;
      case itype_amulet:
         jmax = imax_amulet;
         break;
      case itype_shield:
         jmax = imax_shield;
         break;
      case itype_bow:
         jmax = imax_bow;
         break;
      case itype_raft:
         jmax = imax_raft;
         break;
      case itype_ladder:
         jmax = imax_ladder;
         break;
      case itype_book:
         jmax = imax_book;
         break;
      case itype_magickey:
         jmax = imax_magickey;
         break;
      case itype_bracelet:
         jmax = imax_bracelet;
         break;
      case itype_flippers:
         jmax = imax_flippers;
         break;
      case itype_boots:
         jmax = imax_boots;
         break;
      case itype_hookshot:
         jmax = imax_hookshot;
         break;
      case itype_lens:
         jmax = imax_lens;
         break;
      case itype_hammer:
         jmax = imax_hammer;
         break;
      case itype_dinsfire:
         jmax = imax_dinsfire;
         break;
      case itype_faroreswind:
         jmax = imax_faroreswind;
         break;
      case itype_nayruslove:
         jmax = imax_nayruslove;
         break;
      case itype_bomb:
      case itype_sbomb:
         return can_use_item(item_type, 1) ? game.items[item_type] : 0;
         break;
      case itype_clock:
         return has_item(itype_clock, 1) ? 1 : 0;
         break;
      case itype_key:
         return game.keys;
      case itype_magiccontainer:
         return game.maxmagic / MAGICPERBLOCK;
      case itype_triforcepiece:
      {
         int count = 0;
         for (int i = 0; i < MAXLEVELS; i++)
            count += (game.lvlitems[i] & liTRIFORCE) ? 1 : 0;
         return 0;
         break;
      }
      case itype_map:
      {
         int count = 0;
         for (int i = 0; i < MAXLEVELS; i++)
            count += (game.lvlitems[i] & liMAP) ? 1 : 0;
         return count;
         break;
      }
      case itype_compass:
      {
         int count = 0;
         for (int i = 0; i < MAXLEVELS; i++)
            count += (game.lvlitems[i] & liCOMPASS) ? 1 : 0;
         return count;
         break;
      }
      case itype_bosskey:
      {
         int count = 0;
         for (int i = 0; i < MAXLEVELS; i++)
            count += (game.lvlitems[i] & liBOSSKEY) ? 1 : 0;
         return count;
         break;
      }
      default:
         return 0;
         break;
   }
   return high_item(jmax, item_type, consecutive, 0, false);
}

int item_tile_mod(void)
{
   long tile = 0;
   int ret = 0;

   ret = current_item(itype_sword, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iSword].ltm;
         break;
      case 2:
         ret = itemsbuf[iWSword].ltm;
         break;
      case 3:
         ret = itemsbuf[iMSword].ltm;
         break;
      case 4:
         ret = itemsbuf[iXSword].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_brang, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iBrang].ltm;
         break;
      case 2:
         ret = itemsbuf[iMBrang].ltm;
         break;
      case 3:
         ret = itemsbuf[iFBrang].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_arrow, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iArrow].ltm;
         break;
      case 2:
         ret = itemsbuf[iSArrow].ltm;
         break;
      case 3:
         ret = itemsbuf[iGArrow].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_candle, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iBCandle].ltm;
         break;
      case 2:
         ret = itemsbuf[iRCandle].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_whistle, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iWhistle].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_bait, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iBait].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_letter, true);
   switch (ret)
   {
      case 1:
      case 2:
         ret = itemsbuf[iLetter].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_potion, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iBPotion].ltm;
         break;
      case 2:
         ret = itemsbuf[iRPotion].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_wand, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iWand].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_ring, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iBRing].ltm;
         break;
      case 2:
         ret = itemsbuf[iRRing].ltm;
         break;
      case 3:
         ret = itemsbuf[iGRing].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_wallet, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iWallet500].ltm;
         break;
      case 2:
         ret = itemsbuf[iWallet999].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_amulet, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iAmulet].ltm;
         break;
      case 2:
         ret = itemsbuf[iL2Amulet].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_shield, true);
   switch (ret)
   {
      case 1:
         ret = 0;
         break;
      case 2:
         ret = itemsbuf[iShield].ltm;
         break;
      case 3:
         ret = itemsbuf[iMShield].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_bow, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iBow].ltm;
         break;
      case 2:
         ret = itemsbuf[iBow2].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_raft, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iRaft].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_ladder, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iLadder].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_book, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iBook].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_magickey, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iMKey].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_bracelet, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iBracelet].ltm;
         break;
      case 2:
         ret = itemsbuf[iL2Bracelet].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_flippers, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iFlippers].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_boots, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iBoots].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_hookshot, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iHookshot].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_lens, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iLens].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_hammer, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iHammer].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_dinsfire, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iDinsFire].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_faroreswind, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iFaroresWind].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_nayruslove, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iNayrusLove].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_bomb, true);
   switch (ret)
   {
      case 0:
         ret = 0;
         break;
      default:
         ret = itemsbuf[iBombs].ltm;
         break;
   }
   tile += ret;

   ret = current_item(itype_sbomb, true);
   switch (ret)
   {
      case 0:
         ret = 0;
         break;
      default:
         ret = itemsbuf[iSBomb].ltm;
         break;
   }
   tile += ret;

   ret = current_item(itype_clock, true);
   switch (ret)
   {
      case 1:
         ret = itemsbuf[iClock].ltm;
         break;
      default:
         ret = 0;
         break;
   }
   tile += ret;

   ret = current_item(itype_key, true);
   switch (ret)
   {
      case 0:
         ret = 0;
         break;
      default:
         ret = itemsbuf[iKey].ltm;
         break;
   }
   tile += ret;

   ret = current_item(itype_map, true);
   switch (ret)
   {
      case 0:
         ret = 0;
         break;
      default:
         ret = itemsbuf[iMap].ltm;
         break;
   }
   tile += ret;

   ret = current_item(itype_compass, true);
   switch (ret)
   {
      case 0:
         ret = 0;
         break;
      default:
         ret = itemsbuf[iCompass].ltm;
         break;
   }
   tile += ret;

   ret = current_item(itype_bosskey, true);
   switch (ret)
   {
      case 0:
         ret = 0;
         break;
      default:
         ret = itemsbuf[iBossKey].ltm;
         break;
   }
   tile += ret;

   ret = current_item(itype_magiccontainer, true);
   switch (ret)
   {
      case 0:
         ret = 0;
         break;
      default:
         ret = itemsbuf[iMagicC].ltm;
         break;
   }
   tile += ret;

   ret = current_item(itype_triforcepiece, true);
   switch (ret)
   {
      case 0:
         ret = 0;
         break;
      default:
         ret = itemsbuf[iTriforce].ltm;
         break;
   }
   tile += ret;
   return tile;
}

void draw_lens_under(void)
{
   int strike_hint_table[11] =
   {
      mfARROW, mfBOMB, mfBRANG, mfWANDMAGIC,
      mfSWORD, mfREFMAGIC, mfHOOKSHOT,
      mfREFFIREBALL, mfHAMMER, mfSWORDBEAM, mfWAND
   };

   {
      int blink_rate = 1;
      int tempitem, tempweapon;
      strike_hint = strike_hint_table[strike_hint_counter];
      if (strike_hint_timer > 32)
      {
         strike_hint_timer = 0;
         strike_hint_counter = ((strike_hint_counter + 1) % 11);
      }
      ++strike_hint_timer;

      for (int i = 0; i < 176; i++)
      {
         int x = (i & 15) << 4;
         int y = (i & 0xF0) + 56;
         int tempitemx = -16, tempitemy = -16;
         int tempweaponx = -16, tempweapony = -16;

         switch (tmpscr->sflag[i])
         {
            case 0:
            case mfZELDA:
            case mfPUSHED:
            case mfENEMY0:
            case mfENEMY1:
            case mfENEMY2:
            case mfENEMY3:
            case mfENEMY4:
            case mfENEMY5:
            case mfENEMY6:

            case mfENEMY7:
            case mfENEMY8:
            case mfENEMY9:
               break;

            case mfPUSHUD:
            case mfPUSHLR:
            case mfPUSH4:
            case mfPUSHU:
            case mfPUSHD:
            case mfPUSHL:
            case mfPUSHR:
            case mfPUSHUDNS:
            case mfPUSHLRNS:
            case mfPUSH4NS:
            case mfPUSHUNS:
            case mfPUSHDNS:
            case mfPUSHLNS:
            case mfPUSHRNS:
            case mfPUSHUDINS:
            case mfPUSHLRINS:
            case mfPUSH4INS:

            case mfPUSHUINS:
            case mfPUSHDINS:
            case mfPUSHLINS:
            case mfPUSHRINS:
               if (lensclk & 16)
                  putcombo(framebuf, x, y, tmpscr->undercombo, tmpscr->undercset);
               if (lensclk & blink_rate)
               {
                  if (get_bit(quest_rules, qr_LENSHINTS))
                  {
                     switch (combobuf[tmpscr->data[i]].type)
                     {
                        case cPUSH_HEAVY:
                        case cPUSH_HW:
                           tempitem = iBracelet;
                           tempitemx = x, tempitemy = y;
                           putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                                    lens_hint_item[tempitem][1], 0);
                           break;
                        case cPUSH_HEAVY2:
                        case cPUSH_HW2:
                           tempitem = iL2Bracelet;
                           tempitemx = x, tempitemy = y;
                           putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                                    lens_hint_item[tempitem][1], 0);
                           break;
                     }
                  }
               }
               break;

            case mfWHISTLE:
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iWhistle;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfFAIRY:
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iFairyMoving;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfBCANDLE:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sBCANDLE],
                        tmpscr->secretcset[sBCANDLE]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iBCandle;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfRCANDLE:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sRCANDLE],
                        tmpscr->secretcset[sRCANDLE]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iRCandle;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfWANDFIRE:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sWANDFIRE],
                        tmpscr->secretcset[sWANDFIRE]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iWand;
                  tempweapon = wFire;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  else
                  {
                     tempweaponx = x;
                     tempweapony = y;
                  }
                  putweapon(framebuf, tempweaponx, tempweapony, tempweapon, 0, up,
                            lens_hint_weapon[tempweapon][0], lens_hint_weapon[tempweapon][1]);
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfDINSFIRE:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sDINSFIRE],
                        tmpscr->secretcset[sDINSFIRE]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iDinsFire;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfARROW:
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  putcombo(framebuf, x, y, tmpscr->secretcombo[sARROW],
                           tmpscr->secretcset[sARROW]);
                  tempitem = iArrow;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfSARROW:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sSARROW],
                        tmpscr->secretcset[sSARROW]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iSArrow;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfGARROW:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sGARROW],
                        tmpscr->secretcset[sGARROW]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iGArrow;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfBOMB:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sBOMB], tmpscr->secretcset[sBOMB]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempweapon = wBomb;
                  if (lensclk & blink_rate)
                  {
                     tempweaponx = x;
                     tempweapony = y;
                  }
                  putweapon(framebuf, tempweaponx, tempweapony + lens_hint_weapon[tempweapon][4],
                            tempweapon, 0, up, lens_hint_weapon[tempweapon][0],
                            lens_hint_weapon[tempweapon][1]);
               }
               break;

            case mfSBOMB:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sSBOMB],
                        tmpscr->secretcset[sSBOMB]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempweapon = wSBomb;
                  if (lensclk & blink_rate)
                  {
                     tempweaponx = x;
                     tempweapony = y;
                  }
                  putweapon(framebuf, tempweaponx, tempweapony + lens_hint_weapon[tempweapon][4],
                            tempweapon, 0, up, lens_hint_weapon[tempweapon][0],
                            lens_hint_weapon[tempweapon][1]);
               }
               break;

            case mfARMOS_SECRET:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sSTAIRS],
                        tmpscr->secretcset[sSTAIRS]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
               }
               break;

            case mfBRANG:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sBRANG],
                        tmpscr->secretcset[sBRANG]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iBrang;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfMBRANG:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sMBRANG],
                        tmpscr->secretcset[sMBRANG]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iMBrang;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfFBRANG:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sFBRANG],
                        tmpscr->secretcset[sFBRANG]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iFBrang;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfWANDMAGIC:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sWANDMAGIC],
                        tmpscr->secretcset[sWANDMAGIC]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iWand;
                  tempweapon = wMagic;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  else
                  {
                     tempweaponx = x;
                     tempweapony = y;
                     --lens_hint_weapon[wMagic][4];
                     if (lens_hint_weapon[wMagic][4] < -8)
                        lens_hint_weapon[wMagic][4] = 8;
                  }
                  putweapon(framebuf, tempweaponx, tempweapony + lens_hint_weapon[tempweapon][4],
                            tempweapon, 0, up, lens_hint_weapon[tempweapon][0],
                            lens_hint_weapon[tempweapon][1]);
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfREFMAGIC:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sREFMAGIC],
                        tmpscr->secretcset[sREFMAGIC]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iMShield;
                  tempweapon = ewMagic;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  else
                  {
                     tempweaponx = x;
                     tempweapony = y;
                     if (lens_hint_weapon[ewMagic][2] == up)
                        --lens_hint_weapon[ewMagic][4];

                     else
                        ++lens_hint_weapon[ewMagic][4];
                     if (lens_hint_weapon[ewMagic][4] > 8)
                        lens_hint_weapon[ewMagic][2] = up;
                     if (lens_hint_weapon[ewMagic][4] <= 0)
                        lens_hint_weapon[ewMagic][2] = down;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
                  putweapon(framebuf, tempweaponx, tempweapony + lens_hint_weapon[tempweapon][4],
                            tempweapon, 0, lens_hint_weapon[ewMagic][2], lens_hint_weapon[tempweapon][0],
                            lens_hint_weapon[tempweapon][1]);
               }
               break;

            case mfREFFIREBALL:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sREFFIREBALL],
                        tmpscr->secretcset[sREFFIREBALL]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iMShield;
                  tempweapon = ewFireball;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                     tempweaponx = x;
                     tempweapony = y;
                     ++lens_hint_weapon[ewFireball][3];
                     if (lens_hint_weapon[ewFireball][3] > 8)
                     {
                        lens_hint_weapon[ewFireball][3] = -8;
                        lens_hint_weapon[ewFireball][4] = 8;
                     }
                     if (lens_hint_weapon[ewFireball][3] > 0)
                        ++lens_hint_weapon[ewFireball][4];

                     else
                        --lens_hint_weapon[ewFireball][4];
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
                  putweapon(framebuf, tempweaponx + lens_hint_weapon[tempweapon][3],
                            tempweapony + lens_hint_weapon[ewFireball][4], tempweapon, 0, up,
                            lens_hint_weapon[tempweapon][0], lens_hint_weapon[tempweapon][1]);
               }
               break;

            case mfSWORD:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sSWORD],
                        tmpscr->secretcset[sSWORD]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iSword;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfWSWORD:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sWSWORD],
                        tmpscr->secretcset[sWSWORD]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iWSword;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfMSWORD:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sMSWORD],
                        tmpscr->secretcset[sMSWORD]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iMSword;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfXSWORD:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sXSWORD],
                        tmpscr->secretcset[sXSWORD]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iXSword;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfSWORDBEAM:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sSWORDBEAM],
                        tmpscr->secretcset[sSWORDBEAM]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iSword;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 1);
               }
               break;

            case mfWSWORDBEAM:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sWSWORDBEAM],
                        tmpscr->secretcset[sWSWORDBEAM]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iWSword;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 2);
               }
               break;

            case mfMSWORDBEAM:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sMSWORDBEAM],
                        tmpscr->secretcset[sMSWORDBEAM]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iMSword;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 3);
               }
               break;

            case mfXSWORDBEAM:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sXSWORDBEAM],
                        tmpscr->secretcset[sXSWORDBEAM]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iXSword;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 4);
               }
               break;

            case mfHOOKSHOT:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sHOOKSHOT],
                        tmpscr->secretcset[sHOOKSHOT]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iHookshot;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfWAND:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sWAND], tmpscr->secretcset[sWAND]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iWand;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfHAMMER:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sHAMMER],
                        tmpscr->secretcset[sHAMMER]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  tempitem = iHammer;
                  if (lensclk & blink_rate)
                  {
                     tempitemx = x;
                     tempitemy = y;
                  }
                  putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                           lens_hint_item[tempitem][1], 0);
               }
               break;

            case mfSTRIKE:
               putcombo(framebuf, x, y, tmpscr->secretcombo[sSTRIKE],
                        tmpscr->secretcset[sSTRIKE]);
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
                  switch (strike_hint)
                  {
                     case mfARROW:
                        tempitem = iArrow;
                        if (lensclk & blink_rate)
                        {
                           tempitemx = x;
                           tempitemy = y;
                        }
                        putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                                 lens_hint_item[tempitem][1], 0);
                        break;
                     case mfBOMB:
                        tempweapon = wBomb;
                        if (lensclk & blink_rate)
                        {
                           tempweaponx = x;
                           tempweapony = y;
                        }
                        putweapon(framebuf, tempweaponx, tempweapony + lens_hint_weapon[tempweapon][4],
                                  tempweapon, 0, up, lens_hint_weapon[tempweapon][0],
                                  lens_hint_weapon[tempweapon][1]);
                        break;
                     case mfBRANG:
                        tempitem = iBrang;
                        if (lensclk & blink_rate)
                        {
                           tempitemx = x;
                           tempitemy = y;
                        }
                        putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                                 lens_hint_item[tempitem][1], 0);
                        break;
                     case mfWANDMAGIC:
                        tempitem = iWand;
                        tempweapon = wMagic;
                        if (lensclk & blink_rate)
                        {
                           tempitemx = x;
                           tempitemy = y;
                        }
                        else
                        {
                           tempweaponx = x;
                           tempweapony = y;
                           --lens_hint_weapon[wMagic][4];
                           if (lens_hint_weapon[wMagic][4] < -8)
                              lens_hint_weapon[wMagic][4] = 8;
                        }
                        putweapon(framebuf, tempweaponx, tempweapony + lens_hint_weapon[wMagic][4],
                                  tempweapon, 0, up, lens_hint_weapon[tempweapon][0],
                                  lens_hint_weapon[tempweapon][1]);
                        putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                                 lens_hint_item[tempitem][1], 0);
                        break;
                     case mfREFMAGIC:
                        tempitem = iMShield;
                        tempweapon = ewMagic;
                        if (lensclk & blink_rate)
                        {
                           tempitemx = x;
                           tempitemy = y;
                        }
                        else
                        {
                           tempweaponx = x;
                           tempweapony = y;
                           if (lens_hint_weapon[ewMagic][2] == up)
                              --lens_hint_weapon[ewMagic][4];

                           else
                              ++lens_hint_weapon[ewMagic][4];
                           if (lens_hint_weapon[ewMagic][4] > 8)
                              lens_hint_weapon[ewMagic][2] = up;
                           if (lens_hint_weapon[ewMagic][4] <= 0)
                              lens_hint_weapon[ewMagic][2] = down;
                        }
                        putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                                 lens_hint_item[tempitem][1], 0);
                        putweapon(framebuf, tempweaponx, tempweapony + lens_hint_weapon[ewMagic][4],
                                  tempweapon, 0, lens_hint_weapon[ewMagic][2], lens_hint_weapon[tempweapon][0],
                                  lens_hint_weapon[tempweapon][1]);
                        break;
                     case mfREFFIREBALL:
                        tempitem = iMShield;
                        tempweapon = ewFireball;
                        if (lensclk & blink_rate)
                        {
                           tempitemx = x;
                           tempitemy = y;
                           tempweaponx = x;
                           tempweapony = y;
                           ++lens_hint_weapon[ewFireball][3];
                           if (lens_hint_weapon[ewFireball][3] > 8)
                           {
                              lens_hint_weapon[ewFireball][3] = -8;
                              lens_hint_weapon[ewFireball][4] = 8;
                           }
                           if (lens_hint_weapon[ewFireball][3] > 0)
                              ++lens_hint_weapon[ewFireball][4];

                           else
                              --lens_hint_weapon[ewFireball][4];
                        }
                        putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                                 lens_hint_item[tempitem][1], 0);
                        putweapon(framebuf, tempweaponx + lens_hint_weapon[ewFireball][3],
                                  tempweapony + lens_hint_weapon[ewFireball][4], tempweapon, 0, up,
                                  lens_hint_weapon[tempweapon][0], lens_hint_weapon[tempweapon][1]);
                        break;
                     case mfSWORD:
                        tempitem = iSword;
                        if (lensclk & blink_rate)
                        {
                           tempitemx = x;
                           tempitemy = y;
                        }
                        putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                                 lens_hint_item[tempitem][1], 0);
                        break;
                     case mfSWORDBEAM:
                        tempitem = iSword;
                        if (lensclk & blink_rate)
                        {
                           tempitemx = x;
                           tempitemy = y;
                        }
                        putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                                 lens_hint_item[tempitem][1], 1);
                        break;
                     case mfHOOKSHOT:
                        tempitem = iHookshot;
                        if (lensclk & blink_rate)
                        {
                           tempitemx = x;
                           tempitemy = y;
                        }
                        putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                                 lens_hint_item[tempitem][1], 0);
                        break;
                     case mfWAND:
                        tempitem = iWand;
                        if (lensclk & blink_rate)
                        {
                           tempitemx = x;
                           tempitemy = y;
                        }
                        putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                                 lens_hint_item[tempitem][1], 0);
                        break;
                     case mfHAMMER:
                        tempitem = iHammer;
                        if (lensclk & blink_rate)
                        {
                           tempitemx = x;
                           tempitemy = y;
                        }
                        putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                                 lens_hint_item[tempitem][1], 0);
                        break;
                  }
               }
               break;
            case mfARMOS_ITEM:
            case mfDIVE_ITEM:
               if (get_bit(quest_rules, qr_LENSHINTS))
               {
               }
               if (!getmapflag())
                  putitem2(framebuf, x, y, tmpscr->catchall, lens_hint_item[tmpscr->catchall][0],
                           lens_hint_item[tmpscr->catchall][1], 0);
               break;

            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28:
            case 29:
            case 30:
            case 31:
               putcombo(framebuf, x, y, tmpscr->secretcombo[(tmpscr->sflag[i]) - 16 + 4],
                        tmpscr->secretcset[(tmpscr->sflag[i]) - 16 + 4]);
               break;

            default:
               if (lensclk & 1)
                  rectfill(framebuf, x, y, x + 15, y + 15, WHITE);
               break;
         }
      }

      if (tmpscr->door[0] == dWALK)
         rectfill(framebuf, 120, 16 + 56, 135, 31 + 56, WHITE);

      if (tmpscr->door[1] == dWALK)
         rectfill(framebuf, 120, 144 + 56, 135, 159 + 56, WHITE);

      if (tmpscr->door[2] == dWALK)
         rectfill(framebuf, 16, 80 + 56, 31, 95 + 56, WHITE);

      if (tmpscr->door[3] == dWALK)
         rectfill(framebuf, 224, 80 + 56, 239, 95 + 56, WHITE);

      if (tmpscr->door[0] == dBOMB)
         showbombeddoor(0);

      if (tmpscr->door[1] == dBOMB)
         showbombeddoor(1);

      if (tmpscr->door[2] == dBOMB)
         showbombeddoor(2);

      if (tmpscr->door[3] == dBOMB)
         showbombeddoor(3);

      if (tmpscr->stairx + tmpscr->stairy)
      {
         putcombo(framebuf, tmpscr->stairx, tmpscr->stairy + 56,
                  tmpscr->secretcombo[sSTAIRS], tmpscr->secretcset[sSTAIRS]);
         if (get_bit(quest_rules, qr_LENSHINTS))
         {
            if (tmpscr->flags & fWHISTLE)
            {
               tempitem = iWhistle;
               int tempitemx = -16;
               int tempitemy = -16;
               if (lensclk & (blink_rate / 4))
               {
                  tempitemx = tmpscr->stairx;
                  tempitemy = tmpscr->stairy + 56;
               }
               putitem2(framebuf, tempitemx, tempitemy, tempitem, lens_hint_item[tempitem][0],
                        lens_hint_item[tempitem][1], 0);
            }
         }
      }
   }
}

void draw_lens_over(void)
{
   clear_to_color(tempbuf, BLACK);
   circlefill(tempbuf, LinkX() + 8, LinkY() + 8 + 56, 60, 0);
   circle(tempbuf, LinkX() + 8, LinkY() + 8 + 56, 62, 0);
   circle(tempbuf, LinkX() + 8, LinkY() + 8 + 56, 65, 0);
   masked_blit(tempbuf, framebuf, 0, 56, 0, 56, 256, 168);
}

//----------------------------------------------------------------

void draw_wavy(int amplitude)
{
   BITMAP *wavebuf = create_bitmap(288, 224);
   clear_to_color(wavebuf, 0);
   blit(framebuf, wavebuf, 0, 0, 16, 0, 256, 224);

   int ofs;
   int amp2 = 168;
   int i = frame % amp2;
   for (int j = 0; j < 168; j++)
   {
      ofs = 0;
      if (j & 1)
         ofs = int(sin((double(i + j) * 2 * PI / amp2)) * amplitude);

      else
         ofs -= int(sin((double(i + j) * 2 * PI / amp2)) * amplitude);
      for (int k = 0; k < 256; k++)
         framebuf->line[j + 56][k] = wavebuf->line[j + 56][k + ofs + 16];
   }
   destroy_bitmap(wavebuf);
}

void draw_fuzzy(int fuzz)
// draws from right half of scrollbuf to framebuf
{
   int firstx, firsty, xstep, ystep, i, y, dx, dy;
   uint8_t *start, *si, *di;

   if (fuzz < 1)
      fuzz = 1;

   xstep = 128 % fuzz;
   if (xstep > 0)
      xstep = fuzz - xstep;

   ystep = 112 % fuzz;
   if (ystep > 0)
      ystep = fuzz - ystep;

   firsty = 1;

   for (y = 0; y < 224;)
   {
      start = &(scrollbuf->line[y][256]);

      for (dy = 0; dy < ystep && dy + y < 224; dy++)
      {
         si = start;
         di = &(framebuf->line[y + dy][0]);
         i = xstep;
         firstx = 1;

         for (dx = 0; dx < 256; dx++)
         {
            *(di++) = *si;
            if (++i >= fuzz)
            {
               if (!firstx)
                  si += fuzz;

               else
               {
                  si += fuzz - xstep;
                  firstx = 0;
               }
               i = 0;
            }
         }
      }

      if (!firsty)
         y += fuzz;

      else
      {
         y += ystep;
         ystep = fuzz;
         firsty = 0;
      }
   }
}

void update_video_frame(void)
{
   if (!is_playing)
      black_opening_count = 0;

   if (black_opening_count < 0)   //shape is opening up
   {
      black_opening(framebuf, black_opening_x, black_opening_y,
                    (66 + black_opening_count), 66);
      ++black_opening_count;
   }
   else if (black_opening_count > 0)     //shape is closing
   {
      black_opening(framebuf, black_opening_x, black_opening_y, 
                    black_opening_count, 66);
      --black_opening_count;
   }

   if (Link.DrunkClock())
      draw_wavy(Link.DrunkClock() / (MAXDRUNKCLOCK / 32));

   /* Don't show subscreen */
   if (tmpscr->flags3 & fNOSUBSCR)
   {
      clear_bitmap(tempbuf);
      blit(framebuf, tempbuf, 0, 56, 0, 56 / 2, 256, 224 - 56);
      
      zc_canvas = tempbuf;
   }
   else
      zc_canvas = framebuf;
}

void zc_action(int state)
{
   music_pause();
   pause_all_sfx();
   zc_state = state;
   eat_buttons();
}

// 99*360 + 59*60
#define MAXTIME  21405240

void advanceframe(void)
{
   if (zc_state)
      return;

   if (is_playing && game.time < MAXTIME)
      ++game.time;

   ++frame;

   /* process midi music */
   midi_fill_buffer();
   /* prepare the video frame */
   update_video_frame();

   /* signal main thread to continue
    * and wait for it to ask for the
    * next iteration. */
   scond_signal(cond);
   scond_wait(cond, mutex);

   sfx_cleanup();
}

void zapout(void)
{
   // draw screen on right half of scrollbuf
   blit(framebuf, scrollbuf, 0, 0, 256, 0, 256, 224);

   // zap out
   for (int i = 1; i <= 24; i++)
   {
      draw_fuzzy(i);
      advanceframe();
      if (zc_state)
         break;
   }
}

void zapin(void)
{
   // draw screen on right half of scrollbuf
   draw_screen(tmpscr, 0, 0);
   putsubscr(framebuf, 0, 0);
   blit(framebuf, scrollbuf, 0, 0, 256, 0, 256, 224);

   // zap out
   for (int i = 24; i >= 1; i--)
   {
      draw_fuzzy(i);
      advanceframe();
      if (zc_state)
         break;
   }
}


void wavyout(void)
{
   draw_screen(tmpscr, 0, 0);
   putsubscr(framebuf, 0, 0);

   BITMAP *wavebuf = create_bitmap(288, 224);
   clear_to_color(wavebuf, 0);
   blit(framebuf, wavebuf, 0, 0, 16, 0, 256, 224);

   PALETTE wavepal;
   
   /* use this as main palette
    * temporary for this process */
   zc_palette = wavepal;   

   int ofs;
   int amplitude = 8;

   int wavelength = 4;
   double palpos = 0, palstep = 4, palstop = 126;
   for (int i = 0; i < 168; i += wavelength)
   {
      for (int l = 0; l < 256; l++)
      {
         wavepal[l].r = vbound(int(RAMpal[l].r + ((palpos / palstop) *
                                   (63 - RAMpal[l].r))), 0, 63);
         wavepal[l].g = vbound(int(RAMpal[l].g + ((palpos / palstop) *
                                   (63 - RAMpal[l].g))), 0, 63);
         wavepal[l].b = vbound(int(RAMpal[l].b + ((palpos / palstop) *
                                   (63 - RAMpal[l].b))), 0, 63);
      }

      palpos += palstep;

      for (int j = 0; j < 168; j++)
      {
         for (int k = 0; k < 256; k++)
         {
            ofs = 0;
            if ((j < i) && (j & 1))
               ofs = int(sin((double(i + j) * 2 * PI / 168.0)) * amplitude);
            framebuf->line[j + 56][k] = wavebuf->line[j + 56][k + ofs + 16];
         }
      }
      
      /* ensure changes get applied */
      zc_sync_pal = true;
      advanceframe();

      if (zc_state)
         break;
   }

   destroy_bitmap(wavebuf);
   
   /* restore system palette */
   zc_palette = RAMpal;
   zc_sync_pal = true;
}

void wavyin(void)
{
   draw_screen(tmpscr, 0, 0);
   putsubscr(framebuf, 0, 0);

   BITMAP *wavebuf = create_bitmap(288, 224);
   clear_to_color(wavebuf, 0);
   blit(framebuf, wavebuf, 0, 0, 16, 0, 256, 224);

   PALETTE wavepal;
   
   /* use this as main palette
    * temporary for this process */
   zc_palette = wavepal;   
   
   loadfullpal();
   loadlvlpal(DMaps[currdmap].color);
   ringcolor();
   int ofs;
   int amplitude = 8;
   int wavelength = 4;
   double palpos = 168, palstep = 4, palstop = 126;
   for (int i = 0; i < 168; i += wavelength)
   {
      for (int l = 0; l < 256; l++)
      {
         wavepal[l].r = vbound(int(RAMpal[l].r + ((palpos / palstop) *
                                   (63 - RAMpal[l].r))), 0, 63);
         wavepal[l].g = vbound(int(RAMpal[l].g + ((palpos / palstop) *
                                   (63 - RAMpal[l].g))), 0, 63);
         wavepal[l].b = vbound(int(RAMpal[l].b + ((palpos / palstop) *
                                   (63 - RAMpal[l].b))), 0, 63);
      }

      palpos -= palstep;

      for (int j = 0; j < 168; j++)
      {
         for (int k = 0; k < 256; k++)
         {
            ofs = 0;
            if ((j < (167 - i)) && (j & 1))
               ofs = int(sin((double(i + j) * 2 * PI / 168.0)) * amplitude);
            framebuf->line[j + 56][k] = wavebuf->line[j + 56][k + ofs + 16];
         }
      }
      
      /* ensure the palette change gets applied */
      zc_sync_pal = true;
      advanceframe();

      if (zc_state)
         break;
   }

   destroy_bitmap(wavebuf);

   /* restore system palette */
   zc_palette = RAMpal;
   zc_sync_pal = true;
}

void blackscr(int fcnt, bool showsubscr)
{
   reset_pal_cycling();
   while (fcnt > 0)
   {
      clear_bitmap(framebuf);
      if (showsubscr)
         putsubscr(framebuf, 0, 0);
      advanceframe();
      if (zc_state)
         break;
      ;
      --fcnt;
   }
}

void openscreen(void)
{
   reset_pal_cycling();
   black_opening_count = 0;

   if (COOLSCROLL)
   {
      open_black_opening(LinkX() + 8, LinkY() + 8 + 56, true);
      return;
   }
   else
   {
      Link.setDontDraw(true);
      show_subscreen_dmap_dots = false;
      show_subscreen_numbers = false;
      show_subscreen_items = false;
      show_subscreen_life = false;
   }

   int x = 128;

   for (int i = 0; i < 80; i++)
   {
      draw_screen(tmpscr, 0, 0);
      putsubscr(framebuf, 0, 0);
      x = 128 - (((i * 128 / 80) / 8) * 8);
      if (x > 0)
      {
         rectfill(framebuf, 0, 56, x, 223, 0);
         rectfill(framebuf, 256 - x, 56, 255, 223, 0);
      }
      advanceframe();
      if (zc_state)
         break;
   }
   Link.setDontDraw(false);
   show_subscreen_items = true;
   show_subscreen_dmap_dots = true;
}

int TriforceCount(void)
{
   int c = 0;
   for (int i = 1; i <= 8; i++)
      if (game.lvlitems[i]&liTRIFORCE)
         ++c;
   return c;
}

void color_layer(RGB *src, RGB *dest, char r, char g, char b, char pos,
                 int from, int to)
{
   PALETTE tmp;
   for (int i = 0; i < 256; i++)
   {
      tmp[i].r = r;
      tmp[i].g = g;
      tmp[i].b = b;
   }
   fade_interpolate(src, tmp, dest, pos, from, to);
}

void music_pause(void)
{
   midi_pause();
}

void music_resume(void)
{
   midi_resume();
}

void music_stop(void)
{
   midi_stop();
}

/*****************************/
/**** Custom Sound System ****/
/*****************************/

inline int mixvol(int v1, int v2)
{
   return (min(v1, 255) * min(v2, 255)) >> 8;
}

void jukebox(int index)
{
   if (index < 0)
      index = MAXMUSIC - 1;
   if (index >= MAXMUSIC)
      index = 0;

   // do nothing if it's already playing
   if (index == sel_music && midi_isplaying())
      return;

   sel_music = index;

   music_stop();
   update_music_volume();
   midi_play(tunes[sel_music].midi, tunes[sel_music].loop);
   
   if (tunes[sel_music].start > 0)
      midi_fastforward(tunes[sel_music].start);

   midi_loopend(tunes[sel_music].loop_end);
   midi_loopstart(tunes[sel_music].loop_start);
}

void play_DmapMusic(void)
{
   int m = DMaps[currdmap].midi;
   switch (m)
   {
      case 1:
         jukebox(MUSIC_OVERWORLD);
         break;
      case 2:
         jukebox(MUSIC_DUNGEON);
         break;
      case 3:
         jukebox(MUSIC_LEVEL9);
         break;
      default:
         if (m >= 4 && m < 4 + MAXMIDIS)
            jukebox(m - 4 + MUSIC_COUNT);

         else
            music_stop();
   }
}

void update_music_volume(void)
{
   int i = min(max(sel_music, 0), MAXMUSIC - 1);
   int vol = mixvol(tunes[i].volume, music_vol);
   midi_set_volume(vol);
}

/*****************/
/*****  SFX  *****/
/*****************/

// array of voices, one for each sfx sample in the data file
// 0+ = voice #
// -1 = voice not allocated
static int sfx_voice[SFX_COUNT];

bool zc_initsound(void)
{
   char temp[MAX_STRLEN];

   /* Init mixer */
   if (!mixer_init(sampling_rate / TIMING_FPS, sampling_rate, mix_quality,
         MIXER_MAX_SFX))
      return false;

   /* calculate the Sound Font path to load with the midi engine */
   sprintf(temp, "%s%c%s", system_path, OTHER_PATH_SEPARATOR, ZCSF_FILE);
   
   if (!file_exists(temp))
      return false;

   /* Setup the midi processor */
   if (!midi_init(sampling_rate, 1 / TIMING_FPS, temp))
      return false;

   /* Apply master volume */
   mixer_set_volume(master_vol);

   for (int i = 0; i < SFX_COUNT; i++)
      sfx_voice[i] = -1;

   for (int i = 0; i < MUSIC_COUNT; i++)
      tunes[i].midi = (void *)mididata[i].dat;
      
   return true;
}

void zc_deinitsound(void)
{
   midi_deinit();
   mixer_exit();
}

/* clean up finished samples */
void sfx_cleanup(void)
{
   for (int i = 0; i < SFX_COUNT; i++)
      if (sfx_voice[i] != -1 && voice_get_position(sfx_voice[i]) < 0)
      {
         deallocate_voice(sfx_voice[i]);
         sfx_voice[i] = -1;
      }
}

void update_sfx_volume(void)
{
   for (int i = 0; i < SFX_COUNT; i++)
      if (sfx_voice[i] != -1 && voice_get_position(sfx_voice[i]) >= 0)
         voice_set_volume(sfx_voice[i], sfx_vol);
}

// allocates a voice for the sample "sfx_index" (index into zcdata.dat)
// if a voice is already allocated (and/or playing), then it just returns true
// Returns true:  voice is allocated
//         false: unsuccessful
bool sfx_init(int index)
{
   // check index
   if (index < 0 || index >= SFX_COUNT)
      return false;

   if (sfx_voice[index] == -1)
   {
      // allocate voice
      sfx_voice[index] = allocate_voice((SAMPLE *)sfxdata[index].dat);
   }

   return sfx_voice[index] != -1;
}

// plays an sfx sample
void sfx(int index, int pan, bool loop)
{
   if (!sfx_init(index))
      return;

   voice_set_playmode(sfx_voice[index], loop ? PLAYMODE_LOOP : PLAYMODE_PLAY);
   voice_set_pan(sfx_voice[index], pan);
   voice_set_volume(sfx_voice[index], sfx_vol);

   int pos = voice_get_position(sfx_voice[index]);
   voice_set_position(sfx_voice[index], 0);
   if (pos <= 0)
      voice_start(sfx_voice[index]);
}

// start it (in loop mode) if it's not already playing,
// otherwise just leave it in its current position
void cont_sfx(int index)
{
   if (!sfx_init(index))
      return;

   if (voice_get_position(sfx_voice[index]) <= 0)
   {
      voice_set_position(sfx_voice[index], 0);
      voice_set_playmode(sfx_voice[index], PLAYMODE_LOOP);
      voice_set_volume(sfx_voice[index], sfx_vol);
      voice_start(sfx_voice[index]);
   }
}

// adjust parameters while playing
void adjust_sfx(int index, int pan, bool loop)
{
   if (index < 0 || index >= SFX_COUNT || sfx_voice[index] == -1)
      return;

   voice_set_playmode(sfx_voice[index], loop ? PLAYMODE_LOOP : PLAYMODE_PLAY);
   voice_set_pan(sfx_voice[index], pan);
}

// pauses a voice
void pause_sfx(int index)
{
   if (index >= 0 && index < SFX_COUNT && sfx_voice[index] != -1)
      voice_stop(sfx_voice[index]);
}

// resumes a voice
void resume_sfx(int index)
{
   if (index >= 0 && index < SFX_COUNT && sfx_voice[index] != -1)
      voice_start(sfx_voice[index]);
}

// pauses all active voices
void pause_all_sfx(void)
{
   for (int i = 0; i < SFX_COUNT; i++)
      if (sfx_voice[i] != -1)
         voice_stop(sfx_voice[i]);
}

// resumes all paused voices
void resume_all_sfx(void)
{
   for (int i = 0; i < SFX_COUNT; i++)
      if (sfx_voice[i] != -1)
         voice_start(sfx_voice[i]);
}

// stops an sfx and deallocates the voice
void stop_sfx(int index)
{
   if (index < 0 || index >= SFX_COUNT)
      return;

   if (sfx_voice[index] != -1)
   {
      deallocate_voice(sfx_voice[index]);
      sfx_voice[index] = -1;
   }
}

void kill_sfx(void)
{
   for (int i = 0; i < SFX_COUNT; i++)
      if (sfx_voice[i] != -1)
      {
         deallocate_voice(sfx_voice[i]);
         sfx_voice[i] = -1;
      }
}

int pan(int x)
{
   int p = 128; /* MONO */

   switch (pan_style)
   {
      case 1:  /* 1/2 */
         p = vbound((x >> 1) + 68, 0, 255); break;
      case 2:  /* 3/4 */
         p = vbound(((x * 3) >> 2) + 36, 0, 255);  break;
      case 3:  /* FULL */
         p = vbound(x, 0, 255);  break;
   }

   return p;
}

/*******************************/
/******* Input Handlers ********/
/*******************************/

static bool rButton(bool(proc)(), bool &flag)
{
   if (!proc())
      flag = false;

   else if (!flag)
   {
      flag = true;
      return true;
   }
   return false;
}

bool Up(void)
{
   return DUkey;
}
bool Down(void)
{
   return DDkey;
}
bool Left(void)
{
   return DLkey;
}
bool Right(void)
{
   return DRkey;
}
bool cAbtn(void)
{
   return Akey;
}
bool cBbtn(void)
{
   return Bkey;
}
bool cEbtn(void)
{
   return Ekey;
}
bool cSbtn(void)
{
   return Skey;
}
bool cLbtn(void)
{
   return Lkey;
}
bool cRbtn(void)
{
   return Rkey;
}
bool cMbtn(void)
{
   return Mkey;
}
bool cCbtn(void)
{
   return Ckey;
}


bool rUp(void)
{
   return rButton(Up, Udown);
}
bool rDown(void)
{
   return rButton(Down, Ddown);
}
bool rLeft(void)
{
   return rButton(Left, Ldown);
}
bool rRight(void)
{
   return rButton(Right, Rdown);
}
bool rAbtn(void)
{
   return rButton(cAbtn, Adown);
}
bool rBbtn(void)
{
   return rButton(cBbtn, Bdown);
}
bool rEbtn(void)
{
   return rButton(cEbtn, Edown);
}
bool rSbtn(void)
{
   return rButton(cSbtn, Sdown);
}
bool rLbtn(void)
{
   return rButton(cLbtn, LBdown);
}
bool rRbtn(void)
{
   return rButton(cRbtn, RBdown);
}
bool rMbtn(void)
{
   return rButton(cMbtn, Mdown);
}
bool rCbtn(void)
{
   return rButton(cCbtn, Cdown);
}

bool drunk(void)
{
   //  return ((!(frame%((rand()%100)+1)))&&(rand()%MAXDRUNKCLOCK<Link.DrunkClock()));
   return false;
}

bool DrunkUp(void)
{
   return drunk() ? (rand() % 2) ? 0 : !Up() : Up();
}
bool DrunkDown(void)
{
   return drunk() ? (rand() % 2) ? 0 : !Down() : Down();
}
bool DrunkLeft(void)
{
   return drunk() ? (rand() % 2) ? 0 : !Left() : Left();
}
bool DrunkRight(void)
{
   return drunk() ? (rand() % 2) ? 0 : !Right() : Right();
}
bool DrunkcBbtn(void)
{
   return drunk() ? (rand() % 2) ? 0 : !cBbtn() : cBbtn();
}

bool DrunkrAbtn(void)
{
   return drunk() ? (rand() % 2) ? 0 : !rAbtn() : rAbtn();
}
bool DrunkrBbtn(void)
{
   return drunk() ? (rand() % 2) ? 0 : !rBbtn() : rBbtn();
}

void eat_buttons(void)
{
   rAbtn();
   rBbtn();
   rSbtn();
   rEbtn();
   rLbtn();
   rRbtn();
   rMbtn();
   rCbtn();
}

char *time_str(uint32_t time)
{
   static char s[32];

   uint32_t secs = (time / 60) % 60;
   uint32_t mins = (time / 3600) % 60;
   uint32_t hours = time / 216000;

   sprintf(s, "%d:%02d:%02d", hours, mins, secs);
   return s;
}

int vbound(int x, int low, int high)
{
   if (x < low)
      x = low;
   if (x > high)
      x = high;
   return x;
}

float vbound(float x, float low, float high)
{
   if (x < low)
      x = low;
   if (x > high)
      x = high;
   return x;
}

char datapwd[8] = { char('l' + 11), char('o' + 22), char('n' + 33), 
                    char('g' + 44), char('t' + 55), char('a' + 66), 
                    char('n' + 77), char(0 + 88) };

void resolve_password(char *pwd)
{
   for (int i = 0; i < 8; i++)
      pwd[i] -= (i + 1) * 11;
}

void set_bit(uint8_t *bitstr, int bit, uint8_t val)
{
   bitstr += bit >> 3;
   uint8_t mask = 1 << (bit & 7);

   if (val)
      *bitstr |= mask;

   else
      *bitstr &= ~mask;
}

int get_bit(uint8_t *bitstr, int bit)
{
   bitstr += bit >> 3;
   return ((*bitstr) >> (bit & 7)) & 1;
}

int anim_3_4(int clk, int speed)
{
   clk /= speed;
   switch (clk & 3)
   {
      case 0:
      case 2:
         clk = 0;
         break;
      case 1:
         clk = 1;
         break;
      case 3:
         clk = 2;
         break;
   }
   return clk;
}

/**********  Encryption Stuff  *****************/

static unsigned int seed = 0;
static unsigned int enc_mask[3] = {0x4C358938, 0x91B2A2D1, 0x4A7C1B87};
static unsigned int pvalue[3] = {0x62E9, 0x7D14, 0x1A82};
static unsigned int qvalue[3] = {0x3619, 0xA26B, 0xF03C};

static int rand_007(int method)
{
   short BX = seed >> 8;
   short CX = (seed & 0xFF) << 8;
   char AL = seed >> 24;
   char C = AL >> 7;
   char D = BX >> 15;
   AL <<= 1;
   BX = (BX << 1) | C;
   CX = (CX << 1) | D;
   CX += seed & 0xFFFF;
   BX += (seed >> 16) + C;
   CX += pvalue[method];
   BX += qvalue[method] + D;
   seed = (BX << 16) + CX;
   return (CX << 16) + BX;
}

//
// RETURNS:
//   0 - OK
//   1 - srcfile not opened
//   2 - destfile not opened
//
int encode_file_007(const char *srcfile, const char *destfile, unsigned int key,
                    const char *header, int method)
{
   FILE *src, *dest;
   int tog = 0, c, r = 0;
   short c1 = 0, c2 = 0;

   seed = key;
   src = fopen(srcfile, "rb");
   if (!src)
      return 1;

   dest = fopen(destfile, "wb");
   if (!dest)
   {
      fclose(src);
      return 2;
   }

   // write the header
   if (header)
   {
      for (c = 0; header[c]; c++)
         fputc(header[c], dest);
   }

   // write the key, XORed with MASK
   key ^= enc_mask[method];
   fputc(key >> 24, dest);
   fputc((key >> 16) & 255, dest);
   fputc((key >> 8) & 255, dest);
   fputc(key & 255, dest);

   // encode the data
   while ((c = fgetc(src)) != EOF)
   {
      c1 += c;
      c2 = (c2 << 4) + (c2 >> 12) + c;
      if (tog)
         c += r;

      else
      {
         r = rand_007(method);
         c ^= r;
      }
      tog ^= 1;

      fputc(c, dest);
   }

   // write the checksums
   r = rand_007(method);
   c1 ^= r;
   c2 += r;
   fputc(c1 >> 8, dest);
   fputc(c1 & 255, dest);
   fputc(c2 >> 8, dest);
   fputc(c2 & 255, dest);

   fclose(src);
   fclose(dest);
   return 0;
}

//
// RETURNS:
//   0 - OK
//   1 - srcfile not opened
//   2 - destfile not opened
//   3 - scrfile too small
//   4 - srcfile EOF
//   5 - checksum mismatch
//   6 - header mismatch
//
int decode_file_007(const char *srcfile, const char *destfile,
                    const char *header, int method, bool packed)
{
   FILE *normal_src = NULL, *dest = NULL;
   PACKFILE *packed_src = NULL;
   int tog = 0, c, r = 0, err;
   long size, i;
   short c1 = 0, c2 = 0, check1, check2;

   // open files
   size = file_size(srcfile);
   
   if (size < 1)
      return 1;
   
   size -= 8;  // get actual data size, minus key and checksums
   
   if (size < 1)
      return 3;

   if (!packed)
   {
      normal_src = fopen(srcfile, "rb");
      if (!normal_src)
         return 1;
   }
   else
   {
      packed_src = pack_fopen(srcfile, F_READ_PACKED);
      if (!packed_src)
      {
         packed_src = pack_fopen(srcfile, F_READ);
         if (!packed_src)
            return 1;
      }
   }

   dest = fopen(destfile, "wb");
   if (!dest)
   {
      if (packed)
         pack_fclose(packed_src);

      else
         fclose(normal_src);
      return 2;
   }

   // read the header
   err = 4;
   if (header)
   {
      for (i = 0; header[i]; i++)
      {
         if (packed)
         {
            if ((c = pack_getc(packed_src)) == EOF)
               goto error;
         }
         else
         {
            if ((c = fgetc(normal_src)) == EOF)
               goto error;
         }
         if ((c & 255) != header[i])
         {
            err = 6;
            goto error;
         }
         --size;
      }
   }

   // read the key
   if (packed)
   {
      if ((c = pack_getc(packed_src)) == EOF)
         goto error;
   }
   else
   {
      if ((c = fgetc(normal_src)) == EOF)
         goto error;
   }
   seed = c << 24;
   if (packed)
   {
      if ((c = pack_getc(packed_src)) == EOF)
         goto error;
   }
   else
   {
      if ((c = fgetc(normal_src)) == EOF)
         goto error;
   }
   seed += (c & 255) << 16;
   if (packed)
   {
      if ((c = pack_getc(packed_src)) == EOF)
         goto error;
   }
   else
   {
      if ((c = fgetc(normal_src)) == EOF)
         goto error;
   }
   seed += (c & 255) << 8;
   if (packed)
   {
      if ((c = pack_getc(packed_src)) == EOF)
         goto error;
   }
   else
   {
      if ((c = fgetc(normal_src)) == EOF)
         goto error;
   }
   seed += c & 255;
   seed ^= enc_mask[method];

   // decode the data
   for (i = 0; i < size; i++)
   {
      if (packed)
      {
         if ((c = pack_getc(packed_src)) == EOF)
            goto error;
      }
      else
      {
         if ((c = fgetc(normal_src)) == EOF)
            goto error;
      }

      if (tog)
         c -= r;

      else
      {
         r = rand_007(method);
         c ^= r;
      }
      tog ^= 1;

      c &= 255;
      c1 += c;
      c2 = (c2 << 4) + (c2 >> 12) + c;

      fputc(c, dest);
   }

   // read checksums
   if (packed)
   {
      if ((c = pack_getc(packed_src)) == EOF)
         goto error;
   }
   else
   {
      if ((c = fgetc(normal_src)) == EOF)
         goto error;
   }
   check1 = c << 8;
   if (packed)
   {
      if ((c = pack_getc(packed_src)) == EOF)
         goto error;
   }
   else
   {
      if ((c = fgetc(normal_src)) == EOF)
         goto error;
   }
   check1 += c & 255;
   if (packed)
   {
      if ((c = pack_getc(packed_src)) == EOF)
         goto error;
   }
   else
   {
      if ((c = fgetc(normal_src)) == EOF)
         goto error;
   }
   check2 = c << 8;
   if (packed)
   {
      if ((c = pack_getc(packed_src)) == EOF)
         goto error;
   }
   else
   {
      if ((c = fgetc(normal_src)) == EOF)
         goto error;
   }
   check2 += c & 255;

   // verify checksums
   r = rand_007(method);
   check1 ^= r;
   check2 -= r;
   check1 &= 0xFFFF;
   check2 &= 0xFFFF;
   if (check1 != c1 || check2 != c2)
   {
      err = 5;
      goto error;
   }

   if (packed)
      pack_fclose(packed_src);

   else
      fclose(normal_src);
   fclose(dest);
   return 0;

error:
   if (packed)
      pack_fclose(packed_src);

   else
      fclose(normal_src);
   fclose(dest);
   delete_file(destfile);
   return err;
}

/*** end of zc_sys.cpp ***/
