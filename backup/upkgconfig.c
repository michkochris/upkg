/*
author: michkochris
email: michkochris@gmail.com
date: started 12-31-2024
license: GPLV3
notice: This program is free software:
you can redistribute it and/or modify it
under the terms of the GNU General Public Lic>
Only the name of the program is copyrighted...
If you reuse code, please give credits...
file description:
*/
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
