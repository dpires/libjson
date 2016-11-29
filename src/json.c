#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include <ctype.h>
#include "json.h"

char *
TokenString(int type)
{
    char *strings[] = { 
        "OBJECT_OPEN",
        "OBJECT_CLOSE",
        "NAME_SEPARATOR",
        "ARRAY_OPEN",
        "ARRAY_CLOSE",
        "VALUE_SEPARATOR",
        "STRING",
        "NIL",
        "WHITESPACE",
        "NUMBER"
    };
    return strings[type];
}

char *
JsonTypeString(int type) {
    char *f[] = {
        "J_OBJECT",
        "J_ARRAY",
        "J_STRING",
        "J_NUMBER"
    };
    return f[type];
}

Parser *
Parser_create(Vector *tokens)
{
    Parser *p = (Parser *)malloc(sizeof(Parser));
    p->tokens = tokens;
    p->position = 0;
    return p;
} 

Token *
GetToken(Parser *parser, int offset)
{
    return Vector_get(parser->tokens, parser->position + offset);
}

Token *
NextToken(Parser *parser)
{
    return GetToken(parser, 1);
}

void
EatToken(Parser *parser, int offset)
{
    parser->position = parser->position + offset;
}

Token *
CurrentToken(Parser *parser)
{
    return GetToken(parser, 0);
}

Token *
MatchAndEat(Parser *parser, int type)
{
    Token *current = CurrentToken(parser);
    EatToken(parser, 1);
    return current;    
}

JsonValue *
JsonArray_create()
{
    JsonArray *ar = (JsonArray *)malloc(sizeof(JsonArray));
    ar->items = Vector_create(1);
    
    JsonValue *val = JsonValue_create(J_ARRAY);
    val->jsonArray = ar;
    return val;
}

void
AddItem(JsonArray *array, JsonValue *val)
{
    Vector_add(array->items, val);
}


JsonValue *
JsonValue_create(int type)
{
    JsonValue *val = (JsonValue *)malloc(sizeof(JsonValue));
    val->type = type;
    return val;
}

JsonValue *
JsonObject_create()
{
    JsonObject *obj = (JsonObject *)malloc(sizeof(JsonObject));
    obj->names = Vector_create(2);
    obj->values = Vector_create(2);

    JsonValue *val = JsonValue_create(J_OBJECT);
    val->jsonObject = obj;

    return val;
} 

void
JsonObject_add(JsonObject *obj, char *key, JsonValue *value)
{
    Vector_add(obj->names, key);
    Vector_add(obj->values, value);
}

JsonValue *
GetArrayVal(JsonArray *a, int index)
{
    return Vector_get(a->items, index);
}

JsonValue *
GetVal(JsonObject *val, char *key)
{
    VIterator *it = Vector_getIterator(val->names);
    char *n;
    int index = 0;
    while((n = VIterator_getNext(it)) != NULL) {
        if (strcmp(key, n) ==0)
            return Vector_get(val->values, index);

        index++;
    }
    return NULL;

}

JsonValue *
JsonNumber_create(char * value)
{
    JsonNumber *js = (JsonNumber *)malloc(sizeof(JsonNumber));
    js->value = value;
    JsonValue *val = JsonValue_create(J_NUMBER);
    val->jsonNumber = js;
    return val;

}

JsonValue *
JsonString_create(char *value)
{
    JsonString *js = (JsonString *)malloc(sizeof(JsonString));
    js->value = value;

    JsonValue *val = JsonValue_create(J_STRING);
    val->jsonString = js;

    return val;
}

JsonArray *
JsonValue_getArray(JsonValue *v)
{
    return v->jsonArray;
}

JsonObject *
JsonValue_getObject(JsonValue *v)
{
    return v->jsonObject;
}

JsonString *
JsonValue_getString(JsonValue *v)
{
    return v->jsonString;
}

JsonNumber *
JsonValue_getNumber(JsonValue *v)
{
    return v->jsonNumber;
}

Token *
Token_create(char *val, int type)
{
    Token *token = (Token *)malloc(sizeof(Token));
    token->val = val;
    token->type = type;
    return token;
}

