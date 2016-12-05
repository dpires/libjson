# libjson

A JSON parser implementation in C.

## Usage

Given a json file ``test_1.json``
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

The following is an example on how to access ``name`` and iterate the inner ``list`` array
```
    Json *json = Json_parseFile("test_1.json");
    JsonObject *result = Json_getObject(json, "result");
    char *name = JsonObject_getString(result, "name");
    JsonArray *list = JsonObject_getArray(result, "list");
    int index;
    for (index = 0; index < list->items->size; index++) {
        char *item = JsonArray_getString(list, index);
    }
    Json_destroy(json);
```

## TODO

1. Boolean values.
2. String and number encoding.
3. Serialization.
4. Error handling.
5. Refactor API, reduce verbosity.

