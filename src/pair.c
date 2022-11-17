#include "pair.h"


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
