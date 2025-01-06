#ifndef UPKHASH_H
#define UPKHASH_H
#define TABLE_SIZE 20
typedef struct Pkginfo {
    char pkgname[32];
    char version[32];
    char arch[32];
    char maintainer[128];
    char homepage[64];
    char sources[128];
    char section[32];
    char priority[32];
    char depends[128];
    char comment[128];
    char description[1028];
    struct Pkginfo *next;
} Pkginfo;
int hash(char *key);
void addEntry(char *name, char *version, char *arch);
Pkginfo *searchEntry(char *name);
void deleteEntry(char *name);
void list();
void glob();
void testhash();
#endif
