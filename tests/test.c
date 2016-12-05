#include <stdio.h>
#include <string.h>
#include "json.h"

int
main(void)
{
    Json *json;
    JsonArray *list;
    JsonObject *result;

    json = Json_parseFile("test_1.json");
    result = Json_getObject(json, "result");
    list = JsonObject_getArray(result, "list");

    printf("name = %s\n", JsonObject_getString(result, "name"));

    int index;
    for (index = 0; index < list->items->size; index++) {
        printf("%s\n", JsonArray_getString(list, index));
    }

    Json_destroy(json);

    json = Json_parseFile("test_2.json");
    list = Json_getArray(json);

    for (index = 0; index < list->items->size; index++) {
        printf("%s\n", JsonArray_getString(list, index));
    }

    Json_destroy(json);

    return 0;
}
