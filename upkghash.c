#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "upkglib.h"
#include "upkghash.h"
#include "upkgstruct.h"

Pkginfo *hashTable[TABLE_SIZE];
int hash(char *key) {
    int hashValue = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hashValue += key[i];
    }
    return hashValue % TABLE_SIZE;
}
void addEntry(char *name, char *version, char *arch) {
    int index = hash(name);
    Pkginfo *newEntry = (Pkginfo *)malloc(sizeof(Pkginfo));
    strcpy(newEntry->pkgname, name);
    strcpy(newEntry->version, version);
    strcpy(newEntry->arch, arch);
    newEntry->next = hashTable[index];
    hashTable[index] = newEntry;
}
Pkginfo *searchEntry(char *name) {
    int index = hash(name);
    Pkginfo *current = hashTable[index];
    while (current != NULL) {
        if (strcmp(current->pkgname, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
void deleteEntry(char *name) {
    int index = hash(name);
    Pkginfo *current = hashTable[index];
    Pkginfo *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->pkgname, name) == 0) {
            if (prev == NULL) {
                hashTable[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}
void list() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Pkginfo *current = hashTable[i];
        while (current != NULL) {
            printf("%s-%s-%s\n", current->pkgname, current->version, current->arch);
            current = current->next;
        }
    }
}
void glob() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Pkginfo *current = hashTable[i];
        while (current != NULL) {
            printf("%s-%s-%s ", current->pkgname, current->version, current->arch);
            current = current->next;
        }
    }
printf("\n");
}

void testhash() {
struct Pkginfo info = gatherinfo();
printpkginfo(info);
    printf("\nprinting testhash:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
    addEntry("bash", "1.2", "1");
    
    addEntry("nano", "2.0", "1");
    addEntry("binutils", "1.1", "1");
    addEntry("coreutils", "1.2", "1");
    addEntry("findutils", "2.0", "1");
    addEntry("util-linux", "1.1", "1");
    printf("Search for nano: \n");
    Pkginfo *nano = searchEntry("nano");
    printf("%s\n", nano->pkgname);
    Pkginfo *found = searchEntry("nano");
    if (found != NULL) {
        printf("%s-%s-%s\n", found->pkgname, found->version, found->arch);
    } else {
        printf("Not found\n");
    }
    list();
    deleteEntry("nano");
    glob();
}
