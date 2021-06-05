#ifndef ALPORT_PALETTE_H
#define ALPORT_PALETTE_H

#ifdef __cplusplus
extern "C" {
#endif

#define PAL_SIZE 256


typedef struct RGB
{
   unsigned char b;
   unsigned char g;
   unsigned char r;
   unsigned char filler;
} RGB;


typedef RGB PALETTE[PAL_SIZE];


typedef struct
{
   unsigned char data[32][32][32];
} RGB_MAP;


typedef struct
{
   unsigned char data[PAL_SIZE][PAL_SIZE];
} COLOR_MAP;


extern RGB_MAP *rgb_map;
extern COLOR_MAP *color_map;


extern PALETTE black_palette;


int bestfit_color(const PALETTE pal, int r, int g, int b);
void create_rgb_table(RGB_MAP *table, const PALETTE pal);
void fade_interpolate(const PALETTE source, const PALETTE dest, PALETTE output,
                      int pos, int from, int to);

#ifdef __cplusplus
}
#endif

#endif          /* ifndef ALPORT_PALETTE_H */
