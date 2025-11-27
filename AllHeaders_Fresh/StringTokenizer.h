#ifndef STRINGTOKENIZER_H
#define STRINGTOKENIZER_H

#include "core_ds.h"

static int tokenizeWhitespace(char *line, char **outTokens, int maxTokens) {
    int cnt=0; char *p=line; while (*p && cnt < maxTokens) { while (*p && isspace((unsigned char)*p)) ++p; if (!*p) break; outTokens[cnt++]=p; while (*p && !isspace((unsigned char)*p)) ++p; if (*p) { *p=0; ++p; } }
    return cnt;
}

#endif // STRINGTOKENIZER_H