void
Token_destroy(Token *t)
{
    free(t->val);
    free(t);
}

int
GetType(char c) 
{
    int type = NIL;
    switch (c) {
        case ' ':
        case '\n':
        case '\t':
            type = WHITESPACE;
            break;
        case '{':
            type = OBJECT_OPEN;
            break;
        case '}':
            type = OBJECT_CLOSE;
            break;
        case '[':
            type = ARRAY_OPEN;
            break;
        case ']':
            type = ARRAY_CLOSE;
            break;
        case ':':
            type = NAME_SEPARATOR;
            break;
        case ',':
            type = VALUE_SEPARATOR;
            break; 
        case '"':
            type = STRING;
            break;
    }
    return type;
}

void
GetString(char *buffer, int pos, char *input)
{
    int index=0;
    for (index=pos;input[index]!= '"'; index++) {
        buffer[index] = input[index];
        buffer = realloc(buffer, (sizeof(char)*index));
        printf("b =%s", buffer);
    }
}

void
UpdateIndex(int *index, char *dump)
{
    *index = *index + 1;
    if (*index == 20) {
       dump = realloc(dump, sizeof(char)  * (20 + *index)); 
    }
}

char *
JsonDump(Vector *tokens)
{
    char *dump = (char *)malloc(sizeof(char) * 190);
    int index=0;
    VIterator *it = Vector_getIterator(tokens);
    Token *token;
    int len;
    int i;
    while ((token = VIterator_getNext(it)) != NULL) {
        switch (token->type) {
            case OBJECT_OPEN:
                dump[index] = '{';
                index++;
                break;
            case OBJECT_CLOSE:
                dump[index] = '}';
                index++;
                break;
            case NAME_SEPARATOR:
                dump[index] = ':';
                index++;
                break;
            case VALUE_SEPARATOR:
                dump[index] = ',';
                index++;
                break;
            case STRING:
                dump[index] = '"';
                index++;
                len = strlen(token->val);
                for (i=0; i<len; i++) {
                    dump[index] = token->val[i]; 
                    index++;
                }
                dump[index] = '"';
                index++;
                break;
            case ARRAY_OPEN:
                dump[index] = '[';
                index++;
                break;
            case ARRAY_CLOSE:
                dump[index] = ']';
                index++;
                break;
            case NUMBER:
                len = strlen(token->val);
                for (i=0; i<len; i++) {
                    dump[index] = token->val[i]; 
                    index++;
                }
                break;
        }
    }
    VIterator_destroy(it);
    dump[index] = '\0';
    return dump;
}

Vector *
Tokenize(char *input)
{
    Vector *tokens = Vector_create(10);
    int pos = 0;
    int state = STATE_NORMAL;
    int type;
    char *tokenBuf;
    int curIndex = 0;
    int reset = 0;
    int buffer = 10;
    for (pos=pos; pos < strlen(input); pos++) {
        char c = input[pos];
        switch (state) {
            case STATE_NORMAL:
                type = GetType(c);
                if (type == STRING) {
                    tokenBuf = (char *)malloc(sizeof(char)* buffer);
                    state = STATE_STRING;
                } else if (type == WHITESPACE) {

                } else if (isdigit(c)) {
                    tokenBuf = (char *)malloc(sizeof(char) * buffer);
                    tokenBuf[curIndex] = c;  
                    curIndex++; 
                    state = STATE_NUMBER;
                } else {
                    char buf[2];
                    buf[0] = c;
                    buf[1] = '\0';
                    Vector_add(tokens, Token_create(strdup(buf), type));
                }
            break;

            case STATE_STRING:
                type = GetType(c);
                int p = pos;
                p--;
                char prev = input[p]; 
                if (type == STRING && prev != '\\') {
                    state = STATE_NORMAL;
                    tokenBuf[curIndex] = '\0';
                    Vector_add(tokens, Token_create(strdup(tokenBuf), type));
                    free(tokenBuf);
                    curIndex = 0;
                } else if (type == STRING && prev == '\\') {
                    tokenBuf[curIndex] = c;                                     
                    curIndex++;                                                 
                    if (curIndex == buffer) {
                        tokenBuf = realloc(tokenBuf, sizeof(char) * (curIndex * 2));
                    }
                } else {
                    tokenBuf[curIndex] = c;                                     
                    curIndex++;                                                 
                    if (curIndex == buffer) {
                        tokenBuf = realloc(tokenBuf, sizeof(char) * (curIndex * 2));
                    }
                } 
            break;

            case STATE_NUMBER:
                if (isdigit(c)) {
                    tokenBuf[curIndex] = c;                                     
                    curIndex++;                                                 
                    if (curIndex == buffer) {
                        tokenBuf = realloc(tokenBuf, sizeof(char) * (curIndex * 2));
                    }
                } else {
                    tokenBuf[curIndex] = '\0';
                    char *ptr;                                                  
                    long num = strtol(tokenBuf, &ptr, 10);                      
                    int len = num+1;                                            
                    char bb[len];                                               
                    snprintf(bb,len, "%lu", num);                               
                    Vector_add(tokens, Token_create(strdup(bb), NUMBER));
                    curIndex = 0;                                           
                    free(tokenBuf);                                         
                    state = STATE_NORMAL;                                      
                    pos--; 
                }
            break;
        }
    }
    return tokens;
}

