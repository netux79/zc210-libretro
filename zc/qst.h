//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  qst.h
//
//  Code for loading '.qst' files in ZC and ZQuest.
//
//--------------------------------------------------------

#ifndef _ZC_QST_H_
#define _ZC_QST_H_

#include "zdefs.h"

#define checkstatus(ret)\
   switch (ret) \
   { \
   case 0:\
      break;\
   case qe_invalid:\
      goto invalid;\
      break;\
   default:\
      pack_fclose(f);\
      if(!oldquest)\
         delete_file(tmpfilename);\
      return ret;\
      break;\
   }

extern bool bad_version(int ver);

enum
{
   qe_OK, qe_notfound, qe_invalid, qe_version, qe_obsolete,
   qe_missing, qe_internal, qe_pwd, qe_match, qe_minver,
   qe_nomem, qe_debug
};

enum
{
   zgp_tiles, zgp_combos, zgp_pals, zgp_items, zgp_wpns, zgp_misc,
   zgp_maps, zgp_doors
};

extern const char *qst_error[];
extern word msg_count;
extern word door_combo_set_count;

char *VerStr(int version);

void clear_combo(int i);
void clear_combos();
void pack_combos();
void fix_maps(mapscr *buf, int cnt);
void reset_midi(music *m);
void reset_midis(music *m);
void reset_scr(int scr);
void get_qst_buffers();
void free_qst_buffers();
int count_dmaps();
int count_shops(miscQdata *misc);
int count_infos(miscQdata *misc);
int count_warprings(miscQdata *misc);
int count_palcycles(miscQdata *misc);
int count_windwarps(miscQdata *misc);
int loadquest(char *filename, zquestheader *Header, miscQdata *Misc,
              music *midis);

char *byte_conversion(int number1, int number2, int format1, int format2);

int readheader(PACKFILE *f, zquestheader *header, bool keepdata);
int readrules(PACKFILE *f, zquestheader *header, bool keepdata);
int readstrings(PACKFILE *f, zquestheader *header, bool keepdata);
int readdoorcombosets(PACKFILE *f, zquestheader *header, bool keepdata);
int readdmaps(PACKFILE *f, zquestheader *header, word start_dmap,
              word max_dmaps, bool keepdata);
int readmisc(PACKFILE *f, zquestheader *header, miscQdata *misc, bool keepdata);
int readitems(PACKFILE *f, word version, word build, bool keepdata);
int readweapons(PACKFILE *f, zquestheader *header, bool keepdata);
int readguys(PACKFILE *f, zquestheader *header, bool keepdata);
int readmapscreen(PACKFILE *f, zquestheader *header, mapscr *temp_mapscr);
int readmaps(PACKFILE *f, zquestheader *header, bool keepdata);
int readcombos(PACKFILE *f, zquestheader *header, word version, word build,
               word start_combo, word max_combos, bool keepdata);
int readcolordata(PACKFILE *f, miscQdata *misc, word version, word build,
                  bool keepdata);
int readtiles(PACKFILE *f, byte *buf, zquestheader *header, word version,
              word build, word start_tile, word max_tiles, bool from_init, bool keepdata);
int readmidis(PACKFILE *f, zquestheader *header, music *midis, bool keepdata);
int readcheatcodes(PACKFILE *f, zquestheader *header, bool keepdata);
int readinitdata(PACKFILE *f, zquestheader *header, bool keepdata);
#endif                                                      // _ZC_QST_H_
