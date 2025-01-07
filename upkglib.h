#ifndef UPKGLIB_H
#define UPKGLIB_H
void badmsg(char *text);
void errormsg(char *text);
void goodmsg(char *text);
void usermsg(char *text);
void success(char *text);
void testmsg();
void usage();
void helpmsg();
void shortversion();
void longversion();
int remove_dir(const char *destruct_dir);
void extract_deb(const char *deb_file, const char *dest_dir);
void extract_tar_xz(const char *tarxz,const char *tdest);
char *search_file(const char *control, const char *str_str);
char *searchandreadtoend(const char* filename, const char* searchString);
void rmstr(char *str, char *sub);
void remove_white(char *str);
#endif
