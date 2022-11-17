#ifndef INCL_KCNF_H
#define INCL_KCNF_H

#include "base64.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

enum config_type {
    CONFIG_VALUE = 0,
    CONFIG_LIST = 1,
    CONFIG_PAIR = 2
};

typedef struct Config Config;
typedef struct Config {
    // Meta
    enum config_type type;
    uint32_t length;
    // Values
    void *value;
    size_t datalen;
    char *key;
    Config *child;
    Config *prev;
    Config *next;
} Config;


Config *config_parse_file(FILE *);
Config *config_parse(char **);

Config *config_value_put(Config *, void *, size_t);
Config *config_list_put(Config *, Config *);
Config *config_pair_put(Config *config, char *, Config *);

void *config_value_get(Config *);
Config *config_list_get(Config *, uint32_t);
Config *config_pair_get(Config *, char *);

#endif