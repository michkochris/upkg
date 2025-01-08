/******************************************************************************
 *  Filename:    upkgconfig.c
 *  Author:      <michkochris@gmail.com>
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
/*file description: function for reading values from a  config file*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "upkglib.h"
#include "upkghash.h"
#include "upkgstruct.h"
#include "upkgconfig.h"

char *get_config_value(const char *filename, const char *key) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
    char line[64];
    char* value = NULL;
    while (fgets(line, sizeof(line), file) != NULL) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }
        // Split the line into key and value
        char* key_ptr = strtok(line, "=");
        char* value_ptr = strtok(NULL, "=");
        if (key_ptr != NULL && value_ptr != NULL) {
            // Trim whitespace from key and value
            key_ptr = strtok(key_ptr, " \t\n\r");
            value_ptr = strtok(value_ptr, " \t\n\r");

            if (strcmp(key_ptr, key) == 0) {
                value = strdup(value_ptr);
                break;
            }
        }
    }
    fclose(file);
    return value;
}
void check_upkgconfig() {
    char *configfile = "upkgconfig";
    char *upkg_dir = get_config_value(configfile, "upkg_dir");
    char *control_dir = get_config_value(configfile, "control_dir");
    char *unpack_dir = get_config_value(configfile, "unpack_dir");
    char *install_dir = get_config_value(configfile, "install_dir");

    if (access(configfile, F_OK) == 0) {
        printf("config=%s\n", configfile);
    } else {
        printf("upkgconfig File does not exist!\n");
    }
    if (upkg_dir != NULL) {
	create_dir(upkg_dir);
        printf("upkg_dir=%s\n", upkg_dir);
        free(upkg_dir);
    } else {
        printf("%s not found in config file!\n", upkg_dir);
    }
    if (control_dir != NULL) {
	remove_dir(control_dir);
	create_dir(control_dir);
        printf("control_dir=%s\n", control_dir);
        free(control_dir);
    } else {
        printf("%s not found in config file!\n", control_dir);
    }
    if (unpack_dir != NULL) {
	remove_dir(unpack_dir);
	create_dir(unpack_dir);
        printf("unpack_dir=%s\n", unpack_dir);
        free(unpack_dir);
    } else {
        printf("%s not found in config file!\n", unpack_dir);
    }
    if (install_dir != NULL) {
	create_dir(install_dir);
        printf("install_dir=%s\n\n", install_dir);
        free(install_dir);
    } else {
        printf("%s not found in config file!\n", install_dir);
    }
}
