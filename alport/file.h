#ifndef ALPORT_FILE_H
#define ALPORT_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

int delete_file(const char *filename);
int file_exists(const char *filename);
size_t file_size(const char *filename);
char *get_filename(const char *path);
char *get_extension(const char *filename);
void put_backslash(char *filename);
char *replace_filename(char *dest, const char *path, const char *filename);
char *replace_extension(char *dest, const char *filename, const char *ext);

#ifdef __cplusplus
}
#endif

#endif          /* ifndef ALPORT_FILE_H */
