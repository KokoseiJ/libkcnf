#include "dump.h"

int config_value_dump(Config *, FILE *);
int config_list_dump(Config *, FILE *);
int config_pair_dump(Config *, FILE *);


int config_dump(Config *config, FILE *file) {
    if (config == NULL || file == NULL) return 1;

    fprintf(file, "%d\n", config->type);

    // TODO: Have ENUM for return values
    switch (config->type) {
    case (CONFIG_VALUE):
        return config_value_dump(config,file);
    case (CONFIG_LIST):
        return config_list_dump(config, file);
    case (CONFIG_PAIR):
        return config_pair_dump(config, file);
    }

    return 1;
}

int config_value_dump(Config *config, FILE *file) {
    int check;
    char *b64out;
    size_t b64outlen;

    b64outlen = b64enclen(config->datalen);
    b64out = calloc(b64outlen, sizeof(char));

    check = b64encode(config->value, config->datalen, b64out, b64outlen);
    printf("b64encode check: %d\n", check);


    fprintf(file, "%s\n", b64out);

    return 0;
}

int config_list_dump(Config *config, FILE *file) {
    Config *cur;

    fprintf(file, "%d\n", config->length);

    if (config->child == NULL) {
        if (config->length != 0) {
            printf("SANITY CHECK FAILED! config->child == NULL but config->length != 0\n");
            return 1;
        }
        return 0;
    }

    for (cur=config->child; cur != NULL; cur=cur->next)
        config_dump(cur, file);

    return 0;
}

int config_pair_dump(Config *config, FILE *file) {
    Config *cur;

    fprintf(file, "%d\n", config->length);

    if (config->child == NULL) {
        if (config->length != 0) {
            printf("SANITY CHECK FAILED! config->child == NULL but config->length != 0\n");
            return 1;
        }
        return 0;
    }

    for (cur=config->child; cur!=NULL; cur=cur->next) {
        fprintf(file, "%s\n", cur->key);
        config_dump(cur, file);
    }

    return 1;
}
