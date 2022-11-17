#include <kcnf/kcnf.h>
#include <stdio.h>


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

    FILE *file_new = fopen("dump.kcnf", "w");

    config_dump(config, file_new);

    return 0;
}
