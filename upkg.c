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
#include <unistd.h>
#include <sys/stat.h>

#include "upkglib.h"

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
