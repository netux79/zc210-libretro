#ifndef ALPORT_PRIMITIVE_H
#define ALPORT_PRIMITIVE_H

#ifdef __cplusplus
extern "C" {
#endif

void putpixel(BITMAP *bmp, int x, int y, int color);
void hline(BITMAP *bmp, int x1, int y, int x2, int color);
void vline(BITMAP *bmp, int x, int y1, int y2, int color);
void line(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
void rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
void circle(BITMAP *bmp, int x, int y, int radius, int color);
void circlefill(BITMAP *bmp, int x, int y, int radius, int color);

#ifdef __cplusplus
}
#endif

#endif          /* ifndef ALPORT_PRIMITIVE_H */
