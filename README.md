# libjson

A JSON parser implementation in C.

## Usage

Given a json file ``example.json``
```
{
    "result": {
        "name":"value",
        "list":[
            "one",
            "two"
        ]
    }
}
```

The following is an example on how to iterate the inner ``list`` array
```
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
```

## TODO

1. Boolean values.
2. String and number encoding.
3. Serialization.
4. Error handling.
5. Refactor API, reduce verbosity.

