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
#include <sys/stat.h>

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
void testmsg() {
badmsg("hello error!");
errormsg("hello error!");
goodmsg("hello error!");
usermsg("hello error!");
success("hello error!");
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
int remove_dir(const char *destruct_dir) {
    if (rmdir(destruct_dir) == 0) {
        return 0;
    }
    char command[256];
    snprintf(command, sizeof(command), "rm -rf %s", destruct_dir);
    return system(command);
}
void extract_deb(const char *deb_file, const char *dest_dir) {
    if (remove_dir("installdir") == 0) {
        printf("Directory removed successfully\n");
    } else {
        printf("Failed to remove directory\n");
    }
    struct stat st = {0};
    if (stat(dest_dir, &st) == -1) {
        // Directory doesn't exist, create it with default permissions (0755)
        if (mkdir(dest_dir, 0755) == -1) {
            perror("mkdir failed");
        }
    }
    char command[256];
    // Construct the command to extract the .deb file using 'ar'
    snprintf(command, sizeof(command), "ar -x --output %s %s", dest_dir, deb_file);
    // Execute the command
    int result = system(command);
    if (result == 0) {
        printf("Successfully extracted %s\n", deb_file);
    } else {
        fprintf(stderr, "Error extracting %s\n", deb_file);
    }
}
void extract_tar_xz(const char *tarxz,const char *tdest) {
    char command[256];
    snprintf(command, sizeof(command), "tar -xf %s -C %s", tarxz, tdest);
    int result = system(command);
    if (result != 0) {
    fprintf(stderr, "Error extracting %s\n", tarxz);
    exit(1);
    }
}
char *search_file(const char *control, const char *str_str) {
    FILE* file = fopen(control, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", control);
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
char* searchAndReadToEnd(const char* filename, const char* searchString) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
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

// end of file
