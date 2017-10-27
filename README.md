# Small C
A collection of memory efficient C stuff.

## Small String
Strings using 6-bit characters, when you really need
that extra 25% memory. These are

* The 26 letters of the lowercase alphabet
* Ten digits `0` to `9`
* `{ } [ ] ( ) : ; ' " , . % & - _ + = ! #`
* `| ~ \0 / > < ^ *`

## Super Small String
String using 4-bit characters, when you <strong>really</strong>
need the extra 50% memory.

* The ten digits `0` to `9`
* `{ } [ ] : ,`

## Small JSON
A minimalistic JSON implementation using `small_string`