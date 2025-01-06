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

struct Pkginfo gatherinfo() {
    struct Pkginfo controlinfo;
    char *pkgname_search = search_file("installdir/control", "Package: ");
    if (pkgname_search != NULL) {
	rmstr(pkgname_search, "Package: ");
	remove_white(pkgname_search);
	strcpy(controlinfo.pkgname, pkgname_search);
	free(pkgname_search);
        if (strlen(pkgname_search) == 0) {
            printf("error: Package: field is empty!\n");
        }
    } else {
        printf("error: Package: is not present!\n");
    }
    char *version_search = search_file("installdir/control", "Version: ");
    if (version_search != NULL) {
	rmstr(version_search, "Version: ");
	remove_white(version_search);
	strcpy(controlinfo.version, version_search);
	free(version_search);
        if (strlen(version_search) == 0) {
            printf("error: Version: field is empty!\n");
        }
    } else {
        printf("error: Version: is not present!\n");
    }
    char *arch_search = search_file("installdir/control", "Architecture: ");
    if (arch_search != NULL) {
	rmstr(arch_search, "Architecture: ");
	remove_white(arch_search);
	strcpy(controlinfo.arch, arch_search);
	free(arch_search);
        if (strlen(arch_search) == 0) {
            printf("error: Architecture: field is empty!\n");
        }
    } else {
        printf("error: Architecture: is not present!\n");
    }
    char *maintainer_search = search_file("installdir/control", "Maintainer: ");
    if (maintainer_search != NULL) {
	rmstr(maintainer_search, "Maintainer: ");
	remove_white(maintainer_search);
	strcpy(controlinfo.maintainer, maintainer_search);
	free(maintainer_search);
        if (strlen(maintainer_search) == 0) {
            printf("error: Maintainer: field is empty!\n");
        }
    } else {
        printf("error: Maintainer: is not present!\n");
    }
    char *homepage_search = search_file("installdir/control", "Homepage: ");
    if (homepage_search != NULL) {
	rmstr(homepage_search, "Homepage: ");
	strcpy(controlinfo.homepage, homepage_search);
	free(homepage_search);
        if (strlen(homepage_search) == 0) {
            printf("error: Homepage: field is empty!\n");
        }
    } else {
        printf("error: Homepage: is not present!\n");
    }
    char *sources_search = search_file("installdir/control", "Source: ");
    if (sources_search != NULL) {
	rmstr(sources_search, "Source: ");
	strcpy(controlinfo.sources, sources_search);
	free(sources_search);
        if (strlen(sources_search) == 0) {
            printf("error: Source: field is empty!\n");
        }
    } else {
        printf("error: Source: is not present!\n");
    }
    char *section_search = search_file("installdir/control", "Section: ");
    if (section_search != NULL) {
        rmstr(section_search, "Section: ");
        strcpy(controlinfo.section, section_search);
        free(section_search);
        if (strlen(section_search) == 0) {
            printf("warning: Section: field is empty!\n");
        }
    } else {
        printf("warning: Section: is not present!\n");
    }
    char *priority_search = search_file("installdir/control", "Priority: ");
    if (priority_search != NULL) {
        rmstr(priority_search, "Priority: ");
        strcpy(controlinfo.priority, priority_search);
        free(priority_search);
        if (strlen(priority_search) == 0) {
            printf("warning: Priority: field is empty!\n");
        }
    } else {
        printf("warning: Priority: is not present!\n");
    }
    char *depends_search = search_file("installdir/control", "Depends: ");
    if (depends_search != NULL) {
	rmstr(depends_search, "Depends: ");
	strcpy(controlinfo.depends, depends_search);
	free(depends_search);
        if (strlen(depends_search) == 0) {
            printf("warning: Depends: field is empty!\n");
        }
    } else {
        printf("warning: Depends: is not present!\n");
    }
    char *comment_search = search_file("installdir/control", "Comment: ");
    if (comment_search != NULL) {
	rmstr(comment_search, "Comment: ");
	strcpy(controlinfo.comment, comment_search);
	free(comment_search);
    }
    char *description_search = searchandreadtoend("installdir/control", "Description: ");
    if (description_search != NULL) {
	rmstr(description_search, "Description: ");
	strcpy(controlinfo.description, description_search);
	free(description_search);
        if (strlen(description_search) == 0) {
            printf("error: Description field is empty!\n");
        }
    } else {
        printf("error: Description: is not present!\n");
    }
   return controlinfo;
}

void printpkginfo(struct Pkginfo controlinfo) {
    printf("\nprinting pkg struct:\n");
    if (strlen(controlinfo.pkgname) > 0) {
    printf("Package: %s\n", controlinfo.pkgname);}
    if (strlen(controlinfo.version) > 0) {
    printf("Version: %s\n", controlinfo.version);}
    if (strlen(controlinfo.arch) > 0) {
    printf("Architecture: %s\n", controlinfo.arch);}
    if (strlen(controlinfo.maintainer) > 0) {
    printf("Maintainer: %s\n", controlinfo.maintainer);}
    if (strlen(controlinfo.homepage) > 0) {
    printf("Homepage: %s", controlinfo.homepage);}
    if (strlen(controlinfo.sources) > 0) {
    printf("Source: %s", controlinfo.sources);}
    if (strlen(controlinfo.section) > 0) {
    printf("Section: %s", controlinfo.section);}
    if (strlen(controlinfo.priority) > 0) {
    printf("Priority: %s", controlinfo.priority);}
    if (strlen(controlinfo.depends) > 0) {
    printf("Depends: %s\n", controlinfo.depends);}
    if (strlen(controlinfo.comment) > 0) {
    printf("Comment: %s\n", controlinfo.comment);}
    if (strlen(controlinfo.description) > 0) {
    printf("Description: %s\n", controlinfo.description);}
    
}

