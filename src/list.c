#include "list.h"


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


Config *config_list_get(Config *config, uint32_t index) {
    int i;
    Config *cur;

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
