# String

Strings are created with `"`'s. You cannot use a `'` to create strings.

```
"Hello, world!" // Ok!
'Hello, world!' // Error!
```

There are a few escape characters you can use.

```
"Hello\nWorld!" // Uses \n (New Line)
"Hello\tWorld!" // Uses \t (Tab)
"Hello\rWorld!" // Uses \r (Carriage Return)
```

## Methods

Strings have a few special methods you can use to manipulate the string.

### Length

Returns the length of the string

```
val $str is "Hello, world!"
$str->length() // Returns 13
```

### lower

Returns the string in lowercase

```
val $str is "Hello, world!"
$str->lower() // Returns "hello, world!"
```

### upper

Returns the string in uppercase

```
val $str is "Hello, world!"
$str->lower() // Returns "HELLO, WORLD!"
```

### replace

Replaces a set of characters in a string with another set of characters

```
val $str is "Hello, world!"
$str->replace("Hello", "Goodbye") // Returns "Goodbye, world!"
```

### at

Returns the character at a index

```
val $str is "Hello, world!"
$str->at(1) // Returns "e"
```
