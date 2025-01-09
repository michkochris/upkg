/******************************************************************************
 *  Filename:    upkgstruct.c
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
/*file description: file to gather pkg info into a struct from a .deb control file*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "upkglib.h"
#include "upkghash.h"
#include "upkgstruct.h"
#include "upkgconfig.h"

struct Pkginfo gatherinfo() {
    struct Pkginfo controlinfo;
    char *configfile = "upkgconfig";
    char *control_dir = get_config_value(configfile, "control_dir");
    char *controlfile = concat_path(control_dir, "control");
    //printf("controltar=%s\n", controlfile);
    char *pkgname_search = search_file(controlfile, "Package: ");
    if (pkgname_search != NULL) {
	rmstr(pkgname_search, "Package: ");
	remove_white(pkgname_search);
	strcpy(controlinfo.pkgname, pkgname_search);
	free(pkgname_search);
        if (strlen(pkgname_search) == 0) {
            printf("Package: field is found but is empty!\n");
        }
    } else {
        printf("Package: is mandatory!\n");
    }
    char *version_search = search_file(controlfile, "Version: ");
    if (version_search != NULL) {
	rmstr(version_search, "Version: ");
	remove_white(version_search);
	strcpy(controlinfo.version, version_search);
	free(version_search);
        if (strlen(version_search) == 0) {
            printf("Version: field is found but is empty!\n");
        }
    } else {
        printf("Version: is mandatory!\n");
    }
    char *arch_search = search_file(controlfile, "Architecture: ");
    if (arch_search != NULL) {
	rmstr(arch_search, "Architecture: ");
	remove_white(arch_search);
	strcpy(controlinfo.arch, arch_search);
	free(arch_search);
        if (strlen(arch_search) == 0) {
            printf("Architecture: field is found but is empty!\n");
        }
    } else {
        printf("Architecture: is mandatory!\n");
    }
    char *maintainer_search = search_file(controlfile, "Maintainer: ");
    if (maintainer_search != NULL) {
	rmstr(maintainer_search, "Maintainer: ");
	remove_white(maintainer_search);
	strcpy(controlinfo.maintainer, maintainer_search);
	free(maintainer_search);
        if (strlen(maintainer_search) == 0) {
            printf("Maintainer: field is found but is empty!\n");
        }
    } else {
        printf("Maintainer: is mandatory!\n");
    }
    char *homepage_search = search_file(controlfile, "Homepage: ");
    if (homepage_search != NULL) {
	rmstr(homepage_search, "Homepage: ");
	strcpy(controlinfo.homepage, homepage_search);
	free(homepage_search);
        if (strlen(homepage_search) == 0) {
            printf("Homepage: field is found but is empty!\n");
        }
    } else {
        printf("Homepage: is recommended!\n");
    }
    char *sources_search = search_file(controlfile, "Source: ");
    if (sources_search != NULL) {
	rmstr(sources_search, "Source: ");
	strcpy(controlinfo.sources, sources_search);
	free(sources_search);
        if (strlen(sources_search) == 0) {
            printf("Source: field is found but is empty!\n");
        }
    } else {
        printf("Source: is recommended!\n");
    }
    char *section_search = search_file(controlfile, "Section: ");
    if (section_search != NULL) {
        rmstr(section_search, "Section: ");
        strcpy(controlinfo.section, section_search);
        free(section_search);
        if (strlen(section_search) == 0) {
            printf("Section: field is found but is empty!\n");
        }
    } else {
        printf("Section: is recommended!\n");
    }
    char *priority_search = search_file(controlfile, "Priority: ");
    if (priority_search != NULL) {
        rmstr(priority_search, "Priority: ");
        strcpy(controlinfo.priority, priority_search);
        free(priority_search);
        if (strlen(priority_search) == 0) {
            printf("Priority: field is found but is empty!\n");
        }
    } else {
        printf("Priority: is recommended!\n");
    }
    char *depends_search = search_file(controlfile, "Depends: ");
    if (depends_search != NULL) {
	rmstr(depends_search, "Depends: ");
	strcpy(controlinfo.depends, depends_search);
	free(depends_search);
        if (strlen(depends_search) == 0) {
            printf("Depends: field is found but is empty!\n");
        }
    } else {
        printf("Depends: is recommended!\n");
    }
    char *comment_search = search_file(controlfile, "Comment: ");
    if (comment_search != NULL) {
	rmstr(comment_search, "Comment: ");
	strcpy(controlinfo.comment, comment_search);
	free(comment_search);
    } else {
        printf("Comment: is not recommended!\n");
    }
    char *description_search = searchandreadtoend(controlfile, "Description: ");
    if (description_search != NULL) {
	rmstr(description_search, "Description: ");
	strcpy(controlinfo.description, description_search);
	free(description_search);
        if (strlen(description_search) == 0) {
            printf("Description: field is found but is empty!\n");
        }
    } else {
        printf("Description: field is mandatory!\n");
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
void resetstruct(struct Pkginfo *p) {
    memset(p, 0, sizeof(struct Pkginfo));
}
// end of file

