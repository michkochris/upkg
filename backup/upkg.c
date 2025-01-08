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
#include "upkghash.h"
#include "upkgstruct.h"
#include "upkgconfig.h"

void process_upkg(char *deb_file) {
    char *configfile = "upkgconfig";
    char *upkg_dir = get_config_value(configfile, "upkg_dir");
    char *control_dir = get_config_value(configfile, "control_dir");
    char *unpack_dir = get_config_value(configfile, "unpack_dir");
    char *install_dir = get_config_value(configfile, "install_dir");
    char *controltar = concat_path(control_dir, "control.tar.xz");
    //printf("controltar=%s\n", controltar);
    extract_deb(deb_file, control_dir);
    extract_tar_xz(controltar, control_dir);
}

int main(int argc, char *argv[]) {
check_upkgconfig();
if (argc < 2) {usage();helpmsg();exit(1);}
for (int i = 1; i < argc; i++) {
char *filename = argv[i];
char *extension = strrchr(filename, '.');
if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
     usage();helpmsg();exit(1);
   } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
     shortversion();longversion();exit(1);
   } else if (extension != NULL && strcmp(extension, ".deb") == 0) {
     printf("processing %s\n\n", argv[i]);
     process_upkg(argv[i]);
   } else {
     printf("Invalid option: %s\n", argv[i]);
     exit(1);
   }
}

testhash();
return 0;
}
