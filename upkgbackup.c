/*
author: michkochris
email: michkochris@gmail.com
date: started 12-31-2024
license: GPLV3
notice: This program is free software:
you can redistribute it and/or modify it
under the terms of the GNU General Public License.
Only the name of the program is copyrighted...
If you reuse code, please give credits...
file description:
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME	"upkg"
#define VERSION	"1.0"
#define RED	"\x1b[31m"
#define GREEN	"\x1b[32m"
#define YELLOW	"\x1b[33m"
#define BLUE	"\x1b[34m"
#define MAGENTA	"\x1b[35m"
#define CYAN	"\x1b[36m"
#define WHITE	"\x1b[0;37m"
#define RESET	"\x1b[0m"

void badmsg(char *text) {
printf(YELLOW "==> " WHITE "%s\n" RESET, text);
}
void errormsg(char *text) {
printf(RED "error: " WHITE "%s\n" RESET, text);
}
void goodmsg(char *text) {
printf(GREEN "==> " WHITE "%s\n" RESET, text);
}
void usermsg(char *text) {
printf(CYAN NAME ": " WHITE "%s\n" RESET, text);
}
void success(char *text) {
printf(MAGENTA "==> " WHITE "%s\n" RESET, text);
}

void usage() {
printf("Usage: " NAME " [option] input_file.deb \n");
printf("Options:\n");
printf("  -v  --version Display version info\n");
printf("  -h  --help    Display help messages\n");
}
void helpmsg() {
printf("\n");
printf("Report bugs directly to: michkochris@gmail.com\n");
printf("Or file a bug report on github... \n");
printf("upkg (ulinux) home page: <https://www.ulinux.com>\n");
printf("upkg github page <github/upkg>\n");
printf("ulinux github page <github/ulinux>\n");
printf("General help using upkg and ulinux: <facebook.group>\n");
}
void shortversion() {
printf("upkg (ulinux) 1.0\n");
}
void versionmsg() {
printf("\n");
printf("Copyright (C) 2007 Free Software Foundation, Inc.\n");
printf("License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>\n");
printf("This is free software: you are free to change and redistribute it.\n");
printf("There is NO WARRANTY, to the extent permitted by law.\n");
}
int main(int argc, char *argv[]) {
badmsg("hello error!");
errormsg("hello error!");
goodmsg("hello error!");
usermsg("hello error!");
success("hello error!");

if (argc < 2) {
   usage();helpmsg();exit(1);
   }
for (int i = 1; i < argc; i++) {
char *filename = argv[i];
char *extension = strrchr(filename, '.');
if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
     usage();helpmsg();exit(1);
   } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
     shortversion();versionmsg();exit(1);
   } else if (extension != NULL && strcmp(extension, ".deb") == 0) {
     printf("print found .deb:\n%s\n", argv[i]);
   } else {
     printf("Invalid option: %s\n", argv[i]);
     exit(1);
   }
}
return 0;
}
