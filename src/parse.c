#include "parse.h"

Config *config_value_parse(Config *, char **);
Config *config_list_parse(Config *, char **);
Config *config_pair_parse(Config *, char **);


Config *config_parse_file(FILE *file) {
    char **cursor;
    int orig_pos, filesize;

    cursor = calloc(1, sizeof(char *));

    orig_pos = ftell(file);
    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    fseek(file, orig_pos, SEEK_SET);

    //printf("filesize: %d\n", filesize);

    *cursor = calloc(filesize + 1, sizeof(char));
    fread(*cursor, sizeof(char), filesize, file);

    //printf("cursor: %s\n", *cursor);

    return config_parse(cursor);
}


Config *config_parse(char **cur) {
    int type;
    char *next, *numstr;
    Config *config = (Config *) calloc(1, sizeof(Config));

    if (cur == NULL) return NULL;

    numstr = *cur;
    next = splitstr(*cur, '\n');
    if (next == NULL) return NULL;
    *cur = next;

    if (numstr == NULL) return NULL;
    //printf("\n\nnumstr: %s\n", numstr);

    type = atoi(numstr);
    config->type = type;
    //printf("Type:\t%d\n\n", type);

    switch (type) {
    case CONFIG_VALUE:
        return config_value_parse(config, cur);
    case CONFIG_LIST:
        return config_list_parse(config, cur);
    case CONFIG_PAIR:
        return config_pair_parse(config, cur);
    }

    return NULL;
}


Config *config_value_parse(Config *config, char **cur) {
    char *next, *b64str;
    void *b64out;
    size_t b64inlen, b64outlen;

    b64str = *cur;
    next = splitstr(*cur, '\n');
    if (next == NULL) return NULL;
    *cur = next;

    //printf("Read: %s\n", b64str);
    b64inlen = strlen(b64str);
    b64outlen = b64declen(b64str);
    b64out = calloc(b64outlen, 1);

    b64decode(b64str, b64inlen, b64out, b64outlen);

    //printf("Decoded: %s\n", (char *)b64out);

    config->value = b64out;
    config->datalen = b64outlen;

    return config;
}

Config *config_list_parse(Config *config, char **cur) {
    Config *child;
    char *next, *lenstr;
    uint32_t len;
    int i;

    lenstr = *cur;
    next = splitstr(*cur, '\n');
    if (next == NULL) return NULL;
    *cur = next;

    len = atoi(lenstr);
    //printf("lenstr: %s, len: %d\n", lenstr, len);

    config->child = NULL;

    for (i=0; i<len; i++) {
        child = config_parse(cur);
        config_list_put(config, child);
    }

    return config;
}

Config *config_pair_parse(Config *config, char **cur) {
    Config *child;
    char *next, *lenstr, *name_orig, *name_dup;
    uint32_t len;
    int i;

    lenstr = *cur;
    next = splitstr(*cur, '\n');
    if (next == NULL) return NULL;
    *cur = next;

    len = atoi(lenstr);
    //printf("lenstr: %s, len: %d\n", lenstr, len);

    config->child = NULL;

    for (i=0; i<len; i++) {
        name_orig = *cur;
        next = splitstr(*cur, '\n');
        if (next == NULL) return NULL;
        *cur = next;

        name_dup = strdup(name_orig);
        //printf("Name: %s\n", name_dup);

        child = config_parse(cur);
        config_pair_put(config, name_dup, child);
    }

    return config;
}
