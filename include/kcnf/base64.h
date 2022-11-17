#ifndef INCL_KCNF_BASE64_H
#define INCL_KCNF_BASE64_H

#include "util.h"
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

size_t b64enclen(size_t datalen);
size_t b64declen(char *b64string);

int b64encode(void *in, size_t inlen, char *out, size_t outlen);
int b64decode(char *in, size_t inlen, void *out, size_t outlen);

#endif
