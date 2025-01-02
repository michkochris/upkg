#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

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
        printf("Directory removed successfully (or did not exist)\n");
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

char *rmstr(char *str, char *sub) {
    char *result = NULL;
    char *temp = (char *)str;
    int len_sub = strlen(sub);
    if (len_sub == 0) {
        return strdup(str);
    }
    while ((temp = strstr(temp, sub)) != NULL) {
        int len_front = temp - str;
        result = (char *)malloc(strlen(str) - len_sub + 1);
        if (result == NULL) {
            return NULL; // Memory allocation failed
        }
        strncpy(result, str, len_front);
        strcat(result, temp + len_sub);
        strcpy(str, result); // Update the original string for next iteration
        temp = result + len_front;
    }
    if (result == NULL) {
        return strdup(str); // No substring found
    }
    return result;
}

int main(int argc, char *argv[]) {
if (argc != 2) {
    fprintf(stderr, "Usage: %s <deb_file>\n", argv[0]);
    return 1;
    }
    
    extract_deb(argv[1], "installdir");
    extract_tar_xz("installdir/control.tar.xz", "installdir");
    char *pkg = search_file("installdir/control", "Package: ");
    if (pkg != NULL) {
        printf("\n%s\n", pkg);
        //free(pkg);
    } else {
        printf("String not found.\n");
    }
    char *new_str = rmstr(pkg, "Package: ");
    if (new_str == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    printf("New string: \n%s\n", new_str);
    free(new_str);
    return 0;
}
