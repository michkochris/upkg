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

struct Controlinfo {
    char pkgname[32];
    char version[32];
    char maintainer[128];
    char homepage[64];
    char sources[128];
    char arch[32];
    char depends[128];
    char comment[128];
    char description[1028];
};

struct Controlinfo gatherinfo() {
    struct Controlinfo controlinfo;
    char *pkgname_search = search_file("installdir/control", "Package: ");
    if (pkgname_search != NULL) {
        if (strlen(pkgname_search) > 0) {
            rmstr(pkgname_search, "Package: ");
            strcpy(controlinfo.pkgname, pkgname_search);
            free(pkgname_search);
        } else {
            printf("error: pkgname field is not null but empty\n");
        }
    } else {
        printf("error: pkgname is null\n");
    }
    char *version_search = search_file("installdir/control", "Version: ");
    if (version_search != NULL) {
        if (strlen(version_search) > 0) {
            rmstr(version_search, "Version: ");
	    //sanitize_version(version_search);
            strcpy(controlinfo.version, version_search);
            free(version_search);
        } else {
            printf("error: version field is not null but empty\n");
        }
    } else {
        printf("error: version is null\n");
    }
    char *arch_search = search_file("installdir/control", "Architecture: ");
    if (arch_search != NULL) {
        if (strlen(arch_search) > 0) {
            rmstr(arch_search, "Architecture: ");
            strcpy(controlinfo.arch, arch_search);
            free(arch_search);
        } else {
            printf("error: arch field is not null but empty\n");
        }
    } else {
        printf("error: arch is null\n");
    }
    char *maintainer_search = search_file("installdir/control", "Maintainer: ");
    if (maintainer_search != NULL) {
        if (strlen(maintainer_search) > 0) {
            rmstr(maintainer_search, "Maintainer: ");
            strcpy(controlinfo.maintainer, maintainer_search);
            free(maintainer_search);
        } else {
            printf("error: maintainer field is not null but empty\n");
        }
    } else {
        printf("error: maintainer is null\n");
    }
    char *homepage_search = search_file("installdir/control", "Homepage: ");
    if (homepage_search != NULL) {
        if (strlen(homepage_search) > 0) {
            rmstr(homepage_search, "Homepage: ");
            strcpy(controlinfo.homepage, homepage_search);
            free(homepage_search);
        } else {
            printf("error: homepage field is not null but empty\n");
        }
    } else {
        printf("error: homepage is null\n");
    }
    char *sources_search = search_file("installdir/control", "Source: ");
    if (sources_search != NULL) {
        if (strlen(sources_search) > 0) {
            rmstr(sources_search, "Source: ");
            strcpy(controlinfo.sources, sources_search);
            free(sources_search);
        } else {
            printf("error: source field is not null but empty\n");
        }
    } else {
        printf("error: source is null\n");
    }
    char *depends_search = search_file("installdir/control", "Depends: ");
    if (depends_search != NULL) {
        if (strlen(depends_search) > 0) {
            rmstr(depends_search, "Depends: ");
            strcpy(controlinfo.depends, depends_search);
            free(depends_search);
        } else {
            printf("error: depends field is not null but empty\n");
        }
    } else {
        printf("error: depends is null\n");
    }
    char *comment_search = search_file("installdir/control", "Comment: ");
    if (comment_search != NULL) {
        if (strlen(comment_search) > 0) {
            rmstr(comment_search, "Comment: ");
            strcpy(controlinfo.comment, comment_search);
            free(comment_search);
        } else {
            printf("error: comment field is not null but empty\n");
        }
    } else {
        printf("error: comment is null\n");
    }
    char *description_search = searchAndReadToEnd("installdir/control", "Description: ");
    if (description_search != NULL) {
        if (strlen(description_search) > 0) {
            rmstr(description_search, "Description: ");
            strcpy(controlinfo.description, description_search);
            free(description_search);
        } else {
            printf("error: description field is not null but empty\n");
        }
    } else {
        printf("error: description is null\n");
    }
   return controlinfo;
}
// Function to print the strings in a StringGroup struct
void printpkginfo(struct Controlinfo controlinfo) {
    printf("\nprinting pkg struct:\n");
    printf("%s", controlinfo.pkgname);
    printf("%s", controlinfo.version);
    printf("%s", controlinfo.arch);
    printf("%s", controlinfo.maintainer);
    printf("%s", controlinfo.homepage);
    printf("%s", controlinfo.sources);
    printf("%s", controlinfo.depends);
    printf("%s", controlinfo.comment);
    printf("%s", controlinfo.description);
}

int main(int argc, char *argv[]) {
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
     extract_deb(argv[i], "installdir");
     extract_tar_xz("installdir/control.tar.xz", "installdir");
   } else {
     printf("Invalid option: %s\n", argv[i]);
     exit(1);
   }
}
struct Controlinfo info = gatherinfo();
printpkginfo(info);
testhash();
return 0;
}
