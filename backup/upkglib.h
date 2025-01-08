#ifndef UPKGLIB_H
#define UPKGLIB_H
#define NAME    "upkg"
#define VERSION "1.0"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[0;37m"
#define RESET   "\x1b[0m"
void badmsg(char *text);
void errormsg(char *text);
void goodmsg(char *text);
void usermsg(char *text);
void success(char *text);
void medusa();
void usage();
void helpmsg();
void shortversion();
void longversion();
int create_dir(const char *path);
int remove_dir(const char *destruct_dir);
char *concat_path(const char *dir, const char *filename);
void extract_deb(const char *deb_file, const char *dest_dir);
void extract_tar_xz(const char *tarxz,const char *tdest);
char *search_file(const char *control, const char *str_str);
char *searchandreadtoend(const char* filename, const char* searchString);
void rmstr(char *str, char *sub);
void remove_white(char *str);
#endif
