/******************************************************************************
 *  Filename:    upkglib.c
 *  Author:      <michkochris@gmail.com>
 *  Date:        started 12-31-2024
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
/*file description: file with misc functions to help upkg*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "upkglib.h"
#include "upkghash.h"
#include "upkgstruct.h"
#include "upkgconfig.h"

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
void medusa() {
badmsg("hello error!");
errormsg("hello error!");
goodmsg("hello error!");
usermsg("hello error!");
success("hello error!");
}
void usage_info() {
printf("Usage: " NAME " [option] input_file.deb \n");
printf("Options:\n");
printf("  -v  --version Display version info\n");
printf("  -h  --help    Display help messages\n");
}
void help_msg() {
printf("\n\n");
printf("Report bugs directly to: michkochris@gmail.com\n");
printf("Or file a bug report on github... \n");
printf("upkg (ulinux) home page: <https://www.ulinux.com>\n");
printf("upkg github page <github/upkg>\n");
printf("ulinux github page <github/ulinux>\n");
printf("General help using upkg and ulinux: <facebook.group>\n");
}
void version_info() {
printf("upkg (ulinux) 1.0\n");
}
void license_info() {
printf("\n");
printf("Copyright (C) 2025 upkg (ulinux) Christoper Michko\n");
printf("License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>\n");
printf("This is free software: you are free to change and redistribute it.\n");
printf("There is NO WARRANTY, to the extent permitted by law.\n");
}
int create_dir(const char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        if (errno == ENOENT) {
            if (mkdir(path, 0755) == -1) {
                perror("mkdir failed");
                return -1;
            }
        } else {
            perror("stat failed");
            return -1;
        }
    } else if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "%s exists, but is not a directory\n", path);
        return -1;
    }
    return 0;
}
int remove_dir(const char *destruct_dir) {
    if (rmdir(destruct_dir) == 0) {
        return 0;
    }
    char command[256];
    snprintf(command, sizeof(command), "rm -rf %s", destruct_dir);
    return system(command);
}
char *concat_path(const char *dir, const char *filename) {
    // Calculate the length of the resulting path
    size_t dir_len = strlen(dir);
    size_t filename_len = strlen(filename);
    size_t total_len = dir_len + filename_len + 2; // +2 for '/' and '\0'
    // Allocate memory for the resulting path
    char* result = (char*)malloc(total_len);
    if (result == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }
    // Concatenate the directory, separator, and filename
    strcpy(result, dir);
    //strcat(result, "/");
    strcat(result, filename);
    return result;
}
void extract_deb(const char *deb_file, const char *dest_dir) {
    if (remove_dir("upkgdir/staging") == 0) {
        //printf("Directory removed successfully\n");
    } else {
        printf("Failed to clean staging directory!\n");
	exit(1);
    }
    struct stat st = {0};
    if (stat(dest_dir, &st) == -1) {
        if (mkdir(dest_dir, 0755) == -1) {
            printf("Failed to create staging directory!");
        }
    }
    char command[256];
    snprintf(command, sizeof(command), "ar -x --output %s %s", dest_dir, deb_file);
    int result = system(command);
    if (result == 0) {
        //printf("unpacking %s\n", deb_file);
    } else {
        fprintf(stderr, "Error unpacking %s!\n", deb_file);
    }
}
void extract_tar_xz(const char *tarxz,const char *tdest) {
    char command[256];
    snprintf(command, sizeof(command), "tar -xf %s -C %s", tarxz, tdest);
    int result = system(command);
    if (result != 0) {
    fprintf(stderr, "Error extracting %s!\n", tarxz);
    exit(1);
    }
}
char *search_file(const char *control, const char *str_str) {
    FILE* file = fopen(control, "r");
    if (file == NULL) {
        printf("Error opening file: %s!\n", control);
        return NULL;
    }
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, file)) != -1) {
        if (strstr(line, str_str) != NULL) {
            fclose(file);
            return line;
        }
    }
    fclose(file);
    free(line);
    return NULL;
}
char *searchandreadtoend(const char* filename, const char* searchString) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s!\n", filename);
        return NULL;
    }
    char* result = NULL;
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (found) {
            result = realloc(result, strlen(result) + strlen(line) + 1); 
            strcat(result, line);
        } else if (strstr(line, searchString) != NULL) {
            found = 1;
            result = malloc(strlen(line) + 1); 
            strcpy(result, line);
        }
    }
    fclose(file);
    return result;
}
void rmstr(char *str, char *sub) {
    char *result = NULL;
    char *temp = (char *)str;
    int len_sub = strlen(sub);
    while ((temp = strstr(temp, sub)) != NULL) {
        int len_front = temp - str;
        result = (char *)malloc(strlen(str) - len_sub + 1);
        strncpy(result, str, len_front);
        strcat(result, temp + len_sub);
        strcpy(str, result);
        temp = result + len_front;
    }
}
void remove_white(char *str) {
    int i, j = 0;
    for (i = 0; str[i]; i++) {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0'; // Null-terminate the modified string
}

// end of file
