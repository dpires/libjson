#include "vector.h"

enum states {
    STATE_NORMAL,
    STATE_STRING,
    STATE_NUMBER
};

enum tokenTypes {
    OBJECT_OPEN,
    OBJECT_CLOSE,
    NAME_SEPARATOR,
    ARRAY_OPEN,
    ARRAY_CLOSE,
    VALUE_SEPARATOR,
    STRING,
    NIL,
    WHITESPACE,
    NUMBER
};

char *
TokenString(int type);

typedef struct Token {
    int type;
    char *val;
} Token;

enum JsonType {
    J_OBJECT,
    J_ARRAY,
    J_STRING,
    J_NUMBER
};

char *
JsonTypeString(int type);

typedef struct Parser {
    int position;
    Vector *tokens;
} Parser;

typedef struct JsonValue JsonValue;
typedef struct JsonObject JsonObject;
typedef struct JsonArray JsonArray;
typedef struct JsonString JsonString;
typedef struct JsonNumber JsonNumber;

typedef struct JsonValue {
    int type;
    union {
        JsonObject *jsonObject;
        JsonArray *jsonArray;
        JsonString *jsonString;
        JsonNumber *jsonNumber;
    };
    Parser *parser;    
} JsonValue;

typedef struct Json {
	Parser *parser;
	JsonValue *value;
} Json;

Parser *
Parser_create(Vector *tokens);

void
Parser_destroy(Parser *parser);

Token *
GetToken(Parser *parser, int offset);

Token *
NextToken(Parser *parser);

void
EatToken(Parser *parser, int offset);

Token *
CurrentToken(Parser *parser);

Token *
MatchAndEat(Parser *parser, int type);

JsonValue *
JsonValue_create(int type);

typedef struct JsonArray {
    Vector *items;
} JsonArray;

JsonValue *
JsonArray_create();

void
AddItem(JsonArray *array, JsonValue *val);

typedef struct JsonObject {
    Vector *names;
    Vector *values;
} JsonObject;

JsonValue *
JsonValue_create(int type);

JsonValue *
JsonObject_create();

void
JsonObject_add(JsonObject *obj, char *key, JsonValue *value);

JsonValue *
GetArrayVal(JsonArray *a, int index);

JsonValue *
GetVal(JsonObject *val, char *key);

typedef struct JsonString {
    char *value;
} JsonString;

typedef struct JsonNumber {
    char *value;
} JsonNumber;

JsonValue *
JsonNumber_create(char * value);

JsonValue *
JsonString_create(char *value);

JsonArray *
JsonValue_getArray(JsonValue *v);

JsonObject *
JsonValue_getObject(JsonValue *v);

JsonString *
JsonValue_getString(JsonValue *v);

JsonNumber *
JsonValue_getNumber(JsonValue *v);

Token *
Token_create(char *val, int type);

void
Token_destroy(Token *t);

int
GetType(char c);

void
GetString(char *buffer, int pos, char *input);

void
UpdateIndex(int *index, char *dump);

char *
JsonDump(Vector *tokens);

Vector *
Tokenize(char *input);

JsonValue *
ParseValue(Parser *p);

JsonValue *
ParseArray(Parser *p);

JsonValue *
ParseObject(Parser *p);

char *
ReadFile(char *path);

JsonValue *
JsonValue_parseFile(char *file);

Json *
Json_parseFile(char *file);

void
JsonValue_destroy(JsonValue *value);

JsonObject *
Json_getObject(Json *json, char *name);

JsonArray *
Json_getArray(Json *json);

char *
JsonObject_getString(JsonObject *obj, char *key);

JsonArray *
JsonObject_getArray(JsonObject *obj, char *key);

char *
JsonArray_getString(JsonArray *array, int index);

int
JsonArray_getNumber(JsonArray *array, int index);

JsonObject *
JsonArray_getJsonObject(JsonArray *array, int index);

void
Json_destroy(Json *json);
