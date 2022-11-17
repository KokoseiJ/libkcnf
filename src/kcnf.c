#include "kcnf.h"


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


Config *config_value_put(Config *config, void *data, size_t datalen) {
    if (config == NULL) {
        config = calloc(1, sizeof(Config));
        config->type = CONFIG_VALUE;
    }

    config->value = data;
    config->datalen = datalen;

    return config;
}

Config *config_list_put(Config *parent, Config *child) {
    Config *cur;
    if (parent == NULL) {
        parent = calloc(1, sizeof(Config));
        parent->type = CONFIG_LIST;
        parent->child = NULL;
    }

    if (parent->child == NULL) {
        parent->length = 1;
        parent->child = child;
        return parent;
    }

    for (cur = parent->child; cur->next != NULL; cur = cur->next);

    cur->next = child;
    child->prev = cur;
    parent->length++;

    return parent;
}

Config *config_pair_put(Config *parent, char *key, Config *child) {
    Config *cur;
    
    child->key = key;

    if (parent == NULL) {
        parent = calloc(1, sizeof(Config));
        parent->type = CONFIG_PAIR;
        parent->child = NULL;
    }

    if (parent->child == NULL) {
        parent->length = 1;
        parent->child = child;
        return parent;
    }


    for (cur = parent->child; cur->next != NULL; cur = cur->next);

    cur->next = child;
    child->prev = cur;
    parent->length++;

    // printf("Keytest: %s %s\n", key, child->key);

    return parent;
}


void *config_value_get(Config *config) {
    if (config == NULL || config->type != CONFIG_VALUE) return NULL;
    else return config->value;
}

Config *config_list_get(Config *config, uint32_t index) {
    int i;
    Config *cur;

    printf("%d ", config->length);

    if (config == NULL ||
        !(config->type == CONFIG_LIST ||
          config->type == CONFIG_PAIR) ||
        config->length <= index)
            return NULL;

    cur = config->child;
    for (i=0; i<index && cur != NULL; i++) cur=cur->next;

    if (i != index) return NULL;

    return cur;
}

Config *config_pair_get(Config *config, char *key) {
    Config *cur;

    if (config == NULL ||
        config->type != CONFIG_PAIR ||
        key == NULL ||
        *key == '\0')
            return NULL;

    //printf("Called with key: %s\n", key);
    for (cur = config->child; cur != NULL && strcmp(cur->key, key) != 0; cur = cur->next)
        //printf("%p: %s\n", cur, cur->key);
        continue;

    return cur;
}


int main() {
    Config *config, *subpair, *sublist, *subvalue, *value;
    FILE *file = fopen("test.kcnf", "r");
    config = config_parse_file(file);

    printf("%p\n", config->child->key);
    subpair = config_pair_get(config, "nested_pair");

    value = config_pair_get(subpair, "nest_1");
    printf("nested_pair.nest_1: %s\n", config_value_get(value));

    value = config_pair_get(subpair, "nest_2");
    printf("nested_pair.nest_2: %s\n", config_value_get(value));

    value = config_pair_get(subpair, "nest_3");
    printf("nested_pair.nest_3: %s\n", config_value_get(value));


    sublist = config_pair_get(config, "nested_list");
    value = config_list_get(sublist, 0);
    printf("nested_list[0]: %s\n", config_value_get(value));

    value = config_list_get(sublist, 1);
    printf("nested_list[1]: %s\n", config_value_get(value));

    value = config_list_get(sublist, 2);
    printf("nested_list[2]: %s\n", config_value_get(value));


    subvalue = config_pair_get(config, "nested_value");
    printf("nested_value: %s\n", config_value_get(subvalue));

    return 0;
}
