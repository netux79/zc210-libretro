#ifndef ALPORT_BITMAP_H
#define ALPORT_BITMAP_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct BITMAP            /* a bitmap structure */
{
   void *dat;                    /* the memory we allocated for the bitmap */
   unsigned char **line;
   int w, h;                     /* width and height in pixels */
   int clip;                     /* flag if clipping is turned on */
   int cl, cr, ct, cb;           /* clip left, right, top and bottom values */
} BITMAP;


BITMAP *create_bitmap(int width, int height);
BITMAP *create_sub_bitmap(BITMAP *parent, int x, int y, int width, int height);
void destroy_bitmap(BITMAP *bitmap);
void clear_to_color(BITMAP *bitmap, int color);
void clear_bitmap(BITMAP *bitmap);
void blit(BITMAP *src, BITMAP *dst, int sx, int sy, int dx, int dy, int w,
          int h);
void masked_blit(BITMAP *src, BITMAP *dst, int sx, int sy, int dx, int dy,
                 int w, int h);
void draw_sprite(BITMAP *bmp, BITMAP *sprite, int dx, int dy);
void draw_sprite_v_flip(BITMAP *bmp, BITMAP *sprite, int dx, int dy);
void set_clip_rect(BITMAP *bitmap, int x1, int y1, int x2, int y2);
void stretch_blit(BITMAP *src, BITMAP *dst, int sx, int sy, int sw, int sh,
                  int dx, int dy, int dw, int dh);

#ifdef __cplusplus
}
#endif

#endif          /* ifndef ALPORT_BITMAP_H */