JsonValue *
ParseArray(Parser *p)
{
    JsonValue *obj = JsonArray_create();
    Token *t = CurrentToken(p);
    JsonValue *val;
    while (t->type != ARRAY_CLOSE) {
        if (t->type == VALUE_SEPARATOR) {
            MatchAndEat(p, VALUE_SEPARATOR);
        }
        val = ParseValue(p);
        AddItem(obj->jsonArray, val);
        t = CurrentToken(p); 
    }

    MatchAndEat(p, ARRAY_CLOSE);

    return obj;
}

JsonValue *
ParseObject(Parser *p)
{
    JsonValue *obj = JsonObject_create();
    JsonValue *val;
    char *key;

    Token *t = CurrentToken(p);

    while(t->type != OBJECT_CLOSE) {
        if (t->type == VALUE_SEPARATOR) {
            MatchAndEat(p, VALUE_SEPARATOR);
        }

        Token *tok = MatchAndEat(p, STRING);
        key = tok->val;
        MatchAndEat(p,NAME_SEPARATOR); 
        val = ParseValue(p);
        JsonObject_add(obj->jsonObject, key, val);
        t = CurrentToken(p);
    }

    MatchAndEat(p, OBJECT_CLOSE);

    return obj; 
}

JsonValue *
ParseString(Parser *p)
{
    Token *l = MatchAndEat(p, STRING);
    return JsonString_create(l->val);
}

JsonValue *
ParseValue(Parser *p)
{
    Token *t = NULL;
    JsonValue *val = NULL;
    Token *token = CurrentToken(p);;
    switch (token->type) {
        case ARRAY_OPEN:
            MatchAndEat(p, ARRAY_OPEN);
            return ParseArray(p);
            break;
        case OBJECT_OPEN:
            MatchAndEat(p, OBJECT_OPEN);
            return ParseObject(p);
        break;
        case STRING:
            t = MatchAndEat(p, STRING);
            return JsonString_create(t->val);
            break;
        case NUMBER:
            t = MatchAndEat(p, NUMBER);
            return JsonNumber_create(t->val);
            break;
    }
    return val;
}

char *
ReadFile(char *path)
{
    char *input;
    FILE *fp = fopen(path, "r"); 
    if (fseek(fp, 0L, SEEK_END) == 0) {
        long sze = ftell(fp);
        input = (char *)malloc(sizeof(char) * (sze+1));
        if (fseek(fp, 0L, SEEK_SET) != 0) { }
        int len = fread(input, sizeof(char), sze,  fp);
        input[len++] = '\0';
    }
    fclose(fp);
    return input;
}


JsonValue *
JsonValue_parseFile(char *file)
{
    char *input = ReadFile(file); 
    Vector *tokens = Tokenize(input);
    free(input);
    Parser *p = Parser_create(tokens);
    return ParseValue(p);
}

void
JsonValue_destroy(JsonValue *value)
{
    free(value);
}
