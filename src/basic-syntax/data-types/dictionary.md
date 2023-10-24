# Dictionary

A key value pair of values. Both the key and value can be multiple types.

```
{
    "key" -> "value",
    "foo" -> 1,
    "bar" -> true,
    1 -> 2
}
```

## Methods

Dictionaries have a few methods to manipulate them.

### get

Returns the value of a key

```
val $dict is {
    "key" -> "value",
    "foo" -> 1,
    "bar" -> true,
    1 -> 2
}

$dict->get("key") // Returns "value"
```

### set

Sets the value of a key

```
val $dict is {
    "key" -> "value",
    "foo" -> 1,
    "bar" -> true,
    1 -> 2
}

$dict->set("key", "new value") // Sets the value of "key" to "new value"
$dict->set("baz", "qux") // Adds a new key "baz" with the value "qux"
```
