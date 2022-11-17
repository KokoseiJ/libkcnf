#include "util.h"

int countchr(char *str, char chr) {
    int i;
    for (i=0; *str != '\0'; *str++ == chr ? i++ : 0);
    return i;
}

char *splitstr(char *str, char split) {
    char *cur;

    if (str == NULL) return NULL;
    for (cur = str; *cur != split && *cur != '\0'; cur++);
    if (*cur == '\0') return NULL;
    for (; *cur == split && *cur != '\0'; cur++) *cur = '\0';
    return cur;
}
