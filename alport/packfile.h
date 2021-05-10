#ifndef ALPORT_PACKFILE_H
#define ALPORT_PACKFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EOF
#define EOF    (-1)
#endif

#define F_READ          "r"
#define F_WRITE         "w"
#define F_READ_PACKED   "rp"
#define F_WRITE_PACKED  "wp"
#define F_WRITE_NOPACK  "w!"

#define F_BUF_SIZE      4096           /* 4K buffer for caching data */
#define F_PACK_MAGIC    0x736C6821L    /* magic number for packed files */
#define F_NOPACK_MAGIC  0x736C682EL    /* magic number for autodetect */
#define F_EXE_MAGIC     0x736C682BL    /* magic number for appended data */

#define PACKFILE_FLAG_WRITE      1     /* the file is being written */
#define PACKFILE_FLAG_PACK       2     /* data is compressed */
#define PACKFILE_FLAG_CHUNK      4     /* file is a sub-chunk */
#define PACKFILE_FLAG_EOF        8     /* reached the end-of-file */
#define PACKFILE_FLAG_ERROR      16    /* an error has occurred */
#define PACKFILE_FLAG_OLD_CRYPT  32    /* backward compatibility mode */
#define PACKFILE_FLAG_EXEDAT     64    /* reading from our executable */


typedef struct PACKFILE_VTABLE PACKFILE_VTABLE;
typedef struct PACKFILE PACKFILE;

struct LZSS_PACK_DATA;
struct LZSS_UNPACK_DATA;

struct _al_normal_packfile_details
{
   int hndl;                              /* DOS file handle */
   int flags;                             /* PACKFILE_FLAG_* constants */
   unsigned char *buf_pos;                /* position in buffer */
   int buf_size;                          /* # of bytes in the buffer */
   long todo;                             /* # of bytes still on the disk */
   struct PACKFILE *parent;               /* nested, parent file */
   struct LZSS_PACK_DATA *pack_data;      /* for LZSS compression */
   struct LZSS_UNPACK_DATA *unpack_data;  /* for LZSS decompression */
   char *filename;                        /* name of the file */
   char *passdata;                        /* encryption key data */
   char *passpos;                         /* current key position */
   unsigned char buf[F_BUF_SIZE];         /* the actual data buffer */
};

struct PACKFILE
{
   const PACKFILE_VTABLE *vtable;
   void *userdata;
   int is_normal_packfile;

   /* The following is only to be used for the "normal" PACKFILE vtable,
    * i.e. what is implemented by Allegro itself. If is_normal_packfile is
    * false then the following is not even allocated. This must be the last
    * member in the structure.
    */
   struct _al_normal_packfile_details normal;
};

struct PACKFILE_VTABLE
{
   int (*pf_fclose)(void *userdata);
   int (*pf_getc)(void *userdata);
   int (*pf_ungetc)(int c, void *userdata);
   long (*pf_fread)(void *p, long n, void *userdata);
   int (*pf_putc)(int c, void *userdata);
   long (*pf_fwrite)(const void *p, long n, void *userdata);
   int (*pf_fseek)(void *userdata, int offset);
   int (*pf_feof)(void *userdata);
   int (*pf_ferror)(void *userdata);
};


void packfile_password(const char *password);
PACKFILE *pack_fopen(const char *filename, const char *mode);
int pack_fclose(PACKFILE *f);
int pack_fseek(PACKFILE *f, int offset);
PACKFILE *pack_fopen_chunk(PACKFILE *f, int pack);
PACKFILE *pack_fclose_chunk(PACKFILE *f);
int pack_getc(PACKFILE *f);
int pack_putc(int c, PACKFILE *f);
int pack_feof(PACKFILE *f);
int pack_ferror(PACKFILE *f);
int pack_igetw(PACKFILE *f);
long pack_igetl(PACKFILE *f);
int pack_iputw(int w, PACKFILE *f);
long pack_iputl(long l, PACKFILE *f);
int pack_mgetw(PACKFILE *f);
long pack_mgetl(PACKFILE *f);
int pack_mputw(int w, PACKFILE *f);
long pack_mputl(long l, PACKFILE *f);
long pack_fread(void *p, long n, PACKFILE *f);
long pack_fwrite(const void *p, long n, PACKFILE *f);
int pack_ungetc(int c, PACKFILE *f);
char *pack_fgets(char *p, int max, PACKFILE *f);
int pack_fputs(const char *p, PACKFILE *f);
void *pack_get_userdata(PACKFILE *f);


#ifdef __cplusplus
}
#endif

#endif          /* ifndef ALPORT_PACKFILE_H */
