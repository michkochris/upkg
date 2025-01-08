/******************************************************************************
 *  Filename:    upkg.c
 *  Author:      michkochris@gmail.com
 *  Date:        started0 12-31-2024
 *  Description: upkg manages linux .deb pkg's
 *
 *  Copyright (c) 2025 upkg (ulinux) All rights reserved.
 *  GPLV3
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation, either version 3 of the License, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program. If not, see <https://www.gnu.org/licenses/>.
 ******************************************************************************/
/*file description: main c file for managing intc argv*/

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
