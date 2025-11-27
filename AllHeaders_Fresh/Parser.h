#ifndef PARSER_H
#define PARSER_H

#include "Graph.h"
#include "StringTokenizer.h"
#include <cstdio>
#include <cstdlib>

static void parsePortChargesFile(Graph *g, const char *path) {
    FILE *f = fopen(path, "r"); if (!f) { printf("Cannot open %s\n", path); return; }
    char buf[1024]; while (fgets(buf, sizeof(buf), f)) { char tmp[1024]; strncpy(tmp, buf, sizeof(tmp)); int L = strlen(tmp); int i=L-1; while (i>=0 && isspace((unsigned char)tmp[i])) --i; int end=i; while (i>=0 && !isspace((unsigned char)tmp[i])) --i; int startLast = i+1; char last[64]; int len = end-startLast+1; if (len>63) len=63; strncpy(last, tmp+startLast, len); last[len]=0; int charge = atoi(last); char portname[512]; strncpy(portname, tmp, startLast); portname[startLast]=0; SimpleString pn(portname); pn.trimInPlace(); g->setPortCharge(pn, charge); }
    fclose(f);
}

static void parseRoutesFile(Graph *g, const char *path, HashMapStringToInt *companyCounts) {
    FILE *f = fopen(path, "r"); if (!f) { printf("Cannot open %s\n", path); return; }
    char buf[2048]; while (fgets(buf, sizeof(buf), f)) { char tmp[2048]; strncpy(tmp, buf, sizeof(tmp)); char *tokens[16]; int tcnt=0; char *p=tmp; while (*p && tcnt<16) { while (*p && isspace((unsigned char)*p)) p++; if (!*p) break; tokens[tcnt++]=p; while (*p && !isspace((unsigned char)*p)) p++; if (*p) { *p=0; p++; } } if (tcnt < 6) continue; SimpleString origin(tokens[0]); SimpleString dest(tokens[1]); SimpleString dateTok(tokens[2]); SimpleString depTok(tokens[3]); SimpleString arrTok(tokens[4]); SimpleString costTok(tokens[5]); SimpleString company("Unknown"); if (tcnt>=7) { company.assign(tokens[6]); } g->addPortIfMissing(origin); g->addPortIfMissing(dest); int from = g->nameToId->get(origin.c_str()); int to = g->nameToId->get(dest.c_str()); DateTimeSimple dt; sscanf(dateTok.c_str(), "%d/%d/%d", &dt.day, &dt.month, &dt.year); DateTimeSimple dep = dt; sscanf(depTok.c_str(), "%d:%d", &dep.hour, &dep.minute); DateTimeSimple arr = dt; sscanf(arrTok.c_str(), "%d:%d", &arr.hour, &arr.minute); long cost = atol(costTok.c_str()); g->addVoyage(from, to, dep, arr, cost, company); if (companyCounts) companyCounts->increment(company.c_str()); }
    fclose(f);
}

#endif // PARSER_H
