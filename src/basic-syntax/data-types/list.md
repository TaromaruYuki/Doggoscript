# List

A collection of values. A collection of values. Lists are denoted with square brackets. Values inside the list are separated by a comma. Values can be any type.

```
> [1, 2, 3]
  [1, 2, 3]
  
> ["foo", "bar"]
  ["foo", "bar"]
```

There are a few methods you can use to manipulate lists.

## Methods

### length

Returns the length of the list

```
val $list is [1, 2, 3]
$list->length() // Returns 3
```

### append

Adds a value to the end of the list

```
val $list is [1, 2, 3]
$list->append(4) // Returns 4 (The new length)
```

### extend

Combines two lists

```
val $list is [1, 2, 3]
$list->extend([4, 5, 6]) // Returns [1, 2, 3, 4, 5, 6]
```

### pop

Removes a value from the list by index

```
val $list is [1, 2, 3]
$list->pop(1) // Returns 2
```

### at

Returns the value at a index

```
val $list is [1, 2, 3]
$list->at(1) // Returns 2
```

### set

Sets the value at a index

```
val $list is [1, 2, 3]
$list->set(1, 4) // Returns 4
```
