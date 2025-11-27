#ifndef STRING_H
#define STRING_H

#include "core_ds.h"

static void simpleStringTrim(SimpleString &s) { s.trimInPlace(); }
static int simpleStringCompare(const SimpleString &a, const SimpleString &b) { return strcmp(a.c_str(), b.c_str()); }
static void simpleStringAssign(SimpleString &dst, const char *src) { dst.assign(src); }

#endif // STRING_H
