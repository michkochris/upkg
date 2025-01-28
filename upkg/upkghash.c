/******************************************************************************
 *  Filename:    upkghash.c
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
/*file description: file for adding pkginfo into hash table for storage...*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "upkglib.h"
#include "upkghash.h"
#include "upkgstruct.h"

Node* hashTable[TABLE_SIZE];

int hashFunction(char* name) {
    int hash = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        hash += name[i];
    }
    return hash % TABLE_SIZE;
}
void addpkg(char* name) {
    int index = hashFunction(name);
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->data.pkgname, name);
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}
Pkginfo* search(char* name) {
    int index = hashFunction(name);
    Node* current = hashTable[index];
    while (current != NULL) {
        if (strcmp(current->data.pkgname, name) == 0) {
            return &(current->data);
        }
        current = current->next;
    }
    return NULL;
}
char *search_hash(char *name) {
    int index = hashFunction(name);
    Node *entry = hashTable[index];
    while (entry != NULL) {
        if (strcmp(entry->data.pkgname, name) == 0) {
            return entry->data.pkgname;
        }
        entry = entry->next;
    }
    return NULL;
}
void removepkg(char* name) {
    int index = hashFunction(name);
    Node* current = hashTable[index];
    Node* prev = NULL;
    while (current != NULL && strcmp(current->data.pkgname, name) != 0) {
        prev = current;
        current = current->next;
    }
    if (current != NULL) {
        if (prev == NULL) {
            hashTable[index] = current->next;
        } else {
            prev->next = current->next;
        }
        free(current);
    }
}
void list() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = hashTable[i];
        while (current != NULL) {
            printf("%s\n", current->data.pkgname);
            current = current->next;
        }
    }
printf("\n");
}
void glob() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = hashTable[i];
        while (current != NULL) {
            printf("%s ", current->data.pkgname);
            current = current->next;
        }
    }
printf("\n");
}
// Print all items in the hash table, showing collisions
void print_hash_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Index %d: ", i);
        Node *current = hashTable[i];
        while (current != NULL) {
            printf("(%s) -> ", current->data.pkgname);
            current = current->next;
        }
        printf("NULL\n");
    }
}
#define MAX_SUGGESTIONS 10
char **suggestions(char *name) {
    char **suggestions = (char **)malloc(MAX_SUGGESTIONS * sizeof(char *));
    int count = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashTable[i] != NULL && strncmp(name, hashTable[i]->data.pkgname, strlen(name)) == 0) {
            suggestions[count++] = strdup(hashTable[i]->data.pkgname);
            if (count >= MAX_SUGGESTIONS) break;
        }
    }
    suggestions[count] = NULL; // Null-terminate the array
    return suggestions;
}
void print_suggestions(char *prefix) {
    int prefix_len = strlen(prefix);
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = hashTable[i];
        while (current != NULL) {
            if (strncmp(current->data.pkgname, prefix, strlen(prefix)) == 0) {
                printf("%s\n", current->data.pkgname);
            }
            current = current->next;
        }
    }
}

void initialadd() {
    struct Pkginfo info = gatherinfo();
    //printpkginfo(info);
    int index = hashFunction(info.pkgname);
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->data.pkgname, info.pkgname);
    strcpy(newNode->data.version, info.version);
    strcpy(newNode->data.arch, info.arch);
    strcpy(newNode->data.maintainer, info.maintainer);
    strcpy(newNode->data.homepage, info.homepage);
    strcpy(newNode->data.sources, info.sources);
    strcpy(newNode->data.section, info.section);
    strcpy(newNode->data.priority, info.priority);
    strcpy(newNode->data.depends, info.depends);
    strcpy(newNode->data.comment, info.comment);
    strcpy(newNode->data.description, info.description);
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
    resetstruct(&info);
}
void status_search(char *name) {
    Pkginfo *found = search(name);
    if (found != NULL) {
        printf("status search:\n\n");
        if (strlen(found->pkgname) > 0) {
            printf("Package: %s\n", found->pkgname);
        }
        if (strlen(found->version) > 0) {
            printf("Version: %s\n", found->version);
        }
        if (strlen(found->arch) > 0) {
            printf("Architecture: %s\n", found->arch);
        }
        if (strlen(found->maintainer) > 0) {
            printf("Maintainer: %s\n", found->maintainer);
        }
        if (strlen(found->homepage) > 0) {
            printf("Homepage: %s", found->homepage);
        }
        if (strlen(found->sources) > 0) {
            printf("Source: %s\n", found->sources);
        }
        if (strlen(found->section) > 0) {
            printf("Section: %s", found->section);
        }
        if (strlen(found->priority) > 0) {
            printf("Priority: %s", found->priority);
        }
        if (strlen(found->depends) > 0) {
            printf("Depends: %s", found->depends);
        }
        if (strlen(found->comment) > 0) {
            printf("Comment: %s\n", found->comment);
        }
        if (strlen(found->description) > 0) {
            printf("Description: %s\n", found->description);
        }
    } else {
        printf("status search: %s Not found\n\n", name);
    }
    //resetstruct(&found);
    free(found);
}
void testhash() {
addpkg("fbinutils");
addpkg("findutils");
addpkg("fcoreutils");
addpkg("futil-linux");
addpkg("fgawk");
addpkg("fbash");
addpkg("fneofetch");
addpkg("fnano");
initialadd();
//initialsearch("file");
//removepkg("bash");
//removepkg("nano");
//Pkginfo *srch = search("file");
//printf("search: %s\n", srch->pkgname);
//list();
//glob();
//print_hash_table();
//print_suggestions("f");
}
