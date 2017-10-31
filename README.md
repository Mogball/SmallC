# Small C
A collection of memory efficient C stuff.

## Small String
Strings using 6-bit characters, when you really need
that extra 25% memory. These are

* The 26 letters of the lowercase alphabet
* Ten digits `0` to `9`
* `{ } [ ] ( ) : ; ' " , . % & - _ + = ! #`
* `| ~ \0 / > < ^ *`

### Usage
Compress a C string:
```C
char c_str[] = "bicycles";
small_char* small_str = MakeSmallString(c_str, strlen(c_str));
// do stuff with small_str
free(small_str);
```
Create a small string from a literal:
```C
small_char small_str[] = "\x42\x32\x64\x03\x53\x4c"; // 6 bytes
char* c_str = (char*) malloc(8);
ToCString(small_str, c_str, 8);
printf("%s", c_str); // output 'bicycles'
```

## Small JSON
An efficient and minimalistic JSON implementation using `small_string`

### Usage
Reading JSON
```C
small_char json[] = ...; // '{"key":["abra","cad","abra"]}'
small_char key[] = ...; // 'key'
uint16_t len = 31;
ValidateSmallJson(json, len);
JsonElement element = AsJsonElement(json, len);
HashTable* index = IndexJson(json, len);
JsonElement array = JsonObjectGetValue(element, index, key, 3);
JsonElement string = JsonArrayElementAt(array, index, 2);
JsonStringGet(MakeString(4), val); // 'abra'
```
Writing JSON
```C
JsonElement number = IntAsJsonElement(512, MakeString(3), 3);
JsonElement boolean = BoolAsJsonElement(true, MakeString(4), 4);
JsonElement string = StringAsJsonElement(MakeSmallString("bicycles", 8), MakeString(10), 10);
ArrayList* list = MakeArrayList(3);
ListAppend(list, number);
ListAppend(list, boolean);
ListAppend(list, string);
small_char* arry_str = MakeString(JsonArrayCompileLength(list));
JsonElement array = JsonArrayCompile(list, array_str, JsonArrayCompileLength(list));
JsonElement key = StringAsJsonElement(MakeSmallString("keys", 4), MakeString(6), 6);
HashMap* map = MakeMap(...);
MapPut(key, array);
small_char* obj_str = MakeString(JsonObjectCompileLength(map));
JsonElement obj = JsonObjectCompile(map, obj_str, JsonObjectCompileLength(map));
// '{"keys":[512,true,"bicycles"]}'
```
