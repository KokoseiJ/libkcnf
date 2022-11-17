#include "value.h"


Config *config_value_put(Config *config, void *data, size_t datalen) {
    if (config == NULL) {
        config = calloc(1, sizeof(Config));
        config->type = CONFIG_VALUE;
    }

    config->value = data;
    config->datalen = datalen;

    return config;
}


void *config_value_get(Config *config) {
    if (config == NULL || config->type != CONFIG_VALUE) return NULL;
    else return config->value;
}
