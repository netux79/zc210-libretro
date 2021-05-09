#ifndef ALPORT_LZSS_H
#define ALPORT_LZSS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LZSS_PACK_DATA LZSS_PACK_DATA;
typedef struct LZSS_UNPACK_DATA LZSS_UNPACK_DATA;

LZSS_PACK_DATA *create_lzss_pack_data(void);
void free_lzss_pack_data(LZSS_PACK_DATA *dat);
int lzss_write(PACKFILE *file, LZSS_PACK_DATA *dat, int size,
               unsigned char *buf, int last);
LZSS_UNPACK_DATA *create_lzss_unpack_data(void);
void free_lzss_unpack_data(LZSS_UNPACK_DATA *dat);
int lzss_read(PACKFILE *file, LZSS_UNPACK_DATA *dat, int s, unsigned char *buf);
int _al_lzss_incomplete_state(const LZSS_UNPACK_DATA *dat);

#ifdef __cplusplus
}
#endif

#endif          /* ifndef ALPORT_LZSS_H */
