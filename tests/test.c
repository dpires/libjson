#include <stdio.h>
#include <string.h>
#include "../src/json.h"

int
main(void)
{
    JsonValue *root = JsonValue_parseFile("example.json");
    JsonObject *rootObject = JsonValue_getObject(root);
    JsonValue *resultValue = GetVal(rootObject, "result");
    JsonObject *resultObject = JsonValue_getObject(resultValue);
    JsonValue *listValue = GetVal(resultObject, "list");
    JsonArray *list = JsonValue_getArray(listValue);
    int index;
    for (index = 0; index < list->items->size; index++) {
        JsonValue *value = GetArrayVal(list, index);
        JsonString *string = JsonValue_getString(value);
        printf("%s\n", string->value);
    }
    JsonValue_destroy(root);
    return 0;
}
