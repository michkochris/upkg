/******************************************************************************
 *  Filename:    upkghash.h
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
/*file description: file to export functions from upkghash.c*/
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
} Pkginfo;
typedef struct Node {
    Pkginfo data;
    struct Node* next;
} Node;
void testhash();
#endif
