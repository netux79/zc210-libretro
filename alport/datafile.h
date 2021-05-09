#ifndef ALPORT_DATAFILE_H
#define ALPORT_DATAFILE_H

#ifdef __cplusplus
extern "C" {
#endif

struct PACKFILE;

#define DAT_ID(a,b,c,d)    (((a)<<24) | ((b)<<16) | ((c)<<8) | (d))

#define DAT_MAGIC       DAT_ID('A','L','L','.')
#define DAT_FILE        DAT_ID('F','I','L','E')
#define DAT_PALETTE     DAT_ID('P','A','L',' ')
#define DAT_SAMPLE      DAT_ID('S','A','M','P')
#define DAT_MIDI        DAT_ID('M','I','D','I')
#define DAT_FONT        DAT_ID('F','O','N','T')
#define DAT_PROPERTY    DAT_ID('p','r','o','p')
#define DAT_NAME        DAT_ID('N','A','M','E')
#define DAT_END         -1

#define MAX_DATAFILE_TYPES    32

/* information about a datafile object */
typedef struct DATAFILE_TYPE
{
   int type;
   void *(*load)(PACKFILE *f, long size);
   void (*destroy)(void *);
} DATAFILE_TYPE;


typedef struct DATAFILE_PROPERTY
{
   char *dat;                          /* pointer to the data */
   int type;                           /* property type */
} DATAFILE_PROPERTY;


typedef struct DATAFILE
{
   void *dat;                          /* pointer to the data */
   int type;                           /* object type */
   long size;                          /* size of the object */
   DATAFILE_PROPERTY *prop;            /* object properties */
} DATAFILE;


typedef struct DATAFILE_INDEX
{
   char *filename;                     /* datafile name (path) */
   long *offset;                       /* list of offsets */
} DATAFILE_INDEX;


extern int _packfile_type;


DATAFILE *load_datafile(const char *filename);
DATAFILE *load_datafile_callback(const char *filename,
                                 void (*callback)(DATAFILE *));
DATAFILE_INDEX *create_datafile_index(const char *filename);
void unload_datafile(DATAFILE *dat);
void destroy_datafile_index(DATAFILE_INDEX *index);
void *read_block(PACKFILE *f, int size, int alloc_size);

DATAFILE *load_datafile_object(const char *filename, const char *objectname);
DATAFILE *load_datafile_object_indexed(const DATAFILE_INDEX *index, int item);
void unload_datafile_object(DATAFILE *dat);

DATAFILE *find_datafile_object(const DATAFILE *dat, const char *objectname);
const char *get_datafile_property(const DATAFILE *dat, int type);
void register_datafile_object(int id, void *(*load)(PACKFILE *f, long size),
                              void (*destroy)(void *data));


#ifdef __cplusplus
}
#endif

#endif          /* ifndef ALPORT_DATAFILE_H */
